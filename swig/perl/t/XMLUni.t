# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# XMLUni.t'
#########################################

# use blib;
use Test::More tests => 8;
BEGIN {use_ok("XML::Xerces::DOM")};
BEGIN {use_ok("XML::Xerces::SAX")};

use strict;

# once the unicode constants were not being properly exported
# these tests guard against that happening again
is($XML::Xerces::XMLUni::fgPCDATAString, '#PCDATA',
   'XMLUni constants exported');
is($XML::Xerces::XMLUni::fgPubIDString, 'PUBLIC',
   'XMLUni constants exported');

TODO: {
  local $TODO = 'SWIG is exporting all class variables into top-level namespace';
  ok((not defined $XML::Xerces::XMLUni_fgPCDATAString),
     'XMLUni constants not in XML::Xerces namespace');
  ok((not defined $XML::Xerces::XMLUni_fgPubIDString),
     'XMLUni constants not in XML::Xerces namespace');
}

eval {
  my $parser = XML::Xerces::XMLReaderFactory::createXMLReader();
  $parser->setFeature($XML::Xerces::XMLUni::fgSAX2CoreNameSpaces, 1)
};
ok(!$@,
   "Xerces method arguments now handle magic stringify - SAX2")
  or diag(XML::Xerces::error($@));

my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $writer = $impl->createLSSerializer();
eval{$writer->getDomConfig()->setParameter($XML::Xerces::XMLUni::fgDOMWRTFormatPrettyPrint,1)};

ok(!$@,
   "Xerces method arguments now handle magic stringify - DOM")
  or diag(XML::Xerces::error($@));
