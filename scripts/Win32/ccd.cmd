@ECHO OFF

%DevDrive%

IF "%1"=="build" GOTO CD_BUILD
IF "%1"=="data" GOTO CD_DATA
IF "%1"=="dom" GOTO CD_DOM
IF "%1"=="framework" GOTO CD_FRAMEWORK
IF "%1"=="internal" GOTO CD_INTERNAL
IF "%1"=="intl" GOTO CD_INTL
IF "%1"=="parsers" GOTO CD_PARSERS
IF "%1"=="root" GOTO CD_ROOT
IF "%1"=="samples" GOTO CD_SAMPLES
IF "%1"=="sax" GOTO CD_SAX
IF "%1"=="scripts" GOTO CD_SCRIPTS
IF "%1"=="src" GOTO CD_SRC
IF "%1"=="tests" GOTO CD_TESTS
IF "%1"=="tools" GOTO CD_TOOLS
IF "%1"=="util" GOTO CD_UTIL


ECHO Unknown target directory
GOTO :EXIT

:CD_BUILD
CD %XML4CRoot%\Build\Win32\VC6
GOTO EXIT

:CD_DATA
CD %XML4CRoot%\samples\data
GOTO EXIT

:CD_DOM
CD %XML4CRoot%\src\dom
GOTO EXIT

:CD_FRAMEWORK
CD %XML4CRoot%\src\framework
GOTO EXIT

:CD_INTERNAL
CD %XML4CRoot%\src\internal
GOTO EXIT

:CD_INTL
CD %XML4CRoot%\intlFiles
GOTO EXIT

:CD_PARSERS
CD %XML4CRoot%\src\parsers
GOTO EXIT

:CD_ROOT
CD %XML4CRoot%
GOTO EXIT

:CD_SAMPLES
CD %XML4CRoot%\samples
GOTO EXIT

:CD_SAX
CD %XML4CRoot%\src\sax
GOTO EXIT

:CD_SCRIPTS
CD %XML4CRoot%\scripts\Win32
GOTO EXIT

:CD_SRC
CD %XML4CRoot%\src
GOTO EXIT

:CD_TESTS
CD %XML4CRoot%\tests
GOTO EXIT

:CD_TOOLS
CD %XML4CRoot%\tools
GOTO EXIT

:CD_UTIL
CD %XML4CRoot%\src\util
GOTO EXIT

:EXIT
