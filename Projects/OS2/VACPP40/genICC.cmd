/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.apache.org.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 * Object Rexx script to generate ICC include files for Xerces-C project.
 */

IF LoadRexxUtil() THEN EXIT

msgloader  = 'InMemory'
transcoder = 'Iconv'

argv = cli(arg(1))
DO i = 1 TO argv~items
  SELECT
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
  END
END

msgloaderdef  = 'XML_USE_' || msgloader~translate || '_MSGLOADER'
transcoderdef = 'XML_USE_' || transcoder~translate || '_TRANSCODER'

/* Figure out the project's base directory */
here = Directory()~translate('* ', ' \')
base = here~subword(1, here~words - 3)~strip~translate(' \', '* ')

basedir = .subdirlist~new(base'\src')
basedir~removeAnyContaining("\CVS")

/* Generate the project_options file */
projOpt = .array~of( ,
            'BASE_DIR = "' || (base)~changestr('\', '\\') || '"', ,
            '',                                                   ,
            'option project_options =',                           ,
            '    gen(unreferenced, "yes"),',                      ,
            '    link(linkwithmultithreadlib, "yes"),',           ,
            '    link(map, "Yes"),',                              ,
            '    link(dllinitinstance, "yes"),',                  ,
            '    link(pmtype, "vio"),',                           ,
            '    link(stub, "No"),',                              ,
            '    link(exportall, "yes"),',                        ,
            '    opt(inline, "yes"),',                            ,
            '    opt(level, "2"),',                               ,
            '    opt(tune, "pentium"),',                          ,
            '    incl(searchpath, BASE_DIR "\\src"),',            ,
            '    incl(searchpath, BASE_DIR "\\src\\dom"),',       ,
            '    define("' || msgloaderdef || '", 1),',           ,
            '    define("' || transcoderdef || '", 1)'            )

fl = .stream~new('project_options.icc')
fl~open('Write Replace')
fl~arrayout(projOpt)
fl~close

/* List of directories to ignore */
ignore = .array~of( ,
           "\CVS", ,
           "\Transcoders\", ,
           "\MsgLoaders\", ,
           "\Platforms\", ,
           "\Compilers\", ,
           "\NetAccessors\", ,
           "\dom\AncestorNode.cpp", ,
           "\com\", ,
           "\NLS\", ,
           "Win32", ,
           "LibWWW" )

/* List to build up contents of base project file */
incl = .list~new
incl~insert('include "project_options.icc"')
targ = .list~new
targ~insert('')
targ~insert('option')
targ~insert('    project_options')
targ~insert('{')
targ~insert('   target "xerces-c.dll"')
targ~insert('   {')

/* Walk over each directory and produce a project file for that
 * directory. Directories that don't apply to the OS/2 build are
 * removed from the list.
 */
DO dir OVER basedir
  thisdir = FileSpec('N', dir)
  grpname = 'xerces_' || thisdir
  thisfile = grpname || '.icc'
  files = .filelist~new(dir || '\*.cpp')
  DO ignoreThis OVER ignore
    files~removeAnyContaining(ignoreThis)
  END
  IF thisdir = 'util' THEN
   DO
    f1 = .filelist~new(dir || '\Transcoders\*.cpp')
    f1~removeAllBut('\'transcoder'\')
    files~merge(f1)
    f2 = .filelist~new(dir || '\MsgLoaders\*.cpp')
    f2~removeAllBut('\'msgloader'\')
    files~merge(f2)
   END
  IF files~items > 0 THEN
   DO
    fl = .stream~new(thisfile)
    fl~open('Write Replace')
    fl~lineout('group' grpname '=')
    incl~insert('include "' || thisfile || '"')
    targ~insert('    source type("cpp")' grpname)
    delim = ','
    count = files~items
    DO file OVER files
      count = count - 1
      IF count = 0 THEN
        delim = ''
      fn = file~changestr(base, 'BASE_DIR "') || '"'
      fl~lineout('    ' || fn~changestr('\', '\\') || delim)
    END
   END
END

/* Generate a group for the OS/2-specific files */
flos2 = .filelist~new(base'\src\util\*.cpp')
flos2~removeAllBut('Platforms\OS2\')
thisfile = 'xerces_OS2.icc'
fl = .stream~new(thisfile)
fl~open('Write Replace')
fl~lineout('group xerces_os2 =')
incl~insert('include "' || thisfile || '"')
targ~insert('    source type("cpp") xerces_os2')
delim = ','
count = flos2~items
DO file OVER flos2
  count = count - 1
  IF count = 0 THEN
    delim = ''
  fn = file~changestr(base, 'BASE_DIR "') || '"'
  fl~lineout('   ' || fn~changestr('\', '\\') || delim)
END

/* Generate the root project file */
thisfile = 'Xerces.icc'
fl = .stream~new(thisfile)
fl~open('Write Replace')
here = incl~first
DO WHILE here <> .nil
  fl~lineout(incl[here])
  here = incl~next(here)
END
targ~insert('   }')
targ~insert('}')
here = targ~first
DO WHILE here <> .nil
  fl~lineout(targ[here])
  here = targ~next(here)
END
fl~close

EXIT

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

/******************************** SubdirList ********************************
 */
::class SubdirList PUBLIC

::method init
  EXPOSE !root !dirs
  USE ARG !root

  !dirs = .list~new

  CALL SysFileTree !root'\*', 'dirs.', 'DO'
  DO i = 1 TO dirs.0
    !dirs~insert(dirs.i)
  END

/*================================= basedir =================================
 */
::method basedir
  EXPOSE !root
  RETURN !root

/*================================ makearray ================================
 */
::method makearray
  EXPOSE !dirs
  RETURN !dirs~makearray

/*=========================== removeAnyContaining ===========================
 */
::method removeAnyContaining
  EXPOSE !dirs
  USE ARG sub

  i = !dirs~first
  DO WHILE i <> .nil
    n = i
    i = !dirs~next(i)
    IF !dirs[n]~pos(sub) <> 0 THEN
     DO
      !dirs~remove(n)
     END
  END

/*============================== removeAllBut ===============================
 */
::method removeAllBut
  EXPOSE !dirs
  USE ARG sub

  DO i = !dirs~first TO !dirs~last
    n = i
    i = !dirs~next(i)
    IF !dirs[n]~pos(sub) = 0 THEN
     DO
      !dirs~remove(n)
     END
  END

/********************************* FileList *********************************
 */
::class FileList PUBLIC

::method init
  EXPOSE !pattern !files
  USE ARG !pattern

  !files = .list~new

  CALL SysFileTree !pattern, 'files.', 'FOS'
  DO i = 1 TO files.0
    !files~insert(files.i)
  END

/*================================= basedir =================================
 */
::method basedir
  EXPOSE !pattern
  RETURN (FileSpec('D', !pattern) || FileSpec('P', !pattern))~strip('T', '\')

/*================================ makearray ================================
 */
::method makearray
  EXPOSE !files
  RETURN !files~makearray

/*================================== items ==================================
 */
::method items
  EXPOSE !files
  RETURN !files~items

/*=========================== removeAnyContaining ===========================
 */
::method removeAnyContaining
  EXPOSE !files
  USE ARG sub

  i = !files~first
  DO WHILE i <> .nil
    n = i
    i = !files~next(i)
    IF !files[n]~pos(sub) <> 0 THEN
     DO
      !files~remove(n)
     END
  END

/*============================== removeAllBut ===============================
 */
::method removeAllBut
  EXPOSE !files
  USE ARG sub

  i = !files~first
  DO WHILE i <> .nil
    n = i
    i = !files~next(i)
    IF !files[n]~pos(sub) = 0 THEN
     DO
      !files~remove(n)
     END
  END

/*================================== merge ==================================
 */
::method merge
  EXPOSE !files
  USE ARG fl
  DO f OVER fl
    !files~insert(f)
  END

