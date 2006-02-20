# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# Grammar.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 2;

use lib 't';
use TestUtils qw($DOM $PERSONAL_FILE_NAME);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

$DOM->parse($PERSONAL_FILE_NAME);

# test that we can fetch the grammar
my $grammar = $DOM->getValidator->getGrammar->getGrammarType();
ok($grammar == $XML::Xerces::Grammar::DTDGrammarType);
