# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# XMLValidator.t'

######################### Begin module loading

# use blib;
use Test::More tests => 38;
BEGIN { use_ok(XML::Xerces::DOM) };
BEGIN { use_ok(XML::Xerces::SAX) };

use lib 't';
use TestUtils qw($PERSONAL_SCHEMA_FILE_NAME $PERSONAL_FILE_NAME);

use strict;

######################### Begin Test

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

foreach my $sub (\&getDOMParser, \&getSAXParser, \&getSAX2Parser) {
  my $val_to_use = XML::Xerces::DTDValidator->new();
  isa_ok($val_to_use, "XML::Xerces::DTDValidator",
	 'validator');

  ok($val_to_use->handlesDTD(),
     'validator handles DTD');

  ok((not $val_to_use->handlesSchema()),
     'validator handles Schema');

  ok((not $val_to_use->requiresNamespaces()),
     'validator requires namespaces');

  my $schema = 0;
  my ($parser,$name) = $sub->($val_to_use, $schema = 0);

  eval {$parser->parse ($PERSONAL_FILE_NAME)};
  XML::Xerces::error($@) if $@;

  is($parser->getErrorCount(),0,
     "$name - successful parse");

  my $grammar = $val_to_use->getGrammar();
  isa_ok($grammar, "XML::Xerces::DTDGrammar",
	 "$name - grammar");

  $val_to_use = XML::Xerces::SchemaValidator->new();
  isa_ok($val_to_use, "XML::Xerces::SchemaValidator",
	 "$name - validator");

  ok((not $val_to_use->handlesDTD()),
     'validator handles DTD');

  ok($val_to_use->handlesSchema(),
     'validator handles Schema');

  ok($val_to_use->requiresNamespaces(),
     'validator requires namespaces');

  ($parser,$name) = $sub->($val_to_use, $schema = 1);

  eval {$parser->parse ($PERSONAL_SCHEMA_FILE_NAME)};
  XML::Xerces::error($@) if $@;

  is($parser->getErrorCount(),0,
     "$name - successful parse");

  $grammar = $val_to_use->getGrammar();
  isa_ok($grammar, "XML::Xerces::SchemaGrammar",
	 "$name - grammar");
}

  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
XML::Xerces::XMLPlatformUtils::Terminate();


sub getDOMParser {
  my $validator = shift;
  my $schema = shift;
  my $parser = XML::Xerces::XercesDOMParser->new($validator);
  $parser->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Auto);
  $parser->setErrorHandler(XML::Xerces::PerlErrorHandler->new());
  if ($schema) {
    $parser->setDoNamespaces(1);
    $parser->setDoSchema(1);
  }
  return $parser, 'DOM';
}

sub getSAXParser {
  my $validator = shift;
  my $schema = shift;
  my $parser = XML::Xerces::SAXParser->new($validator);
  $parser->setValidationScheme($XML::Xerces::SAXParser::Val_Auto);
  $parser->setErrorHandler(XML::Xerces::PerlErrorHandler->new());
  if ($schema) {
    $parser->setDoNamespaces(1);
    $parser->setDoSchema(1);
  }
  return $parser, 'SAX';
}

sub getSAX2Parser {
  my $validator = shift;
  my $schema = shift;
  my $parser = XML::Xerces::XMLReaderFactory::createXMLReader();
  $parser->setErrorHandler(XML::Xerces::PerlErrorHandler->new());
  $parser->setValidator($validator);
  $parser->setFeature($XML::Xerces::XMLUni::fgSAX2CoreValidation, 1);
  $parser->setFeature($XML::Xerces::XMLUni::fgXercesDynamic, 0);
  if ($schema) {
    $parser->setFeature($XML::Xerces::XMLUni::fgXercesSchema, 1);
  }
  return $parser, 'SAX2';
}
