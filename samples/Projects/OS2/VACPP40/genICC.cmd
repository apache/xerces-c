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
 * Object Rexx script to generate ICC include files for samples in the
 * Xerces-C project.
 */

IF LoadRexxUtil() THEN EXIT

here = Directory()~translate('* ', ' \')
base = here~subword(1, here~words - 4)~strip~translate(' \', '* ')

basedir = .subdirlist~new(base'\samples')
basedir~removeAnyContaining("\CVS")
basedir~removeAnyContaining("\Projects")
basedir~removeAnyContaining("\data")

fl = .stream~new('basedir.icc')
fl~open('Write Replace')
fl~lineout('SRC_DIR = "' || (base)~changestr('\', '\\') || '"')
fl~lineout('SAMPLES = SRC_DIR "\\samples"')
fl~lineout('BASE_DIR = SRC_DIR')
fl~lineout('LIB_DIR = BASE_DIR "\\lib"')
fl~lineout('BIN_DIR = BASE_DIR "\\bin"')
fl~lineout('')
fl~lineout('group xerces_lib = LIB_DIR "\\xerces-c.lib"')
fl~close

cm = .stream~new('bldsamples.cmd')
cm~open('Write Replace')
cm~lineout("@echo off")

DO dir OVER basedir
  thisdir = FileSpec('N', dir)
  grpname = thisdir
  files = .filelist~new(dir || '\*.cpp')
  files~removeAnyContaining("\CVS")
  IF files~items > 0 THEN
   DO
    fname = grpname || '.icc'
    cm~lineout('echo making' grpname)
    cm~lineout('vacbld' fname)
    fl = .stream~new(fname)
    fl~open('Write Replace')

    fl~lineout('include "basedir.icc"')
    fl~lineout('include "sample_options.icc"')
    fl~lineout('')
    fl~lineout('option sample_options')
    fl~lineout('{')
    fl~lineout('  target BIN_DIR "\\' || grpname || '.exe"')
    fl~lineout('  {')
    DO file OVER files
      n = file~pos('\samples\') + 8
      fileloc = 'SAMPLES "' || file~substr(n)~changestr('\', '\\') || '"'
      fl~lineout('    source type("cpp")' fileloc)
    END
    fl~lineout('    source type("lib") xerces_lib')
    fl~lineout('  }')
    fl~lineout('}')
    fl~close
   END
END
cm~close

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

::method basedir
  EXPOSE !root
  RETURN !root

::method makearray
  EXPOSE !dirs
  RETURN !dirs~makearray

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

::method basedir
  EXPOSE !pattern
  RETURN (FileSpec('D', !pattern) || FileSpec('P', !pattern))~strip('T', '\')

::method makearray
  EXPOSE !files
  RETURN !files~makearray

::method items
  EXPOSE !files
  RETURN !files~items

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

::method merge
  EXPOSE !files
  USE ARG fl
  DO f OVER fl
    !files~insert(f)
  END

