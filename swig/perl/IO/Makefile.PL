 #
 # Copyright 2002,2004 The Apache Software Foundation.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #      http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

use strict;

my $BLIB_DIR = '../blib/lib/XML/Xerces';
if ($XERCES_DEVEL) {
  print STDERR "\tWelcome Xerces Developer!\n";

  sub Xerces_IO_postamble {
    my $retval = <<TERMINUS;
IO-tmp.pm: $SCRIPTS_DIR/postModule.pl $INTERFACE_DIR/Perl/dom-shadow.i
	\$(SWIG) $SWIG_ARGS -o Xerces_IO-tmp.cpp -pm IO-tmp.pm $INTERFACE_DIR/Xerces_IO.i

IO.pm: $SCRIPTS_DIR/postModule.pl IO-tmp.pm
	perl -I$SCRIPTS_DIR $SCRIPTS_DIR/postModule.pl --in=IO-tmp.pm --out=IO.pm

Xerces_IO.cpp: $SCRIPTS_DIR/postSource.pl Xerces_IO-tmp.cpp
	perl -I$SCRIPTS_DIR $SCRIPTS_DIR/postSource.pl --in=Xerces_IO-tmp.cpp --out=Xerces_IO.cpp

Xerces_IO-tmp.cpp: $INTERFACE_DIR/Xerces_IO.i @IO_INTERFACE_FILES
	\$(SWIG) $SWIG_ARGS -o Xerces_IO-tmp.cpp -pm IO-tmp.pm $INTERFACE_DIR/Xerces_IO.i
	perl  -I$SCRIPTS_DIR $SCRIPTS_DIR/postModule.pl --in=IO-tmp.pm --out=IO.pm
	mkdir -p $BLIB_DIR
	cp -f IO.pm $BLIB_DIR/IO.pm

TERMINUS
    return $retval;
  }

  *MY::postamble = \&Xerces_IO_postamble;
}

WriteMakefile(
    'NAME'        => 'XML::Xerces::IO',
    'INC'       => $INCLUDES,
    'LIBS'      => [$LIBS],
    'MYEXTLIB'  => "$TRANSCODER_LIB $HANDLER_LIB",
    'OBJECT'    => 'Xerces_IO$(OBJ_EXT)',
    'CCFLAGS'   => $CFLAGS,
    'CC'        => $CXX,
    'SKIP'      => [qw( test makeaperl xs_o)],
  @OPTIMIZE,
  @LDFLAGS,
);

