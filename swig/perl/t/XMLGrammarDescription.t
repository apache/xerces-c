# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# XMLGrammarDescription.t'

######################### Begin module loading

# use blib;
use Test::More tests => 23;
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

# test dynamic cast for GrammarDescription
my $desc = $grammar->getGrammarDescription();
isa_ok($desc, "XML::Xerces::XMLGrammarDescription",
       'grammar description');
isa_ok($desc, "XML::Xerces::XMLDTDDescription",
       'grammar description');

is($desc->getGrammarType, $XML::Xerces::Grammar::DTDGrammarType,
   'getGrammarType - DTD');
ok((not defined $desc->getRootName()),
   "getRootName - not defined");
is($desc->getSystemId(),'[dtd]',
   "getSystemId");
is($desc->getGrammarKey(), '[dtd]',
   "getGrammarKey");

$DOM->setDoNamespaces(1);
$DOM->setDoSchema(1);
$DOM->setErrorHandler(undef);
my $file = "$SAMPLE_DIR/TypeInfoNoDTD.xml";
$DOM->parse($file);

$grammar = $DOM->getValidator->getGrammar();
isa_ok($grammar, "XML::Xerces::SchemaGrammar",
       'schema grammar');

# test dynamic cast for GrammarDescription
$desc = $grammar->getGrammarDescription();
isa_ok($desc, "XML::Xerces::XMLGrammarDescription",
       'getGrammarDescription');
isa_ok($desc, "XML::Xerces::XMLSchemaDescription",
       'getGrammarDescription');

is($desc->getGrammarType, $XML::Xerces::Grammar::SchemaGrammarType,
   'getGrammarType - Schema');

is($desc->getGrammarKey(),'',
   "getGrammarKey");
is($desc->getTargetNamespace(), '',
   "getTargetNamespace");
is($desc->getContextType(), $XML::Xerces::XMLSchemaDescription::CONTEXT_PREPARSE,
   'getContextType - PREPARSE');
ok((not defined $desc->getEnclosingElementName()),
   "getEnclosingElementName");
ok((not defined $desc->getTriggeringComponent()),
     "getTriggeringComponent");
ok((not defined $desc->getAttributes()),
   "getAttributes");

my $hints = $desc->getLocationHints();
isa_ok($hints, "XML::Xerces::XMLChVector",
       'getLocationHints');
is($hints->size(),1,
   "getLocationHints");

$file =~ s/\.xml/.xsd/;
is($hints->elementAt(0), $file,
   'hint is schema');

my @hints = $desc->getLocationHints();
is((scalar @hints),1,
   "overloaded getLocationHints");

is($hints[0], $hints->elementAt(0),
   'XMLChVector equality');
