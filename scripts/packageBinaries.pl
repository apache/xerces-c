#!/usr/local/bin/perl5

push(@INC, "/home/xerces-c/bin", "/home/xerces-c/bin/perl/perl-RUN/opt/perl5/lib", "/Development2/cupert/bin/perl/perl-RUN/opt/perl5/lib", "/Development/cupert/usr/local/perl/perl-RUN/opt/perl5/lib");

require "getopt.pl";

$|=1;   # Force a flush after every print


# Extract the source and target directories
&Getopt('sopcxmntr');
$XERCESCROOT = $opt_s;
$targetdir = $opt_o;

# Check for the environment variables and exit if error
if (!length($XERCESCROOT) || !length($targetdir) || (length($opt_h) > 0) ) {
        print ("Usage is: packageBinaries <options>\n");
		print ("          options are:  -s <source_directory>\n");
		print ("                        -o <target_directory>\n");
        print ("                        -c <C compiler name> (e.g. gcc or xlc)\n");
        print ("                        -x <C++ compiler name> (e.g. g++ or xlC)\n");
        print ("                        -m <message loader> can be 'inmem', 'icu' or 'iconv'\n");
        print ("                        -n <net accessor> can be 'fileonly' or 'libwww'\n");
        print ("                        -t <transcoder> can be 'icu' or 'native'\n");
        print ("                        -r <thread option> can be 'pthread' or 'dce' (only used on HP-11)\n");
        print ("                        -h to get help on these commands\n");
        print ("Example: perl packageBinaries.pl -s\$HOME/xerces-c_1_0_0d01 -o\$HOME/xerces-c_1_0_0d01bin -cgcc -xg++ -minmem -nfileonly -tnative\n");
        exit(-1);
}

# Set up the environment variables for ICU
# As of Version 3, ICU is not a required component of XERCES-C
$ICUROOT = $ENV{'ICUROOT'};
if (!length($ICUROOT)) {
       print "You have not defined your ICU install directory.\n";
	   print "You must set an environment variable called ICUROOT to package ICU with XERCES-C.\n";
	   print "Proceeding to build XERCES-C without ICU...\n";
}

# Check if the source directory exists or not
if (!(-e $XERCESCROOT)) {
        print ("The directory $XERCESCROOT does not exist. Cannot proceed any further.\n");
        exit(-1);
}

# Check if the target directory exists, exit if it does
if (-e $targetdir) {
        print ("Error: The target directory \'$targetdir\' already exists.\n");
        print ("       You must start with a clean directory to package your product.\n");
        exit(1);
}

#Fix the backslashes on the Windows platform
$XERCESCROOT =~ s/\\/\//g;
$ICUROOT =~ s/\\/\//g;
$targetdir =~ s/\\/\//g;

# Find out the platform from 'uname -a'
open(PLATFORM, "uname -s|");
$platform = <PLATFORM>;
chomp($platform);
close (PLATFORM);

print "Packaging binaries for \`" . $platform . "\` in " . $targetdir . " ...\n";

#Construct the name of the zip file by extracting the last directory name
$zipfiles = $targetdir;
$zipfiles =~ s/.*\/([\w|-]*)$/$1/g;
$zipfiles = $zipfiles . "/*";

$buildmode = "Release";    # Universally, why do you want to package Debug builds anyway?

