#!/usr/bin/perl

push(@INC, "/home/xerces-c/bin", "/home/xerces-c/bin/perl/perl-RUN/opt/perl5/lib", "/Development2/cupert/bin/perl/perl-RUN/opt/perl5/lib", "/Development/cupert/usr/local/perl/perl-RUN/opt/perl5/lib");
require "getopt.pl";

$|=1;   # Force a flush after every print

# Set up the environment variables for XERCES-C and ICU
$XERCESCROOT = $ENV{'XERCESCROOT'};
$ICUROOT = $ENV{'ICUROOT'};

# Check for the environment variables and exit if error
if (!length($XERCESCROOT)) {
        print "You must set an environment variable called XERCESCROOT to work with this script.\n";
        exit(-1);
}

&Getopt('o');
$OUTPUTDIR = $opt_o;

# Check for the environment variables and exit if error
if (!length($XERCESCROOT) || !length($OUTPUTDIR)) {
        print ("Usage is: packageSources -o<output_directory>\n");
        print ("Example: perl packageSources.pl -od:\\xerces-c_1_0_0d01\n");
        print ("         perl packageSources.pl -o\$HOME/xerces-c_1_0_0d01\n");
        exit(-1);
}

# We need ICUROOT only if we intend to use ICU for transcoding or message loading
# As of Version 3, this is not mandatory any more.
# if (!length($ICUROOT)) {
#        print "You have not defined your International directory.\n You must set an environment variable called ICUROOT to proceed.\n";
#        exit (-1);
# }

#Fix the backslashes on the Windows platform
$XERCESCROOT =~ s/\\/\//g;
$ICUROOT =~ s/\\/\//g;

# Read the target version from the file $XERCESCROOT/src/util/XML4CDefs.hpp
$versionfile = "$XERCESCROOT/src/util/XML4CDefs.hpp";
$openresult = open (VERSIONFILE, "<$versionfile");

if ($openresult == 0) {
   print "Sorry, I could not find the file $versionfile to pick up the version. Aborting ...\n";
   exit(-1);
}

while ($fileline = <VERSIONFILE>) {
   if ($fileline =~ /gXML4CFullVersionStr = \"(.*)\"/) {
     $binarytargetdir = $1;  # We found the version string inside this file
   }
}
close(VERSIONFILE);

$binarytargetdir =~ s/\./_/g;    # Substitute the dots
$binarytargetdir =~ s/\s/_/g;    # Substitute the blanks
$binarytargetdir =~ s/\\/\//g;   # Fix the backslashes, if they exist, probably doesn't

# Now check if the target directory exists, exit if it does
if (-e $OUTPUTDIR) {
        print ("Error: The target directory \'$OUTPUTDIR\' already exists.\n");
        print ("       You must start with a clean directory to package your product.\n");
        exit(1);
}

#Construct the name of the zip file by extracting the last directory name
$srczipfiles = $OUTPUTDIR;
$srczipfiles =~ s/.*\/([\w|-]*)$/$1/g;
$srczipfiles = $srczipfiles . "/*";
$srctargetdir = $OUTPUTDIR;

# Find out the platform from 'uname -a'
open(PLATFORM, "uname -a|");
$platform = <PLATFORM>;
$platform =~ m/(^\w*\s)/;
$platform = $1;
close (PLATFORM);

print "\nPackaging XERCES-C sources in " . $srctargetdir . " on platform " . $platform . "...\n";

# Build the API docs
$docppfilelist = "";

# First scan the 'src/sax' directory
$hppdir = "$XERCESCROOT/src/sax";
chdir ($hppdir);
opendir (THISDIR, $hppdir);
@allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
@allhppfiles = grep(/\.hpp/, @allfiles);
closedir(THISDIR);
foreach $hppfile (@allhppfiles) {
       $docppfilelist = $docppfilelist . " " . $hppdir . "/" . $hppfile;
}

# Next scan the 'src/dom' directory
$hppdir = "$XERCESCROOT/src/dom";
chdir ($hppdir);
opendir (THISDIR, $hppdir);
@allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
@allhppfiles = grep(/\.hpp/, @allfiles);
@alldomhppfiles = grep(/DOM_/, @allhppfiles);
closedir(THISDIR);
foreach $hppfile (@alldomhppfiles) {
   $docppfilelist = $docppfilelist . " " . $hppdir . "/" . $hppfile;
}

