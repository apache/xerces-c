# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMNode.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;

# use blib;
use XML::Xerces;
use Test::More tests => 5;
use Config;

use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

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

my $DOM1 = new XML::Xerces::XercesDOMParser;
my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
$DOM1->setErrorHandler($ERROR_HANDLER);
my $is = eval{XML::Xerces::MemBufInputSource->new($document)};
XML::Xerces::error($@) if $@;
eval {$DOM1->parse($is)};
XML::Xerces::error($@) if $@;

my $DOM2 = new XML::Xerces::XercesDOMParser;
$DOM2->setErrorHandler($ERROR_HANDLER);
eval {$DOM2->parse(XML::Xerces::MemBufInputSource->new($document, 'foo'))};
XML::Xerces::error($@) if $@;

my $doc1 = $DOM1->getDocument();
my $doc2 = $DOM2->getDocument();

my $root1 = $doc1->getDocumentElement();
my @persons1 = $doc1->getElementsByTagName('person');
my @names1 = $doc1->getElementsByTagName('name');
my $root2 = $doc2->getDocumentElement();
my @persons2 = $doc2->getElementsByTagName('person');
my @names2 = $doc1->getElementsByTagName('name');

ok(($root1 != $root2), 
   "root1 and root2 are different");

# importing a child from a different document
eval {
  my $copy = $doc1->importNode($persons1[0],0);
  $root1->appendChild($copy);
};
ok(!$@ &&
   scalar @persons1 < scalar ($root1->getElementsByTagName('person')),
   "import node");

# test the equality operators
my @people = $doc1->getElementsByTagName('person');
ok(($root1 != $root2),
   "root nodes are unequal");
ok(($people[0] == $persons1[0]),
   "nodes are equal");
