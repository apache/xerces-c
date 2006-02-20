# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# actualCast.t'

######################### We start with some black magic to print on failure.

END {fail() unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 9;

use lib 't';
use TestUtils qw($DOM $PERSONAL $PERSONAL_FILE_NAME $PERSONAL_SCHEMA_FILE_NAME);
use vars qw($loaded);
use strict;

$loaded = 1;
pass("module loaded");

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):

$DOM->parse($PERSONAL_FILE_NAME);

# test that we get a subclass of DOMNode back
my $name = $DOM->getDocument->getElementsByTagName('link')->item(0);
isa_ok($name,'XML::Xerces::DOMNode');

# test that it really is a subclass
isa_ok($name,'XML::Xerces::DOMElement');

SKIP: {
  skip "Grammars not yet re-implemented", 4 if 0;
# now test the grammars
my $grammar = $DOM->getRootGrammar();
isa_ok($grammar,'XML::Xerces::Grammar');
isa_ok($grammar,'XML::Xerces::DTDGrammar');

# now with a W3C XML Schema
$DOM->setDoNamespaces (1);
$DOM->setDoSchema (1);
$DOM->parse($PERSONAL_SCHEMA_FILE_NAME);

$grammar = $DOM->getRootGrammar();
isa_ok($grammar,'XML::Xerces::Grammar');
isa_ok($grammar,'XML::Xerces::SchemaGrammar');
}

# now test the CallbackHandler's
SKIP: {
  skip "return of CallbackHandler's not implemented", 2;
  my $handler = $DOM->getErrorHandler();
  isa_ok($handler,'XML::Xerces::PerlCallbackHandler');
  isa_ok($handler,'XML::Xerces::PerlErrorCallbackHandler');
}
