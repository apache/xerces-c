# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMAttr.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use utf8;
use XML::Xerces;
use Test::More tests => 6;

use lib 't';
use TestUtils qw($DOM $PERSONAL_FILE_NAME);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):

eval{$DOM->parse($PERSONAL_FILE_NAME)};
XML::Xerces::error($@) if $@;

my $doc = $DOM->getDocument();
my $doctype = $doc->getDoctype();
my @persons = $doc->getElementsByTagName('person');
ok(UNIVERSAL::isa($persons[0],'XML::Xerces::DOMElement'));

# test getting the attribute node
my $attr = $persons[0]->getAttributeNode('id');
ok(UNIVERSAL::isa($attr,'XML::Xerces::DOMAttr'));

# test getting the attribute value
ok($attr->getValue() eq $persons[0]->getAttribute('id'));

# test that we can use integers and floats as values for setting attribtes
eval {$attr->setValue(3)};
ok(!$@);

eval {$attr->setValue(.03)};
ok(!$@);
