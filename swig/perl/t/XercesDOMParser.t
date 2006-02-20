# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# XercesDOMParser.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use Test::More tests => 8;
use XML::Xerces;

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME $PERSONAL_NO_DOCTYPE $PERSONAL $DOM);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

######################### End of black magic.

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

my $is = eval {XML::Xerces::MemBufInputSource->new($PERSONAL_NO_DOCTYPE)};
XML::Xerces::error($@) if $@;

eval {$DOM->parse($is)};
XML::Xerces::error($@) if $@;

my $serialize = $DOM->getDocument->serialize;
ok($serialize eq $PERSONAL_NO_DOCTYPE);

# now test reparsing a file
$DOM->reset();
$is = eval {XML::Xerces::MemBufInputSource->new($document)};
XML::Xerces::error($@) if $@;

eval {$DOM->parse($is)};
XML::Xerces::error($@) if $@;
ok(!$@);

my $doc = $DOM->getDocument();
my @persons = $doc->getElementsByTagName('person');
ok(scalar @persons == 3);

ok($persons[0]->getAttributes()->getLength == 1);

ok($persons[0]->getAttribute('Role') eq 'manager');

# now test the overloaded methods in XercesDOMParser
eval {
  $DOM->parse($PERSONAL_FILE_NAME);
};
ok(!$@);
XML::Xerces::error($@) if $@;

$DOM = XML::Xerces::XercesDOMParser->new();
$DOM->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Always);
$DOM->setIncludeIgnorableWhitespace(0);
eval {
  $DOM->parse($PERSONAL_FILE_NAME);
};
XML::Xerces::error($@) if $@;

# now check that we do *not* get whitespace nodes
my @nodes = $DOM->getDocument->getDocumentElement->getChildNodes();
ok(scalar @nodes == 6);

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
