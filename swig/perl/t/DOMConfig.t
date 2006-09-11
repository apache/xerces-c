# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMConfig.t'

######################### Begin module loading

# use blib;
use Test::More tests => 20;

BEGIN{use_ok('XML::Xerces::DOM')};

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME);
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
ok(scalar @accepted_param_names, "got parameter name list");

my %param_names = map {$_=>1} @param_names;
foreach my $param_name (@param_names) {
  ok(exists $param_names{$param_name}, "can set $param_name");
}
my $handler = XML::Xerces::PerlErrorHandler->new();
