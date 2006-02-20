# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMNodeIterator.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;

use blib;
use XML::Xerces;
use Test::More tests => 7;
use Config;

use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

package MyNodeFilter;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlNodeFilter);
sub acceptNode {
  my ($self,$node) = @_;
  return $XML::Xerces::DOMNodeFilter::FILTER_ACCEPT;
}

package main;

######################### End of black magic.

# Create a couple of identical test documents
my $document = q[<?xml version="1.0" encoding="UTF-8"?>
<contributors>
	<person Role="manager">
		<name>Mike Pogue</name>
		<email>mpogue@us.ibm.com</email>
	</person>
	<person Role="developer">
		<name>Tom Watson</name>
		<email>rtwatson@us.ibm.com</email>
	</person>
	<person Role="tech writer">
		<name>Susan Hardenbrook</name>
		<email>susanhar@us.ibm.com</email>
	</person>
</contributors>];

my $DOM = new XML::Xerces::XercesDOMParser;
my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
$DOM->setErrorHandler($ERROR_HANDLER);
$DOM->parse(XML::Xerces::MemBufInputSource->new($document));

my $doc = $DOM->getDocument();
isa_ok($doc,'XML::Xerces::DOMDocumentTraversal');
my $root = $doc->getDocumentElement();
my $filter = MyNodeFilter->new();
isa_ok($filter,'XML::Xerces::PerlNodeFilter');
my $what = $XML::Xerces::DOMNodeFilter::SHOW_ELEMENT;
my $iterator = $doc->createNodeIterator($root,$what,$filter,1);
isa_ok($iterator,'XML::Xerces::DOMNodeIterator');

# test that nextNode() returns the first node in the set
ok($iterator->nextNode() == $root,
  "first node is root");

my $success = 1;
my $count = 0;
while (my $node = $iterator->nextNode()) {
  $count++;
  $success = 0 unless $node->isa('XML::Xerces::DOMElement');
}
# test that we only got elements
ok($success,
  "TreeWalker returns only DOMElements");

#test that we got all the elements
ok($count == 9,
  "TreeWalker traverses all elements in tree");
