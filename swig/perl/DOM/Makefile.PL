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
use Env qw(XERCES_DEVEL
	   SWIG);
my @MACRO;

my $BLIB_DIR = '../blib/lib/XML/Xerces';
if ($XERCES_DEVEL) {
  print STDERR "\tWelcome Xerces Developer!\n";

  sub Xerces_DOM_postamble {
    my $retval = <<TERMINUS;
DOM-tmp.pm: $SCRIPTS_DIR/postModule.pl $INTERFACE_DIR/Perl/dom-shadow.i
	\$(SWIG) $SWIG_ARGS -o Xerces_DOM-tmp.cpp -pm DOM-tmp.pm $INTERFACE_DIR/Xerces_DOM.i

DOM.pm: $SCRIPTS_DIR/postModule.pl DOM-tmp.pm
	perl -I$SCRIPTS_DIR $SCRIPTS_DIR/postModule.pl --in=DOM-tmp.pm --out=DOM.pm

Xerces_DOM.cpp: $SCRIPTS_DIR/postSource.pl Xerces_DOM-tmp.cpp
	perl -I$SCRIPTS_DIR $SCRIPTS_DIR/postSource.pl --in=Xerces_DOM-tmp.cpp --out=Xerces_DOM.cpp

Xerces_DOM-tmp.cpp: $INTERFACE_DIR/Xerces_DOM.i @DOM_INTERFACE_FILES
	\$(SWIG) $SWIG_ARGS -o Xerces_DOM-tmp.cpp -pm DOM-tmp.pm $INTERFACE_DIR/Xerces_DOM.i
	perl  -I$SCRIPTS_DIR $SCRIPTS_DIR/postModule.pl --in=DOM-tmp.pm --out=DOM.pm
	mkdir -p $BLIB_DIR
	cp -f DOM.pm $BLIB_DIR/DOM.pm

TERMINUS
    return $retval;
  }

  my $swig = $SWIG || 'swig';
  @MACRO = ('macro'       => {
    'SWIG'         => "$swig",
  });

  *MY::postamble = \&Xerces_DOM_postamble;
}

WriteMakefile(
    'NAME'        => 'XML::Xerces::DOM',
    'INC'       => $INCLUDES,
    'LIBS'      => [$LIBS],
    'MYEXTLIB'  => "$TRANSCODER_LIB $HANDLER_LIB",
    'OBJECT'    => 'Xerces_DOM$(OBJ_EXT)',
    'CCFLAGS'   => $CFLAGS,
    'CC'        => $CXX,
    'SKIP'      => [qw( test makeaperl xs_o)],
  @OPTIMIZE,
  @MACRO,
  @LDFLAGS,
);

