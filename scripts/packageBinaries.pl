#!/usr/local/bin/perl5
#
# The Apache Software License, Version 1.1
#
# Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
# reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#
# 3. The end-user documentation included with the redistribution,
#    if any, must include the following acknowledgment:
#       "This product includes software developed by the
#        Apache Software Foundation (http://www.apache.org/)."
#    Alternately, this acknowledgment may appear in the software itself,
#    if and wherever such third-party acknowledgments normally appear.
#
# 4. The names "Xerces" and "Apache Software Foundation" must
#    not be used to endorse or promote products derived from this
#    software without prior written permission. For written
#    permission, please contact apache\@apache.org.
#
# 5. Products derived from this software may not be called "Apache",
#    nor may "Apache" appear in their name, without prior written
#    permission of the Apache Software Foundation.
#
# THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
# ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
# ====================================================================
#
# This software consists of voluntary contributions made by many
# individuals on behalf of the Apache Software Foundation, and was
# originally based on software copyright (c) 1999, International
# Business Machines, Inc., http://www.ibm.com .  For more information
# on the Apache Software Foundation, please see
# <http://www.apache.org/>.
#

push(@INC, "/home/xml4c/bin", "/home/xml4c/bin/perl/perl-RUN/opt/perl5/lib", "/Development2/cupert/bin/perl/perl-RUN/opt/perl5/lib", "/Development/cupert/usr/local/perl/perl-RUN/opt/perl5/lib");

require "getopt.pl";
require 5.0;

$|=1;   # Force a flush after every print


# Extract the source and target directories
&Getopt('sopcxmntrb');
$XERCESCROOT = $opt_s;
$targetdir = $opt_o;

