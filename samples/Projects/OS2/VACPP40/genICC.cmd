/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