if ($platform =~ m/Windows/) {

        $platformname = 'Win32';    # Needed this way by nmake
        if (-e $targetdir.".zip") {
                print ("Error: The target file \'$targetdir.zip\' already exists.\n");
                print ("       You must delete the file \'$targetdir.zip\' to package your product.\n");
                exit(1);
        }

        # Make the target directory and its main subdirectories
        mkdir ($targetdir, "0644");
        mkdir ($targetdir . "/bin", "0644");
        mkdir ($targetdir . "/lib", "0644");
        mkdir ($targetdir . "/include", "0644");
        mkdir ($targetdir . "/include/sax", "0644");
        mkdir ($targetdir . "/include/framework", "0644");
        mkdir ($targetdir . "/include/internal", "0644");
        mkdir ($targetdir . "/include/parsers", "0644");
        mkdir ($targetdir . "/include/util", "0644");
        mkdir ($targetdir . "/include/dom", "0644");
        mkdir ($targetdir . "/include/icu", "0644");
        mkdir ($targetdir . "/include/validators", "0644");
        mkdir ($targetdir . "/samples", "0644");
        mkdir ($targetdir . "/samples/Projects", "0644");
        mkdir ($targetdir . "/samples/Projects/Win32", "0644");
        mkdir ($targetdir . "/samples/data", "0644");
        mkdir ($targetdir . "/samples/SAXCount", "0644");
        mkdir ($targetdir . "/samples/SAXPrint", "0644");
        mkdir ($targetdir . "/samples/DOMCount", "0644");
        mkdir ($targetdir . "/samples/DOMPrint", "0644");
        mkdir ($targetdir . "/samples/Redirect", "0644");
        mkdir ($targetdir . "/samples/MemParse", "0644");
        mkdir ($targetdir . "/samples/PParse", "0644");
        mkdir ($targetdir . "/samples/StdInParse", "0644");
        mkdir ($targetdir . "/doc", "0644");
        mkdir ($targetdir . "/doc/apiDocs", "0644");
        mkdir ($targetdir . "/bin/icu", "0644");
        mkdir ($targetdir . "/bin/icu/data", "0644");

        #Clean up all the dependency files, causes problems for nmake
		if (length($ICUROOT) > 0) {
			chdir ("$ICUROOT");
			system ("del /s /f *.dep");

			print ("Since you have defined ICUROOT in your environment, I am building ICU too ...");
			# Make the icu dll
			chdir ("$ICUROOT/source/common");
			print "Executing: nmake -f common.mak clean CFG=\"common - $platformname $buildmode\"";
			system("nmake -f common.mak clean CFG=\"common - $platformname $buildmode\"");
			print "Executing: nmake -f common.mak all CFG=\"common - $platformname $buildmode\"";
			system("nmake -f common.mak all CFG=\"common - $platformname $buildmode\"");

			# Make the makeconv utility
			chdir ("$ICUROOT/source/tools/makeconv");
			system "nmake -f makeconv.mak clean CFG=\"makeconv - $platformname $buildmode\"";
			print "Executing: nmake -f makeconv.mak CFG=\"makeconv - $platformname $buildmode\"";
			system("nmake -f makeconv.mak CFG=\"makeconv - $platformname $buildmode\"");
		}

        #Clean up all the dependency files, causes problems for nmake
        chdir ("$XERCESCROOT");
        system ("del /s /f *.dep");

        # Make the XERCES-C dll
        chdir ("$XERCESCROOT/Projects/Win32/VC6/xerces-all/XercesLib");
        print "Executing: nmake -f XercesLib.mak clean CFG=\"XercesLib - $platformname $buildmode\"";
        system("nmake -f XercesLib.mak clean CFG=\"XercesLib - $platformname $buildmode\"");
        print "Executing: nmake -f XercesLib.mak all CFG=\"XercesLib - $platformname $buildmode\"";
        system("nmake -f XercesLib.mak all CFG=\"XercesLib - $platformname $buildmode\"");

        # Make the SAXCount sample
        chdir ("$XERCESCROOT/Projects/Win32/VC6/xerces-all/SAXCount");
        system "nmake -f SAXCount.mak clean CFG=\"SAXCount - $platformname $buildmode\"";
        print "Executing: nmake -f SAXCount.mak all CFG=\"SAXCount - $platformname $buildmode\"";
        system("nmake -f SAXCount.mak all CFG=\"SAXCount - $platformname $buildmode\"");

        # Make the SAXPrint sample
        chdir ("$XERCESCROOT/Projects/Win32/VC6/xerces-all/SAXPrint");
        system "nmake -f SAXPrint.mak clean CFG=\"SAXPrint - $platformname $buildmode\"";
        print "Executing: nmake -f SAXPrint.mak all CFG=\"SAXPrint - $platformname $buildmode\"";
        system("nmake -f SAXPrint.mak all CFG=\"SAXPrint - $platformname $buildmode\"");

        # Make the DOMCount sample
        chdir ("$XERCESCROOT/Projects/Win32/VC6/xerces-all/DOMCount");
        system "nmake -f DOMCount.mak clean CFG=\"DOMCount - $platformname $buildmode\"";
        print "Executing: nmake -f DOMCount.mak all CFG=\"DOMCount - $platformname $buildmode\"";
        system("nmake -f DOMCount.mak all CFG=\"DOMCount - $platformname $buildmode\"");

        # Make the DOMPrint sample
        chdir ("$XERCESCROOT/Projects/Win32/VC6/xerces-all/DOMPrint");
        system "nmake -f DOMPrint.mak clean CFG=\"DOMPrint - $platformname $buildmode\"";
        print "Executing: nmake -f DOMPrint.mak all CFG=\"DOMPrint - $platformname $buildmode\"";
        system("nmake -f DOMPrint.mak all CFG=\"DOMPrint - $platformname $buildmode\"");

        # Make the Redirect sample
        chdir ("$XERCESCROOT/Projects/Win32/VC6/xerces-all/Redirect");
        system "nmake -f Redirect.mak clean CFG=\"Redirect - $platformname $buildmode\"";
        print "Executing: nmake -f Redirect.mak all CFG=\"Redirect - $platformname $buildmode\"";
        system("nmake -f Redirect.mak all CFG=\"Redirect - $platformname $buildmode\"");


        # Make the MemParse sample
        chdir ("$XERCESCROOT/Projects/Win32/VC6/xerces-all/MemParse");
        system "nmake -f MemParse.mak clean CFG=\"MemParse - $platformname $buildmode\"";
        print "Executing: nmake -f MemParse.mak all CFG=\"MemParse - $platformname $buildmode\"";
        system("nmake -f MemParse.mak all CFG=\"MemParse - $platformname $buildmode\"");

        # Make the PParse sample
        chdir ("$XERCESCROOT/Projects/Win32/VC6/xerces-all/PParse");
        system "nmake -f PParse.mak clean CFG=\"PParse - $platformname $buildmode\"";
        print "Executing: nmake -f PParse.mak all CFG=\"PParse - $platformname $buildmode\"";
        system("nmake -f PParse.mak all CFG=\"PParse - $platformname $buildmode\"");

        # Make the StdInParse sample
        chdir ("$XERCESCROOT/Projects/Win32/VC6/xerces-all/StdInParse");
        system "nmake -f StdInParse.mak clean CFG=\"StdInParse - $platformname $buildmode\"";
        print "Executing: nmake -f StdInParse.mak all CFG=\"StdInParse - $platformname $buildmode\"";
        system("nmake -f StdInParse.mak all CFG=\"StdInParse - $platformname $buildmode\"");

		if (length($ICUROOT) > 0) {
			# run makeconv now
			chdir ("$ICUROOT/data");
			opendir (THISDIR, "$ICUROOT/data");
			@allfiles = grep(!/^\.\.?$/, readdir(THISDIR));
			@allucmfiles = grep(/\.ucm/, @allfiles);
			closedir(THISDIR);
			foreach $ucmfile (@allucmfiles) {
					system ("$ICUROOT/source/tools/makeconv/$buildmode/makeconv.exe $ucmfile");
			}
		}

        # Decide where you want the build copied from
        chdir ($targetdir);
        $BUILDDIR = $XERCESCROOT . "/Build/Win32/VC6/" . $buildmode;
        print "\nBuild is being copied from \'" . $BUILDDIR . "\'";

        # Populate the include output directory
        print ("\n\nCopying headers files ...\n");
        $xcopycommand = "xcopy $XERCESCROOT\\src\\*.hpp $targetdir\\include";
        $xcopycommand =~ s/\//\\/g;
        system ("$xcopycommand /S /C /I /R");
        $xcopycommand = "xcopy $XERCESCROOT\\src\\*.c $targetdir\\include";
        $xcopycommand =~ s/\//\\/g;
        system ("$xcopycommand /S /C /I /R");

		if (length($ICUROOT) > 0) {
        	system("cp -Rfv $ICUROOT/include/* $targetdir/include/icu");
		}

        # Populate the binary output directory
        print ("\n\nCopying binary outputs ...\n");
        system("cp -fv $BUILDDIR/*.dll $targetdir/bin");
        system("cp -fv $BUILDDIR/*.exe $targetdir/bin");
		if (length($ICUROOT) > 0) {
			system("cp -fv $ICUROOT/bin/$buildmode/icuuc.dll $targetdir/bin");
			system("cp -fv $ICUROOT/lib/$buildmode/icuuc.lib $targetdir/lib");
			system("cp -fv $ICUROOT/source/tools/makeconv/$buildmode/makeconv.exe $targetdir/bin");
		}
        system("cp -fv $BUILDDIR/xerces-c_1.lib $targetdir/lib");

        # Copy the locale files
		if (length($ICUROOT) > 0) {
			system("cp -fv $ICUROOT/data/*.cnv $targetdir/bin/icu/data/");
			system("cp -fv $ICUROOT/data/convrtrs.txt $targetdir/bin/icu/data/");
		}

        # Populate the samples directory
        print ("\n\nCopying sample files ...\n");
        system("cp -Rfv $XERCESCROOT/samples/Projects/* $targetdir/samples/Projects");
        system("cp -Rfv $XERCESCROOT/samples/SAXCount/* $targetdir/samples/SAXCount");
        system("cp -Rfv $XERCESCROOT/samples/SAXPrint/* $targetdir/samples/SAXPrint");
        system("cp -Rfv $XERCESCROOT/samples/DOMCount/* $targetdir/samples/DOMCount");
        system("cp -Rfv $XERCESCROOT/samples/DOMPrint/* $targetdir/samples/DOMPrint");
        system("cp -Rfv $XERCESCROOT/samples/Redirect/* $targetdir/samples/Redirect");
        system("cp -Rfv $XERCESCROOT/samples/MemParse/* $targetdir/samples/MemParse");
        system("cp -Rfv $XERCESCROOT/samples/PParse/* $targetdir/samples/PParse");
        system("cp -Rfv $XERCESCROOT/samples/StdInParse/* $targetdir/samples/StdInParse");
        system("cp -Rfv $XERCESCROOT/samples/data/* $targetdir/samples/data");

        # Populate the docs directory
        print ("\n\nCopying documentation ...\n");
        system("cp -Rfv $XERCESCROOT/doc/* $targetdir/doc");
        system("cp -Rfv $XERCESCROOT/doc/apiDocs/* $targetdir/doc/apiDocs");
        system("cp $XERCESCROOT/Readme.html $targetdir");
        system("cp $XERCESCROOT/doc/license.html $targetdir");

        # Now package it all up using ZIP
        chdir ("$targetdir/..");
        print ("\n\nZIPping up all files ...\n");
        $zipname = $targetdir . ".zip";
        print ("zip -r $zipname $zipfiles");
        system ("zip -r $zipname $zipfiles");
}


