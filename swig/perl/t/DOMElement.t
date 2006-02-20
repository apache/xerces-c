# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMElement.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
use blib;
use XML::Xerces;
use Test::More tests => 20;

use lib 't';
use TestUtils qw($DOM $PERSONAL_FILE_NAME);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

$DOM->parse($PERSONAL_FILE_NAME);

my $doc = $DOM->getDocument();
my $doctype = $doc->getDoctype();
my @persons = $doc->getElementsByTagName('person');
my @names = $doc->getElementsByTagName('name');

# try to set an Attribute, 'bar', to a constant string
eval{$persons[0]->setAttribute('bar',"a nice happy attribute value")};
ok(!$@,
  "setAttribute - string value succeeds");
ok(($persons[0]->getAttribute('bar') eq "a nice happy attribute value"),
  "setAttribute - string constant value has proper value");

# try to set an Attribute, 'bar', to a variable string
my $value = "a nice happy attribute value";
eval{$persons[0]->setAttribute('bar',$value)};
ok(!$@,
  "setAttribute - string value succeeds");
ok(($persons[0]->getAttribute('bar') eq $value),
  "setAttribute - string variable value has proper value");

# try to set an Attribute, 'bar', to a constant int
eval{$persons[0]->setAttribute('bar', 42)};
ok(!$@,
  "setAttribute - constant int value succeeds");
ok(($persons[0]->getAttribute('bar') eq 42),
  "setAttribute - constant int value has proper value");

# try to set an Attribute, 'bar', to an int variable
$value = 42;
eval{$persons[0]->setAttribute('bar', $value)};
ok(!$@,
  "setAttribute - int variable value succeeds");
ok(($persons[0]->getAttribute('bar') eq $value),
  "setAttribute - int variable value has proper value");

# try to set an Attribute, 'bar', to a constant double
eval{$persons[0]->setAttribute('bar', 42.0)};
ok(!$@,
  "setAttribute - double value succeeds");
ok(($persons[0]->getAttribute('bar') eq 42.0),
  "setAttribute - constant double value has proper value");

# try to set an Attribute, 'bar', to an double variable
$value = 42.0;
eval{$persons[0]->setAttribute('bar', $value)};
ok(!$@,
  "setAttribute - double variable value succeeds");
ok(($persons[0]->getAttribute('bar') eq $value),
  "setAttribute - double variable value has proper value");

# try to set an Attribute, 'foo', to undef
eval{$persons[0]->setAttribute('foo',undef)};
ok($@,
  "setAttribute - undef value fails");

# try to set an Attribute, undef, to 'foo'
eval{$persons[0]->setAttribute(undef,'foo')};
ok($@,
  "setAttribute - undef name fails");

# ensure that actual_cast() is being called
isa_ok($persons[0],'XML::Xerces::DOMElement');

#
# Test the DOM Level 2 methods
# 
my $uri = 'http://www.foo.bar/';
my $document = <<EOT;
<list xmlns:qs="$uri">
  <element qs:one='1' qs:two='2' qs:three='3' one='27'/>
</list>
EOT

$DOM->setDoNamespaces(1);
$DOM->parse(XML::Xerces::MemBufInputSource->new($document));
$doc = $DOM->getDocument();

# get the single <element> node
my ($element) = $doc->getElementsByTagName('element');

# try to set an Attribute, 'foo', to 'foo'
$element->setAttributeNS($uri,'foo','foo');
ok($element->getAttributeNS($uri,'foo') eq 'foo',
  "setAttributeNS - correct value")
  or diag("found attributes [" . $element->serialize() . "]");

# try to set an Attribute, 'foo', to undef
eval{$element->setAttributeNS($uri,'foo',undef)};
ok($@,
  "setAttributeNS - undef value fails");

# try to set an Attribute, undef, to 'foo'
eval{$element->setAttributeNS($uri,undef,'foo')};
ok($@,
  "setAttributeNS - undef qname fails");

# try to set an Attribute, with uri == undef
eval{$element->setAttributeNS(undef,'foo','foo')};
ok($@,
  "setAttributeNS - undef uri fails");

$document = <<XML;
<root>
<a>
<t:a/>
</a>
</root>
XML

$DOM->parse(XML::Xerces::MemBufInputSource->new($document));

my $doc = $DOM->getDocument();
ok($doc->getElementsByTagName('a')->getLength == 1);
ok($doc->getElementsByTagNameNS('http://example.org/', 'a')->getLength == 2);
ok($doc->getElementsByTagNameNS('http://example.com/test', 'root')->getLength == 1);
