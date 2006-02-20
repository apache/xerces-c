# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# SAXParser.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;

# use blib;
use XML::Xerces;
use Test::More tests => 4;
use Config;

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME);
use vars qw($loaded $error);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

my $uri = eval{XML::Xerces::XMLUri->new(undef,"")};
my $error = $@;
ok($error,
  "exception when XMLUri constructor called with NULL baseUri, and empty uriSpec");
isa_ok($error,'XML::Xerces::XMLException');

my $expected = $XML::Xerces::XMLExcepts::XMLNUM_URI_Component_Empty;
my $code = $error->getCode();
is($code, $expected,
   "got correct error code")
  or diag("Expected: $XML::Xerces::XMLExcepts::CODES[$expected], Found: $XML::Xerces::XMLExcepts::CODES[$code]");

