# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMConfig.t'

######################### Begin module loading

# use blib;
use Test::More tests => 106;

BEGIN{use_ok('XML::Xerces::DOM')};

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME);
use vars qw($error);
use strict;

######################### Begin Test

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

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
my $conf = $DOM->getDomConfig();
isa_ok($conf, 'XML::Xerces::DOMConfiguration');

my @other_param_names = ($XML::Xerces::XMLUni::fgDOMErrorHandler,
			 $XML::Xerces::XMLUni::fgXercesEntityResolver,
			 $XML::Xerces::XMLUni::fgDOMResourceResolver,
			 $XML::Xerces::XMLUni::fgXercesSecurityManager,
			 $XML::Xerces::XMLUni::fgXercesScannerName,
			 $XML::Xerces::XMLUni::fgXercesSchemaExternalSchemaLocation,
			 $XML::Xerces::XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation,
			 $XML::Xerces::XMLUni::fgXercesParserUseDocumentFromImplementation,
			);

my @false_bool_param_names = (
		      $XML::Xerces::XMLUni::fgDOMSupportedMediatypesOnly,
);

my @true_bool_param_names = (
		      $XML::Xerces::XMLUni::fgDOMNamespaceDeclarations,
		      $XML::Xerces::XMLUni::fgDOMWellFormed,
		      $XML::Xerces::XMLUni::fgDOMInfoset,
		      $XML::Xerces::XMLUni::fgDOMCharsetOverridesXMLEncoding,
		      $XML::Xerces::XMLUni::fgDOMNamespaces,
		      $XML::Xerces::XMLUni::fgDOMValidate,
		      $XML::Xerces::XMLUni::fgDOMValidateIfSchema,
		      $XML::Xerces::XMLUni::fgDOMComments,
		      $XML::Xerces::XMLUni::fgDOMDatatypeNormalization,
		      $XML::Xerces::XMLUni::fgDOMElementContentWhitespace,
		      $XML::Xerces::XMLUni::fgDOMEntities,
		      $XML::Xerces::XMLUni::fgXercesSchema,
		      $XML::Xerces::XMLUni::fgXercesSchemaFullChecking,
		      $XML::Xerces::XMLUni::fgXercesUserAdoptsDOMDocument,
		      $XML::Xerces::XMLUni::fgXercesLoadExternalDTD,
		      $XML::Xerces::XMLUni::fgXercesContinueAfterFatalError,
		      $XML::Xerces::XMLUni::fgXercesValidationErrorAsFatal,
		      $XML::Xerces::XMLUni::fgXercesCacheGrammarFromParse,
		      $XML::Xerces::XMLUni::fgXercesUseCachedGrammarInParse,
		      $XML::Xerces::XMLUni::fgXercesCalculateSrcOfs,
		      $XML::Xerces::XMLUni::fgXercesStandardUriConformant,
		      $XML::Xerces::XMLUni::fgXercesDOMHasPSVIInfo,
		      $XML::Xerces::XMLUni::fgXercesGenerateSyntheticAnnotations,
		      $XML::Xerces::XMLUni::fgXercesValidateAnnotations,
		      $XML::Xerces::XMLUni::fgXercesIdentityConstraintChecking,
		      $XML::Xerces::XMLUni::fgXercesIgnoreCachedDTD,
		      $XML::Xerces::XMLUni::fgXercesIgnoreAnnotations,
		      $XML::Xerces::XMLUni::fgXercesDisableDefaultEntityResolution,
		      $XML::Xerces::XMLUni::fgXercesSkipDTDValidation);

my @todo_param_names = ($XML::Xerces::XMLUni::fgDOMSchemaLocation,
			$XML::Xerces::XMLUni::fgDOMSchemaType,
			$XML::Xerces::XMLUni::fgDOMDisallowDoctype,
			$XML::Xerces::XMLUni::fgDOMCanonicalForm,
			$XML::Xerces::XMLUni::fgDOMIgnoreUnknownCharacterDenormalization,
			$XML::Xerces::XMLUni::fgDOMCDATASections,
			$XML::Xerces::XMLUni::fgDOMCheckCharacterNormalization,
			$XML::Xerces::XMLUni::fgDOMNormalizeCharacters,
			$XML::Xerces::XMLUni::fgDOMSplitCDATASections,
		       );

