#!/usr/local/bin/perl5

push(@INC, "/home/xml4c/bin", "/home/xml4c/bin/perl/perl-RUN/opt/perl5/lib", "/Development2/cupert/bin/perl/perl-RUN/opt/perl5/lib", "/Development/cupert/usr/local/perl/perl-RUN/opt/perl5/lib");

require "getopt.pl";
require 5.0;

$|=1;   # Force a flush after every print


# Extract the source and target directories
&Getopt('sopcxmntr');
$XERCESCROOT = $opt_s;
$targetdir = $opt_o;

# Check for the environment variables and exit if error
if (!length($XERCESCROOT) || !length($targetdir) || (length($opt_h) > 0) ) {
    print ("Usage is: packageBinaries <options>\n");
    print ("  where options are:\n");
    print ("    -s <source_directory>\n");
    print ("    -o <target_directory>\n");
    print ("    -c <C compiler name> (e.g. gcc, cc or xlc_r)\n");
    print ("    -x <C++ compiler name> (e.g. g++, CC, aCC, c++ or xlC_r)\n");
    print ("    -m <message loader> can be 'inmem' \(default\), 'icu' or 'iconv'\n");
    print ("    -n <net accessor> can be 'fileonly' or 'socket' \(default\)\n");
    print ("    -t <transcoder> can be 'icu' or 'native' \(default\)\n");
    print ("    -r <thread option> can be 'pthread' \(default\)or 'dce' (only used on HP-11)\n");
    print ("    -h to get help on these commands\n\n");
    print ("Example: Under unix's\n");
    print ("    perl packageBinaries.pl -s \$HOME/xerces-c-src_1_2_0");
    print (" -o \$HOME/xerces-c_1_2_0-linux -c gcc -x g++ -m inmem -n fileonly -t native\n\n");
    print ("Example: Under Windows\n");
    print ("    perl packageBinaries.pl -s \\xerces-c-src_1_2_0");
    print (" -o\\xerces-c_1_2_0-win32 [-n fileonly] [-t icu]\n\n");
    print ("Note:\n");
    print ("    Under Windows, by default the XercesLib project files is\n");
    print ("    configured to use Win32 resource file based message loader,\n");
    print ("    WinSocket based net accessor and native Win32 based transcoder.\n");
    print ("    The two options mentioned in the example above are the only\n");
    print ("    options relevant under Windows on the command line for this script.\n");
    exit(-1);
}

