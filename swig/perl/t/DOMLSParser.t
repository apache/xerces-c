# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMLSParser.t'

######################### Begin module loading

# use blib;
use Test::More tests => 16;

BEGIN{use_ok('XML::Xerces::DOM')};

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME $PERSONAL_DTD_NAME $PERSONAL_SCHEMA_NAME);
use vars qw($error);
use strict;

######################### Begin Test

my $document = q[<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
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

my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $DOM = $impl->createLSParser($XML::Xerces::DOMImplementationLS::MODE_SYNCHRONOUS,'');

my $doc = eval{$DOM->parseURI($PERSONAL_FILE_NAME)};
ok((not $@),'parseURI');
isa_ok($doc,'XML::Xerces::DOMDocument');

my @persons = $doc->getElementsByTagName('person');
is(scalar @persons, 6,'getting <person>s');

# test parseURI
$doc = eval{$DOM->parseURI("file:///$PERSONAL_FILE_NAME")};
ok((not $@),'parseURI with file:');
isa_ok($doc,'XML::Xerces::DOMDocument');

@persons = $doc->getElementsByTagName('person');
is(scalar @persons, 6,'getting <person>s');

# test parse with an input source
# my $sax_is;
my $dom_is;
eval{
  my $sax_is = XML::Xerces::LocalFileInputSource->new($PERSONAL_FILE_NAME);
  $dom_is = XML::Xerces::Wrapper4InputSource->new($sax_is);
};
ok((not $@),'Creating InputSource wrapper')
  or diag(XML::Xerces::error($@));
# isa_ok($sax_is,'XML::Xerces::LocalFileInputSource');
isa_ok($dom_is,'XML::Xerces::Wrapper4InputSource');

$doc = eval{$DOM->parse($dom_is)}
  or diag(XML::Xerces::error($@));
ok((not $@),'parse with InputSource');
isa_ok($doc,'XML::Xerces::DOMDocument');

@persons = $doc->getElementsByTagName('person');
is(scalar @persons, 6,'getting <person>s');
my $to_cache;
my $grammar = $DOM->loadGrammar($PERSONAL_DTD_NAME,
				$XML::Xerces::Grammar::DTDGrammarType,
				$to_cache = 1);
isa_ok($grammar, "XML::Xerces::DTDGrammar",
       'loadGrammar(path)');

eval{
  my $sax_is = XML::Xerces::LocalFileInputSource->new($PERSONAL_DTD_NAME);
  $dom_is = XML::Xerces::Wrapper4InputSource->new($sax_is);
};
$grammar = $DOM->loadGrammar($dom_is,
			     $XML::Xerces::Grammar::DTDGrammarType,
			     $to_cache = 1);
isa_ok($grammar, "XML::Xerces::DTDGrammar",
       'loadGrammar(is)');

$grammar = $DOM->loadGrammar($PERSONAL_SCHEMA_NAME,
				$XML::Xerces::Grammar::SchemaGrammarType,
				$to_cache = 1);
isa_ok($grammar, "XML::Xerces::SchemaGrammar",
       'loadGrammar(path)');

eval{
  my $sax_is = XML::Xerces::LocalFileInputSource->new($PERSONAL_SCHEMA_NAME);
  $dom_is = XML::Xerces::Wrapper4InputSource->new($sax_is);
};
$grammar = $DOM->loadGrammar($dom_is,
			     $XML::Xerces::Grammar::SchemaGrammarType,
			     $to_cache = 1);
isa_ok($grammar, "XML::Xerces::SchemaGrammar",
       'loadGrammar(is)');
