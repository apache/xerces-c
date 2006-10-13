# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMLSInput.t'

######################### Begin module loading

# use blib;
use Test::More tests => 20;
BEGIN { use_ok("XML::Xerces::DOM") };

use strict;

######################### Begin Test

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

my $domImpl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $input = $domImpl->createLSInput();
isa_ok($input,"XML::Xerces::DOMLSInput");

my $test_pub_id = 'FOO';
$input->setPublicId($test_pub_id);
is($input->getPublicId(), $test_pub_id,
   'public id');

my $test_sys_id = 'FOO';
$input->setSystemId($test_sys_id);
is($input->getSystemId(), $test_sys_id,
   'system id');

my $test_base_uri = 'FOO';
$input->setBaseURI($test_base_uri);
is($input->getBaseURI(), $test_base_uri,
   'base uri');

my $test_encoding = 'UTF-8';
$input->setEncoding($test_encoding);
is($input->getEncoding(), $test_encoding,
   'encoding');

my $doc = '<some_tag></some_tag>';

$input->setStringData($doc);
is($input->getStringData(), $doc,
   'string data');

my $is = XML::Xerces::MemBufInputSource->new($doc);
$input->setByteStream($is);
isa_ok($input->getByteStream(), "XML::Xerces::InputSource",
       'byte stream');

isa_ok($input->getByteStream(), "XML::Xerces::MemBufInputSource",
      'overloaded return value in getByteStream');

$is = XML::Xerces::LocalFileInputSource->new('/tmp/foo.xml');
$input->setByteStream($is);
isa_ok($input->getByteStream(), "XML::Xerces::InputSource",
       'byte stream');
isa_ok($input->getByteStream(), "XML::Xerces::LocalFileInputSource",
      'overloaded return value in getByteStream');

$is = XML::Xerces::URLInputSource->new(XML::Xerces::XMLURL->new('http://google.com'));
$input->setByteStream($is);
isa_ok($input->getByteStream(), "XML::Xerces::InputSource",
       'byte stream');
isa_ok($input->getByteStream(), "XML::Xerces::URLInputSource",
      'overloaded return value in getByteStream');

$is = XML::Xerces::StdInInputSource->new();
isa_ok($is, "XML::Xerces::StdInInputSource",
      'input source to set');
$input->setByteStream($is);
isa_ok($input->getByteStream(), "XML::Xerces::InputSource",
       'byte stream');
isa_ok($input->getByteStream(), "XML::Xerces::StdInInputSource",
      'overloaded return value in getByteStream');

my $wrapper = XML::Xerces::Wrapper4DOMLSInput->new($input);
isa_ok($wrapper, "XML::Xerces::InputSource",
      'input source to set');
isa_ok($wrapper, "XML::Xerces::Wrapper4DOMLSInput",
      'input source to set');
$input->setByteStream($wrapper);
isa_ok($input->getByteStream(), "XML::Xerces::InputSource",
       'byte stream');
isa_ok($input->getByteStream(), "XML::Xerces::Wrapper4DOMLSInput",
       'byte stream');

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
