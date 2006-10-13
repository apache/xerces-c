# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# SAXException.t'

######################### Begin module loading

# use blib;
use Test::More tests => 17;
BEGIN { use_ok("XML::Xerces::SAX") };

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME);
use strict;

######################### Begin Test

# test that we get an SAXNotRecognizedException
my $parser = XML::Xerces::XMLReaderFactory::createXMLReader();
isa_ok($parser,"XML::Xerces::SAX2XMLReader");

my $feature = "http://xml.org/sax/features/foospaces";
my $error;

SKIP: {
  eval {$parser->setFeature($feature, 0)};
  $error = $@;
  ok($error,"setFeature: $feature");

  skip "setFeature didn't raise an exception", 2, if not $error;

  my $skip = 0;
  isa_ok($error,'XML::Xerces::SAXNotRecognizedException')
    or $skip = 1;

  skip "bad exception: $error", 1, if $skip;
  ok($error->getMessage(),"message");
}

SKIP: {
  eval {$parser->getFeature($feature)};
  $error = $@;
  ok($error,"getFeature: $feature");

  skip "setFeature didn't raise an exception", 2, if not $error;

  my $skip = 0;
  isa_ok($error,'XML::Xerces::SAXNotRecognizedException')
    or $skip = 1;

  skip "bad exception: $error", 1, if $skip;
  ok($error->getMessage(),"message");
}

SKIP: {
  eval {$parser->getProperty($feature)};
  $error = $@;
  ok($error,"getProperty: $feature");

  skip "setFeature didn't raise an exception", 2, if not $@;

  my $skip = 0;
  isa_ok($error,'XML::Xerces::SAXNotRecognizedException')
    or $skip = 1;

  skip "bad exception: $error", 1, if $skip;
  ok($error->getMessage(),"message");
}

SKIP: {
  eval {$parser->setProperty($feature, $parser)};
  $error = $@;
  ok($@,"setProperty: $feature");

  skip "setFeature didn't raise an exception", 2, if not $error;

  my $skip = 0;
  isa_ok($error,'XML::Xerces::SAXNotRecognizedException')
    or $skip = 1;

  skip "bad exception: $error", 1, if $skip;
  ok($error->getMessage(),"message");
}

# test that modifying a feature during a parse raises a not supported exception
package MyHandler;
use strict;
use base qw(XML::Xerces::PerlContentHandler);

sub start_element {
  my ($self,$name,$attrs) = @_;
  $parser->setProperty('http://xml.org/sax/features/namespaces', $parser);
}
sub end_element {
}
sub characters {
}
sub ignorable_whitespace {
}

package main;
my $handler = MyHandler->new();

# we must reset this - because $parser has been stringified by an earlier test
$parser = XML::Xerces::XMLReaderFactory::createXMLReader();
$parser->setContentHandler($handler);

SKIP: {
  eval {$parser->parse(XML::Xerces::LocalFileInputSource->new($PERSONAL_FILE_NAME))};
  $error = $@;
  ok($error,"setting a feature during parse");

  skip "modifying a feature during a parse didn't raise a not supported exception", 2,
    if not $error;

  my $skip = 0;
  isa_ok($error,'XML::Xerces::SAXNotSupportedException')
    or $skip = 1;

  skip "bad exception: $error", 1, if $skip;
  ok($error->getMessage(),"message");
}