my @bool_param_names = (@true_bool_param_names, @false_bool_param_names);

my @ls_param_names = (@bool_param_names, @other_param_names, @todo_param_names);

my @accepted_param_names = $conf->getParameterNames();
ok(scalar @accepted_param_names, "got parameter name list");

my %accepted_param_names = map {$_=>1} @accepted_param_names;
foreach my $param_name (@ls_param_names) {
  ok(exists $accepted_param_names{$param_name}, "$param_name in param list");
}

# test bool value
foreach my $param_name (@true_bool_param_names) {
  ok($conf->canSetParameter($param_name, 1),
     "setting bool value: $param_name");
}

foreach my $param_name (@false_bool_param_names) {
  ok($conf->canSetParameter($param_name, 0),
     "setting bool value: $param_name");
}

TODO: {
  local $TODO = "unimplemented params";

  foreach my $param_name (@todo_param_names) {
    ok($conf->canSetParameter($param_name, 0),
       "todo param: $param_name");
  }
}

#
# test setting object values
#
my $handler;

$handler = XML::Xerces::PerlErrorHandler->new();
ok($conf->canSetParameter($XML::Xerces::XMLUni::fgDOMErrorHandler, undef),
   'can set error handler');

eval {XML::Xercesc::DOMConfiguration_canSetParameter($conf, $XML::Xerces::XMLUni::fgXercesEntityResolver, $handler)};
ok($@,
   'can set with bad value causes exception');

$handler = XML::Xerces::PerlEntityResolver->new();
ok($conf->canSetParameter($XML::Xerces::XMLUni::fgXercesEntityResolver, undef),
   'can set entity resolver');

eval {$conf->setParameter($XML::Xerces::XMLUni::fgDGXMLScanner, $handler)};
ok($@,
   'setParameter with bad value causes exception');
isa_ok($@, "XML::Xerces::DOMException");

$handler = XML::Xerces::PerlDOMErrorHandler->new();
isa_ok($handler, "XML::Xerces::DOMErrorHandler",
       'error handler to set');
eval {$conf->setParameter($XML::Xerces::XMLUni::fgDOMErrorHandler, $handler)};
ok((not $@),
   'setting error handler')
  or XML::Xerces::error($@);

isa_ok($conf->getParameter($XML::Xerces::XMLUni::fgDOMErrorHandler), "XML::Xerces::DOMErrorHandler",
      'getParameter');

eval {$conf->setParameter($XML::Xerces::XMLUni::fgDOMErrorHandler, undef)};
ok((not $@),
   'setting null error handler')
  or XML::Xerces::error($@);

ok((not defined $conf->getParameter($XML::Xerces::XMLUni::fgDOMErrorHandler)),
   'getting null error handler');

$handler = XML::Xerces::PerlEntityResolver->new();
isa_ok($handler, "XML::Xerces::XMLEntityResolver",
       'handler to set');
eval {$conf->setParameter($XML::Xerces::XMLUni::fgXercesEntityResolver, $handler)};
ok((not $@),
   'setting XMLEntityResolver')
  or XML::Xerces::error($@);

isa_ok($conf->getParameter($XML::Xerces::XMLUni::fgXercesEntityResolver), "XML::Xerces::XMLEntityResolver",
      'getParameter');

eval {$conf->setParameter($XML::Xerces::XMLUni::fgXercesEntityResolver, undef)};
ok((not $@),
   'setting null XMLEntityResolver')
  or XML::Xerces::error($@);

ok((not defined $conf->getParameter($XML::Xerces::XMLUni::fgXercesEntityResolver)),
   'getting null XMLEntityResolver');

eval {$conf->setParameter($XML::Xerces::XMLUni::fgDOMResourceResolver, $handler)};
ok((not $@),
   'setting DOMLSResourceResolver')
  or XML::Xerces::error($@);

isa_ok($conf->getParameter($XML::Xerces::XMLUni::fgDOMResourceResolver), "XML::Xerces::DOMLSResourceResolver");

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