# Next scan the 'src/framework' directory
$hppdir = "$XERCESCROOT/src/framework";
chdir ($hppdir);
opendir (THISDIR, $hppdir);
@allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
@allhppfiles = grep(/\.hpp/, @allfiles);
closedir(THISDIR);
foreach $hppfile (@allhppfiles) {
       $docppfilelist = $docppfilelist . " " . $hppdir . "/" . $hppfile;
}

# Add a few more files that we left out
$docppfilelist = $docppfilelist . " $XERCESCROOT/src/dom/DOMString.hpp";
$docppfilelist = $docppfilelist . " $XERCESCROOT/src/util/XMLString.hpp";
$docppfilelist = $docppfilelist . " $XERCESCROOT/src/util/PlatformUtils.hpp";
$docppfilelist = $docppfilelist . " $XERCESCROOT/src/parsers/DOMParser.hpp";
$docppfilelist = $docppfilelist . " $XERCESCROOT/src/parsers/SAXParser.hpp";

system ("doc++ -d $XERCESCROOT/doc/html/apiDocs -B $XERCESCROOT/doc/html/apiDocs/tail.html -a -G -k -H -S $docppfilelist");

# Now create the User documentation from the XML sources
chdir ("$XERCESCROOT");
system("createdocs.bat");  # You must have Xerces-Java and Stylebook installed in addition to JDK1.2.2

&package_sources();

sub package_sources {

   # Packaging of source files begins here
   # We just copy over the relevant files from the actual source tree into
   # a mirror location, and then zip (or tar) it up

   chdir ("$srctargetdir");

   # Now copy over the files directory-by-directory
   print ("\nCopying source tree ...\n");
   system ("mkdir $srctargetdir");
   print ("Targetdir is : " . $srctargetdir . "\n");
   system("cp -Rf $XERCESCROOT/* $srctargetdir");

   if ($platform =~ m/Windows/) {
      $RM = "rm";
      system("$RM -rf *.obj");
      system("$RM -rf *.dep");
      system("$RM -rf *.mak");
      system("$RM -rf Makefile");
   }
   else {   # all UNIX flavors
      $RM = "\\rm";
      system("find $srctargetdir -name \"*.o\" -print -exec rm -f {} \\;");
      system("find $srctargetdir -name \"core\" -print -exec rm -f {} \\;");
      system("find $srctargetdir -name \"a.out\" -print -exec rm -f {} \\;");
      system("find $srctargetdir -name \"Makefile\" -print -exec rm -f {} \\;");
      system("find $srctargetdir -name \"*.dep\" -print -exec rm -f {} \\;");
      system("find $srctargetdir -name \"*.mak\" -print -exec rm -f {} \\;");

      # Change the directory permissions
      system ("chmod 644 `find $srctargetdir -type f`");
      system ("chmod 755 `find $srctargetdir -type d`");
      # Change the script file permissions
      system ("chmod 755 $srctargetdir/src/runConfigure $srctargetdir/src/configure $srctargetdir/src/install-sh");
      system ("chmod 755 $srctargetdir/src/config.sub $srctargetdir/src/config.guess");
      system ("chmod 755 $srctargetdir/samples/runConfigure $srctargetdir/samples/configure $srctargetdir/samples/install-sh");
      system ("chmod 755 $srctargetdir/samples/config.sub $srctargetdir/samples/config.guess");
      # To make sure that configure script is updated, run autoconf
      # chdir ("$srctargetdir/src");
      # system("autoconf"); # Our configure script is special, it has OS390 and AS400 tweaks
      # chdir ("$srctargetdir/samples");
      # system("autoconf"); # Our configure script is special, it has OS390 and AS400 tweaks
   }
   
   # Delete the irrelevant parts before the packaging
   system("$RM -f $srctargetdir/CMVC.GON");
   system("$RM -rf $srctargetdir/*.\$??");
   system("$RM -rf $srctargetdir/*.o");
   system("$RM -rf $srctargetdir/Build");
   system("$RM -rf $srctargetdir/build");
   system("$RM -rf $srctargetdir/bin");
   system("$RM -rf $srctargetdir/obj/*.o");
   system("$RM -rf $srctargetdir/obj/Makefile");
   system("$RM -rf $srctargetdir/obj/*.obj");
   system("$RM -rf $srctargetdir/lib");
   system("$RM -rf $srctargetdir/include");
   system("$RM -rf $srctargetdir/doc/*.zip");

   system("$RM -rf $srctargetdir/Projects/Win32/Unsupported");
   system("$RM -rf $srctargetdir/Projects/Win32/BCB4");
   system("$RM $srctargetdir/scripts/packageSources.pl");

   chdir ($srctargetdir);
   system("$RM -rf *.opt");
   system("$RM -rf *.o");
   system("$RM -rf *.so");
   system("$RM -rf *.sl");
   system("$RM -rf *.a");
   system("$RM -rf *.ncb");
   system("$RM -rf *.plg");
   system("$RM -rf #*");

   # Walk through the source directory structure and delete all CVS directories
   &deleteCVSdirs($srctargetdir);

   # remove the export clauses
   chdir ("$srctargetdir/doc/html/apiDocs");
   opendir (THISDIR, "$srctargetdir/doc/html/apiDocs");
   @allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
   @allhtmlfiles = grep(/\.html/, @allfiles);
   closedir(THISDIR);
   foreach $htmlfile (@allhtmlfiles) {
       &remove_export_clauses ("$srctargetdir/doc/html/apiDocs/" . $htmlfile);
   }

   chdir ("$srctargetdir/..");
   if ($platform =~ m/Windows/) {

      # Now package it all up using ZIP
      print ("\n\nZIPping up all source files ...\n");
      $srczipname = $srctargetdir . ".zip";
      print ("zip -r $srczipname $srczipfiles");
      system ("zip -r $srczipname $srczipfiles");
   }
   else {
      # Now package it all up using TAR
      print ("\n\nTARing all source files ...\n");
      $srczipname = $srctargetdir . ".tar";
      print ("tar -cvf $srczipname $srczipfiles");
      system ("tar -cvf $srczipname $srczipfiles");

      system ("gzip $srczipname");
   }
   print ("Done with packaging sources.\n");
}

