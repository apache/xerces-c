/****************************************************************************
 * Create directories and package binaries for Xerces-C OS/2 port
 */

IF LoadRexxUtil() THEN EXIT

sourcedir  = ''
targetdir  = ''
msgloader  = 'InMemory'
transcoder = 'Iconv'

argv = cli(arg(1))
DO i = 1 TO argv~items
  SELECT
    WHEN argv[i] = "-s" THEN
     DO
      i = i + 1
      sourcedir = argv[i]
     END

    WHEN argv[i] = "-o" THEN
     DO
      i = i + 1
      targetdir = argv[i]
     END

    WHEN argv[i] = "-t" THEN
     DO
      i = i + 1
      transcoder = argv[i]
     END

    WHEN argv[i] = "-m" THEN
     DO
      i = i + 1
      msgloader = argv[i]
     END

    WHEN argv[i] = "-h" THEN
     DO
      CALL Usage
      EXIT -1
     END

    OTHERWISE
      NOP
  END
END

IF targetdir = '' | sourcedir = '' THEN
 DO
  CALL Usage
  EXIT -1
 END

CALL MakeTargetDirectories targetdir
CALL CopyHeaderFiles targetdir, sourcedir
CALL CopyBinaries targetdir, sourcedir
CALL CopySamples targetdir, sourcedir

EXIT

/*---------------------------------- Usage ----------------------------------
 * Display a usage message
 */
::routine Usage
  SAY "Usage is:"
  SAY "  packageBinaries <options>"
  SAY "     options are:"
  SAY "         -s <source_directory>"
  SAY "         -o <target_directory>"
/*
  SAY "         -c <C compiler name> (e.g. gcc, cc or xlc_r)"
  SAY "         -x <C++ compiler name> (e.g. g++, CC, aCC, c++ or xlC_r)"
*/
  SAY "         -m <message loader> can be 'inmem' (default), 'icu' or 'iconv'"
  SAY "         -n <net accessor> can be 'fileonly' or 'socket' (default)"
  SAY "         -t <transcoder> can be 'icu' or 'native' (default)"
  SAY "         -h to get help on these commands"
  SAY "Example:"
  SAY "  packageBinaries -s D:\xml-xerces\c -o E:\xerces-c_1_1_0 -m inmem -n fileonly"

  RETURN

/*-------------------------- MakeTargetDirectories --------------------------
 * Make the target directory and its main subdirectories
 */
::routine MakeTargetDirectories
  USE ARG targetdir

  CALL SysMkDir targetdir
  CALL SysMkDir targetdir || "\bin"
  CALL SysMkDir targetdir || "\lib"
  CALL SysMkDir targetdir || "\include"
  CALL SysMkDir targetdir || "\include\xercesc"
  CALL SysMkDir targetdir || "\samples"
  CALL SysMkDir targetdir || "\samples\Projects"
  CALL SysMkDir targetdir || "\samples\Projects\OS2"
  CALL SysMkDir targetdir || "\samples\Projects\OS2\VACPP40"
  CALL SysMkDir targetdir || "\samples\data"
  CALL SysMkDir targetdir || "\samples\SAXCount"
  CALL SysMkDir targetdir || "\samples\SAX2Count"
  CALL SysMkDir targetdir || "\samples\SAXPrint"
  CALL SysMkDir targetdir || "\samples\SAX2Print"
  CALL SysMkDir targetdir || "\samples\DOMCount"
  CALL SysMkDir targetdir || "\samples\DOMPrint"
  CALL SysMkDir targetdir || "\samples\IDOMCount"
  CALL SysMkDir targetdir || "\samples\IDOMPrint"
  CALL SysMkDir targetdir || "\samples\Redirect"
  CALL SysMkDir targetdir || "\samples\MemParse"
  CALL SysMkDir targetdir || "\samples\PParse"
  CALL SysMkDir targetdir || "\samples\StdInParse"
  CALL SysMkDir targetdir || "\samples\EnumVal"
  CALL SysMkDir targetdir || "\samples\CreateDOMDocument"
  CALL SysMkDir targetdir || "\doc"
  CALL SysMkDir targetdir || "\doc\html"
  CALL SysMkDir targetdir || "\doc\html\apiDocs"

  RETURN

  /*------------------------------- CopySamples -------------------------------
 */
