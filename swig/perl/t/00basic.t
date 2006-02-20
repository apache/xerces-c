# use blib;
use XML::Xerces;
use Cwd;
use Test::More tests => 19;
use lib 't';
use TestUtils qw(
		 $SYSTEM_RESOLVER_FILE_NAME
		);

pass("loaded");

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

my $parser = XML::Xerces::XercesDOMParser->new();
isa_ok($parser,'XML::Xerces::XercesDOMParser');

my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
isa_ok($ERROR_HANDLER,'XML::Xerces::PerlErrorHandler');

$parser->setErrorHandler($ERROR_HANDLER);
my $tmp = $parser->getErrorHandler();
isa_ok($tmp,'XML::Xerces::ErrorHandler');

my $cwd = cwd();
$cwd =~ s|/t/?$||;
my $SAMPLE_DIR = "$cwd/samples";
my $PERSONAL_FILE_NAME = "$SAMPLE_DIR/personal.xml";
my $PERSONAL_INVALID_FILE_NAME = "$SAMPLE_DIR/personal-invalid.xml";
my $PERSONAL_NOT_WELL_FORMED_FILE_NAME = "$SAMPLE_DIR/personal-not-well-formed.xml";

$parser->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Never);
$tmp = $parser->getValidationScheme();
ok($tmp == $XML::Xerces::AbstractDOMParser::Val_Never,
  '{s,g}etValidationScheme to off');

$parser->parse($PERSONAL_FILE_NAME);
pass("parse valid file with validation off");

eval{$parser->parse($PERSONAL_INVALID_FILE_NAME)};
ok(!$@,
  "parse succeeds: validation = off, file = invalid");

eval{$parser->parse($PERSONAL_NOT_WELL_FORMED_FILE_NAME)};
ok($@,
  "parse succeeds: validation = off, file = not-well-formed");

$parser = XML::Xerces::XercesDOMParser->new();
isa_ok($parser,'XML::Xerces::XercesDOMParser');

$parser->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Always);
$tmp = $parser->getValidationScheme();
ok($tmp == $XML::Xerces::AbstractDOMParser::Val_Always,
  '{s,g}etValidationScheme to on');

$parser->setErrorHandler($ERROR_HANDLER);

eval{$parser->parse($PERSONAL_INVALID_FILE_NAME)};
ok($@,
  "parse fails: validation = on, file = invalid");

eval{$parser->parse($PERSONAL_NOT_WELL_FORMED_FILE_NAME)};
ok($@,
  "parse fails: validation = on, file = not-well-formed");

# see if we can create and set an entity resolver
$parser = XML::Xerces::XercesDOMParser->new();

my $res = eval{XML::Xerces::XMLCatalogResolver->new("$SAMPLE_DIR/catalog.xml")};
XML::Xerces::error($@) if $@;
isa_ok($res,'XML::Xerces::PerlEntityResolver');
$parser->setEntityResolver($res);
pass("set XMLCatalogResolver");
eval {
  $parser->parse($SYSTEM_RESOLVER_FILE_NAME);
};
if ($@) {
  print STDERR $@->getMessage()
    if ref $@;
  print STDERR $@;
}
ok(! $@,
   "entity resolver invoked");

my $ENTITY_RESOLVER = TestUtils->new();
$parser->setEntityResolver($ENTITY_RESOLVER);

# now lets see if the resolver gets invoked
eval {
  $parser->parse($SYSTEM_RESOLVER_FILE_NAME);
};
if ($@) {
  print STDERR $@->getMessage()
    if ref $@;
  print STDERR $@;
}
ok($::test, "entity resolver invoked");


# Test that the document is being remembered
my $doc = $parser->getDocument();
isa_ok($doc,"XML::Xerces::DOMDocument");
ok(exists($XML::Xerces::REMEMBER{tied(%{$doc})}->{__PARSER}),
   "document is remembered");
$doc->DESTROY();
ok(! exists($XML::Xerces::REMEMBER{tied(%{$doc})}->{__PARSER}),
   "document is forgotten");

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
