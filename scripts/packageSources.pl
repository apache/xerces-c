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
        print ("Example: perl packageSources.pl -oc:\\xerces-c_1_1_1\n");
        print ("         perl packageSources.pl -o\$HOME/xerces-c_1_1_1\n");
        exit(-1);
}

# We need ICUROOT only if we intend to use ICU for transcoding or message loading
# As of Version 3, this is not mandatory any more.
if (length($ICUROOT) > 0) {
       print "Using $ICUROOT as the ICU root directory.\n";
}

#Fix the backslashes on the Windows platform
$XERCESCROOT =~ s/\\/\//g;
$ICUROOT =~ s/\\/\//g;

# Read the target version from the file $XERCESCROOT/src/util/XercesDefs.hpp
$versionfile = "$XERCESCROOT/src/util/XercesDefs.hpp";
$openresult = open (VERSIONFILE, "<$versionfile");

if ($openresult == 0) {
   print "Sorry, I could not find the file $versionfile to pick up the version. Aborting ...\n";
   exit(-1);
}

while ($fileline = <VERSIONFILE>) {
   if ($fileline =~ /gXML4CFullVersionStr = \"(.*)\"/) {   # "
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
$platform =~ m/(\w*)/;
$platform = $1;
close (PLATFORM);

print "\nPackaging XERCES-C sources in $srctargetdir on platform $platform ...\n";

&package_sources();

exit(0);

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

   # Select APache style or IBM Alphaworks style docs
   if (length($ICUROOT) > 0) {
   	change_documentation_entities("$srctargetdir/doc/dtd/entities.ent");
   	change_doxygen("$srctargetdir/doc/Doxyfile");
   	change_createdocs_bat("$srctargetdir/createdocs.bat");
   }

   # Now create the API documentation from the XML sources
   chdir ("$srctargetdir/doc");
   system ("doxygen");

   # Now create the User documentation from the XML sources

   if ($platform =~ m/Windows/ || $platform =~ m/CYGWIN_NT/) {
      $RM = "rm";
      system("$RM -rf *.obj");
      system("$RM -rf *.dep");
      system("$RM -rf *.mak");
      system("$RM -rf Makefile");
      chdir ("$srctargetdir");
      system("createdocs");

   }
   else {   # all UNIX flavors

#   Docs are only building on Windows for now...

      $RM = "\\rm";
      system("find $srctargetdir -name \"*.o\" -print -exec rm -f {} \\;");
      system("find $srctargetdir -name \"core\" -print -exec rm -f {} \\;");
      system("find $srctargetdir -name \"a.out\" -print -exec rm -f {} \\;");
      system("find $srctargetdir -name \"Makefile\" -print -exec rm -f {} \\;");
      system("find $srctargetdir -name \"*.dep\" -print -exec rm -f {} \\;");
      system("find $srctargetdir -name \"*.mak\" -print -exec rm -f {} \\;");

      print "Changing directory permissions of the following files ...\n";
      # Change the directory permissions
      system ("chmod 644 `find $srctargetdir -type f -print`");
      system ("chmod 755 `find $srctargetdir -type d -print`");

      # Change the script file permissions
      system ("chmod 755 `find $srctargetdir -name runConfigure -print`");
      system ("chmod 755 `find $srctargetdir -name configure -print`");
      system ("chmod 755 `find $srctargetdir -name install-sh -print`");

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
#  system("$RM -rf $srctargetdir/Projects/Win32/BCB4");
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
   if ($platform =~ m/Windows/ || $platform =~ m/CYGWIN/) {

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
                $line =~ s/SAX2_EXPORT//g;
                $line =~ s/CDOM_EXPORT//g;
                $line =~ s/XMLPARSER_EXPORT//g;
                $line =~ s/PARSERS_EXPORT//g;
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
                       $localName = "$_";
			# print $name, "\n";
			next if $subcount == 0;

			($dev,$ino,$mode,$nlink) = lstat($_);
			next unless -d _;

			if ($localName =~ m/CVS/i) {
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

sub change_documentation_entities()
{
        my ($thefile) = @_;
        print "\nConverting documentation entities ($thefile) for ICU usage...";
        my $thefiledotbak = $thefile . ".bak";
        rename ($thefile, $thefiledotbak);

        open (FIZZLE, $thefiledotbak);
        open (FIZZLEOUT, ">$thefile");
        while ($line = <FIZZLE>) {
                $line =~ s/"Xerces C\+\+ Parser"/"XML for C\+\+ Parser"/g;
                $line =~ s/"Xerces-C\+\+"/"XML4C"/g;
                $line =~ s/"1\.6\.0"/"4\.0\.0"/g;
                $line =~ s/"1\.4\.0"/"3\.3\.1"/g;
                $line =~ s/"Xerces"/"XML4C"/g;
                $line =~ s/"xerces-c1_6_0"/"xml4c-4_0_0"/g;
                $line =~ s/"xerces-c-src1_6_0"/"xml4c-src-4_0_0"/g;
                $line =~ s/xerces-c-dev\@xml\.apache\.org/xml4c\@us\.ibm\.com/g;
                $line =~ s/xml\.apache\.org\/dist\/xerces-c/www\.alphaworks\.ibm\.com\/tech\/xml4c/g;
                print FIZZLEOUT $line;
        }
        close (FIZZLEOUT);
        close (FIZZLE);
        unlink ($thefiledotbak);
}

sub change_doxygen()
{
        my ($thefile) = @_;
        print "\nConverting Doxygen ($thefile) for ICU usage...";
        my $thefiledotbak = $thefile . ".bak";
        rename ($thefile, $thefiledotbak);

        open (FIZZLE, $thefiledotbak);
        open (FIZZLEOUT, ">$thefile");
        while ($line = <FIZZLE>) {
                $line =~ s/Xerces-C\+\+/XML4C/g;
                $line =~ s/1\.6\.0/4\.0\.0/g;
                $line =~ s/header.html/header_ibm.html/g;
                print FIZZLEOUT $line;
        }
        close (FIZZLEOUT);
        close (FIZZLE);
        unlink ($thefiledotbak);
}

sub change_createdocs_bat()
{
        my ($thefile) = @_;
        print "\nConverting createdocs.bat ($thefile) for ICU usage...";
        my $thefiledotbak = $thefile . ".bak";
        rename ($thefile, $thefiledotbak);

        open (FIZZLE, $thefiledotbak);
        open (FIZZLEOUT, ">$thefile");
        while ($line = <FIZZLE>) {
                $line =~ s/style-apachexml.jar/style-ibm.zip/g;
                print FIZZLEOUT $line;
        }
        close (FIZZLEOUT);
        close (FIZZLE);
        unlink ($thefiledotbak);
}