::routine CopySamples
  USE ARG targetdir, sourcedir

  project = "\samples\Projects\OS2\VACPP40"

  /* Copy the project files */
  srcSpec = sourcedir || project || "\*"
  CALL SysFileTree srcSpec, 'F.', 'FO'
  DO i = 1 TO f.0
    srcfn = f.i
    trgfn = targetdir || project || "\" || Filespec('N', f.i)
    'copy' srcfn trgfn '> nul'
  END

  samples = .array~of( ,
                "SAXCount", ,
                "SAX2Count", ,
                "SAXPrint", ,
                "SAX2Print", ,
                "DOMCount", ,
                "DOMPrint", ,
                "IDOMCount", ,
                "IDOMPrint", ,
                "Redirect", ,
                "MemParse", ,
                "PParse", ,
                "StdInParse", ,
                "EnumVal", ,
                "CreateDOMDocument" ,
                )

  /* Copy the sample code */
  DO sample OVER samples
   srcSpec = sourcedir || "\samples\" || sample || "\*.?pp"
   CALL SysFileTree srcSpec, 'F.', 'FO'
   DO i = 1 TO f.0
     srcfn = f.i
     trgfn = targetdir || "\samples\" || sample ||  "\" || Filespec('N', f.i)
     'copy' srcfn trgfn '> nul'
   END
  END

  RETURN

/*------------------------------ CopyBinaries -------------------------------
 * Copy the DLL and LIB files to their destination directories
 */
::routine CopyBinaries
  USE ARG targetdir, sourcedir

  srcdll = sourcedir || "\Projects\OS2\VACPP40\xerces-c.dll"
  trgdll = targetdir || "\bin\xerces-c.dll"

  srclib = sourcedir || "\Projects\OS2\VACPP40\xerces-c.lib"
  trglib = targetdir || "\lib\xerces-c.lib"

  'copy' srcdll trgdll '> nul'
  'copy' srclib trglib '> nul'

  RETURN

/*----------------------------- CopyHeaderFiles -----------------------------
 * Copy the header files to the target include directory
 */
::routine CopyHeaderFiles
  USE ARG targetdir, sourcedir

  hdirs = .array~of( ,
              "sax", ,
			  "sax2", ,
              "framework", ,
              "dom", ,
              "idom", ,
              "internal", ,
              "parsers", ,
              "util", ,
              "util\Compilers", ,
              "util\MsgLoaders", ,
              "util\MsgLoaders\ICU", ,
              "util\MsgLoaders\InMemory", ,
              "util\MsgLoaders\MsgCatalog", ,
              "util\Platforms", ,
              "util\Platforms\AIX", ,
              "util\Platforms\HPUX", ,
              "util\Platforms\Linux", ,
              "util\Platforms\MacOS", ,
              "util\Platforms\OS2", ,
              "util\Platforms\OS390", ,
              "util\Platforms\PTX", ,
              "util\Platforms\Solaris", ,
              "util\Platforms\Tandem", ,
              "util\Platforms\Win32", ,
              "util\Transcoders", ,
              "util\Transcoders\ICU", ,
              "util\Transcoders\Iconv", ,
              "util\regx", ,
              "validators", ,
              "validators\common", ,
              "validators\datatype", ,
              "validators\DTD", ,
              "validators\schema" )

  DO dir OVER hdirs
    CALL SysMkDir targetdir || "\include\xercesc\" || dir

    /* Copy the *.hpp files */
    hppSpec = sourcedir || "\src\xercesc\" || dir || "\*.hpp"
    CALL SysFileTree hppSpec, 'F.', 'FO'
    DO i = 1 TO f.0
      srcfn = f.i
      trgfn = targetdir || "\include\xercesc\" || dir || "\" || Filespec('N', f.i)
      IF srcfn~pos("\dom\") > 0 THEN
        IF srcfn~pos("Impl.hpp") > 0 | srcfn~pos("\dom\DS") > 0 THEN
          ITERATE
      IF srcfn~pos("\idom\") > 0 THEN
        IF srcfn~pos("Impl.hpp") > 0 | srcfn~pos("\idom\IDS") > 0 THEN
          ITERATE
      'copy' srcfn trgfn '> nul'
    END

    /* Copy the *.c files */
    cSpec = sourcedir || "\src\xercesc\" || dir || "\*.c"
    CALL SysFileTree cSpec, 'F.', 'FO'
    DO i = 1 TO f.0
      srcfn = f.i
      trgfn = targetdir || "\include\xercesc\" || dir || "\" || Filespec('N', f.i)
      'copy' srcfn trgfn '> nul'
    END
  END

  RETURN

/*------------------------------ LoadRexxUtil -------------------------------
 * Load the RexxUtil library
 */
::routine LoadRexxUtil
  IF RxFuncQuery('SysLoadFuncs') THEN
   DO
    IF RxFuncAdd('SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs') THEN
     DO
      .output~lineout("Error: Couldn't load RexxUtil library.")
      RETURN 1
     END
    CALL SysLoadFuncs
   END

  RETURN 0

/*----------------------------------- cli -----------------------------------
 * Command Line Interpreter
 */
::routine cli
  USE ARG cmdline
  argv = .array~new
  DO i = 1 WHILE cmdline <> ''
    cmdline = cmdline~strip
    IF cmdline~left(1) = '"' THEN
      PARSE VAR cmdline '"' thisArg '"' cmdline
    ELSE
      PARSE VAR cmdline thisArg cmdline
    argv[i] = thisArg
  END
  RETURN argv