# Check for the environment variables and exit if error
if (!length($XERCESCROOT) || !length($targetdir) || (length($opt_h) > 0) ) {
    print ("Usage is: packageBinaries <options>\n");
    print ("  where options are:\n");
    print ("    -s <source_directory>\n");
    print ("    -o <target_directory>\n");
    print ("    -c <C compiler name> (e.g. gcc, cc or xlc_r)\n");
    print ("    -x <C++ compiler name> (e.g. g++, CC, aCC, c++, xlC_r, cl, ecl or ecc)\n");
    print ("    -m <message loader> can be 'inmem' \(default\), 'icu' or 'iconv'\n");
    print ("    -n <net accessor> can be 'fileonly' or 'socket' \(default\)\n");
    print ("    -t <transcoder> can be 'icu' or 'native' \(default\)\n");
    print ("    -r <thread option> can be 'pthread' \(default\)or 'dce' (only used on HP-11)\n");
    print ("    -b <bitsToBuild> (accepts '64', '32')\n");
    print ("    -j suppress building of ICU (speeds up builds when debugging)\n");
    print ("    -h to get help on these commands\n\n");
    print ("Example: Under unix's\n");
    print ("    perl packageBinaries.pl -s \$HOME/xerces-c-src2_5_0");
    print (" -o \$HOME/xerces-c2_5_0-linux -c gcc -x g++ -m inmem -n fileonly -t native\n\n");
    print ("Example: Under Windows\n");
    print ("    perl packageBinaries.pl -s \\xerces-c-src2_5_0");
    print (" -o\\xerces-c2_5_0-win32 [-n fileonly] [-t icu]\n\n");
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
$ICU_DATA = $ENV{'ICU_DATA'};
if (($opt_t =~ m/icu/i || $opt_m =~ m/icu/i) && (length($ICUROOT) == 0)) {
    print "You have specified an ICU build but you have not defined your ICU install directory.\n";
    print "To build with ICU, you must set an environment variable called ICUROOT\n";
    print "Cannot proceed any further.\n";
    exit(-1);
}
$ICUIsPresent = (($opt_t =~ m/icu/i || $opt_m =~ m/icu/i) && length($ICUROOT) > 0);

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

# Find out the platform from 'uname -s'
open(PLATFORM, "uname -s|");
$platform = <PLATFORM>;
chomp($platform);
close (PLATFORM);

#

#set platform variable if on windows 64bit
my $return_code = system( "ls" );
if ($return_code != 0)
{  $platform = "win64bit";
}

#Fix the backslashes on the Windows platform
if ($platform ne "win64bit")
{
    $XERCESCROOT =~ s/\\/\//g;
    $ICUROOT =~ s/\\/\//g;
    $targetdir =~ s/\\/\//g;
}

print "Packaging binaries for \`" . $platform . "\` in " . $targetdir . " ...\n";   # "

#Construct the name of the zip file by extracting the last directory name
$zipfiles = $targetdir;
$zipfiles =~ s/.*(\/|\\)([\w-\.]*)$/$2/g;
$zipfiles = $zipfiles . "/*";

$buildmode = "Release";         # Universally, why do you want to package Debug builds anyway?

#
#   Architecture: Itanium
#   OS:           Windows XP
#   Compiler:     Intel's cl and/or ecl
#   cygwin:       Not Available
#
if ($platform eq "win64bit" )
{
    if ($opt_x ne "" && $opt_x ne "cl" && $opt_x ne "ecl")
    {
        print("Compiler on Itanium/WindowsXP must be \'cl\' or \'ecl\'\n");
        exit(-1);
    }

    if ($opt_x eq "")
    {
    	$opt_x = 'cl';
    }

    $platformname = 'Win64';    # Needed this way by nmake

    if (-e "$targetdir.zip") {
        print ("Deleting old target file \'$targetdir.zip\' \n");
        unlink("$targetdir.zip");
    }

    # Make the target directory and its main subdirectories
    psystem ("mkdir $targetdir");
    psystem ("mkdir $targetdir\\bin");
    psystem ("mkdir $targetdir\\lib");
    if ( $opt_m =~ m/icu/i ) {
        psystem ("mkdir $targetdir\\msg");
    }
    psystem ("mkdir $targetdir\\etc");
    psystem ("mkdir $targetdir\\include");
    psystem ("mkdir $targetdir\\include\\xercesc");
    psystem ("mkdir $targetdir\\samples");
    psystem ("mkdir $targetdir\\samples\\Projects");
    psystem ("mkdir $targetdir\\samples\\Projects\\Win32");
    psystem ("mkdir $targetdir\\samples\\Projects\\Win32\\VC6");
    psystem ("mkdir $targetdir\\samples\\data");
    psystem ("mkdir $targetdir\\samples\\SAXCount");
    psystem ("mkdir $targetdir\\samples\\SAX2Count");
    psystem ("mkdir $targetdir\\samples\\SAXPrint");
    psystem ("mkdir $targetdir\\samples\\SAX2Print");
    psystem ("mkdir $targetdir\\samples\\DOMCount");
    psystem ("mkdir $targetdir\\samples\\DOMPrint");
    psystem ("mkdir $targetdir\\samples\\Redirect");
    psystem ("mkdir $targetdir\\samples\\MemParse");
    psystem ("mkdir $targetdir\\samples\\PParse");
    psystem ("mkdir $targetdir\\samples\\StdInParse");
    psystem ("mkdir $targetdir\\samples\\EnumVal");
    psystem ("mkdir $targetdir\\samples\\SEnumVal");
    psystem ("mkdir $targetdir\\samples\\CreateDOMDocument");
    psystem ("mkdir $targetdir\\samples\\PSVIWriter");
    psystem ("mkdir $targetdir\\samples\\SCMPrint");
    psystem ("mkdir $targetdir\\scripts");
    psystem ("mkdir $targetdir\\doc");
    psystem ("mkdir $targetdir\\doc\\html");
    psystem ("mkdir $targetdir\\doc\\html\\apiDocs");

    # If 'FileOnly' NetAccessor has been specified, then the project files have to be
    # changed.
    #REVISIT: we are make from makefile, not from *.dsp
    #         need to look at it later
    if ($opt_n =~ m/fileonly/i) {
        changeWindowsProjectForFileOnlyNA("$XERCESCROOT/Projects/Win32/VC6/xerces-all/XercesLib/XercesLib.dsp");
    }

    #
    #	ICU Build happens here, if one is required.
    #
    #REVISIT: icu
    #
    if ($ICUIsPresent) {

        print ("Building ICU from $ICUROOT ...\n");

        #Clean up all the dependency files, causes problems for nmake
        pchdir ("$ICUROOT");
        psystem ("del /s /f *.dep *.ncb *.plg *.opt");

        # Make the icu dlls
        pchdir ("$ICUROOT\\source\\allinone\\all");
        if (!(length($opt_j) > 0)) {   # Optionally suppress ICU build, to speed up overlong builds while debugging.
 	        #For XP 64 bit, we only ship release dlls
            psystem("nmake -f all_win64_release.mak \"CFG=all - $platformname Release\" CPP=$opt_x.exe >buildlog.txt 2>&1");	    	
	        psystem("type buildlog.txt");
            #psystem("nmake -f all_win64_debug.mak \"CFG=all - $platformname Debug\" CPP=$opt_x.exe >buildlog.txt 2>&1");	    	
	        #psystem("type buildlog.txt");
        }

        $transcoder = 0;
        if ($opt_t =~ m/icu/i )
        {
            $transcoder = 1;
        }

        $msgloader = 0;
        if ($opt_m =~ m/icu/i)
        {
            $msgloader = 1;
        }

        change_windows_makefile_for_ICU("$XERCESCROOT\\Projects\\Win32\\VC6\\xerces-all/XercesLib/XercesLib.mak", $transcoder, $msgloader);
    }

    # Build resource bundle library for ICUMsgLoader
    if ($opt_m =~ m/icu/i) {
        pchdir ("$XERCESCROOT\\src\\xercesc\\util\\MsgLoaders\\ICU\\resources");    	
        psystem( "nmake /f resources.mak > buildlog.txt 2>&1 ");
        system("type buildlog.txt");       
    }
    
    # Clean up all the dependency files, causes problems for nmake
    # Also clean up all MSVC-generated project files that just cache the IDE state
    pchdir ("$XERCESCROOT");
    psystem ("del /s /f *.dep *.ncb *.plg *.opt");
   
    # Make all files in the Xerces-C system including libraries, samples and tests
    pchdir ("$XERCESCROOT\\Projects\\Win32\\VC6\\xerces-all\\all");
    psystem( "nmake -f all.mak \"CFG=all - $platformname Release\" CPP=$opt_x.exe >buildlog.txt 2>&1");
    system("type buildlog.txt");

    #For XP 64 bit, we only ship release dlls
    #pchdir ("$XERCESCROOT\\Projects\\Win32\\VC6\\xerces-all\\XercesLib");
    #psystem("nmake -f XercesLib.mak \"CFG=XercesLib - $platformname Debug\" CPP=$opt_x.exe > buildlog.txt 2>&1 ");
    #system("type buildlog.txt");

    # Decide where you want the build copied from
    pchdir ($targetdir);
    $BUILDDIR = $XERCESCROOT . "\\Build\\Win64\\VC6\\" . $buildmode;
    print "\nBuild is being copied from \'" . $BUILDDIR . "\'";

    if ($opt_m =~ m/icu/i) {
        pchdir ("$XERCESCROOT\\src\\xercesc\\util\\MsgLoaders\\ICU\\resources");    	
        psystem("copy XercesMessages*.DLL $BUILDDIR");
        pchdir ($targetdir);        
    }
    
    # Populate the include output directory
    print ("\n\nCopying headers files ...\n");

    @headerDirectories =
     qw'sax
		sax2
        framework
        framework\\psvi        
        dom
        dom\\deprecated
        internal
        parsers
        util
        util\\Compilers
        util\\MsgLoaders
        util\\MsgLoaders\\ICU
        util\\MsgLoaders\\InMemory
        util\\MsgLoaders\\MsgCatalog
        util\\MsgLoaders\\Win32
        util\\Platforms
        util\\Platforms\\AIX
        util\\Platforms\\HPUX
        util\\Platforms\\BeOS
        util\\Platforms\\Linux
        util\\Platforms\\MacOS
        util\\Platforms\\OS2
        util\\Platforms\\OS390
        util\\Platforms\\PTX
        util\\Platforms\\Solaris
        util\\Platforms\\Tandem
        util\\Platforms\\Win32
        util\\regx
        util\\Transcoders
        util\\Transcoders\\ICU
        util\\Transcoders\\Iconv
        util\\Transcoders\\Win32
        validators
        validators\\common
        validators\\datatype
        validators\\DTD
        validators\\schema
        validators\\schema\\identity';

    foreach $dir (@headerDirectories) {
        $inclDir = "include\\xercesc\\$dir";
        if (! (-e $inclDir)) {
            psystem("mkdir $inclDir");
        }
        $srcDir = "$XERCESCROOT\\src\\xercesc\\$dir";

        # Weed out directories that have no files to copy, to avoid a bunch of
        # warnings from the cp command in the build output.
        opendir(dir, $srcDir);
        @allfiles = readdir(dir);
        closedir(dir);
        foreach $fileKind ("hpp", "c") {
            $matches = grep(/\.$fileKind$/, @allfiles);
            if ($matches > 0) {
                psystem("copy /y $srcDir\\*.$fileKind  $inclDir\\ ");
            }
        }
    }


    #
    #  Remove internal implementation headers from the DOM include directory.
    #
    psystem("del /s /f $targetdir\\include\\xercesc\\dom\\impl");
    psystem("del  $targetdir\\include\\xercesc\\dom\\deprecated\\*Impl.hpp");
    psystem("del  $targetdir\\include\\xercesc\\dom\\deprecated\\DS*.hpp");

    if ($ICUIsPresent) {
        psystem("xcopy /s /y $ICUROOT\\include\\* $targetdir\\include");
    }

    #
    # Populate the binary output directory
    #
    print ("\n \nCopying binary outputs ...\n");
    psystem("copy /y $BUILDDIR\\*.dll $targetdir\\bin");
    psystem("copy /y $BUILDDIR\\*.exe $targetdir\\bin");

    if ($ICUIsPresent) {

        # Copy the ICU dlls and libs
        psystem("copy /y $ICUROOT\\bin\\icuuc26.dll $targetdir\\bin");
        psystem("copy /y $ICUROOT\\bin\\icuuc26d.dll $targetdir\\bin");

        # it seems icudt26*.DLL is generated (upper case dll)
        # but just case, try lower case .dll as well
        psystem("copy /y $ICUROOT\\bin\\icudt26*.DLL $targetdir\\bin");
        psystem("copy /y $ICUROOT\\bin\\icudt26*.dll $targetdir\\bin");

        psystem("copy /y $ICUROOT\\lib\\icudata.lib $targetdir\\lib");

        psystem("copy /y $ICUROOT\\lib\\icuuc.lib $targetdir\\lib");
        psystem("copy /y $ICUROOT\\lib\\icuucd.lib $targetdir\\lib");

        # Copy the Resouce Bundle for ICUMsgLoader
        if ( $opt_m =~ m/icu/i) {
            psystem("copy /y $XERCESCROOT\\src\\xercesc\\util\\MsgLoaders\\ICU\\resources\\XercesMessage*.res $targetdir\\msg");
            psystem("copy /y $XERCESCROOT\\src\\xercesc\\util\\MsgLoaders\\ICU\\resources\\XercesMessages*.DLL $targetdir\\bin");
            psystem("copy /y $XERCESCROOT\\src\\xercesc\\util\\MsgLoaders\\ICU\\resources\\XercesMessages*.lib $targetdir\\lib");
        }        	

    }

    psystem("copy /y $BUILDDIR\\xerces-c_*.lib $targetdir\\lib");

    if ($buildmode ne "Debug") {
        $DEBUGBUILDDIR = "$XERCESCROOT\\Build\\Win64\\VC6\\Debug";
        psystem("copy /y $DEBUGBUILDDIR\\xerces-c_*D.lib $targetdir\\lib");
        psystem("copy /y $DEBUGBUILDDIR\\xerces*D.dll $targetdir\\bin");
    }

    # Populate the etc output directory like config.status and the map file
    print ("\n \nCopying misc output to etc ...\n");
    psystem("copy /y $XERCESCROOT\\Build\\Win64\\VC6\\Release\\obj\\*.map $targetdir\\etc");


    # Populate the samples directory
    print ("\n \nCopying sample files ...\n");
    psystem("copy $XERCESCROOT\\version.incl $targetdir");
    psystem("xcopy /s /y $XERCESCROOT\\samples\\Projects\\Win32\\VC6\\* $targetdir\\samples\\Projects\\Win32\\VC6");

    psystem("copy /y $XERCESCROOT\\samples\\SAXCount\\* $targetdir\\samples\\SAXCount");
    psystem("del /f $targetdir\\samples\\SAXCount\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\SAX2Count\\* $targetdir\\samples\\SAX2Count");
    psystem("del /f $targetdir\\samples\\SAX2Count\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\SAXPrint\\* $targetdir\\samples\\SAXPrint");
    psystem("del /f $targetdir\\samples\\SAXPrint\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\SAX2Print\\* $targetdir\\samples\\SAX2Print");
    psystem("del /f $targetdir\\samples\\SAX2Print\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\DOMCount\\* $targetdir\\samples\\DOMCount");
    psystem("del /f $targetdir\\samples\\DOMCount\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\DOMPrint\\* $targetdir\\samples\\DOMPrint");
    psystem("del /f $targetdir\\samples\\DOMPrint\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\Redirect\\* $targetdir\\samples\\Redirect");
    psystem("del /f $targetdir\\samples\\Redirect\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\MemParse\\* $targetdir\\samples\\MemParse");
    psystem("del /f $targetdir\\samples\\MemParse\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\PParse\\* $targetdir\\samples\\PParse");
    psystem("del /f $targetdir\\samples\\PParse\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\StdInParse\\* $targetdir\\samples\\StdInParse");
    psystem("del /f $targetdir\\samples\\StdInParse\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\EnumVal\\* $targetdir\\samples\\EnumVal");
    psystem("del /f $targetdir\\samples\\EnumVal\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\SEnumVal\\* $targetdir\\samples\\SEnumVal");
    psystem("del /f $targetdir\\samples\\SEnumVal\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\CreateDOMDocument\\* $targetdir\\samples\\CreateDOMDocument");
    psystem("del /f $targetdir\\samples\\CreateDOMDocument\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\PSVIWriter\\* $targetdir\\samples\\PSVIWriter");
    psystem("del /f $targetdir\\samples\\PSVIWriter\\Makefile.in");
    psystem("copy /y $XERCESCROOT\\samples\\SCMPrint\\* $targetdir\\samples\\SCMPrint");
    psystem("del /f $targetdir\\samples\\SCMPrint\\Makefile.in");
        
    psystem("copy /y $XERCESCROOT\\samples\\data\\* $targetdir\\samples\\data");

    # Populate the scripts directory
    print ("\n \nCopying script files ...\n");
    psystem("copy $XERCESCROOT\\scripts\\sanityTest* $targetdir\\scripts");

    # Populate the docs directory
    print ("\n \nCopying documentation ...\n");
    psystem("xcopy /s /y $XERCESCROOT\\doc\\* $targetdir\\doc");
    psystem("copy $XERCESCROOT\\Readme.html $targetdir");
    psystem("copy $XERCESCROOT\\credits.txt $targetdir");
    psystem("copy $XERCESCROOT\\LICENSE.txt $targetdir");

    if ($ICUIsPresent) {
        psystem("copy $XERCESCROOT\\license.html $targetdir");
        psystem("copy $XERCESCROOT\\XLicense.html $targetdir");
    }
    psystem("del /f  $targetdir\\doc\\Doxyfile");
    psystem("del /s /f $targetdir\\doc\\dtd");
    psystem("del /f  $targetdir\\doc\\*.xml");
    psystem("del /f  $targetdir\\doc\\*.ent");
    psystem("del /f  $targetdir\\doc\\*.gif");

    # Now package it all up using ZIP
    pchdir ("$targetdir\\..");
    print ("\n \nZIPping up all files ...\n");
    $zipname = $targetdir . ".zip";
    psystem ("zip -r $zipname $zipfiles");
}
#
#     End of Itanium Builds.

#
#   WINDOWS builds happen here, as long as they
#     aren't using gcc...
#
if ($platform =~ m/Windows/  || ($platform =~ m/CYGWIN/ && !($opt_c =~ m/gcc/))) {

    if ($opt_b eq "64")
    {
        $platformname = 'Win64';
    }
    else
    {
        $platformname = 'Win32';
    }
    if (-e "$targetdir.zip") {
        print ("Deleting old target file \'$targetdir.zip\' \n");
        unlink("$targetdir.zip");
    }

    #
    # AUTODETECT VISUAL STUDIO VERSION
    #

    $msc_response= `cl 2>&1`;

    print "$ENV{PATH}\n";

    chomp($msc_response);
    print "-----------------------\n";
    print "Detected: $msc_response\n";

    $msc_response =~ /Version *([0-9.]+)/;
    $msc_full_version = $1;
    $msc_response =~ /Version *([0-9]+)\./;
    $msc_major_version = $1;
    $msc_response =~ /Version *[0-9]+\.([0-9]+)\./;
    $msc_minor_version = $1;

    print "-----------------------\n";
    print "msc_full_version  =$msc_full_version\n";
    print "msc_major_version =$msc_major_version\n";
    print "msc_minor_version =$msc_minor_version\n";
    print "-----------------------\n";

    $DevStudioVer = "6.0"; #default
    $VCBuildDir = "VC6"; #default
    if ($msc_major_version eq "12") {
        print "Configuring environment for Visual Studio 6.0\n";
        $DevStudioVer = "6.0";
    } elsif ($msc_major_version eq "13") {
        print "Configuring environment for Visual Studio .NET\n";
        $DevStudioVer = "7.0";
        $VCBuildDir = "VC7"; #default
    }
    print "DevStudioVer =$DevStudioVer\n";
    print "VCBuildDir   =$VCBuildDir\n";

    # Make the target directory and its main subdirectories
    psystem ("mkdir $targetdir");
    psystem ("mkdir $targetdir/bin");
    psystem ("mkdir $targetdir/lib");
    if ( $opt_m =~ m/icu/i ) {
        psystem ("mkdir $targetdir/msg");
    }
    psystem ("mkdir $targetdir/etc");
    psystem ("mkdir $targetdir/include");
    psystem ("mkdir $targetdir/include/xercesc");
    psystem ("mkdir $targetdir/samples");
    psystem ("mkdir $targetdir/samples/Projects");
    psystem ("mkdir $targetdir/samples/Projects/Win32");
    if ($DevStudioVer eq "6.0") {
    psystem ("mkdir $targetdir/samples/Projects/Win32/VC6");
    } elsif ($DevStudioVer eq "7.0") {
        psystem ("mkdir $targetdir/samples/Projects/Win32/VC7");
    }
    psystem ("mkdir $targetdir/samples/data");
    psystem ("mkdir $targetdir/samples/SAXCount");
    psystem ("mkdir $targetdir/samples/SAX2Count");
    psystem ("mkdir $targetdir/samples/SAXPrint");
    psystem ("mkdir $targetdir/samples/SAX2Print");
    psystem ("mkdir $targetdir/samples/DOMCount");
    psystem ("mkdir $targetdir/samples/DOMPrint");
    psystem ("mkdir $targetdir/samples/Redirect");
    psystem ("mkdir $targetdir/samples/MemParse");
    psystem ("mkdir $targetdir/samples/PParse");
    psystem ("mkdir $targetdir/samples/StdInParse");
    psystem ("mkdir $targetdir/samples/EnumVal");
    psystem ("mkdir $targetdir/samples/SEnumVal");
    psystem ("mkdir $targetdir/samples/CreateDOMDocument");
    psystem ("mkdir $targetdir/samples/PSVIWriter");    
    psystem ("mkdir $targetdir/samples/SCMPrint");        
    psystem ("mkdir $targetdir/scripts");
    psystem ("mkdir $targetdir/doc");
    psystem ("mkdir $targetdir/doc/html");
    psystem ("mkdir $targetdir/doc/html/apiDocs");

    # If 'FileOnly' NetAccessor has been specified, then the project files have to be
    # changed.
    if ($opt_n =~ m/fileonly/i) {
        if ($DevStudioVer eq "6.0") {
            changeWindowsProjectForFileOnlyNA("$XERCESCROOT/Projects/Win32/VC6/xerces-all/XercesLib/XercesLib.dsp");
        } elsif ($DevStudioVer eq "7.0") {
            changeWindowsProjectForFileOnlyNA_VC7("$XERCESCROOT/Projects/Win32/VC7/xerces-all/XercesLib/XercesLib.vcproj");
        }
    }


    #
    #	ICU Build happens here, if one is required.
    #
    if ($ICUIsPresent) {
        print ("Building ICU from $ICUROOT ...\n");

        #Clean up all the dependency files, causes problems for nmake
        pchdir ("$ICUROOT");
        psystem ("del /s /f *.dep *.ncb *.plg *.opt");

        # Make the icu dlls
        if (!(length($opt_j) > 0)) {   # Optionally suppress ICU build, to speed up overlong builds while debugging.
            #we ship both debug and release dlls
            if ($DevStudioVer eq "6.0") {
                pchdir ("$ICUROOT/source/allinone");
                if ($platformname eq "Win64")
                {
                    psystem("msdev allinone.dsw /MAKE \"all - $platformname Release\" /USEENV /REBUILD /OUT buildlog.txt");
                }
                else
                {
                    psystem("msdev allinone.dsw /MAKE \"all - $platformname Release\" /REBUILD /OUT buildlog.txt");            	
                }
                psystem("type buildlog.txt");
                if ($platformname eq "Win64")
                {
                    psystem("msdev allinone.dsw /MAKE \"all - $platformname Debug\" /USEENV /REBUILD /OUT buildlog.txt");
                }
                else
                {
                    psystem("msdev allinone.dsw /MAKE \"all - $platformname Debug\" /REBUILD /OUT buildlog.txt");            	
                }	
                psystem("type buildlog.txt");
            } elsif ($DevStudioVer eq "7.0") {
                pchdir ("$ICUROOT/source/allinone");

                psystem("devenv /rebuild Release /out buildlog_release.txt /project all allinone.sln");
                psystem("type buildlog_release.txt");

                psystem("devenv /rebuild debug /out buildlog_debug.txt /project all allinone.sln");
                psystem("type buildlog_debug.txt");                            	
            }
        }

        $transcoder = 0;
        if ($opt_t =~ m/icu/i )
        {
            $transcoder = 1;
        }

        $msgloader = 0;
        if ($opt_m =~ m/icu/i)
        {
            $msgloader = 1;
        }

        if ($DevStudioVer eq "6.0") {
            change_windows_project_for_ICU("$XERCESCROOT/Projects/Win32/VC6/xerces-all/XercesLib/XercesLib.dsp", $transcoder , $msgloader);
        } elsif ($DevStudioVer eq "7.0") {
            change_windows_project_for_ICU_VC7("$XERCESCROOT/Projects/Win32/VC7/xerces-all/XercesLib/XercesLib.vcproj", $transcoder , $msgloader);
        }
    }

    # Build resource bundle library for ICUMsgLoader, before XercesLib
    if ($opt_m =~ m/icu/i) {
        pchdir ("$XERCESCROOT/src/xercesc/util/MsgLoaders/ICU/resources");    	
        psystem( "nmake /f resources.mak > buildlog.txt 2>&1 ");
        system("type buildlog.txt");              
    }
    
    # Clean up all the dependency files, causes problems for nmake
    # Also clean up all MSVC-generated project files that just cache the IDE state
    pchdir ("$XERCESCROOT");
    psystem ("del /s /f *.dep *.ncb *.plg *.opt");

    # Make all files in the Xerces-C system including libraries, samples and tests
    pchdir ("$XERCESCROOT/Projects/Win32/$VCBuildDir/xerces-all");
    if ($DevStudioVer eq "6.0") {
        if ($platformname eq "Win64")
        {
            psystem( "msdev xerces-all.dsw /MAKE \"all - $platformname $buildmode\" /USEENV /REBUILD /OUT buildlog.txt");
        }
        else
        {
            psystem( "msdev xerces-all.dsw /MAKE \"all - $platformname $buildmode\" /REBUILD /OUT buildlog.txt");
        }	
    } elsif ($DevStudioVer eq "7.0") {
        psystem( "devenv /rebuild Release /out buildlog.txt /project all xerces-all.sln");
    }
    system("type buildlog.txt");

    # Build the debug xerces dll.  Both debug and release DLLs
    #   are in the standard binary distribution of Xerces.
    if ($buildmode ne "Debug") {
        if ($DevStudioVer eq "6.0") {
            if ($platformname eq "Win64")
            {
                psystem("msdev xerces-all.dsw /MAKE \"XercesLib - $platformname Debug\" /USEENV /REBUILD /OUT buildlog.txt");
            }
            else
            {
                psystem("msdev xerces-all.dsw /MAKE \"XercesLib - $platformname Debug\" /REBUILD /OUT buildlog.txt");        	
            }
        } elsif ($DevStudioVer eq "7.0") {
            psystem( "devenv /rebuild debug /out buildlog.txt /project XercesLib xerces-all.sln");
        }
        system("type buildlog.txt");
    }

    # Decide where you want the build copied from
    pchdir ($targetdir);
    $BUILDDIR = $XERCESCROOT . "/Build/Win32/$VCBuildDir/" . $buildmode;
    print "\nBuild is being copied from \'" . $BUILDDIR . "\'";

    if ($opt_m =~ m/icu/i) {
        pchdir ("$XERCESCROOT/src/xercesc/util/MsgLoaders/ICU/resources");    	
        psystem("cp -fv XercesMessages*.DLL $BUILDDIR");
        psystem("cp -fv XercesMessages*.lib $BUILDDIR");
        pchdir ($targetdir);                
    }
    
    # Populate the include output directory
    print ("\n\nCopying headers files ...\n");

    @headerDirectories =
     qw'sax
		sax2
        framework
        framework/psvi        
        dom
        dom/deprecated
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
        util/Platforms/BeOS
        util/Platforms/Linux
        util/Platforms/MacOS
        util/Platforms/OS2
        util/Platforms/OS390
        util/Platforms/PTX
        util/Platforms/Solaris
        util/Platforms/Tandem
        util/Platforms/Win32
        util/regx
        util/Transcoders
        util/Transcoders/ICU
        util/Transcoders/Iconv
        util/Transcoders/Win32
        validators
        validators/common
        validators/datatype
        validators/DTD
        validators/schema
        validators/schema/identity';

    foreach $dir (@headerDirectories) {
        $inclDir = "include/xercesc/$dir";
        if (! (-e $inclDir)) {
            psystem("mkdir $inclDir");
        }
        $srcDir = "$XERCESCROOT/src/xercesc/$dir";

        # Weed out directories that have no files to copy, to avoid a bunch of
        # warnings from the cp command in the build output.
        opendir(dir, $srcDir);
        @allfiles = readdir(dir);
        closedir(dir);
        foreach $fileKind ("hpp", "c") {
            $matches = grep(/\.$fileKind$/, @allfiles);
            if ($matches > 0) {
                psystem("cp -f $srcDir/*.$fileKind  $inclDir/");
            }
        }
    }


    #
    #  Remove internal implementation headers from the DOM include directory.
    #
    psystem ("rm -rf $targetdir/include/xercesc/dom/impl");
    psystem ("rm  $targetdir/include/xercesc/dom/deprecated/*Impl.hpp");
    psystem ("rm  $targetdir/include/xercesc/dom/deprecated/DS*.hpp");

    if ($ICUIsPresent) {
        psystem("cp -Rfv $ICUROOT/include/* $targetdir/include");
    }

    #
    # Populate the binary output directory
    #
    print ("\n\nCopying binary outputs ...\n");
    psystem("cp -fv $BUILDDIR/*.dll $targetdir/bin");
    psystem("cp -fv $BUILDDIR/*.exe $targetdir/bin");

    if ($ICUIsPresent) {    	
        # Copy the ICU dlls and libs
        psystem("cp -fv $ICUROOT/bin/icuuc26.dll $targetdir/bin");
        psystem("cp -fv $ICUROOT/bin/icuuc26d.dll $targetdir/bin");

        # it seems icudt26*.DLL is generated (upper case dll)
        # but just case, try lower case .dll as well
        psystem("cp -fv $ICUROOT/bin/icudt26*.DLL $targetdir/bin");
        psystem("cp -fv $ICUROOT/bin/icudt26*.dll $targetdir/bin");

        psystem("cp -fv $ICUROOT/lib/icudata.lib $targetdir/lib");

        psystem("cp -fv $ICUROOT/lib/icuuc.lib $targetdir/lib");
        psystem("cp -fv $ICUROOT/lib/icuucd.lib $targetdir/lib");

        # Copy the Resouce Bundle for ICUMsgLoader
        if ( $opt_m =~ m/icu/i) {
            psystem("cp -fv $XERCESCROOT/src/xercesc/util/MsgLoaders/ICU/resources/XercesMessages*.res $targetdir/msg");
            psystem("cp -fv $XERCESCROOT/src/xercesc/util/MsgLoaders/ICU/resources/XercesMessages*.DLL $targetdir/bin");
            psystem("cp -fv $XERCESCROOT/src/xercesc/util/MsgLoaders/ICU/resources/XercesMessages*.lib $targetdir/lib");
        }        	

    }
    psystem("cp -fv $BUILDDIR/xerces-c_*.lib $targetdir/lib");
    if ($buildmode ne "Debug") {
        $DEBUGBUILDDIR = "$XERCESCROOT/Build/Win32/$VCBuildDir/Debug";
        psystem("cp -fv $DEBUGBUILDDIR/xerces-c_*D.lib $targetdir/lib");
        psystem("cp -fv $DEBUGBUILDDIR/xerces-c*D.dll $targetdir/bin");
    }


    # Populate the etc output directory like config.status and the map file
    print ("\n\nCopying misc output to etc ...\n");
    psystem("cp -fv $XERCESCROOT/Build/Win32/$VCBuildDir/Release/obj/*.map $targetdir/etc");


    # Populate the samples directory
    print ("\n\nCopying sample files ...\n");
    psystem("cp $XERCESCROOT/version.incl $targetdir");
    psystem("cp -Rfv $XERCESCROOT/samples/Projects/Win32/$VCBuildDir/* $targetdir/samples/Projects/Win32/$VCBuildDir");

    psystem("cp -Rfv $XERCESCROOT/samples/SAXCount/* $targetdir/samples/SAXCount");
    psystem("rm -f $targetdir/samples/SAXCount/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/SAX2Count/* $targetdir/samples/SAX2Count");
    psystem("rm -f $targetdir/samples/SAX2Count/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/SAXPrint/* $targetdir/samples/SAXPrint");
    psystem("rm -f $targetdir/samples/SAXPrint/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/SAX2Print/* $targetdir/samples/SAX2Print");
    psystem("rm -f $targetdir/samples/SAX2Print/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/DOMCount/* $targetdir/samples/DOMCount");
    psystem("rm -f $targetdir/samples/DOMCount/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/DOMPrint/* $targetdir/samples/DOMPrint");
    psystem("rm -f $targetdir/samples/DOMPrint/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/Redirect/* $targetdir/samples/Redirect");
    psystem("rm -f $targetdir/samples/Redirect/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/MemParse/* $targetdir/samples/MemParse");
    psystem("rm -f $targetdir/samples/MemParse/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/PParse/* $targetdir/samples/PParse");
    psystem("rm -f $targetdir/samples/PParse/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/StdInParse/* $targetdir/samples/StdInParse");
    psystem("rm -f $targetdir/samples/StdInParse/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/EnumVal/* $targetdir/samples/EnumVal");
    psystem("rm -f $targetdir/samples/EnumVal/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/SEnumVal/* $targetdir/samples/SEnumVal");
    psystem("rm -f $targetdir/samples/SEnumVal/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/CreateDOMDocument/* $targetdir/samples/CreateDOMDocument");
    psystem("rm -f $targetdir/samples/CreateDOMDocument/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/PSVIWriter/* $targetdir/samples/PSVIWriter");
    psystem("rm -f $targetdir/samples/PSVIWriter/Makefile");
    psystem("cp -Rfv $XERCESCROOT/samples/SCMPrint/* $targetdir/samples/SCMPrint");
    psystem("rm -f $targetdir/samples/SCMPrint/Makefile");
        
    psystem("cp -Rfv $XERCESCROOT/samples/data/* $targetdir/samples/data");

    # Populate the scripts directory
    print ("\n\nCopying script files ...\n");
    psystem("cp $XERCESCROOT/scripts/sanityTest* $targetdir/scripts");

    # Populate the docs directory
    print ("\n\nCopying documentation ...\n");
    psystem("cp -Rfv $XERCESCROOT/doc/* $targetdir/doc");
    psystem("cp $XERCESCROOT/Readme.html $targetdir");
    psystem("cp $XERCESCROOT/credits.txt $targetdir");
    psystem("cp $XERCESCROOT/LICENSE.txt $targetdir");

    if ($ICUIsPresent) {
        psystem("cp $XERCESCROOT/license.html $targetdir");
        psystem("cp $XERCESCROOT/XLicense.html $targetdir");
    }
    psystem("rm -f $targetdir/doc/Doxyfile");
    psystem("rm -rf $targetdir/doc/style");
    psystem("rm -f $targetdir/doc/*.xml");
    psystem("rm -f $targetdir/doc/*.ent");
    psystem("rm -f $targetdir/doc/*.gif");

    # Now package it all up using ZIP
    pchdir ("$targetdir/..");
    print ("\n\nZIPping up all files ...\n");
    $zipname = $targetdir . ".zip";
    psystem ("zip -r $zipname $zipfiles");
}
#
#     End of Windows Builds.



#
#  UNIX builds happen here ...
#
if ( ($platform =~ m/AIX/i)   || ($platform =~ m/HP-UX/i) || ($platform =~ m/BeOS/i) ||
     ($platform =~ m/SunOS/i) || ($platform =~ m/Linux/i) || ($platform =~ m/ptx/i) ||
	 ($platform =~ m/Darwin/i)  || ($platform =~ m/CYGWIN/ && ($opt_c =~ m/gcc/))) {

    # Echo the current PATH to see what compiler it picks up
    psystem ("echo PATH=$ENV{'PATH'}");

    # Set defaults for platform-specific options.
    if ($platform =~ m/AIX/i) {
        $platform = "aix";
        if ($opt_c eq "") {$opt_c = "xlc_r"; }
        if ($opt_x eq "") {$opt_x = "xlC_r"; }

        $icu_cxxflags = '"-w -O2 -qmaxmem=-1"';
        $icu_cflags = '"-w -O2 -qmaxmem=-1"';

        if ($opt_m =~ m/icu/i) {
        	$ENV{'LIBPATH'}="$ICUROOT/lib:$XERCESCROOT/lib:$ENV{'LIBPATH'}";
        	$ENV{'PATH'}="$ICUROOT/bin:$ENV{'PATH'}";        	
        }
        psystem ("echo LIBPATH=$ENV{'LIBPATH'}");
    }
	#	Mac OS X
    if ($platform =~ m/Darwin/i) {
        $platform = "macosx";
		
		# Set option defaults
        if ($opt_c eq "")	{$opt_c = 'cc'; }
        if ($opt_x eq "")	{$opt_x = 'g++'; }
        if ($opt_n eq "") 	{$opt_n = 'native'; }		# native net accessor
        if ($opt_t eq "")	{$opt_t = 'native'; }		# native transcoder
		
		# Code for mac os specific tools
		if ($TAR eq "")		{ $TAR = 'gnutar'; }
		if ($MAKE eq "")	{ $MAKE = 'make'; }
    }
    if ($platform eq 'HP-UX') {
        # Find out the operating system version from 'uname -r'
        open(OSVERSION, "uname -r|");
        $osversion = <OSVERSION>;
        chomp($osversion);
        close (OSVERSION);
        $platform = 'hp-11' if ($osversion =~ m/11\./);
        $platform = 'hp-10' if ($osversion =~ m/10\./);

        if ($opt_c eq "") {$opt_c = "cc"; }
        if ($opt_x eq "") {
            $opt_x = "CC";
            if ($platform eq "hp-11") {
                $opt_x = "aCC";
            }
        }
        if ($opt_m eq "") {
            $opt_m = "inmem";
        }

        $icu_cxxflags = '"-w +O2 +Ofltacc"';
        $icu_cflags = '"-w +O2 +Ofltacc"';

        if ($opt_m =~ m/icu/i) {
        	$ENV{'SHLIB_PATH'}="$ICUROOT/lib:$XERCESCROOT/lib:$ENV{'SHLIB_PATH'}";
        	$ENV{'PATH'}="$ICUROOT/bin:$ENV{'PATH'}";        	        	
        }

        psystem ("echo SHLIB_PATH=$ENV{'SHLIB_PATH'}");
    }
    if ($platform =~ m/BeOS/i) {
        $platform = "beos";
        if ($opt_c eq "") {$opt_c = "gcc";}
        if ($opt_x eq "") {$opt_x = "g++";}
        $icu_cxxflags = '"-w -O"';
        $icu_cflags = '"-w -O"';
        psystem ("echo LIBRARY_PATH=$ENV{'LIBRARY_PATH'}");
    }
    if ($platform =~ m/Linux/i) {
        $platform = "linux";
        if ($opt_c eq "") {$opt_c = "gcc";}
        if ($opt_x eq "") {$opt_x = "g++";}
        $icu_cxxflags = '"-w -O"';
        $icu_cflags = '"-w -O"';

        if ($opt_m =~ m/icu/i) {
        	$ENV{'LD_LIBRARY_PATH'}="$ICUROOT/lib:$XERCESCROOT/lib:$ENV{'LD_LIBRARY_PATH'}";
        	$ENV{'PATH'}="$ICUROOT/bin:$ENV{'PATH'}";        	        	
        }

        psystem ("echo LD_LIBRARY_PATH=$ENV{'LD_LIBRARY_PATH'}");
    }

    if ($platform =~ m/SunOS/i) {
        $platform = "solaris";
        if ($opt_c eq "") {$opt_c = "cc";}
        if ($opt_x eq "") {$opt_x = "CC";}

        if ($opt_m =~ m/icu/i) {
        	$ENV{'LD_LIBRARY_PATH'}="$ICUROOT/lib:$XERCESCROOT/lib:$ENV{'LD_LIBRARY_PATH'}";
        	$ENV{'PATH'}="$ICUROOT/bin:$ENV{'PATH'}";        	        	
        }
        
        $icu_cxxflags = '"-w -O3"';
        $icu_cflags = '"-w -xO3"'; 

        psystem ("echo LD_LIBRARY_PATH=$ENV{'LD_LIBRARY_PATH'}");
    }

    if ($platform =~ m/ptx/i) {
        # Check if the patches have been applied or not
        $platform = "ptx";
        if (!(-d "$XERCESCROOT/src/xercesc/util/Platforms/PTX")) {
            print ("Error: Could not locate PTX-specific XML4C directory.\n");
            print ("    The PTX-specific patches must be applied to both XML4C and ICU before a build can succeed.\n");
            exit(-1);
        }
        # Generally speaking, ICU must be built, before XML4C can be built, for ptx.
        # If this case causes problems, we can revisit it in the future. Right now,
        # we fail only if ICUROOT is defined but mh-ptx is not present.
        if ($ICUIsPresent) {
            if (!(-e "$ICUROOT/source/config/mh-ptx")) {
                print ("Error: Could not locate PTX-specific ICU files.\n");
                print ("    The PTX-specific patches must be applied to both XML4C and ICU before a build can succeed.\n");
                exit(-1);
            }
        }
        $icu_cxxflags = '"-w -0"';
        $icu_cflags = '"-w -0"';
        
        # XMLINSTALL is a ptx-port-specific variable used for manipulating where the files are installed.
        if (!length($ENV{'XMLINSTALL'})) {
            print ("XMLINSTALL has not been explicitly defined. Setting it to \'$targetdir\'.\n");
            $ENV{'XMLINSTALL'} = $targetdir;
        }
        $XMLINSTALL = $ENV{'XMLINSTALL'};
    }
    if(($platform =~ m/CYGWIN/) && ($opt_c =~ m/gcc/)) {
        $MAKE = "make";
        $platform = "CYGWIN";
    }

    # Set defaults for platform-independent options.
    if ($opt_m eq "")	{$opt_m = "inmem"; }   # In memory  message loader.
    if ($opt_n eq "")	{$opt_n = "socket"; }  # Socket based net accessor.
    if ($opt_t eq "")	{$opt_t = "native"; }  # Native transcoding service.
    if ($opt_b eq "")	{$opt_b = "32"; }      # bitstobuild.
	
	# Set defaults for platform tools
	if ($TAR eq "")		{ $TAR = 'tar'; }
	if ($MAKE eq "")	{ $MAKE = 'gmake'; }

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


    # Make the target directory and its main subdirectories
    psystem ("mkdir $targetdir");
    psystem ("mkdir $targetdir/bin");
    psystem ("mkdir $targetdir/etc");
    psystem ("mkdir $targetdir/lib");
    if ( $opt_m =~ m/iconv/i || $opt_m =~ m/icu/i ) {
        psystem ("mkdir $targetdir/msg");
    }
    psystem ("mkdir $targetdir/include");
    psystem ("mkdir $targetdir/include/xercesc");
    if ($ICUIsPresent) {
        psystem ("mkdir $targetdir/include/unicode");
    }
    psystem ("mkdir $targetdir/include/xercesc/sax");
    psystem ("mkdir $targetdir/include/xercesc/sax2");
    psystem ("mkdir $targetdir/include/xercesc/framework");
    psystem ("mkdir $targetdir/include/xercesc/framework/psvi");    
    psystem ("mkdir $targetdir/include/xercesc/internal");
    psystem ("mkdir $targetdir/include/xercesc/parsers");
    psystem ("mkdir $targetdir/include/xercesc/util");
    psystem ("mkdir $targetdir/include/xercesc/util/Compilers");
    psystem ("mkdir $targetdir/include/xercesc/util/MsgLoaders");
    psystem ("mkdir $targetdir/include/xercesc/util/MsgLoaders/ICU");
    psystem ("mkdir $targetdir/include/xercesc/util/MsgLoaders/InMemory");
    psystem ("mkdir $targetdir/include/xercesc/util/MsgLoaders/MsgCatalog");
    psystem ("mkdir $targetdir/include/xercesc/util/MsgLoaders/Win32");
    psystem ("mkdir $targetdir/include/xercesc/util/Platforms");
    psystem ("mkdir $targetdir/include/xercesc/util/Platforms/AIX");
    psystem ("mkdir $targetdir/include/xercesc/util/Platforms/HPUX");
    psystem ("mkdir $targetdir/include/xercesc/util/Platforms/Linux");
    psystem ("mkdir $targetdir/include/xercesc/util/Platforms/MacOS");
    psystem ("mkdir $targetdir/include/xercesc/util/Platforms/OS2");
    psystem ("mkdir $targetdir/include/xercesc/util/Platforms/OS390");
    psystem ("mkdir $targetdir/include/xercesc/util/Platforms/PTX");
    psystem ("mkdir $targetdir/include/xercesc/util/Platforms/Solaris");
    psystem ("mkdir $targetdir/include/xercesc/util/Platforms/Tandem");
    psystem ("mkdir $targetdir/include/xercesc/util/Platforms/Win32");
    psystem ("mkdir $targetdir/include/xercesc/util/regx");
    psystem ("mkdir $targetdir/include/xercesc/util/Transcoders");
    psystem ("mkdir $targetdir/include/xercesc/util/Transcoders/ICU");
    psystem ("mkdir $targetdir/include/xercesc/util/Transcoders/Iconv");
    psystem ("mkdir $targetdir/include/xercesc/util/Transcoders/Win32");
    psystem ("mkdir $targetdir/include/xercesc/dom");
    psystem ("mkdir $targetdir/include/xercesc/dom/impl");
    psystem ("mkdir $targetdir/include/xercesc/dom/deprecated");
    psystem ("mkdir $targetdir/include/xercesc/validators");
    psystem ("mkdir $targetdir/include/xercesc/validators/common");
    psystem ("mkdir $targetdir/include/xercesc/validators/datatype");
    psystem ("mkdir $targetdir/include/xercesc/validators/DTD");
    psystem ("mkdir $targetdir/include/xercesc/validators/schema");
    psystem ("mkdir $targetdir/include/xercesc/validators/schema/identity");
    psystem ("mkdir $targetdir/samples");
    psystem ("mkdir $targetdir/samples/data");
    psystem ("mkdir $targetdir/samples/SAXCount");
    psystem ("mkdir $targetdir/samples/SAX2Count");
    psystem ("mkdir $targetdir/samples/SAXPrint");
    psystem ("mkdir $targetdir/samples/SAX2Print");
    psystem ("mkdir $targetdir/samples/DOMCount");
    psystem ("mkdir $targetdir/samples/DOMPrint");
    psystem ("mkdir $targetdir/samples/Redirect");
    psystem ("mkdir $targetdir/samples/MemParse");
    psystem ("mkdir $targetdir/samples/PParse");
    psystem ("mkdir $targetdir/samples/StdInParse");
    psystem ("mkdir $targetdir/samples/EnumVal");
    psystem ("mkdir $targetdir/samples/SEnumVal");
    psystem ("mkdir $targetdir/samples/CreateDOMDocument");
    psystem ("mkdir $targetdir/samples/PSVIWriter");    
    psystem ("mkdir $targetdir/samples/SCMPrint");        
    psystem ("mkdir $targetdir/scripts");
    psystem ("mkdir $targetdir/doc");
    psystem ("mkdir $targetdir/doc/html");
    psystem ("mkdir $targetdir/doc/html/apiDocs");

    # Build ICU if needed
    if (($opt_t =~ m/icu/i || $opt_m =~ m/icu/i) && !(length($opt_j) > 0))
    {
        print("\n\nBuild ICU with \'$opt_b\' bit ...\n");

        # First make the ICU files executable
        pchdir ("$ICUROOT/source");
        psystem ("chmod +x configure config.*");
        psystem ("chmod +x install-sh");
        $ENV{'ICU_DATA'} = "$ICUROOT/data";
        if ($platform =~ m/ptx/i) {
            psystem ("chmod +x runConfigureICU");
            psystem ("./runConfigureICU PTX");
        } else {
        # set the 32 bit or 64 bit
            if ($opt_b eq "32") {
                psystem ("CC=$opt_c CXX=$opt_x CXXFLAGS=$icu_cxxflags CFLAGS=$icu_cflags configure --prefix=$ICUROOT --disable-64bit-libs");
            }
            else {
                psystem ("CC=$opt_c CXX=$opt_x CXXFLAGS=$icu_cxxflags CFLAGS=$icu_cflags configure --prefix=$ICUROOT");
            }
        }
        psystem ("$MAKE clean"); # Clean up the build, may want to comment this line out!
        psystem ("rm -f $ICUROOT/data/*.o"); # make clean is not enough
        psystem ("rm -f $ICUROOT/data/*.c"); # same for .c files
        psystem ("$MAKE");       # This will take a long time!
        psystem ("$MAKE install"); # Make this separate since this breaks on Solaris

        # Please check if the following needs any change in Version 1.5
        # For the antiquated CC compiler under HPUX, we need to invoke
        # make one extra time to generate the .cnv files.
        # if ( ($platform =~ m/hp-/i) && ($opt_x eq 'CC') ) {
        #   system ("$MAKE");
        # }
        
        # on AIX, create '.a' version for building libxerces-c
        if ( $platform eq "aix") {
            psystem("ln -s $ICUROOT/lib/libicudata26.1.so $ICUROOT/lib/libicudata26.a");
            psystem("ln -s $ICUROOT/lib/libicuuc26.1.so   $ICUROOT/lib/libicuuc26.a");
        }
        
    }

    # For ptx, ICUROOT must now be set to XMLINSTALL for further work.
    if ($platform =~ m/ptx/i) {
        $ENV{'ICUROOT'} = $ENV{'XMLINSTALL'};
    }

    # make the source files
    print("\n\nBuild the xerces-c library ...\n");
    pchdir ("$XERCESCROOT/src/xercesc");


    psystem ("chmod +x run* con* install-sh");

    if (length($opt_r) > 0) {
        psystem ("./runConfigure -p$platform -c$opt_c -x$opt_x -m$opt_m -n$opt_n -t$opt_t -r$opt_r -b$opt_b");
    } else {
        psystem ("./runConfigure -p$platform -c$opt_c -x$opt_x -m$opt_m -n$opt_n -t$opt_t -b$opt_b");
    }

    psystem ("$MAKE clean");     # May want to comment this line out to speed up
    psystem ("$MAKE");
                
    # Move ICU libs into lib dir, so samples will link.  This matches the structure of
    #   the eventual binary packaging, even though we are doing it in the build directory.
    #
    if ($ICUIsPresent) {
        pchdir ("$XERCESCROOT/lib");

        #
        # copy icudata dll
        # For ICU 2.4:
        # on AIX, it is called libicudata26.1.so
        # on Solaris/Linux, it is called libicudata.so.26.1
        # on HP, it is called libicudata.sl.26.1
        #
        psystem("rm -f libicudata*");
        psystem("cp -f $ICUROOT/lib/libicudata26.1.so .");
        psystem("cp -f $ICUROOT/lib/libicudata.so.26.1 .");
        psystem("cp -f $ICUROOT/lib/libicudata.sl.26.1 .");
        
        psystem("find . -name 'libicudata26.1.so' -exec ln -s {} libicudata.so \\;");
        psystem("find . -name 'libicudata26.1.so' -exec ln -s {} libicudata26.so \\;");

        psystem("find . -name 'libicudata.so.26.1' -exec ln -s {} libicudata.so \\;");
        psystem("find . -name 'libicudata.so.26.1' -exec ln -s {} libicudata.so.26 \\;");

        psystem("find . -name 'libicudata.sl.26.1' -exec ln -s {} libicudata.sl \\;");
        psystem("find . -name 'libicudata.sl.26.1' -exec ln -s {} libicudata.sl.26 \\;");

        #
        # copy icuuc dll
        # on AIX, it is called libicuuc26.1.so
        # on Solaris/Linux, it is called libicuuc.so.26.1
        # on HP, it is called libicuuc.sl.26.1
        #
        psystem("rm -f libicuuc*");
        psystem("cp -f $ICUROOT/lib/libicuuc26.1.so .");
        psystem("cp -f $ICUROOT/lib/libicuuc.so.26.1  .");
        psystem("cp -f $ICUROOT/lib/libicuuc.sl.26.1  .");
        
        psystem("find . -name 'libicuuc26.1.so' -exec ln -s {} libicuuc.so \\;");
        psystem("find . -name 'libicuuc26.1.so' -exec ln -s {} libicuuc26.so \\;");
        
        psystem("find . -name 'libicuuc.so.26.1' -exec ln -s {} libicuuc.so \\;");
        psystem("find . -name 'libicuuc.so.26.1' -exec ln -s {} libicuuc.so.26 \\;");

        psystem("find . -name 'libicuuc.sl.26.1' -exec ln -s {} libicuuc.sl \\;");
        psystem("find . -name 'libicuuc.sl.26.1' -exec ln -s {} libicuuc.sl.26 \\;");
        
        # on AIX, copy '.a' version
        if ( $platform eq "aix") {
            psystem("ln -s libicudata26.so libicudata26.a");
            psystem("ln -s libicuuc26.so   libicuuc26.a");
        }        
    }

    # Now build the samples
    print("\n\nBuild the samples ...\n");
    pchdir ("$XERCESCROOT/samples");
    psystem ("chmod +x run* con* install-sh");
    psystem ("./runConfigure -p$platform -c$opt_c -x$opt_x -b$opt_b");
    psystem ("$MAKE clean");     # May want to comment this line out to speed up
    psystem ("$MAKE");

    # Next build the tests
    print("\n\nBuild the tests ...\n");
    pchdir ("$XERCESCROOT/tests");
    psystem ("chmod +x run* con* install-sh");
    psystem ("./runConfigure -p$platform -c$opt_c -x$opt_x -b$opt_b");
    psystem ("$MAKE clean");     # May want to comment this line out to speed up
    psystem ("$MAKE");

    pchdir ($targetdir);

    # Populate the include output directory
    print ("\n\nCopying headers files ...\n");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/sax/*.hpp $targetdir/include/xercesc/sax");
	psystem("cp -Rf $XERCESCROOT/src/xercesc/sax2/*.hpp $targetdir/include/xercesc/sax2");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/framework/*.hpp $targetdir/include/xercesc/framework");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/framework/psvi/*.hpp $targetdir/include/xercesc/framework/psvi");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/framework/psvi/*.c $targetdir/include/xercesc/framework/psvi");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/dom/*.hpp $targetdir/include/xercesc/dom");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/dom/deprecated/*.hpp $targetdir/include/xercesc/dom/deprecated");

    psystem("cp -Rf $XERCESCROOT/version.incl $targetdir");

    psystem("rm -rf $targetdir/include/xercesc/dom/impl");
    psystem("rm  $targetdir/include/xercesc/dom/deprecated/*Impl.hpp");
    psystem("rm  $targetdir/include/xercesc/dom/deprecated/DS*.hpp");

    psystem("cp -Rf $XERCESCROOT/src/xercesc/internal/*.hpp $targetdir/include/xercesc/internal");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/internal/*.c $targetdir/include/xercesc/internal");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/parsers/*.hpp $targetdir/include/xercesc/parsers");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/parsers/*.c $targetdir/include/xercesc/parsers");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/*.hpp $targetdir/include/xercesc/util");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/*.c $targetdir/include/xercesc/util");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Compilers/*.hpp $targetdir/include/xercesc/util/Compilers");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/MsgLoaders/*.hpp $targetdir/include/xercesc/util/MsgLoaders");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/MsgLoaders/ICU/*.hpp $targetdir/include/xercesc/util/MsgLoaders/ICU");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/MsgLoaders/InMemory/*.hpp $targetdir/include/xercesc/util/MsgLoaders/InMemory");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/MsgLoaders/MsgCatalog/*.hpp $targetdir/include/xercesc/util/MsgLoaders/MsgCatalog");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/MsgLoaders/Win32/*.hpp $targetdir/include/xercesc/util/MsgLoaders/Win32");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Platforms/*.hpp $targetdir/include/xercesc/util/Platforms");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Platforms/AIX/*.hpp $targetdir/include/xercesc/util/Platforms/AIX");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Platforms/HPUX/*.hpp $targetdir/include/xercesc/util/Platforms/HPUX");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Platforms/Linux/*.hpp $targetdir/include/xercesc/util/Platforms/Linux");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Platforms/MacOS/*.hpp $targetdir/include/xercesc/util/Platforms/MacOS");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Platforms/OS2/*.hpp $targetdir/include/xercesc/util/Platforms/OS2");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Platforms/OS390/*.hpp $targetdir/include/xercesc/util/Platforms/OS390");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Platforms/PTX/*.hpp $targetdir/include/xercesc/util/Platforms/PTX");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Platforms/Solaris/*.hpp $targetdir/include/xercesc/util/Platforms/Solaris");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Platforms/Tandem/*.hpp $targetdir/include/xercesc/util/Platforms/Tandem");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Platforms/Win32/*.hpp $targetdir/include/xercesc/util/Platforms/Win32");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/regx/*.hpp $targetdir/include/xercesc/util/regx");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Transcoders/*.hpp $targetdir/include/xercesc/util/Transcoders");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Transcoders/ICU/*.hpp $targetdir/include/xercesc/util/Transcoders/ICU");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Transcoders/Iconv/*.hpp $targetdir/include/xercesc/util/Transcoders/Iconv");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/util/Transcoders/Win32/*.hpp $targetdir/include/xercesc/util/Transcoders/Win32");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/validators/common/*.hpp $targetdir/include/xercesc/validators/common");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/validators/datatype/*.hpp $targetdir/include/xercesc/validators/datatype");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/validators/DTD/*.hpp $targetdir/include/xercesc/validators/DTD");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/validators/schema/*.hpp $targetdir/include/xercesc/validators/schema");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/validators/schema/identity/*.hpp $targetdir/include/xercesc/validators/schema/identity");

    if ($ICUIsPresent) {
        print "\nICU files are being copied from \'$ICUROOT\'";
        psystem("cp -Rf $ICUROOT/include/* $targetdir/include");
    }

    # Populate the binary output directory
    print ("\n\nCopying binary outputs ...\n");
    psystem("cp -Rf $XERCESCROOT/bin/* $targetdir/bin");
    psystem("rm -rf $targetdir/bin/obj");

    # Populate the library output directory
    print ("\n\nCopying library outputs ...\n");
    pchdir ("$targetdir/lib");
    psystem("rm -f libxerces-c* ");

    if ((-e "$XERCESCROOT/lib/libxerces-c.so.25.0" )) {
        psystem("cp -f $XERCESCROOT/lib/libxerces-c.so.25.0 .");
        psystem("ln -s libxerces-c.so.25.0 libxerces-c.so.25 ");
        psystem("ln -s libxerces-c.so.25   libxerces-c.so    ");
    }

    if ((-e "$XERCESCROOT/lib/libxerces-c.sl.25.0" )) {
        psystem("cp -f $XERCESCROOT/lib/libxerces-c.sl.25.0 .");
        psystem("ln -s libxerces-c.sl.25.0 libxerces-c.sl.25 ");
        psystem("ln -s libxerces-c.sl.25   libxerces-c.sl    ");
    }

    if ((-e "$XERCESCROOT/lib/libxerces-c25.0.so" )) {
        psystem("cp -f $XERCESCROOT/lib/libxerces-c25.0.so .");
        psystem("ln -s libxerces-c25.0.so libxerces-c25.so  ");
        psystem("ln -s libxerces-c25.so   libxerces-c.so    ");
    }

    if ((-e "$XERCESCROOT/lib/libxerces-c25.0.a" )) {
        psystem("cp -f $XERCESCROOT/lib/libxerces-c25.0.a . ");
        psystem("ln -s libxerces-c25.0.a  libxerces-c25.a ");
        psystem("ln -s libxerces-c25.a    libxerces-c.a ");         
    }
        
	# Mac OS X
    if ((-e "$XERCESCROOT/lib/libxerces-c.25.0.dylib" )) {
        psystem("cp -f $XERCESCROOT/lib/libxerces-c.25.0.dylib .");
        psystem("ln -s libxerces-c.25.0.dylib libxerces-c.25.dylib ");
        psystem("ln -s libxerces-c.25.dylib   libxerces-c.dylib    ");
    }

    # Populate the Message Catalog Files
    if ( $opt_m =~ m/iconv/i ) {
        print ("\n\nCopying Message Catalog Files ...\n");
        pchdir ("$targetdir/msg");
        psystem("rm -f XercesMessages* ");    	
        psystem("cp -f $XERCESCROOT/msg/XercesMessages*.cat .");
    }

    #
    # Create symbolic link for those ICU libraries
    #
    if ($ICUIsPresent) {
        pchdir ("$targetdir/lib");

        #
        # copy icudata dll
        # on AIX, it is called libicudata26.1.so
        # on Solaris/Linux, it is called libicudata.so.26.1
        # on HP, it is called libicudata.sl.26.1
        #
        psystem("rm -f libicudata*");
        psystem("cp -f $XERCESCROOT/lib/libicudata26.1.so .");
        psystem("cp -f $XERCESCROOT/lib/libicudata.so.26.1 .");
        psystem("cp -f $XERCESCROOT/lib/libicudata.sl.26.1 .");

        psystem("find . -name 'libicudata26.1.so' -exec ln -s {} libicudata.so \\;");
        psystem("find . -name 'libicudata26.1.so' -exec ln -s {} libicudata26.so \\;");

        psystem("find . -name 'libicudata.so.26.1' -exec ln -s {} libicudata.so \\;");
        psystem("find . -name 'libicudata.so.26.1' -exec ln -s {} libicudata.so.26 \\;");

        psystem("find . -name 'libicudata.sl.26.1' -exec ln -s {} libicudata.sl \\;");
        psystem("find . -name 'libicudata.sl.26.1' -exec ln -s {} libicudata.sl.26 \\;");

        #
        # copy icuuc dll
        # on AIX, it is called libicuuc26.1.so
        # on Solaris/Linux, it is called libicuuc.so.26.1
        # on HP, it is called libicuuc.sl.26.1
        #
        psystem("rm -f libicuuc*");
        psystem("cp -f $XERCESCROOT/lib/libicuuc26.1.so .");
        psystem("cp -f $XERCESCROOT/lib/libicuuc.so.26.1  .");
        psystem("cp -f $XERCESCROOT/lib/libicuuc.sl.26.1  .");

        psystem("find . -name 'libicuuc26.1.so' -exec ln -s {} libicuuc.so \\;");
        psystem("find . -name 'libicuuc26.1.so' -exec ln -s {} libicuuc26.so \\;");

        psystem("find . -name 'libicuuc.so.26.1' -exec ln -s {} libicuuc.so \\;");
        psystem("find . -name 'libicuuc.so.26.1' -exec ln -s {} libicuuc.so.26 \\;");

        psystem("find . -name 'libicuuc.sl.26.1' -exec ln -s {} libicuuc.sl \\;");
        psystem("find . -name 'libicuuc.sl.26.1' -exec ln -s {} libicuuc.sl.26 \\;");

        # on AIX, copy '.a' version
        if ( $platform eq "aix") {
            psystem("ln -s libicudata26.so libicudata26.a");
            psystem("ln -s libicuuc26.so   libicuuc26.a");
        }        
                
        # Copy the Resouce Bundle for ICUMsgLoader
        if ( $opt_m =~ m/icu/i) {
            print ("\n\nCopying ICU message bundles ...\n");        	
            psystem("cp -f $XERCESCROOT/msg/XercesMessages*.res $targetdir/msg");
           
            psystem("cp -f $XERCESCROOT/lib/libXercesMessages25.0.so .");
            psystem("find . -name 'libXercesMessages25.0.so' -exec ln -s {} libXercesMessages25.so \\;");
            psystem("find . -name 'libXercesMessages25.so'   -exec ln -s {} libXercesMessages.so \\;");
                    
            psystem("cp -f $XERCESCROOT/lib/libXercesMessages.so.25.0 .");
            psystem("find . -name 'libXercesMessages.so.25.0' -exec ln -s {} libXercesMessages.so.25 \\;");
            psystem("find . -name 'libXercesMessages.so.25'   -exec ln -s {} libXercesMessages.so \\;");
            
            psystem("cp -f $XERCESCROOT/lib/libXercesMessages.sl.25.0 .");
            psystem("find . -name 'libXercesMessages.sl.25.0' -exec ln -s {} libXercesMessages.sl.25 \\;");
            psystem("find . -name 'libXercesMessages.sl.25'   -exec ln -s {} libXercesMessages.sl \\;");            

            # on AIX            
            if ( $platform eq "aix") {
                psystem("cp -f $XERCESCROOT/lib/libXercesMessages25.0.a .");
                psystem("ln -s libXercesMessages25.0.a libXercesMessages25.a ");
                psystem("ln -s libXercesMessages25.a   libXercesMessages.a   ");
            }        
                    
        }        	

    }

    # Populate the etc output directory like config.status and the map file
    print ("\n\nCopying misc output to etc ...\n");
    psystem("cp -Rf $XERCESCROOT/src/xercesc/config.status $targetdir/etc");
    psystem("cp -Rf $XERCESCROOT/obj/*.map $targetdir/etc");

    # Populate the samples directory
    print ("\n\nCopying sample files ...\n");
    foreach $iii ('config.guess', 'config.h.in', 'config.sub', 'configure', 'configure.in',
                  'install-sh', 'runConfigure', 'Makefile.in', 'Makefile.incl', 'Makefile') {
        psystem("cp -f $XERCESCROOT/samples/$iii $targetdir/samples");
    }

    psystem("cp -Rf $XERCESCROOT/samples/data/* $targetdir/samples/data");
    psystem("cp -Rf $XERCESCROOT/samples/SAXCount/* $targetdir/samples/SAXCount");
    psystem("rm -f $targetdir/samples/SAXCount/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/SAX2Count/* $targetdir/samples/SAX2Count");
    psystem("rm -f $targetdir/samples/SAX2Count/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/SAXPrint/* $targetdir/samples/SAXPrint");
    psystem("rm -f $targetdir/samples/SAXPrint/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/SAX2Print/* $targetdir/samples/SAX2Print");
    psystem("rm -f $targetdir/samples/SAX2Print/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/DOMCount/* $targetdir/samples/DOMCount");
    psystem("rm -f $targetdir/samples/DOMCount/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/DOMPrint/* $targetdir/samples/DOMPrint");
    psystem("rm -f $targetdir/samples/DOMPrint/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/Redirect/* $targetdir/samples/Redirect");
    psystem("rm -f $targetdir/samples/Redirect/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/MemParse/* $targetdir/samples/MemParse");
    psystem("rm -f $targetdir/samples/MemParse/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/PParse/* $targetdir/samples/PParse");
    psystem("rm -f $targetdir/samples/PParse/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/StdInParse/* $targetdir/samples/StdInParse");
    psystem("rm -f $targetdir/samples/StdInParse/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/EnumVal/* $targetdir/samples/EnumVal");
    psystem("rm -f $targetdir/samples/EnumVal/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/SEnumVal/* $targetdir/samples/SEnumVal");
    psystem("rm -f $targetdir/samples/SEnumVal/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/CreateDOMDocument/* $targetdir/samples/CreateDOMDocument");
    psystem("rm -f $targetdir/samples/CreateDOMDocument/Makefile");
    psystem("cp -Rf $XERCESCROOT/samples/PSVIWriter/* $targetdir/samples/PSVIWriter");
    psystem("rm -f $targetdir/samples/PSVIWriter/Makefile");    
    psystem("cp -Rf $XERCESCROOT/samples/SCMPrint/* $targetdir/samples/SCMPrint");
    psystem("rm -f $targetdir/samples/SCMPrint/Makefile");        
    psystem("rm -f $targetdir/samples/Makefile");

    # Populate the scripts directory
    print ("\n\nCopying script files ...\n");
    psystem("cp $XERCESCROOT/scripts/sanityTest* $targetdir/scripts");

    # Populate the docs directory
    print ("\n\nCopying documentation ...\n");
    psystem("cp -Rf $XERCESCROOT/doc/* $targetdir/doc");
    psystem("cp $XERCESCROOT/Readme.html $targetdir");
    psystem("cp $XERCESCROOT/credits.txt $targetdir");
    psystem("cp $XERCESCROOT/LICENSE.txt $targetdir");
    if ($ICUIsPresent) {
        psystem("cp $XERCESCROOT/license.html $targetdir");
        psystem("cp $XERCESCROOT/XLicense.html $targetdir");
    }
    psystem("rm -f $targetdir/doc/Doxyfile");
    psystem("rm -rf $targetdir/doc/style");
    psystem("rm -f $targetdir/doc/*.xml");
    psystem("rm -f $targetdir/doc/*.ent");
    psystem("rm -f $targetdir/doc/*.gif");

    # Change the directory permissions
    psystem ("chmod 644 `find $targetdir -type f`");
    psystem ("chmod 755 $targetdir/bin/* $targetdir/lib/*");
    psystem ("chmod +x $targetdir/samples/runConfigure $targetdir/samples/configure $targetdir/samples/install-sh");
    psystem ("chmod +x $targetdir/samples/config.sub $targetdir/samples/config.guess $targetdir/samples/config.status");
    psystem ("chmod 755 `find $targetdir -type d`");

    # Now package it all up using tar
    print ("\n\nTARing up all files ...\n");
    pchdir ("$targetdir/..");
    $zipname = $targetdir . ".tar";
    $platformzipname = $zipname;

    psystem ("$TAR -cvf $platformzipname $zipfiles");

    # Finally compress the files
    print ("Compressing $platformzipname ...\n");
    psystem ("gzip $platformzipname");
}

#
#  psystem subroutine both prints and executes a system command.
#
sub psystem() {
    print("$_[0]\n");
    system($_[0]);
    }


#
#  chdir subroutine both prints and executes a chdir
#
sub pchdir() {
    print("chdir $_[0]\n");
    chdir $_[0];
    }


sub change_windows_project_for_ICU() {
    my ($thefile, $transcoder, $msgloader) = @_;

    print "\nConverting Windows Xerces library project ($thefile) for ICU usage...";
    my $thefiledotbak = $thefile . ".bak";
    rename ($thefile, $thefiledotbak);

    open (FIZZLE, $thefiledotbak);
    open (FIZZLEOUT, ">$thefile");
    while ($line = <FIZZLE>) {
        if ($line =~ /Win32 Debug/) {
            $icuuc = "icuucd";
            }
        if ($line =~ /Win32 Release/) {
            $icuuc = "icuuc";
            }

        $line =~ s[/D "PROJ_XMLPARSER"][/I "$ICUROOT\\include" /D "PROJ_XMLPARSER"];
        $line =~ s[Debug/xerces-c_2D.lib"][Debug/xerces-c_2D.lib" /libpath:"$ICUROOT\\lib" /libpath:"$ICUROOT\\source\\data" /libpath:"$XERCESCROOT\\src\\xercesc\\util\\MsgLoaders\\ICU\\resources"];
        $line =~ s[Release/xerces-c_2.lib"][Release/xerces-c_2.lib" /libpath:"$ICUROOT\\lib" /libpath:"$ICUROOT\\source\\data" /libpath:"$XERCESCROOT\\src\\xercesc\\util\\MsgLoaders\\ICU\\resources"];       
       
        if ($msgloader)
        {
            $line =~ s/user32.lib/user32.lib $icuuc.lib icudata.lib XercesMessages2_5_0.lib/g;
        }        
        elsif ($transcoder)
        {
            $line =~ s/user32.lib/user32.lib $icuuc.lib icudata.lib/g;
        }
        
        if ($transcoder)
        {
            $line =~ s/XML_USE_WIN32_TRANSCODER/XML_USE_ICU_TRANSCODER/g;
            $line =~ s/Transcoders\\Win32\\Win32TransService.cpp/Transcoders\\ICU\\ICUTransService.cpp/g;
            $line =~ s/Transcoders\\Win32\\Win32TransService.hpp/Transcoders\\ICU\\ICUTransService.hpp/g;
        }

        if ($msgloader)
        {
            $line =~ s/XML_USE_WIN32_MSGLOADER/XML_USE_ICU_MESSAGELOADER/g;
            $line =~ s/MsgLoaders\\Win32\\Win32MsgLoader.cpp/MsgLoaders\\ICU\\ICUMsgLoader.cpp/g;
            $line =~ s/MsgLoaders\\Win32\\Win32MsgLoader.hpp/MsgLoaders\\ICU\\ICUMsgLoader.hpp/g;
        }

        print FIZZLEOUT $line;
    }
    close (FIZZLEOUT);
    close (FIZZLE);
    unlink ($thefiledotbak);
}

sub change_windows_makefile_for_ICU() {
    my ($thefile, $transcoder, $msgloader) = @_;
    print "\nConverting Windows Xerces library makefile ($thefile) for ICU usage...";
    my $thefiledotbak = $thefile . ".bak";
    rename ($thefile, $thefiledotbak);

    open (FIZZLE, $thefiledotbak);
    open (FIZZLEOUT, ">$thefile");
    while ($line = <FIZZLE>) {
        if ($line =~ /Win64 Debug/ ){
            $icuuc = "icuucd";
            }
        if ($line =~ /Win64 Release/ ) {
            $icuuc = "icuuc";
        }

        $line =~ s[/D "PROJ_XMLPARSER"][/I "$ICUROOT\\include" /D "PROJ_XMLPARSER"];
        $line =~ s[/machine:IA64][/libpath:"$ICUROOT\\lib" /libpath:"$ICUROOT\\source\\data" /libpath:"$XERCESCROOT\\src\\xercesc\\util\\MsgLoaders\\ICU\\resources" /machine:IA64];

        if ($msgloader)
        {
            $line =~ s/user32.lib/user32.lib $icuuc.lib icudata.lib XercesMessages2_5_0.lib/g;
        }        
        elsif ($transcoder)
        {
            $line =~ s/user32.lib/user32.lib $icuuc.lib icudata.lib/g;
        }
            
        if ($transcoder) {
            $line =~ s/XML_USE_WIN32_TRANSCODER/XML_USE_ICU_TRANSCODER/g;
            $line =~ s/Transcoders\\Win32\\Win32TransService/Transcoders\\ICU\\ICUTransService/g;
            $line =~ s/Win32TransService/ICUTransService/g;
        }

        if ($msgloader)
        {
            $line =~ s/XML_USE_WIN32_MSGLOADER/XML_USE_ICU_MESSAGELOADER/g;
            $line =~ s/MsgLoaders\\Win32\\Win32MsgLoader/MsgLoaders\\ICU\\ICUMsgLoader/g;
            $line =~ s/Win32MsgLoader/ICUMsgLoader/g;
        }

        print FIZZLEOUT $line;
    }
    close (FIZZLEOUT);
    close (FIZZLE);
    unlink ($thefiledotbak);
}

sub change_windows_project_for_ICU_VC7() {
    my ($thefile, $transcoder, $msgloader) = @_;
    print "\nConverting Windows Xerces library project ($thefile) for ICU usage...";
    my $thefiledotbak = $thefile . ".bak";
    rename ($thefile, $thefiledotbak);

    open (FIZZLE, $thefiledotbak);
    open (FIZZLEOUT, ">$thefile");
    while ($line = <FIZZLE>) {
        if ($line =~ /Release\|Win32/) {
            $icuuc = "icuuc";
            }
        if ($line =~ /Debug\|Win32/) {
            $icuuc = "icuucd";
            }
        $line =~ s/AdditionalIncludeDirectories=\"([^"]*)/AdditionalIncludeDirectories=\"$ICUROOT\\include;$1/;
        $line =~ s/AdditionalLibraryDirectories=\"([^"]*)/AdditionalLibraryDirectories=\"$ICUROOT\\lib;$ICUROOT\\source\\data;$XERCESCROOT\\src\\xercesc\\util\\MsgLoaders\\ICU\\resources;$1/;
        
        if ($msgloader)
        {
            $line =~ s/AdditionalDependencies=\"([^"]*)/AdditionalDependencies=\"$icuuc.lib icudata.lib XercesMessages2_5_0.lib $1/;
        }        
        elsif ($transcoder)
        {
            $line =~ s/AdditionalDependencies=\"([^"]*)/AdditionalDependencies=\"$icuuc.lib icudata.lib $1/;
        }

        if ($transcoder) {
            $line =~ s/XML_USE_WIN32_TRANSCODER/XML_USE_ICU_TRANSCODER/g;
            $line =~ s/Transcoders\\Win32\\Win32TransService.cpp/Transcoders\\ICU\\ICUTransService.cpp/g;
            $line =~ s/Transcoders\\Win32\\Win32TransService.hpp/Transcoders\\ICU\\ICUTransService.hpp/g;
        }

        if ($msgloader)
        {
            $line =~ s/XML_USE_WIN32_MSGLOADER/XML_USE_ICU_MESSAGELOADER/g;
            $line =~ s/MsgLoaders\\Win32\\Win32MsgLoader/MsgLoaders\\ICU\\ICUMsgLoader/g;
            $line =~ s/Win32MsgLoader/ICUMsgLoader/g;
        }

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
        $aline =~ s/\/D \"XML_USE_NETACCESSOR_WINSOCK\" //g;  # "
        if ($aline =~ /( )+ws2_32.lib( )*\"/) { # end of line
          $aline =~ s/( )+ws2_32.lib( )*\"/\"/g;
        } else { # beginning or middle of line
          $aline =~ s/ws2_32.lib( )*//g;
        }

        print PROJFILEOUT $aline;
    }
    close (PROJFILEOUT);
    close (PROJFILEIN);
    unlink ($thefiledotbak);
}

sub changeWindowsProjectForFileOnlyNA_VC7() {
    my ($thefile) = @_;
    print "\nConfiguring Xerces library project ($thefile) for FileOnly NetAccessor...\n";
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
        if ($aline =~ /\;XML_USE_NETACCESSOR_WINSOCK/) { # end or middle of line
          $aline =~ s/\;XML_USE_NETACCESSOR_WINSOCK//g;
        } else { # beginning of line
          $aline =~ s/\XML_USE_NETACCESSOR_WINSOCK\;*//g;
        }

        if ($aline =~ /\s+ws2_32\.lib\s*\"/) { # end of line
          $aline =~ s/\s+ws2_32\.lib\s*\"/\"/g;
        } else { # beginning or middle of line
          $aline =~ s/ws2_32\.lib\s*//g;
        }

        print PROJFILEOUT $aline;
    }
    close (PROJFILEOUT);
    close (PROJFILEIN);
    unlink ($thefiledotbak);
}
