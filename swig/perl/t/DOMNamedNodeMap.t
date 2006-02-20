# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMNamedNodeMap.t'

######################### We start with some black magic to print on failure.

# Change 1..1 below to 1..last_test_to_print .
# (It may become useful if the test is moved to ./t subdirectory.)

END {ok(0) unless $loaded;}

use Carp;
use XML::Xerces;
use Test::More tests => 15;

use lib 't';
use TestUtils qw($DOM);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):

my $document = <<EOT;
<list>
  <element one='1' two='2' three='3'/>
  <none>text</none>
</list>
EOT

$DOM->parse(XML::Xerces::MemBufInputSource->new($document));

my $doc = $DOM->getDocument();

# this tests a bug that getAttributes() should return an empty list
# when there are no attributes (even when bogusly called on a text node)
my ($element) = $doc->getElementsByTagName('none');
my @attrs = $element->getFirstChild->getAttributes();
ok(scalar @attrs != 1);

# get the single <element> node
($element) = $doc->getElementsByTagName('element');
my %attrs = $element->getAttributes();
ok(scalar keys %attrs == 3 &&
       $attrs{one} == 1 &&
       $attrs{two} == 2 &&
       $attrs{three} == 3);

# test that we can still get a DOMNodeList object
# and test getLength()
my $dom_node_map = $element->getAttributes();
ok(UNIVERSAL::isa($dom_node_map, 'XML::Xerces::DOMNamedNodeMap')
   && $dom_node_map->getLength() == scalar keys %attrs
  );

# test item()
for (my $i=0;$i<scalar keys %attrs ;$i++) {
  my $node = $dom_node_map->item($i);
  ok($attrs{$node->getNodeName} == $node->getNodeValue);
}

# test getNamedItem()
foreach (keys %attrs) {
  ok($dom_node_map->getNamedItem($_)->getNodeValue eq $attrs{$_});
}

# test setNamedItem()
my $four = $doc->createAttribute('four');
$four->setNodeValue('4');
$dom_node_map->setNamedItem($four);
ok($dom_node_map->getNamedItem('four')->getNodeValue eq $four->getNodeValue);

# test removeNamedItem()
$dom_node_map->removeNamedItem('four');
ok($dom_node_map->getLength() == scalar keys %attrs);

#
# Test the DOM Level 2 methods
# 
my $uri = 'http://www.foo.bar/';
$document = <<EOT;
<list xmlns:qs="$uri">
  <element qs:one='1' qs:two='2' qs:three='3' one='27'/>
</list>
EOT

$DOM->setDoNamespaces(1);
$DOM->parse(XML::Xerces::MemBufInputSource->new($document));
$doc = $DOM->getDocument();

# get the single <element> node
($element) = $doc->getElementsByTagName('element');
%attrs = $element->getAttributes();
$dom_node_map = $element->getAttributes();

# test getNamedItemNS()
my $oneNS = $dom_node_map->getNamedItemNS($uri,'one');
my $one = $dom_node_map->getNamedItem('one');
ok($one->getNodeValue eq '27'
       && $oneNS->getNodeValue eq '1'
      );

# test setNamedItem()
$four = $doc->createAttributeNS($uri,'four');
$four->setNodeValue('4');
$dom_node_map->setNamedItemNS($four);
ok($dom_node_map->getNamedItemNS($uri,'four')->getNodeValue eq $four->getNodeValue);

# test removeNamedItem()
$dom_node_map->removeNamedItemNS($uri,'four');
ok($dom_node_map->getLength() == scalar keys %attrs);

