# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl DOMNodeList.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
use Cwd;
# use blib;
use XML::Xerces;
use Test::More tests => 10;

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

$DOM->parse( new XML::Xerces::LocalFileInputSource($PERSONAL_FILE_NAME) );
my $doc = $DOM->getDocument();

# test automatic conversion to perl list
my @node_list = $doc->getElementsByTagName('person');
ok(scalar @node_list == 6);

# test that we can still get a DOMNodeList object
my $dom_node_list = $doc->getElementsByTagName('person');
ok(UNIVERSAL::isa($dom_node_list,'XML::Xerces::DOMNodeList'));

ok($dom_node_list->getLength() == scalar @node_list);

for (my $i=0;$i<scalar @node_list;$i++) {
  ok($node_list[$i] == $dom_node_list->item($i));
}