# Set up the environment variables for ICU
# As of Version 3, ICU is not a required component of XERCES-C
$ICUROOT = $ENV{'ICUROOT'};
if (!length($ICUROOT)) {
    print "You have not defined your ICU install directory.\n";
    print "To build with ICU, you must set an environment variable called ICUROOT\n";
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

$buildmode = "Release";         # Universally, why do you want to package Debug builds anyway?

if ($platform =~ m/Windows/  || $platform =~ m/CYGWIN/) {
    
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
    mkdir ($targetdir . "/samples/EnumVal", "0644");
    mkdir ($targetdir . "/samples/CreateDOMDocument", "0644");
    mkdir ($targetdir . "/doc", "0644");
    mkdir ($targetdir . "/doc/html", "0644");
    mkdir ($targetdir . "/doc/html/apiDocs", "0644");
    
    # If 'FileOnly' NetAccessor has been specified, then the project files have to be
    # changed.
    if ($opt_n =~ m/fileonly/i) {
        changeWindowsProjectForFileOnlyNA("$XERCESCROOT/Projects/Win32/VC6/xerces-all/XercesLib/XercesLib.dsp");
    }
    
    if ($opt_t =~ m/icu/i && length($ICUROOT) > 0) {
        print ("Building ICU from $ICUROOT ...\n");
        
        #Clean up all the dependency files, causes problems for nmake
        chdir ("$ICUROOT");
        system ("del /s /f *.dep *.ncb *.plg *.opt");
        
        # Make the icu dlls
        chdir ("$ICUROOT/source/allinone");
        print "Executing: msdev allinone.dsw /MAKE \"all - $platformname $buildmode\" /REBUILD inside $ICUROOT/source/allinone";
        system("msdev allinone.dsw /MAKE \"all - $platformname $buildmode\" /REBUILD");
        
        change_windows_project_for_ICU("$XERCESCROOT/Projects/Win32/VC6/xerces-all/XercesLib/XercesLib.dsp");
    }

    # Clean up all the dependency files, causes problems for nmake
    # Also clean up all MSVC-generated project files that just cache the IDE state
    chdir ("$XERCESCROOT");
    system ("del /s /f *.dep *.ncb *.plg *.opt");
    
    # Make all files in the Xerces-C system including libraries, samples and tests
    chdir ("$XERCESCROOT/Projects/Win32/VC6/xerces-all");
    $cmd = "msdev xerces-all.dsw /MAKE \"all - $platformname $buildmode\" /REBUILD";
    print "$cmd\n";
    system($cmd);
    
    # Build the debug xerces dll.  Both debug and release DLLs
    #   are in the standard binary distribution of Xerces.
    if ($buildmode ne "Debug") {
        $cmd = "msdev xerces-all.dsw /MAKE \"XercesLib - $platformname Debug\" /REBUILD";
        print "$cmd\n";
        system($cmd);
    }
    
    # Decide where you want the build copied from
    chdir ($targetdir);
    $BUILDDIR = $XERCESCROOT . "/Build/Win32/VC6/" . $buildmode;
    print "\nBuild is being copied from \'" . $BUILDDIR . "\'";
    
    # Populate the include output directory
    print ("\n\nCopying headers files ...\n");
    
    @headerDirectories =
     qw'sax
        framework
        dom
        internal
        parsers
        util
        util/Compilers  
        util/MsgLoaders
        util/MsgLoaders/ICU
        util/MsgLoaders/InMemory
        util/MsgLoaders/MsgCatalog
        util/MsgLoaders/Win32
        util/Platforms
        util/Platforms/AIX
        util/Platforms/HPUX
        util/Platforms/Linux
        util/Platforms/MacOS
        util/Platforms/OS2
        util/Platforms/OS390
        util/Platforms/PTX
        util/Platforms/Solaris
        util/Platforms/Tandem
        util/Platforms/Win32
        util/Transcoders
        util/Transcoders/ICU
        util/Transcoders/Iconv
        util/Transcoders/Win32
        validators
        validators/DTD';
     
    foreach $dir (@headerDirectories) {
        $inclDir = "include/$dir";
        if (! (-e $inclDir)) {
            mkdir($inclDir, "0644");
        }
        $srcDir = "$XERCESCROOT/src/$dir";
        
        # Weed out directories that have no files to copy, to avoid a bunch of
        # warnings from the cp command in the build output.
        opendir(dir, $srcDir);
        @allfiles = readdir(dir);
        closedir(dir);
        foreach $fileKind ("hpp", "c") {
            $matches = grep(/\.$fileKind$/, @allfiles);
            if ($matches > 0) {
                $cmd = "cp -f $srcDir/*.$fileKind  $targetdir/$inclDir";  
                print ($cmd . "\n");  
                system($cmd);  
            }
        }
    }
    
    
    #
    #  Remove internal implementation headers from the DOM include directory.
    #
    system ("rm  $targetdir/include/dom/*Impl.hpp");
    system ("rm  $targetdir/include/dom/DS*.hpp");    
    
    
    if ($opt_t =~ m/icu/i && length($ICUROOT) > 0) {
        system("cp -Rfv $ICUROOT/include/* $targetdir/include");
    }
    
    #
    # Populate the binary output directory
    #
    print ("\n\nCopying binary outputs ...\n");
    system("cp -fv $BUILDDIR/*.dll $targetdir/bin");
    system("cp -fv $BUILDDIR/*.exe $targetdir/bin");
    
    if ($opt_t =~ m/icu/i && length($ICUROOT) > 0) {
        # Copy the ICU dlls
        system("cp -fv $ICUROOT/bin/$buildmode/icuuc.dll $targetdir/bin");
        system("cp -fv $ICUROOT/data/icudata.dll $targetdir/bin");
        # Copy the ICU libs
        system("cp -fv $ICUROOT/lib/$buildmode/icuuc.lib $targetdir/lib");
        system("cp -fv $ICUROOT/data/icudata.lib $targetdir/lib");
    }
    system("cp -fv $BUILDDIR/xerces-c_*.lib $targetdir/lib");
    if ($buildmode ne "Debug") {
        $DEBUGBUILDDIR = "$XERCESCROOT/Build/Win32/VC6/Debug";
        system("cp -fv $DEBUGBUILDDIR/xerces-c_*D.lib $targetdir/lib");
        system("cp -fv $DEBUGBUILDDIR/xerces*D.dll $targetdir/bin");
    }
    
    
    
    # Populate the samples directory
    print ("\n\nCopying sample files ...\n");
    system("cp -Rfv $XERCESCROOT/samples/Projects/* $targetdir/samples/Projects");
    
    system("cp -Rfv $XERCESCROOT/samples/SAXCount/* $targetdir/samples/SAXCount");
    system("rm -f $targetdir/samples/SAXCount/Makefile");
    system("cp -Rfv $XERCESCROOT/samples/SAXPrint/* $targetdir/samples/SAXPrint");
    system("rm -f $targetdir/samples/SAXPrint/Makefile");
    system("cp -Rfv $XERCESCROOT/samples/DOMCount/* $targetdir/samples/DOMCount");
    system("rm -f $targetdir/samples/DOMCount/Makefile");
    system("cp -Rfv $XERCESCROOT/samples/DOMPrint/* $targetdir/samples/DOMPrint");
    system("rm -f $targetdir/samples/DOMPrint/Makefile");
    system("cp -Rfv $XERCESCROOT/samples/Redirect/* $targetdir/samples/Redirect");
    system("rm -f $targetdir/samples/Redirect/Makefile");
    system("cp -Rfv $XERCESCROOT/samples/MemParse/* $targetdir/samples/MemParse");
    system("rm -f $targetdir/samples/MemParse/Makefile");
    system("cp -Rfv $XERCESCROOT/samples/PParse/* $targetdir/samples/PParse");
    system("rm -f $targetdir/samples/PParse/Makefile");
    system("cp -Rfv $XERCESCROOT/samples/StdInParse/* $targetdir/samples/StdInParse");
    system("rm -f $targetdir/samples/StdInParse/Makefile");
    system("cp -Rfv $XERCESCROOT/samples/EnumVal/* $targetdir/samples/EnumVal");
    system("rm -f $targetdir/samples/EnumVal/Makefile");
    system("cp -Rfv $XERCESCROOT/samples/CreateDOMDocument/* $targetdir/samples/CreateDOMDocument");
    system("rm -f $targetdir/samples/CreateDOMDocument/Makefile");
    
    system("cp -Rfv $XERCESCROOT/samples/data/* $targetdir/samples/data");
    
    # Populate the docs directory
    print ("\n\nCopying documentation ...\n");
    system("cp -Rfv $XERCESCROOT/doc/* $targetdir/doc");
    # system("cp -Rfv $XERCESCROOT/doc/html/apiDocs/* $targetdir/doc/html/apiDocs");
    system("cp $XERCESCROOT/Readme.html $targetdir");
    system("cp $XERCESCROOT/credits.txt $targetdir");   
    system("cp $XERCESCROOT/LICENSE.txt $targetdir");
    if (length($ICUROOT) > 0) {
        system("cp $XERCESCROOT/license.html $targetdir");
        system("cp $XERCESCROOT/license-IBM-public-source.html $targetdir");
    }
    system("rm -f $targetdir/doc/*.xml");
    system("rm -f $targetdir/doc/*.ent");
    system("rm -f $targetdir/doc/*.gif");
    
    # Now package it all up using ZIP
    chdir ("$targetdir/..");
    print ("\n\nZIPping up all files ...\n");
    $zipname = $targetdir . ".zip";
    print ("zip -r $zipname $zipfiles");
    system ("zip -r $zipname $zipfiles");
}
  

if ( ($platform =~ m/AIX/i)    || ($platform =~ m/HP-UX/i) ||
     ($platform =~ m/SunOS/i) || ($platform =~ m/Linux/i) || ($platform =~ m/ptx/i) ) {

    # Decide on the platform specific stuff first
    if ($platform =~ m/AIX/i) {
        $icuCompileFlags = 'CXX="xlC_r -L/usr/lpp/xlC/lib" CC="xlc_r -L/usr/lpp/xlC/lib" C_FLAGS="-w -O" CXX_FLAGS="-w -O"';
    }
    if ($platform eq 'HP-UX') {
        if ($opt_x eq 'CC') {
            $icuCompileFlags = 'CC=cc CXX=CC CXXFLAGS="+eh +DAportable -w -O" CFLAGS="+DAportable -w -O"';
        } else {
            $icuCompileFlags = 'CC=cc CXX=aCC CXXFLAGS="+DAportable -w -O" CFLAGS="+DAportable -w -O"';
        }
        # Find out the operating system version from 'uname -r'
        open(OSVERSION, "uname -r|");
        $osversion = <OSVERSION>;
        chomp($osversion);
        close (OSVERSION);
        $platform = 'hp-11' if ($osversion =~ m/11\./);
        $platform = 'hp-10' if ($osversion =~ m/10\./);
        $opt_r = 'dce' if ($opt_r eq ''); # By default, use dce threads if not specified
        
    }
    if ($platform =~ m/Linux/i) {
        $icuCompileFlags = 'CC=gcc CXX=g++ CXXFLAGS="-w -O" CFLAGS="-w -O"';
        $platform =~ tr/A-Z/a-z/;
    }
    if ($platform =~ m/SunOS/i) {
        $icuCompileFlags = 'CC=cc CXX=CC CXXFLAGS="-w -O" CFLAGS="-w -O"';
    }
    if ($platform =~ m/ptx/i) {
        # Check if the patches have been applied or not
        if (!(-d "$XERCESCROOT/src/util/Platforms/PTX")) {
            print ("Error: Could not locate PTX-specific XML4C directory.\n");
            print ("    The PTX-specific patches must be applied to both XML4C and ICU before a build can succeed.\n");
            exit(-1);
        }
        # Generally speaking, ICU must be built, before XML4C can be built, for ptx.
        # If this case causes problems, we can revisit it in the future. Right now,
        # we fail only if ICUROOT is defined but mh-ptx is not present.
        if (length($ICUROOT)) {
            if (!(-e "$ICUROOT/source/config/mh-ptx")) {
                print ("Error: Could not locate PTX-specific ICU files.\n");
                print ("    The PTX-specific patches must be applied to both XML4C and ICU before a build can succeed.\n");
                exit(-1);
            }
        }
        $icuCompileFlags = 'CC=cc CXX=c++ CXXFLAGS="-w -0" CFLAGS="-w -0"';
        # XMLINSTALL is a ptx-port-specific variable used for manipulating where the files are installed.
        if (!length($ENV{'XMLINSTALL'})) {
            print ("XMLINSTALL has not been explicitly defined. Setting it to \'$targetdir\'.\n");
            $ENV{'XMLINSTALL'} = $targetdir;
        }
        $XMLINSTALL = $ENV{'XMLINSTALL'};
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
        system ("mkdir $targetdir/include/unicode");
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
    system ("mkdir $targetdir/include/util/Platforms/PTX");
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
    system ("mkdir $targetdir/samples/EnumVal");
    system ("mkdir $targetdir/samples/CreateDOMDocument");
    system ("mkdir $targetdir/doc");
    system ("mkdir $targetdir/doc/html");
    system ("mkdir $targetdir/doc/html/apiDocs");
    
    # Build ICU if needed
    if (length($ICUROOT) > 0) {
        # First make the ICU files
        chdir ("$ICUROOT/source");
        system ("chmod 777 configure");
        system ("chmod 777 install-sh");
        if ($platform =~ m/ptx/i) {
            system ("chmod 777 runConfigureICU");
            system ("runConfigureICU PTX");
        } else {
            print ("$icuCompileFlags configure --prefix=$ICUROOT\n");
            system ("$icuCompileFlags configure --prefix=$ICUROOT");
        }
        system ("gmake clean"); # Clean up the build, may want to comment this line out!
        system ("rm -f $ICUROOT/data/*.o"); # gmake clean is not enough
        system ("rm -f $ICUROOT/data/*.c"); # same for .c files
        system ("gmake");       # This will take a long time!
        system ("gmake install"); # Make this separate since this breaks on Solaris
        
        # Please check if the following needs any change in Version 1.4
        # For the antiquated CC compiler under HPUX, we need to invoke
        # gmake one extra time to generate the .cnv files.
        # if ( ($platform =~ m/hp-/i) && ($opt_x eq 'CC') ) {
        #   system ("gmake");
        # }
    }
    
    # For ptx, ICUROOT must now be set to XMLINSTALL for further work.
    if ($platform =~ m/ptx/i) {
        $ENV{'ICUROOT'} = $ENV{'XMLINSTALL'};
    }
    
    # make the source files
    chdir ("$XERCESCROOT/src");
    
    if ( $platform =~ m/sunos/i ) {
        $platform = "solaris";
    }
    if ( $platform =~ m/AIX/i ) {
        $platform = "aix";
    }
    if ( $platform =~ m/ptx/i ) {
        $platform = "ptx";
    }
    
    system ("chmod +x run* con* install-sh");
    
    if (length($opt_r) > 0) {
        system ("runConfigure -p$platform -c$opt_c -x$opt_x -m$opt_m -n$opt_n -t$opt_t -r$opt_r");
    } else {
        system ("runConfigure -p$platform -c$opt_c -x$opt_x -m$opt_m -n$opt_n -t$opt_t");
    }
    
    system ("gmake clean");     # May want to comment this line out to speed up
    system ("gmake");
    
    # Now build the samples
    chdir ("$XERCESCROOT/samples");
    system ("chmod +x run* con* install-sh");
    system ("runConfigure -p$platform -c$opt_c -x$opt_x");
    system ("gmake clean");     # May want to comment this line out to speed up
    system ("gmake");
    
    # Next build the tests
    chdir ("$XERCESCROOT/tests");
    system ("chmod +x run* con* install-sh");
    system ("runConfigure -p$platform -c$opt_c -x$opt_x");
    system ("gmake clean");     # May want to comment this line out to speed up
    system ("gmake");
    
    chdir ($targetdir);
    
    # Populate the include output directory
    print ("\n\nCopying headers files ...\n");
    system("cp -Rf $XERCESCROOT/src/sax/*.hpp $targetdir/include/sax");
    system("cp -Rf $XERCESCROOT/src/framework/*.hpp $targetdir/include/framework");
    system("cp -Rf $XERCESCROOT/src/dom/D*.hpp $targetdir/include/dom");
    
    system("rm -f $targetdir/include/dom/*Impl.hpp");
    system("rm -f $targetdir/include/dom/DS*.hpp");
    system("cp -Rf $XERCESCROOT/src/internal/*.hpp $targetdir/include/internal");
    system("cp -Rf $XERCESCROOT/src/internal/*.c $targetdir/include/internal");
    system("cp -Rf $XERCESCROOT/src/parsers/*.hpp $targetdir/include/parsers");
    system("cp -Rf $XERCESCROOT/src/parsers/*.c $targetdir/include/parsers");
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
    system("cp -Rf $XERCESCROOT/src/util/Platforms/PTX/*.hpp $targetdir/include/util/Platforms/PTX");
    system("cp -Rf $XERCESCROOT/src/util/Platforms/Solaris/*.hpp $targetdir/include/util/Platforms/Solaris");
    system("cp -Rf $XERCESCROOT/src/util/Platforms/Tandem/*.hpp $targetdir/include/util/Platforms/Tandem");
    system("cp -Rf $XERCESCROOT/src/util/Platforms/Win32/*.hpp $targetdir/include/util/Platforms/Win32");
    system("cp -Rf $XERCESCROOT/src/util/Transcoders/*.hpp $targetdir/include/util/Transcoders");
    system("cp -Rf $XERCESCROOT/src/util/Transcoders/ICU/*.hpp $targetdir/include/util/Transcoders/ICU");
    system("cp -Rf $XERCESCROOT/src/util/Transcoders/Iconv/*.hpp $targetdir/include/util/Transcoders/Iconv");
    system("cp -Rf $XERCESCROOT/src/util/Transcoders/Win32/*.hpp $targetdir/include/util/Transcoders/Win32");
    system("cp -Rf $XERCESCROOT/src/validators/DTD/*.hpp $targetdir/include/validators/DTD");
    
    if (length($ICUROOT) > 0) {
        print "\nICU files are being copied from \'" . $ICUROOT . "\'";
        system("cp -Rf $ICUROOT/include/* $targetdir/include");
    }
    
    # Populate the binary output directory
    print ("\n\nCopying binary outputs ...\n");
    system("cp -Rf $XERCESCROOT/bin/* $targetdir/bin");
    if (length($ICUROOT) > 0) {
        system("cp -f $ICUROOT/lib/libicu-uc.* $targetdir/lib");
        system("cp -f $ICUROOT/data/libicudata.* $targetdir/lib");
    }
    system("cp -f $XERCESCROOT/lib/*.a $targetdir/lib");
    system("cp -f $XERCESCROOT/lib/*.so $targetdir/lib");
    system("cp -f $XERCESCROOT/lib/*.sl $targetdir/lib");
    
    system("rm -rf $targetdir/bin/obj");
    
    # Populate the samples directory
    print ("\n\nCopying sample files ...\n");
    foreach $iii ('config.guess', 'config.h.in', 'config.sub', 'configure', 'configure.in',
                  'install-sh', 'runConfigure', 'Makefile.in', 'Makefile.incl', 'Makefile') {
        system("cp -f $XERCESCROOT/samples/$iii $targetdir/samples");
    }
    
    system("cp -Rf $XERCESCROOT/samples/data/* $targetdir/samples/data");
    system("cp -Rf $XERCESCROOT/samples/SAXCount/* $targetdir/samples/SAXCount");
    system("rm -f $targetdir/samples/SAXCount/Makefile");
    system("cp -Rf $XERCESCROOT/samples/SAXPrint/* $targetdir/samples/SAXPrint");
    system("rm -f $targetdir/samples/SAXPrint/Makefile");
    system("cp -Rf $XERCESCROOT/samples/DOMCount/* $targetdir/samples/DOMCount");
    system("rm -f $targetdir/samples/DOMCount/Makefile");
    system("cp -Rf $XERCESCROOT/samples/DOMPrint/* $targetdir/samples/DOMPrint");
    system("rm -f $targetdir/samples/DOMPrint/Makefile");
    system("cp -Rf $XERCESCROOT/samples/Redirect/* $targetdir/samples/Redirect");
    system("rm -f $targetdir/samples/Redirect/Makefile");
    system("cp -Rf $XERCESCROOT/samples/MemParse/* $targetdir/samples/MemParse");
    system("rm -f $targetdir/samples/MemParse/Makefile");
    system("cp -Rf $XERCESCROOT/samples/PParse/* $targetdir/samples/PParse");
    system("rm -f $targetdir/samples/PParse/Makefile");
    system("cp -Rf $XERCESCROOT/samples/StdInParse/* $targetdir/samples/StdInParse");
    system("rm -f $targetdir/samples/StdInParse/Makefile");
    system("cp -Rf $XERCESCROOT/samples/EnumVal/* $targetdir/samples/EnumVal");
    system("rm -f $targetdir/samples/EnumVal/Makefile");
    system("cp -Rf $XERCESCROOT/samples/CreateDOMDocument/* $targetdir/samples/CreateDOMDocument");
    system("rm -f $targetdir/samples/CreateDOMDocument/Makefile");
    system("rm -f $targetdir/samples/Makefile");
    
    # Populate the docs directory
    print ("\n\nCopying documentation ...\n");
    system("cp -Rf $XERCESCROOT/doc/* $targetdir/doc");
    # system("cp -Rf $XERCESCROOT/doc/html/apiDocs/* $targetdir/doc/html/apiDocs");
    system("cp $XERCESCROOT/Readme.html $targetdir");
    system("cp $XERCESCROOT/credits.txt $targetdir");   
    system("cp $XERCESCROOT/LICENSE.txt $targetdir");
    if (length($ICUROOT) > 0) {
        system("cp $XERCESCROOT/license.html $targetdir");
        system("cp $XERCESCROOT/license-IBM-public-source.html $targetdir");
    }
    system("rm -f $targetdir/doc/*.xml");
    system("rm -f $targetdir/doc/*.ent");
    system("rm -f $targetdir/doc/*.gif");
    
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
  
    
sub change_windows_project_for_ICU() {
    my ($thefile) = @_;
    print "\nConverting Windows Xerces library project ($thefile) for ICU usage...";
    my $thefiledotbak = $thefile . ".bak";
    rename ($thefile, $thefiledotbak);
    
    open (FIZZLE, $thefiledotbak);
    open (FIZZLEOUT, ">$thefile");
    while ($line = <FIZZLE>) {
        $line =~ s/\/D "PROJ_XMLPARSER"/\/I \"\.\.\\\.\.\\\.\.\\\.\.\\\.\.\\\.\.\\icu\\include" \/D "PROJ_XMLPARSER"/g;
        $line =~ s/XML_USE_WIN32_TRANSCODER/XML_USE_ICU_TRANSCODER/g;
        $line =~ s/Release\/xerces-c_1.lib"/Release\/xerces-c_1.lib" \/libpath:"\.\.\\\.\.\\\.\.\\\.\.\\\.\.\\\.\.\\icu\\lib\\Release" \/libpath:"\.\.\\\.\.\\\.\.\\\.\.\\\.\.\\\.\.\\icu\\data"/g;
        $line =~ s/Debug\/xerces-c_1D.lib"/Debug\/xerces-c_1D.lib" \/libpath:"\.\.\\\.\.\\\.\.\\\.\.\\\.\.\\\.\.\\icu\\lib\\Debug" \/libpath:"\.\.\\\.\.\\\.\.\\\.\.\\\.\.\\\.\.\\icu\\data"/g;
        $line =~ s/user32\.lib/user32\.lib icuuc\.lib icudata\.lib/g;
        $line =~ s/Transcoders\\Win32\\Win32TransService\.cpp/Transcoders\\ICU\\ICUTransService\.cpp/g;
        $line =~ s/Transcoders\\Win32\\Win32TransService\.hpp/Transcoders\\ICU\\ICUTransService\.hpp/g;
        print FIZZLEOUT $line;
    }
    close (FIZZLEOUT);
    close (FIZZLE);
    unlink ($thefiledotbak);
}
  
      
#
# This subroutine is used to munge the XercesLib project file to remove all
# traces of WinSock based NetAccessor. Once no NetAccessor is specified, the
# project file is configured for using the 'FileOnly' NetAccessor.
#
# For this function to work the assumption is that project file in CMVC is
# preconfigured to already use the WinSock based NetAccessor. So, the changes
# that need to be done are:
#   - to remove references to any #defines
#   - to remove references to wsock32.lib
#   - to remove references to the source files for the WinSock based NetAccessor.
#

sub changeWindowsProjectForFileOnlyNA() {
    my ($thefile) = @_;
    print "\nConfiguring Xerces library project ($thefile) for FileOnly NetAccessor...";
    my $thefiledotbak = $thefile . ".bak";
    rename ($thefile, $thefiledotbak);
    
    open (PROJFILEIN, $thefiledotbak);
    open (PROJFILEOUT, ">$thefile");
    
    while ($aline = <PROJFILEIN>) {
        # By skipping over lines between the NetAccessors group
        # we can references to the WinSock based NetAccessor files.
        if ($aline =~ m/^# Begin Group \"NetAccessors\"/g) {
            # ...found it. Write out the line as a place holder. Also...
            print PROJFILEOUT $aline;
            # ...preserve the next two lines.
            $aline = <PROJFILEIN>;
            print PROJFILEOUT $aline;
            $aline = <PROJFILEIN>;
            print PROJFILEOUT $aline;
            # Skip over the lines till you hit the WinSock NetAccessor 'End Group'.
            while ($aline = <PROJFILEIN>) { # read the next line
                last if ($aline =~ m/^# End Group/g);
            }
            # We need to preserve the 'End Group' line. The last statement of the
            # enclosing while loop prints it out.
        }
        
        # From the remaining lines, remove any references to the #defines and
        # the WinSock library.
        $aline =~ s/\/D \"XML_USE_NETACCESSOR_WINSOCK\" //g;
        $aline =~ s/wsock32.lib //g;
        
        print PROJFILEOUT $aline;
    }
    close (PROJFILEOUT);
    close (PROJFILEIN);
    unlink ($thefiledotbak);
}
