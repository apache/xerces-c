# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMImplementation.t'

######################### We start with some black magic to print on failure.

END {fail("module loaded") unless $loaded;}

use Carp;
# use blib;
use Test::More tests => 6;
use XML::Xerces;

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME $PERSONAL_NO_DOCTYPE $PERSONAL $DOM);
use vars qw($loaded);
use strict;

$loaded = 1;
pass("module loaded");

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

######################### End of black magic.

my $domImpl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
isa_ok($domImpl,"XML::Xerces::DOMImplementation");

my $dt  = eval{$domImpl->createDocumentType('x', 'x', 'x')};
XML::Xerces::error($@) if $@;
isa_ok($dt,"XML::Xerces::DOMDocumentType");

my $doc = $domImpl->createDocument('myns', 'HISTORY', $dt);
isa_ok($doc,"XML::Xerces::DOMDocument");

ok(keys %XML::Xerces::DOMDocument::OWNER == 0,
  "DOMDocument not owned after creation");

$doc->getDocumentElement->setAttributeNS("http://www.w3.org/2000/xmlns/", "xmlns:myns", 'myns');

for (my $i = 0; $i < 10; $i++) {
  my $record = $doc->createElement('RECORD');
  $doc->getDocumentElement->appendChild($record);
  $record->setAttribute('ID', '0');
  $record->setAttribute('PRICE', '1.1');
  $record->setAttribute('VOl', '10');
}
my @elements = $doc->getElementsByTagName('*');
ok(scalar @elements == 11,
  "Found all created elements")
  or diag("Found only " . scalar @elements . " elements");

$doc->release();

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
