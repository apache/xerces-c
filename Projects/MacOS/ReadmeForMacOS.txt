$Log$
Revision 1.4  2000/08/03 21:16:57  jberry
Modify readme file to reflect latest changes: new project files



State of Xerces Mac OS Port. By James Berry, Critical Path Software, Inc.

This readme attempts to document the state of Mac OS support for Xerces.
I suspect that this code has been ported to the Mac on a number of occasions.
My work on Xerces is derived in small part from a previous port by jbellardo@calpoly.edu.

This port aims to bring Xerces to Mac OS, and to use Mac OS services as appropriate.

Key work in these areas is:

 - Development of a transcoder that relies on Mac OS's Unicode Converter. [MacOSUnicodeConverter].

 - Development of a netaccessor that relies on the Mac OS's URL Access routines.
   [MacOSURLAccess].

 - Near-complete rewrite of Mac OS Platform utilities [MacOSPlatformUtils] to support Mac OS
   constructs and the latest changes in Xerces. All file access is done directly on top of the
   Mac OS file system and in fact uses the new FSRef calls and long unicode names
   whereever possible.

 - Code supports both Carbon and Classic APIs.

 - Detection of and support for Codewarrior and Project Builder.
 
 - A project file that builds a static version of the Xerces library for CodeWarrior release 6
   has been checked in. Individual build targets are for classic, carbon, and debug/non-debug
   cases.
   
 - A project file for the DOMPrint sample that may be used as an example to build additional
   samples.

Omissions include:

 - Missing project files for additional sample programs
 
Usage Notes:

 - A major hassle in porting this code to the Mac, and in keeping it up to date, is the
   existance of a number of files with file names greater than 31 characters. Xalan is worse.
   I've written a perl script "ShortenNames.pl" that will (1) copy source to a MacSrc directory
   (2) shorten file names using appropriate hints, and (3) update internal usage of these
   names (includes). To use this:

   - Check out the code somewhere where you've got long names [unix system?]
   - Move ShortenNames.pl into the "c" directory, at the same level as the
     "src" directory.
   - Invoke the script using perl ShortenNames.pl.
   - This (should) create a new directory called MacSrc which is a duplicate of
     the "src" directory but with names shortened appropriately. This has been
     tested on Mac OS and Mac OS X, but in no other environments. YMMV.
   - The files in MacSrc should be used from the Codewarrior environment, which
     doesn't support files with long names. ProjectBuilder projects may/should use
     the unadulterated "src" files with long names.
   - Note that you should delete any previous MacSrc before running ShortenNames.pl.
     It doesn't currently cope well with existing directories/files.

   Note that I'll ascribe most of the blame for the problem to Apple and other Mac tools
   vendors. HFS+ fully supports long file names: it's too bad that Apple (Finder),
   Aladdin (StuffIt), Metrowerks (CodeWarrior) and others (MacCVS) haven't chosen to support
   long names within their products. I actually spent several days on the MacCVS end of this
   task before deciding it was too hard given the timeframe with OS X almost here.
   The situation under Mac OS X is much better: both the "finder" and development tools
   support files with long names.

 - The Mac OS Platform utilities expect that pathnames will be in unix-form with '/' separators.
   There is no support for Mac OS style pathnames with colons. Routines are provided to
   convert back and forth between this style of name and FSRefs or FSSpecs. See
   [MacOSPlatformUtils.hpp]. The choice to use unix-paths only was based on the idea that
   any XML that has external references is likely to have them in unix form, so we have to
   be able to support unix paths. To also support mac paths seems superfluous.

 - I have not brought forward the previous port's rather bizzare support for file sources
   that read from resources. If you want to read from a resource just read it into memory,
   lock it down, and set up a memory based stream to parse through it.

 - If you're building this code, make sure you include the MemoryBased messaged loader.
   There is no Mac specific message loader for now.
   
 - CodeWarrior projects files have been checked in in an exported XML form. Not only does
   this show off XML but it also allows for better cvs revision control and tracking of
   changes to the project files. After checking out a project file, you'll need to "import"
   it into CodeWarrior. Likewise, only an exported version of the project file should be
   committed to cvs.
   
 - The CodeWarrior project file(s) for the samples currently require that you make a minor
   modification to your CodeWarrior runtime, in order to support argc/argv on the Mac
   without modifying the sample files. See the usage notes in
   XercesSampleSupport/XercesSampleStartupFragment.c.


James Berry
Critical Path Software, Inc.
jberry@criticalpath.com
