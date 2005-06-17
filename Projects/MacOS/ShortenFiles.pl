#!/usr/bin/perl

#
#	ShortenFiles.pl
#
#	Shorten file names to fit Mac HFS 31 character name limit. Names
#	are shortened using a table of hints.
#

#
#	$Id$	
#

use Config;
use FindBin;
use File::Find;
use File::Path;

#	Constants
$kMaxLength = 31;

# Globals
$gScriptBase = $FindBin::RealBin;
$gPathSep = GetPathSeparator();

$gSrcBase = $gScriptBase.$gPathSep."src";
$gDstBase = $gScriptBase.$gPathSep."MacSrc";

# Map from unshortened to shortened name
%gBadMap;

# Hints for shortening names.
# Hints are considered in order of occurance in list
# until the name is adaquately shortened or there
# are no more hints, in which case the name is
# more rudely truncated.
@gHints =
(
	["Exception",		"Except"	],
	["Implementation",	"Impl"		],
	["Reference",		"Ref"		],
	["Documentation",	"Doc"		],
	["Document",		"Doc"		],
	["Definition",		"Def"		],
	["Count",			"Cnt"		],
	["Entity",			"Ent"		],
	["Character",		"Char"		],
	["Processing",		"Proc"		],
	["Instruction",		"Instr"		],
	["Parent",			"Par"		],
	["Fragment",		"Frag"		],
	["Construction",	"Constr"	],
	["Default",			"Def"		],
	["Processor",		"Proc"		],
	["Compare",			"Comp"		],
	["Execution",		"Exe"		],
	["Functor",			"Func"		],
	["Collation",		"Col"		],
	["Element",			"El"		],
	["Datatype",		"DT"		],
	["Numeric",		"Num"		],
);

sub GetPathSeparator
{
	if ($^O eq 'MSWin32')
	{
		return '\\';
	}
	elsif ($^O eq 'MacOS')
	{
		return ':';
	}
	else
	{
		return '/';
	}
}


sub ShortenName
{
	$newName = $_;
	
LOOP:
	while (length($newName) > $kMaxLength)
	{
		foreach $hint (@gHints)
		{
			$pattern		= $hint->[0];
			$replacement	= $hint->[1];
			if ($newName =~ s/$pattern/$replacement/)
			{
				#print "\tmatched ".$hint->[0]."; replaced with ".$hint->[1]."\n";
				next LOOP;
			}
		}
		
		# No hint found, so we need to shorten the name manually
		$kill = length($newName) - $kMaxLength + 1;
		substr($newName, $kMaxLength / 2, $kill) = "_";
		#print "\tshorten manually\n";
	}
	
	return $newName;
}


sub Gather
{
	$path = $File::Find::name;
	$name = $_;
	
	if (-f $path)
	{
		if (length($name) > $kMaxLength)
		{
			#print "$name is too long\n";
			$newName = ShortenName($name);
			#print "--> ".$newName."\n";
			
			# Add an entry to our bad map
			$gBadMap{$name} = $newName
		}
	}
	else
	{
		#print "NOT A FILE: $File::Find::dir".$_."\n";
	}
}


sub GatherBadFiles
{
	find(\&Gather, $gSrcBase);
}


sub Copy
{
	# Caller wants $_ the same across this call
	$save_ = $_;
	
	$_ = "" if ($_ eq '.');
	 
	# remap the destination name as needed
	$srcName = $_;
	$dstName = $srcName;
	if (exists $gBadMap{$srcName})
	{
		$dstName = $gBadMap{$srcName};
		print "Renaming file $srcName\n\t--> $dstName\n";
	}
		
	$srcPath = $File::Find::name;
	$dstPath = $srcPath;
		
	# Interpolate paths
	$dstPath =~ s/$srcName$/$dstName/;
	$dstPath =~ s/^$gSrcBase/$gDstBase/;
	
	#print "srcName $srcName, dstName $dstName, srcPath $srcPath, dstPath $dstPath\n";
	
	if (-d $srcPath)
	{
		# make the dst directory if it doesn't already exist
		if (not -e $dstPath)
		{
			print "Making directory $dstPath\n";
			mkpath($dstPath);
		}
	}
	else
	{
		# Copy the file, fixing up includes as we go
		
		# Open files
		open(SRC, "< $srcPath")         or die "can't open $srcPath: $!";
		open(DST, "> $dstPath")         or die "can't open $dstPath: $!";
		
		# Copy the file a line at a time
		while (<SRC>)
		{
			# Look for an include line
			if (	/^(\s*)#(\s*)include(\s*)<(\s*)(.*)(\s*)>(.*)/
				 || /^(\s*)#(\s*)include(\s*)"(\s*)(.*)(\s*)"(.*)/
			   )
			{
				# We've got the include file name (path).
				# Now isolate just the leaf of that
				$includePath = $5;
				
				$includePath =~ /^([^\/]*\/)*([^\/]+)$/;
				$includeLeaf = $2;
				
				# Check for the include file in our bad map, replacing
				# it as necessary
				if (exists $gBadMap{$includeLeaf})
				{
					$dup = $_;
					$was = $includeLeaf;
					$sub = $gBadMap{$was};
					
					s/^(.*)include(.*)$was(.*)/$1include$2$sub$3/;
					print "file $srcName: changed include $includePath\n\t--> $_";
					#print "Change $dup to $_";
				}
			}
			
			# Output the modified (or not) line
			(print DST $_)				or die "can't write to $dstPath: $!";
		}
		
		# Close 'dem files
		close(SRC)                 		or die "can't close $srcPath: $!";
		close(DST)                  	or die "can't close $dstPath: $!";
	}
	
	$_ = $save_;
}


sub CopyFiles
{
	find(\&Copy, $gSrcBase);
}


# Tell what we know:
print "\n";
print "Script:                $0\n";
print "Source Directory:      $gSrcBase\n";
print "Destination Directory: $gDstBase\n";
print "\n";

#-- Scan phase
#NOTE: we actually don't limit our processing to the file types listed here!!!
#for each .cpp or .hpp or .c or .cc file in tree
#	if name too long
#		shorten the name
# store it in a map
# end if
#end for

# Gather up the bad files into global map gBadMap
print "Scanning...\n";
undef %gBadMap;
GatherBadFiles($srcBase);

# Print bad map
#print "BadMap:\n";
#foreach $key (sort(keys %gBadMap)) {
#	print $key, '=', $gBadMap{$key}, "\n";
#}

#-- Copy/Modify phase
#for each file in tree
# copy file to new directory, scanning for include as we go
# if include contains a name from the map
# then replace it
#end for
print "Copying...\n";
CopyFiles($srcBase, $dstBase);

print "\nDone\n";

