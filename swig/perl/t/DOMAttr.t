# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMAttr.t'

######################### Begin module loading

# use blib;
use utf8;
use Test::More tests => 6;
BEGIN { use_ok("XML::Xerces::DOM") };

use lib 't';
use TestUtils qw($DOM $PERSONAL_FILE_NAME);
use strict;

######################### Begin Test

eval{$DOM->parse($PERSONAL_FILE_NAME)};
XML::Xerces::error($@) if $@;

my $doc = $DOM->getDocument();
my $doctype = $doc->getDoctype();
my @persons = $doc->getElementsByTagName('person');
isa_ok($persons[0],'XML::Xerces::DOMElement');

# test getting the attribute node
my $attr = $persons[0]->getAttributeNode('id');
isa_ok($attr,'XML::Xerces::DOMAttr');

# test getting the attribute value
is($attr->getValue(), $persons[0]->getAttribute('id'),
  'getValue()');

# test that we can use integers and floats as values for setting attribtes
eval {$attr->setValue(3)};
ok(!$@,
  'setValue() of int');

eval {$attr->setValue(0.03)};
ok(!$@,
  'setValue() of float');