if ( ($platform =~ m/AIX/)    || ($platform =~ m/HP-UX/) ||
     ($platform =~ m/SunOS/) || ($platform =~ m/Linux/) ) {

        # Decide on the platform specific stuff first
        if ($platform =~ m/AIX/) {
           $icuCompileFlags = 'CXX="xlC_r -L/usr/lpp/xlC/lib" CC="xlc_r -L/usr/lpp/xlC/lib" C_FLAGS="-w -O" CXX_FLAGS="-w -O"'; 
        }
        if ($platform eq 'HP-UX') {
            if ($opt_c eq 'CC') {
                $icuCompileFlags = 'CC=cc CXX=CC CXXFLAGS="+eh +DAportable -w -O" CFLAGS="+DAportable -w -O"'; 
            }
            else {
                $icuCompileFlags = 'CC=cc CXX=aCC CXXFLAGS="+DAportable -w -O" CFLAGS="+DAportable -w -O"'; 
            }
        }
        if ($platform =~ m/Linux/) {
           $icuCompileFlags = 'CC=gcc CXX=g++ CXXFLAGS="-w -O" CFLAGS="-w -O"'; 
	   $platform =~ tr/A-Z/a-z/;
        }
        if ($platform =~ m/SunOS/) {
           $icuCompileFlags = 'CC=cc CXX=CC CXXFLAGS="-w -O" CFLAGS="-w -O"'; 
        }

        # Check if the target directories already exist or not
        if (-e $targetdir.".tar") {
                print ("Error: The target file \'$targetdir.tar\' already exists.\n");
                print ("       You must delete the file \'$targetdir.tar\' to package your product.\n");
                exit(1);
        }

        if (-e $srctargetdir.".tar") {
                print ("Error: The target file \'$srctargetdir.tar\' already exists.\n");
                print ("       You must delete the file \'$srctargetdir.tar\' to package your product.\n");
                exit(1);
        }

        $platform =~ tr/A-Z/a-z/;

        # Make the target directory and its main subdirectories
        system ("mkdir $targetdir");
        system ("mkdir $targetdir/bin");
        system ("mkdir $targetdir/lib");
        system ("mkdir $targetdir/include");
	if (length($ICUROOT) > 0) {
		system ("mkdir $targetdir/lib/icu");
		system ("mkdir $targetdir/lib/icu/data");
		system ("mkdir $targetdir/include/icu");
	}
        system ("mkdir $targetdir/include/sax");
        system ("mkdir $targetdir/include/framework");
        system ("mkdir $targetdir/include/internal");
        system ("mkdir $targetdir/include/parsers");
        system ("mkdir $targetdir/include/util");
	system ("mkdir $targetdir/include/util/Compilers");
	system ("mkdir $targetdir/include/util/MsgLoaders");
	system ("mkdir $targetdir/include/util/MsgLoaders/ICU");
	system ("mkdir $targetdir/include/util/MsgLoaders/InMemory");
	system ("mkdir $targetdir/include/util/MsgLoaders/MsgCatalog");
	system ("mkdir $targetdir/include/util/MsgLoaders/Win32");
	system ("mkdir $targetdir/include/util/Platforms");
	system ("mkdir $targetdir/include/util/Platforms/AIX");
	system ("mkdir $targetdir/include/util/Platforms/HPUX");
	system ("mkdir $targetdir/include/util/Platforms/Linux");
	system ("mkdir $targetdir/include/util/Platforms/MacOS");
	system ("mkdir $targetdir/include/util/Platforms/OS2");
	system ("mkdir $targetdir/include/util/Platforms/OS390");
	system ("mkdir $targetdir/include/util/Platforms/Solaris");
	system ("mkdir $targetdir/include/util/Platforms/Tandem");
	system ("mkdir $targetdir/include/util/Platforms/Win32");
	system ("mkdir $targetdir/include/util/Transcoders");
	system ("mkdir $targetdir/include/util/Transcoders/ICU");
	system ("mkdir $targetdir/include/util/Transcoders/Iconv");
	system ("mkdir $targetdir/include/util/Transcoders/Win32");
        system ("mkdir $targetdir/include/dom");
        system ("mkdir $targetdir/include/validators");
        system ("mkdir $targetdir/include/validators/DTD");
        system ("mkdir $targetdir/samples");
        system ("mkdir $targetdir/samples/data");
        system ("mkdir $targetdir/samples/SAXCount");
        system ("mkdir $targetdir/samples/SAXPrint");
        system ("mkdir $targetdir/samples/DOMCount");
        system ("mkdir $targetdir/samples/DOMPrint");
        system ("mkdir $targetdir/samples/Redirect");
        system ("mkdir $targetdir/samples/MemParse");
        system ("mkdir $targetdir/samples/PParse");
        system ("mkdir $targetdir/samples/StdInParse");
        system ("mkdir $targetdir/doc");
        system ("mkdir $targetdir/doc/apiDocs");

		if (length($ICUROOT) > 0) {
			# First make the ICU files
			chdir ("$ICUROOT/source");
			system ("$icuCompileFlags configure --prefix=$ICUROOT");
			chdir ("$ICUROOT/source/common");
			system ("gmake");
			system ("gmake install");

			chdir ("$ICUROOT/source/tools/makeconv");
			system ("gmake");
			# For the antiquated CC compiler under HPUX, we need to invoke
			# gmake one extra time to generate the .cnv files.
			if ( ($platform eq 'HP-UX') && ($opt_c eq 'CC') ) {
				system ("gmake");
			}
		}

        # make the source files
        chdir ("$XERCESCROOT/src");

        system ("runConfigure -p$platform -c$opt_c -x$opt_x -m$opt_m -n$opt_n -t$opt_t -r$opt_r");
        system ("gmake");

        # Now build the samples
        chdir ("$XERCESCROOT/samples");
        system ("runConfigure -p$platform -c$opt_c -x$opt_x");
        system ("gmake");

        chdir ($targetdir);

        # Populate the include output directory
        print ("\n\nCopying headers files ...\n");
        system("cp -Rf $XERCESCROOT/src/sax/*.hpp $targetdir/include/sax");
        system("cp -Rf $XERCESCROOT/src/framework/*.hpp $targetdir/include/framework");
        system("cp -Rf $XERCESCROOT/src/dom/DOM*.hpp $targetdir/include/dom");
        system("cp -Rf $XERCESCROOT/src/internal/*.hpp $targetdir/include/internal");
        system("cp -Rf $XERCESCROOT/src/internal/*.c $targetdir/include/internal");
        system("cp -Rf $XERCESCROOT/src/parsers/*.hpp $targetdir/include/parsers");
        system("cp -Rf $XERCESCROOT/src/util/*.hpp $targetdir/include/util");
        system("cp -Rf $XERCESCROOT/src/util/*.c $targetdir/include/util");
	system("cp -Rf $XERCESCROOT/src/util/Compilers/*.hpp $targetdir/include/util/Compilers");
	system("cp -Rf $XERCESCROOT/src/util/MsgLoaders/*.hpp $targetdir/include/util/MsgLoaders");
	system("cp -Rf $XERCESCROOT/src/util/MsgLoaders/ICU/*.hpp $targetdir/include/util/MsgLoaders/ICU");
	system("cp -Rf $XERCESCROOT/src/util/MsgLoaders/InMemory/*.hpp $targetdir/include/util/MsgLoaders/InMemory");
	system("cp -Rf $XERCESCROOT/src/util/MsgLoaders/MsgCatalog/*.hpp $targetdir/include/util/MsgLoaders/MsgCatalog");
	system("cp -Rf $XERCESCROOT/src/util/MsgLoaders/Win32/*.hpp $targetdir/include/util/MsgLoaders/Win32");
	system("cp -Rf $XERCESCROOT/src/util/Platforms/*.hpp $targetdir/include/util/Platforms");
	system("cp -Rf $XERCESCROOT/src/util/Platforms/AIX/*.hpp $targetdir/include/util/Platforms/AIX");
	system("cp -Rf $XERCESCROOT/src/util/Platforms/HPUX/*.hpp $targetdir/include/util/Platforms/HPUX");
	system("cp -Rf $XERCESCROOT/src/util/Platforms/Linux/*.hpp $targetdir/include/util/Platforms/Linux");
	system("cp -Rf $XERCESCROOT/src/util/Platforms/MacOS/*.hpp $targetdir/include/util/Platforms/MacOS");
	system("cp -Rf $XERCESCROOT/src/util/Platforms/OS2/*.hpp $targetdir/include/util/Platforms/OS2");
	system("cp -Rf $XERCESCROOT/src/util/Platforms/OS390/*.hpp $targetdir/include/util/Platforms/OS390");
	system("cp -Rf $XERCESCROOT/src/util/Platforms/Solaris/*.hpp $targetdir/include/util/Platforms/Solaris");
	system("cp -Rf $XERCESCROOT/src/util/Platforms/Tandem/*.hpp $targetdir/include/util/Platforms/Tandem");
	system("cp -Rf $XERCESCROOT/src/util/Platforms/Win32/*.hpp $targetdir/include/util/Platforms/Win32");
	system("cp -Rf $XERCESCROOT/src/util/Transcoders/*.hpp $targetdir/include/util/Transcoders");
	system("cp -Rf $XERCESCROOT/src/util/Transcoders/ICU/*.hpp $targetdir/include/util/Transcoders/ICU");
	system("cp -Rf $XERCESCROOT/src/util/Transcoders/Iconv/*.hpp $targetdir/include/util/Transcoders/Iconv");
	system("cp -Rf $XERCESCROOT/src/util/Transcoders/Win32/*.hpp $targetdir/include/util/Transcoders/Win32");
	system("cp -Rf $XERCESCROOT/src/validators/DTD/*.hpp $targetdir/include/validators/DTD");
		
		if (length($ICUROOT) > 0) {
			print "\nInternational files are being copied from \'" . $ICUROOT . "\'";
			system("cp -Rf $ICUROOT/include/* $targetdir/include/icu");
		}

        # Populate the binary output directory
        print ("\n\nCopying binary outputs ...\n");
        system("cp -Rf $XERCESCROOT/bin/* $targetdir/bin");
		if (length($ICUROOT) > 0) {
			system("cp -f $ICUROOT/source/tools/makeconv/makeconv $targetdir/bin");
			system("cp -f $ICUROOT/lib/libicu-uc.* $targetdir/lib");
		}
        system("cp -f $XERCESCROOT/lib/*.a $targetdir/lib");
        system("cp -f $XERCESCROOT/lib/*.so $targetdir/lib");
        system("cp -f $XERCESCROOT/lib/*.sl $targetdir/lib");

        system("rm -rf $targetdir/bin/obj");

		if (length($ICUROOT) > 0) {
			# Copy the locale files
			system("cp -f $ICUROOT/data/*.cnv $targetdir/lib/icu/data/");
			system("cp -f $ICUROOT/data/convrtrs.txt $targetdir/lib/icu/data/");
		}

        # Populate the samples directory
        print ("\n\nCopying sample files ...\n");
        foreach $iii ('config.guess', 'config.h.in', 'config.sub', 'configure', 'configure.in',
                'install-sh', 'runConfigure', 'Makefile.in', 'Makefile.incl', 'Makefile') {
                system("cp -f $XERCESCROOT/samples/$iii $targetdir/samples");
        }

        system("cp -Rf $XERCESCROOT/samples/data/* $targetdir/samples/data");
        system("cp -Rf $XERCESCROOT/samples/SAXCount/* $targetdir/samples/SAXCount");
        system("cp -Rf $XERCESCROOT/samples/SAXPrint/* $targetdir/samples/SAXPrint");
        system("cp -Rf $XERCESCROOT/samples/DOMCount/* $targetdir/samples/DOMCount");
        system("cp -Rf $XERCESCROOT/samples/DOMPrint/* $targetdir/samples/DOMPrint");
        system("cp -Rf $XERCESCROOT/samples/Redirect/* $targetdir/samples/Redirect");
        system("cp -Rf $XERCESCROOT/samples/MemParse/* $targetdir/samples/MemParse");
        system("cp -Rf $XERCESCROOT/samples/PParse/* $targetdir/samples/PParse");
        system("cp -Rf $XERCESCROOT/samples/StdInParse/* $targetdir/samples/StdInParse");

        # Populate the docs directory
        print ("\n\nCopying documentation ...\n");
        system("cp -Rf $XERCESCROOT/doc/* $targetdir/doc");
        system("cp -Rf $XERCESCROOT/doc/apiDocs/* $targetdir/doc/apiDocs");
        system("cp $XERCESCROOT/Readme.html $targetdir");
        system("cp $XERCESCROOT/doc/license.html $targetdir");

        # Change the directory permissions
        system ("chmod 644 `find $targetdir -type f`");
        system ("chmod 755 $targetdir/bin/* $targetdir/lib/*.sl $targetdir/lib/*.so $targetdir/lib/*.a");
        system ("chmod +x $targetdir/samples/runConfigure $targetdir/samples/configure $targetdir/samples/install-sh");
        system ("chmod +x $targetdir/samples/config.sub $targetdir/samples/config.guess $targetdir/samples/config.status");
        system ("chmod 755 `find $targetdir -type d`");

        # Now package it all up using tar
        print ("\n\nTARing up all files ...\n");
        chdir ("$targetdir/..");
        $zipname = $targetdir . ".tar";
        $platformzipname = $zipname;

        print ("tar -cvf $platformzipname $zipfiles\n");
        system ("tar -cvf $platformzipname $zipfiles");

        # Finally compress the files
        print ("Compressing $platformzipname ...\n");
        system ("gzip $platformzipname");

}


