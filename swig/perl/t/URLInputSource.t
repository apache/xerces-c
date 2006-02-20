# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# URLInputSource.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 8;

use vars qw($loaded $error);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):

my $file_name = '/home/user/test.xml';
my $URL = "file:$file_name";
my $xml_url = XML::Xerces::XMLURL->new($URL);
isa_ok($xml_url, 'XML::Xerces::XMLURL');

my $is = eval{XML::Xerces::URLInputSource->new($xml_url)};
XML::Xerces::error($@) if $@;
isa_ok($is,'XML::Xerces::URLInputSource');

# now test the overloaded constructors

$is = eval{XML::Xerces::URLInputSource->new('file:/',"$file_name")};
XML::Xerces::error($@) if $@;
isa_ok($is,'XML::Xerces::URLInputSource');

$is = eval{XML::Xerces::URLInputSource->new('file:/',"$file_name", 'foo')};
XML::Xerces::error($@) if $@;
ok($is->getPublicId() eq 'foo',
  "PUBLIC Id properly set");

# test that a baseId with no '/' causes a relative URL exception
$is = eval{XML::Xerces::URLInputSource->new('file:',$file_name,'foo')};
my $error = $@;
ok(($error &&
   UNIVERSAL::isa($error,'XML::Xerces::XMLException') &&
   $error->getCode() == $XML::Xerces::XMLExcepts::URL_RelativeBaseURL
  ),
   "baseId with no '/' causes a relative URL exception");

# test that a bad protocol
eval {
  $is = XML::Xerces::URLInputSource->new('blorphl:/xs/./foo.html', 'foo');
};
$error = $@;
ok(($error &&
   UNIVERSAL::isa($error,'XML::Xerces::XMLException') &&
   $error->getCode() == $XML::Xerces::XMLExcepts::URL_UnsupportedProto1
  ),
   "bad protocol causes exception");

# test a non-existent protocol
$is = eval{XML::Xerces::URLInputSource->new('foo','', 'foo')};
$error = $@;
# printf STDERR "Got error code: %d\n", $error->getCode();
ok(($error &&
   UNIVERSAL::isa($error,'XML::Xerces::XMLException') &&
   $error->getCode() == $XML::Xerces::XMLExcepts::URL_NoProtocolPresent
  ),
   "non-existent protocol causes exception");
