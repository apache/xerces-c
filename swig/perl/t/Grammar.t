# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# Grammar.t'

######################### Begin module loading

# use blib;
use Test::More tests => 13;
BEGIN { use_ok(XML::Xerces::DOM) };

use lib 't';
use TestUtils qw($DOM $PERSONAL_FILE_NAME $PERSONAL_SCHEMA_FILE_NAME $SAMPLE_DIR);

use strict;

######################### Begin Test

$DOM->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Auto);
$DOM->parse($PERSONAL_FILE_NAME);

# test that we can fetch the grammar
my $grammar = $DOM->getValidator->getGrammar();
isa_ok($grammar, "XML::Xerces::DTDGrammar",
       'DTD grammar');
is($grammar->getGrammarType, $XML::Xerces::Grammar::DTDGrammarType,
   'getGrammarType - DTD');

ok($grammar->getValidated(),
   'grammar validated');

my $desc = $grammar->getGrammarDescription();
isa_ok($desc, "XML::Xerces::XMLGrammarDescription",
       'grammar description');
isa_ok($desc, "XML::Xerces::XMLDTDDescription",
       'grammar description');

$DOM->setDoNamespaces(1);
$DOM->setDoSchema(1);
$DOM->parse($PERSONAL_SCHEMA_FILE_NAME);

$grammar = $DOM->getValidator->getGrammar();
isa_ok($grammar, "XML::Xerces::SchemaGrammar",
       'schema grammar');
is($grammar->getGrammarType, $XML::Xerces::Grammar::SchemaGrammarType,
   'getGrammarType - Schema');
ok($grammar->getValidated(),
   'grammar validated');

my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $DOM = $impl->createLSParser($XML::Xerces::DOMImplementationLS::MODE_SYNCHRONOUS,'');
$grammar = $DOM->loadGrammar("$SAMPLE_DIR/SecondSchema.xsd",
			     $XML::Xerces::Grammar::SchemaGrammarType,
			     my $to_cache = 0);

is($grammar->getTargetNamespace, 'http://www.secondSchema',
   'target namespace');
ok((not $grammar->getValidated()),
   'grammar loaded, not validated');

$desc = $grammar->getGrammarDescription();
isa_ok($desc, "XML::Xerces::XMLGrammarDescription",
       'getGrammarDescription');
isa_ok($desc, "XML::Xerces::XMLSchemaDescription",
       'getGrammarDescription');

