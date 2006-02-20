# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# EntityResolver.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 11;
use Cwd;

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME
		 $PERSONAL_SCHEMA_FILE_NAME
		 $SCHEMA_FILE_NAME
		 $PUBLIC_RESOLVER_FILE_NAME
		 $SYSTEM_RESOLVER_FILE_NAME
		 $PERSONAL_DTD_NAME);
use vars qw($loaded $file $test);
use strict;

$loaded = 1;
pass("module loaded");

######################### End of black magic.

$test = 0;
my $DOM = XML::Xerces::XercesDOMParser->new();
my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
$DOM->setErrorHandler($ERROR_HANDLER);

# see if we can create and set an entity resolver
my $res = eval{XML::Xerces::XMLCatalogResolver->new('samples/catalog.xml')};
XML::Xerces::error($@) if $@;
$DOM->setEntityResolver($res);
pass("set XMLCatalogResolver");

my $ENTITY_RESOLVER = TestUtils->new();
$DOM->setEntityResolver($ENTITY_RESOLVER);
pass("test resolver");

# now lets see if the resolver gets invoked
eval {
  $DOM->parse($SYSTEM_RESOLVER_FILE_NAME);
};
if ($@) {
  print STDERR $@->getMessage()
    if ref $@;
  print STDERR $@;
}
ok($test);

my $doc;
eval {
  $doc = $DOM->getDocument();
};
if ($@) {
  print STDERR $@->getMessage()
    if ref $@;
  print STDERR $@;
}
ok(ref $doc && $doc->isa('XML::Xerces::DOMDocument'));

my $root = $doc->getDocumentElement();
ok(ref $root && 
       $root->isa('XML::Xerces::DOMElement') &&
       $root->getNodeName() eq 'personnel'
      );

$DOM->reset();
$test = 0;
eval {
  $DOM->parse($PUBLIC_RESOLVER_FILE_NAME);
};
if ($@) {
  print STDERR $@->getMessage()
    if ref $@;
  print STDERR $@;
}
ok($test);

$doc = $DOM->getDocument();
ok(ref $doc && $doc->isa('XML::Xerces::DOMDocument'));

$root = $doc->getDocumentElement();
ok(ref $root && 
       $root->isa('XML::Xerces::DOMElement') &&
       $root->getNodeName() eq 'personnel'
      );

my $document = <<'SCHEMA';
<?xml version="1.0" encoding="ISO-8859-1"?>
<personnel xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	   xsi:noNamespaceSchemaLocation='bar.xsd'>

  <person id="Big.Boss" >
    <name><family>Boss</family> <given>Big</given></name>
    <email>chief@foo.com</email>
    <link subordinates="one.worker two.worker three.worker four.worker five.worker"/>
  </person>

  <person id="one.worker">
    <name><family>Worker</family> <given>One</given></name>
    <email>one@foo.com</email>
    <link manager="Big.Boss"/>
  </person>

  <person id="two.worker">
    <name><family>Worker</family> <given>Two</given></name>
    <email>two@foo.com</email>
    <link manager="Big.Boss"/>
  </person>

  <person id="three.worker">
    <name><family>Worker</family> <given>Three</given></name>
    <email>three@foo.com</email>
    <link manager="Big.Boss"/>
  </person>

  <person id="four.worker">
    <name><family>Worker</family> <given>Four</given></name>
    <email>four@foo.com</email>
    <link manager="Big.Boss"/>
  </person>

  <person id="five.worker">
    <name><family>Worker</family> <given>Five</given></name>
    <email>five@foo.com</email>
    <link manager="Big.Boss"/>
  </person>

</personnel>
SCHEMA

$DOM->reset();
$DOM->setDoSchema(1);
$DOM->setDoNamespaces(1);
# $DOM->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Always);
eval {
  $DOM->parse(XML::Xerces::MemBufInputSource->new($document));
};
if ($@) {
  die $@->getMessage()
    if ref $@;
  die $@;
}
ok(1);

package MyEntityResolver;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlEntityResolver);

sub new {
  return bless {}, shift;
}

sub resolve_entity {
  my ($self,$pub,$sys) = @_;
  return undef;
}

package main;
my $document2 = qq[<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<!DOCTYPE bar SYSTEM "foo.dtd" [
<!ELEMENT bar (foo)>
<!ELEMENT foo EMPTY>
<!ATTLIST foo id ID #REQUIRED>
<!ATTLIST foo role CDATA #REQUIRED>
]>
<bar>
  <foo id='baz' role="manager"/>
</bar>];

my $is2 = eval{XML::Xerces::MemBufInputSource->new($document2)};
XML::Xerces::error($@) if $@;
$DOM->setEntityResolver(MyEntityResolver->new());
eval {$DOM->parse($is2)};
my $error = $@;
ok($error,
  "entity resolver which returns NULL for DTD resolution raises exception");

