State of Xerces Mac OS Port. 7/28/00 by James Berry, Critical Path Software, Inc.

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

Key omissions include:

 - Neither Codewarrior nor Project Builder project files are yet provided, though I've built
   both. My Codewarrior projects are built under the new IDE 4.1 which has not yet been
   publicly released. I'll release Project Builder projects after the Mac OS X public beta
   is released. If you want them now...just email me.

Usage Notes:

 - A major hassle in porting this code to the Mac, and in keeping it up to date, is the
   existance of a number of files with file names greater than 31 characters. Xalan is worse.
   I've written a perl script "ShortenNames.pl" which will (1) copy source to a MacSrc directory
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
     doesn't support files with long names. ProjectBuilder projects may used
     the unadulterated "src" files with long names.

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


James Berry
Critical Path Software, Inc.
jberry@criticalpath.com