sub remove_export_clauses()
{
        my ($thefile) = @_;
        print "\nRemoving export clauses in file $thefile";
        my $thefiledotbak = $thefile . ".bak";
        rename ($thefile, $thefiledotbak);

        open (FIZZLE, $thefiledotbak);
        open (FIZZLEOUT, ">$thefile");
        while ($line = <FIZZLE>) {
                $line =~ s/SAX_EXPORT//g;
                $line =~ s/CDOM_EXPORT//g;
                $line =~ s/XMLPARSER_EXPORT//g;
                $line =~ s/XMLUTIL_EXPORT//g;
                print FIZZLEOUT $line;
        }
        close (FIZZLEOUT);
        close (FIZZLE);
        unlink ($thefiledotbak);
}

sub deleteCVSdirs {
	local($dir,$nlink) = @_;
	local($dev,$ino,$mode,$subcount);
	
	($dev,$ino,$mode,$nlink) = stat($dir) unless $nlink;
	
	opendir(DIR, $dir) || die "Cannot open $dir";
	local(@filenames) = readdir(DIR);
	closedir(DIR);
	
	if ($nlink == 2) {
		for (@filenames) {
			next if $_ eq '.';
			next if $_ eq '..';
			print "$dir/$_\n";
		}
	}
	else {
		$subcount = $nlink - 2;
		for (@filenames) {
			next if $_ eq '.';
			next if $_ eq '..';
			$name = "$dir/$_";
			# print $name, "\n";
			next if $subcount == 0;
			
			($dev,$ino,$mode,$nlink) = lstat($_);
			next unless -d _;
			
			if ($name =~ m/CVS/i) {
				print ("Removing $name ...\n");
				system("$RM -rf $name");
				next;
			}
			chdir $_ || die "Cannot cd to $name";
			&deleteCVSdirs($name,$nlink);
			chdir '..';
			--$subcount;
		}
	}
}
