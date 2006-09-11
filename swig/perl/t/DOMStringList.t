# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMStringList.t'

######################### Begin module loading

# use blib;
use Test::More tests => 20;

BEGIN{use_ok('XML::Xerces::DOM')};

use strict;

######################### Begin Test

my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $DOM = $impl->createLSParser($XML::Xerces::DOMImplementationLS::MODE_SYNCHRONOUS,'');
my $conf = $DOM->getDomConfig();
isa_ok($conf, 'XML::Xerces::DOMConfiguration');

my @param_names = ($XML::Xerces::XMLUni::fgDOMErrorHandler,
		   $XML::Xerces::XMLUni::fgDOMSchemaType,
		   $XML::Xerces::XMLUni::fgDOMSchemaLocation,
		   $XML::Xerces::XMLUni::fgDOMCanonicalForm,
		   $XML::Xerces::XMLUni::fgDOMCDATASections,
		   $XML::Xerces::XMLUni::fgDOMComments,
		   $XML::Xerces::XMLUni::fgDOMDatatypeNormalization,
		   $XML::Xerces::XMLUni::fgDOMWRTDiscardDefaultContent,
		   $XML::Xerces::XMLUni::fgDOMEntities,
		   $XML::Xerces::XMLUni::fgDOMInfoset,
		   $XML::Xerces::XMLUni::fgDOMNamespaces,
		   $XML::Xerces::XMLUni::fgDOMNamespaceDeclarations,
		   $XML::Xerces::XMLUni::fgDOMNormalizeCharacters,
		   $XML::Xerces::XMLUni::fgDOMSplitCDATASections,
		   $XML::Xerces::XMLUni::fgDOMValidate,
		   $XML::Xerces::XMLUni::fgDOMValidateIfSchema,
		   $XML::Xerces::XMLUni::fgDOMElementContentWhitespace);

my @accepted_param_names = $conf->getParameterNames();
ok(scalar @accepted_param_names, "StringList to_list()");

my $accepted_params_list = $conf->getParameterNames();
isa_ok($accepted_params_list, "XML::Xerces::DOMStringList");

is($accepted_params_list->getLength(), scalar @accepted_param_names,
   'StringList length equal overload list length')
  or diag("Found items: " . join(':', @accepted_param_names));

my %param_names = map {$_=>1} @param_names;
foreach my $param_name (@param_names) {
  ok($accepted_params_list->contains($param_name),
     "can set $param_name");
}

for (my $i=0; $i<$accepted_params_list->getLength(); $i++) {
  my $param_name = $accepted_params_list->item($i);
#  is($accepted_param_names[$i], $param_name, "$param_name in StringList");
}
