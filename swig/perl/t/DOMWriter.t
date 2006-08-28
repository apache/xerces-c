# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMWriter.t'

######################### We start with some black magic to print on failure.

END {fail() unless $loaded;}

use Carp;

# use blib;
use XML::Xerces;
use Test::More tests => 6;
use Config;

use vars qw($loaded);
use strict;

$loaded = 1;
pass("module loaded");

######################### End of black magic.

# Create a couple of identical test documents
my $document = q[<?xml version="1.0" encoding="UTF-8" standalone="no" ?><contributors>
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

my $dom = XML::Xerces::XercesDOMParser->new();
my $handler = XML::Xerces::PerlErrorHandler->new();
$dom->setErrorHandler($handler);
eval{$dom->parse(XML::Xerces::MemBufInputSource->new($document))};
XML::Xerces::error($@) if $@;

my $doc = $dom->getDocument();
my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $writer = $impl->createLSSerializer();

my $output = $writer->writeToString($doc);

# change the encoding string
$output =~ s/UTF-16/UTF-8/;

ok($output, "output written");
is($output, $document, "got expected document")
  or diag("Found: $output");

my $file = 'foo.xml';
my $rc = eval {$writer->writeToURI($doc, $file)};
XML::Xerces::error($@) if $@;

ok($rc,
   'writeToURI');

SKIP: {
  skip "Couldn't open $file for reading", 2, unless open(TEST, $file);

# slurp the whole file in
  local $/;
  $output = <TEST>;

  # change the encoding string
  $output =~ s/UTF-16/UTF-8/;

  ok($output, "output written");
  is($output, $document, "got expected document")
    or diag("Found: $output");

  unlink($file);
}

