# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMStringList.t'

######################### Begin module loading

# use blib;
use Test::More tests => 52;

BEGIN{use_ok('XML::Xerces::DOM')};

use strict;

######################### Begin Test

my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $DOM = $impl->createLSParser($XML::Xerces::DOMImplementationLS::MODE_SYNCHRONOUS,'');
my $conf = $DOM->getDomConfig();
isa_ok($conf, 'XML::Xerces::DOMConfiguration');

my @ls_param_names = (
		      $XML::Xerces::XMLUni::fgDOMResourceResolver,
		      $XML::Xerces::XMLUni::fgDOMErrorHandler,
		      $XML::Xerces::XMLUni::fgXercesEntityResolver,
		      $XML::Xerces::XMLUni::fgXercesSchemaExternalSchemaLocation,
		      $XML::Xerces::XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation,
		      $XML::Xerces::XMLUni::fgXercesSecurityManager,
		      $XML::Xerces::XMLUni::fgXercesScannerName,
		      $XML::Xerces::XMLUni::fgXercesParserUseDocumentFromImplementation,
		      $XML::Xerces::XMLUni::fgDOMCharsetOverridesXMLEncoding,
		      $XML::Xerces::XMLUni::fgDOMDisallowDoctype,
		      $XML::Xerces::XMLUni::fgDOMIgnoreUnknownCharacterDenormalization,
		      $XML::Xerces::XMLUni::fgDOMNamespaces,
		      $XML::Xerces::XMLUni::fgDOMSupportedMediatypesOnly,
		      $XML::Xerces::XMLUni::fgDOMValidate,
		      $XML::Xerces::XMLUni::fgDOMValidateIfSchema,
		      $XML::Xerces::XMLUni::fgDOMWellFormed,
		      $XML::Xerces::XMLUni::fgDOMCanonicalForm,
		      $XML::Xerces::XMLUni::fgDOMCDATASections,
		      $XML::Xerces::XMLUni::fgDOMCheckCharacterNormalization,
		      $XML::Xerces::XMLUni::fgDOMComments,
		      $XML::Xerces::XMLUni::fgDOMDatatypeNormalization,
		      $XML::Xerces::XMLUni::fgDOMElementContentWhitespace,
		      $XML::Xerces::XMLUni::fgDOMEntities,
		      $XML::Xerces::XMLUni::fgDOMNamespaceDeclarations,
		      $XML::Xerces::XMLUni::fgDOMNormalizeCharacters,
		      $XML::Xerces::XMLUni::fgDOMSchemaLocation,
		      $XML::Xerces::XMLUni::fgDOMSchemaType,
		      $XML::Xerces::XMLUni::fgDOMSplitCDATASections,
		      $XML::Xerces::XMLUni::fgDOMInfoset,
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

my @accepted_param_names = $conf->getParameterNames();
ok(scalar @accepted_param_names, "StringList to_list()");

my $accepted_params_list = $conf->getParameterNames();
isa_ok($accepted_params_list, "XML::Xerces::DOMStringList");

is($accepted_params_list->getLength(), scalar @accepted_param_names,
   'StringList length equal overload list length')
  or diag("Found items: " . join(':', @accepted_param_names));

my %param_names = map {$_=>1} @ls_param_names;
foreach my $param_name (@ls_param_names) {
  ok($accepted_params_list->contains($param_name),
     "can set $param_name");
}

for (my $i=0; $i<$accepted_params_list->getLength(); $i++) {
  my $param_name = $accepted_params_list->item($i);
#  is($accepted_param_names[$i], $param_name, "$param_name in StringList");
}
