# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# MemBufInputSource.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 6;

use lib 't';
use TestUtils qw($DOM $PERSONAL_NO_DOCTYPE);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

my $is = eval{XML::Xerces::MemBufInputSource->new($PERSONAL_NO_DOCTYPE,'foo')};
XML::Xerces::error($@) if $@;
isa_ok($is, 'XML::Xerces::InputSource');
isa_ok($is, 'XML::Xerces::MemBufInputSource');

eval {$DOM->parse($is)};
XML::Xerces::error($@) if $@;
my $serialize = $DOM->getDocument->serialize;
is($serialize, $PERSONAL_NO_DOCTYPE,
   "doctype");

# now test that the fake system ID is optional
$is = eval{XML::Xerces::MemBufInputSource->new($PERSONAL_NO_DOCTYPE)};
XML::Xerces::error($@) if $@;
is($is->getSystemId(), 'FAKE_SYSTEM_ID',
   "system id");

# test that a blank document does not segfault
$is = eval{XML::Xerces::MemBufInputSource->new('')};
XML::Xerces::error($@) if $@;
eval {$DOM->parse($is)};
ok($@,
  "blank document does not segfault")
  or diag($@);
