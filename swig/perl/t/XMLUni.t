# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# XMLUni.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;

use blib;
use XML::Xerces;
use Test::More tests => 4;

use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

# once the unicode constants were not being properly exported
# these tests guard against that happening again
ok($XML::Xerces::XMLUni::fgPCDATAString eq '#PCDATA');
ok($XML::Xerces::XMLUni::fgPubIDString eq 'PUBLIC');

my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $writer = $impl->createDOMWriter();
eval{$writer->setFeature($XML::Xerces::XMLUni::fgDOMWRTFormatPrettyPrint,1)};
ok(!$@,
   "Xerces method arguments now handle magic stringify")
  or diag(XML::Xerces::error($@));
