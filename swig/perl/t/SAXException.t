# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# SAXException.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;

# use blib;
use XML::Xerces;
use Test::More tests => 17;
use Config;

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

# test that we get an SAXNotRecognizedException
my $parser = XML::Xerces::XMLReaderFactory::createXMLReader();
isa_ok($parser,"XML::Xerces::SAX2XMLReader");

my $feature = "http://xml.org/sax/features/foospaces";
eval {
  $parser->setFeature($feature, 0);
};
my $error = $@;
my $skip = 0;
unless (ok($error,"setFeature: $feature")) {
  $skip = 1;
}

TODO: {
  todo_skip "setFeature didn't raise an exception", 2, if $skip;

  isa_ok($error,'XML::Xerces::SAXNotRecognizedException');
  ok($error->getMessage(),"message");
}

$skip = 0;

eval {
  $parser->getFeature($feature);
};
$error = $@;
unless (ok($error,"getFeature: $feature")){
  $skip = 1;
}

TODO: {
  todo_skip "setFeature didn't raise an exception", 2, if $skip;

  isa_ok($error,'XML::Xerces::SAXNotRecognizedException');
  ok($error->getMessage(),"message");
}

$skip = 0;

eval {
  $parser->getProperty($feature);
};
$error = $@;
unless (ok($error,"getProperty: $feature")) {
  $skip = 1;
}

TODO: {
  todo_skip "setFeature didn't raise an exception", 2, if $skip;

  isa_ok($error,'XML::Xerces::SAXNotRecognizedException');
  ok($error->getMessage(),"message");
}

$skip = 0;

eval {
  $parser->setProperty($feature, $parser);
};
$error = $@;
unless (ok($error,"setProperty: $feature")){
  $skip = 1;
}

TODO: {
  todo_skip "setFeature didn't raise an exception", 2, if $skip;

  isa_ok($error,'XML::Xerces::SAXNotRecognizedException');
  ok($error->getMessage(),"message");
}

# test that modifying a feature during a parse raises a not supported exception
package MyHandler;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlContentHandler);

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
$parser->setContentHandler($handler);
eval {
  $parser->parse(XML::Xerces::LocalFileInputSource->new($PERSONAL_FILE_NAME));
};
$error = $@;
unless (ok($error,"setting a feature during parse")) {
  $skip = 1;
}

TODO: {
  todo_skip "modifying a feature during a parse didn't raise a not supported exception", 2, if $skip;

  isa_ok($error,'XML::Xerces::SAXNotSupportedException');
  ok($error->getMessage(),"message");
}
