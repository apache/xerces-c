# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMImplementation.t'

######################### Begin module loading

use blib;
use Test::More tests => 26;
BEGIN { use_ok("XML::Xerces::DOM") };

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME $PERSONAL_NO_DOCTYPE $PERSONAL $DOM);
use strict;

######################### Begin Test

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

my $domImpl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
isa_ok($domImpl,"XML::Xerces::DOMImplementation");
isa_ok($domImpl,"XML::Xerces::DOMImplementationLS");

my $dt  = eval{$domImpl->createDocumentType('x', 'x', 'x')};
XML::Xerces::error($@) if $@;
isa_ok($dt,"XML::Xerces::DOMDocumentType");

$dt  = eval{$domImpl->createDocumentType(undef, 'x', 'x')};
ok($@,
   'exception with undef as qualified name');

$dt  = eval{$domImpl->createDocumentType('x', undef, 'x')};
ok(!$@,
   'no exception with undef as public id');

$dt  = eval{$domImpl->createDocumentType('x', 'x', undef)};
ok(!$@,
   'no exception with undef as system id');

# test exceptions
$dt  = eval{$domImpl->createDocumentType('!', 'x', 'x')};
ok($@,
   'exception with invalid char in qualified name');
isa_ok($@,"XML::Xerces::DOMException");
is($@->getCode(),$XML::Xerces::DOMException::INVALID_CHARACTER_ERR,
  'invalid char exception code is returned');

TODO: {
  local $TODO = 'NAMESPACE_ERR not thrown';
  $dt  = eval{$domImpl->createDocumentType('!malformed', 'x', 'x')};
  is($@->getCode(),$XML::Xerces::DOMException::NAMESPACE_ERR,
     'exception in qualified name');
}

my $mode_synch = $XML::Xerces::DOMImplementationLS::MODE_SYNCHRONOUS;
is($mode_synch,1,
   'mode synchronous');
my $mode_asynch = $XML::Xerces::DOMImplementationLS::MODE_ASYNCHRONOUS;
is($mode_asynch,2,
   'mode asynchronous');

my $dtd_type = $XML::Xerces::XMLUni::fgDOMDTDType;
is($dtd_type, 'http://www.w3.org/TR/REC-xml',
   'dtd type');
my $schema_type = $XML::Xerces::XMLUni::fgDOMXMLSchemaType;
is($schema_type, 'http://www.w3.org/2001/XMLSchema',
   'schema type');

TODO: {
  local $TODO = 'asynchronous mode not supported';
  my $parser = eval {$domImpl->createLSParser($mode_asynch, $dtd_type)};
  ok((not $@),
     'asynch mode, dtd type');
  $parser = eval {$domImpl->createLSParser($mode_asynch, $schema_type)};
  ok((not $@),
     'asynch mode, schema type');
}

my $parser = eval {$domImpl->createLSParser($mode_synch, $dtd_type)};
XML::Xerces::error($@)
  if $@;
ok((not $@),
   'synch mode, dtd type');
isa_ok($parser,"XML::Xerces::DOMLSParser");

$parser = eval {$domImpl->createLSParser($mode_synch, $schema_type)};
XML::Xerces::error($@)
  if $@;
ok((not $@),
   'synch mode, schema type');
isa_ok($parser,"XML::Xerces::DOMLSParser");

my $writer = $domImpl->createLSSerializer();
isa_ok($writer,"XML::Xerces::DOMLSSerializer");

my $input = $domImpl->createLSInput();
isa_ok($input,"XML::Xerces::DOMLSInput");

my $output = $domImpl->createLSOutput();
isa_ok($output,"XML::Xerces::DOMLSOutput");

my $doc = $domImpl->createDocument('myns', 'HISTORY', $dt);
isa_ok($doc,"XML::Xerces::DOMDocument");

ok(keys %XML::Xerces::DOMDocument::OWNER == 0,
  "DOMDocument not owned after creation");

# test exceptions
my $domImpl2 = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $doc2 = eval{$domImpl2->createDocument('myns', 'BAR', $dt)};
XML::Xerces::error($@) if $@;


my $doc3 = eval{$domImpl->createDocument('myns', '!', $dt)};
ok($@,
   'exception with invalid char in qualified name');
isa_ok($@,"XML::Xerces::DOMException");
is($@->getCode(),$XML::Xerces::DOMException::INVALID_CHARACTER_ERR,
  'invalid char exception code is returned');

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
