# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMDocument.t'

######################### We start with some black magic to print on failure.

# Change 1..1 below to 1..last_test_to_print .
# (It may become useful if the test is moved to ./t subdirectory.)

END {ok(0) unless $loaded;}

use Carp;

use blib;
use utf8;
use XML::Xerces;
use Test::More tests => 11;
use Config;

use lib 't';
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

my ($DOM1, $ERROR_HANDLER, $DOM2);
my ($doc1, $doc2);
my ($root1, @persons1, $name1, $root2, @persons2, $name2);

sub refresh {
  $DOM1 = new XML::Xerces::XercesDOMParser;
  $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
  $DOM1->setErrorHandler($ERROR_HANDLER);
  my $is = eval{XML::Xerces::MemBufInputSource->new($document)};
  XML::Xerces::error($@) if $@;
  eval{$DOM1->parse($is)};
  XML::Xerces::error($@) if $@;

  $DOM2 = new XML::Xerces::XercesDOMParser;
  $DOM2->setErrorHandler($ERROR_HANDLER);
  eval {$DOM2->parse($is)};
  XML::Xerces::error($@) if $@;

  $doc1 = $DOM1->getDocument();
  $doc2 = $DOM2->getDocument();

  $root1 = $doc1->getDocumentElement();
  @persons1 = $root1->getChildNodes();
  $name1 = ($persons1[1]->getChildNodes())[1];
  $root2 = $doc2->getDocumentElement();
  @persons2 = $root2->getChildNodes();
  $name2 = ($persons2[1]->getChildNodes())[1];
}

refresh();

# importing a child from a different document
eval {
  my $copy = $doc1->importNode($persons1[0], 0);
  $root1->appendChild($copy);
};
ok(!$@ && scalar @persons1 < scalar ($root1->getElementsByTagName('person')),
   "importing a child from a different document");

refresh();

# check that creating an element with an illegal charater
eval {
  my $el = $doc1->createElement('?');
};
my $error = $@;
ok($error,'creating document with illegal raises exception');

my $isa = "XML::Xerces::DOMException";
isa_ok($error,$isa)
  or diag($error);

my $expect = "$XML::Xerces::DOMException::INVALID_CHARACTER_ERR";
ok(UNIVERSAL::isa($error,$isa)
   && $error->getCode == $expect,
  "received correct error code")
  || diag("found code: $error->{code}, expected code: $expect");

refresh();

# check that an element can't start with a digit
my $el = eval {$doc1->createElement('9')};
$error = $@;
ok($error,'creating an element that starts with a digit raises exception');

isa_ok($error,$isa)
  or diag($error);

ok((UNIVERSAL::isa($error,$isa)
    && $error->getCode == $expect),
    "received correct error code")
  || diag("found code: $error->{code}, expected code: $expect");

# check that an element can start with an underscore
eval {
  my $el = $doc1->createElement('_');
};
ok(!$@,
  "element can start with underscore");

# check that an element can start with an colon
eval {
  my $el = $doc1->createElement(':');
};
ok(!$@,
  "element cannot start with colon");

# check that getElementById() doesn't segfault on undef ID
eval {
  $doc1->getElementById(undef);
};
ok($@,
  "calling getElementById with null string causes exception");

