# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMLSOutput.t'

######################### Begin module loading

# use blib;
use Test::More tests => 10;
BEGIN { use_ok("XML::Xerces::DOM") };

use strict;

######################### Begin Test

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

my $domImpl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $output = $domImpl->createLSOutput();
isa_ok($output,"XML::Xerces::DOMLSOutput");

my $test_sys_id = 'FOO';
$output->setSystemId($test_sys_id);
is($output->getSystemId(), $test_sys_id,
   'system id');

my $test_encoding = 'UTF-8';
$output->setEncoding($test_encoding);
is($output->getEncoding(), $test_encoding,
   'encoding');

my $target = XML::Xerces::MemBufFormatTarget->new();
$output->setByteStream($target);
isa_ok($output->getByteStream(), "XML::Xerces::XMLFormatTarget",
       'byte stream');

isa_ok($output->getByteStream(), "XML::Xerces::MemBufFormatTarget",
       'overloaded return val getByteStream');

$target = XML::Xerces::LocalFileFormatTarget->new('/tmp/foo.xml');
$output->setByteStream($target);
isa_ok($output->getByteStream(), "XML::Xerces::XMLFormatTarget",
       'byte stream');

isa_ok($output->getByteStream(), "XML::Xerces::LocalFileFormatTarget",
       'overloaded return val getByteStream');

$target = XML::Xerces::StdOutFormatTarget->new();
$output->setByteStream($target);
isa_ok($output->getByteStream(), "XML::Xerces::XMLFormatTarget",
       'byte stream');

isa_ok($output->getByteStream(), "XML::Xerces::StdOutFormatTarget",
       'overloaded return val getByteStream');

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
