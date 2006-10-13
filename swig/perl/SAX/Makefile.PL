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

  sub Xerces_SAX_postamble {
    my $retval = <<TERMINUS;
SAX-tmp.pm: $SCRIPTS_DIR/postModule.pl $INTERFACE_DIR/Perl/sax-shadow.i
	\$(SWIG) $SWIG_ARGS -o Xerces_SAX-tmp.cpp -pm SAX-tmp.pm $INTERFACE_DIR/Xerces_SAX.i

SAX.pm: $SCRIPTS_DIR/postModule.pl SAX-tmp.pm
	perl -I$SCRIPTS_DIR $SCRIPTS_DIR/postModule.pl --in=SAX-tmp.pm --out=SAX.pm

Xerces_SAX.cpp: $SCRIPTS_DIR/postSource.pl Xerces_SAX-tmp.cpp
	perl -I$SCRIPTS_DIR $SCRIPTS_DIR/postSource.pl --in=Xerces_SAX-tmp.cpp --out=Xerces_SAX.cpp

Xerces_SAX-tmp.cpp: $INTERFACE_DIR/Xerces_SAX.i @SAX_INTERFACE_FILES
	\$(SWIG) $SWIG_ARGS -o Xerces_SAX-tmp.cpp -pm SAX-tmp.pm $INTERFACE_DIR/Xerces_SAX.i
	perl  -I$SCRIPTS_DIR $SCRIPTS_DIR/postModule.pl --in=SAX-tmp.pm --out=SAX.pm
	mkdir -p $BLIB_DIR
	cp -f SAX.pm $BLIB_DIR/SAX.pm

TERMINUS
    return $retval;
  }

  *MY::postamble = \&Xerces_SAX_postamble;
}

WriteMakefile(
    'NAME'        => 'XML::Xerces::SAX',
    'INC'       => $INCLUDES,
    'LIBS'      => [$LIBS],
    'MYEXTLIB'  => "$TRANSCODER_LIB $HANDLER_LIB",
    'OBJECT'    => 'Xerces_SAX$(OBJ_EXT)',
    'CCFLAGS'   => $CFLAGS,
    'CC'        => $CXX,
    'SKIP'      => [qw( test makeaperl xs_o)],
  @OPTIMIZE,
  @LDFLAGS,
);

