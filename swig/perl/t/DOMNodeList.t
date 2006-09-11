# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl DOMNodeList.t'

######################### Begin module loading

# use blib;
use Test::More tests => 10;
BEGIN { use_ok("XML::Xerces::DOM") };

use lib 't';
use TestUtils qw($DOM $PERSONAL_FILE_NAME);
use strict;

######################### Begin Test

$DOM->parse( new XML::Xerces::LocalFileInputSource($PERSONAL_FILE_NAME) );
my $doc = $DOM->getDocument();

# test automatic conversion to perl list
my @node_list = $doc->getElementsByTagName('person');
is(scalar @node_list, 6,
  'automatic conversion from NodeList to perl array');

# test that we can still get a DOMNodeList object
my $dom_node_list = $doc->getElementsByTagName('person');
isa_ok($dom_node_list,'XML::Xerces::DOMNodeList');

is($dom_node_list->getLength(), scalar @node_list,
  'NodeList length eq to perl array length');

# this test's the overloaded == operator for DOMNode
for (my $i=0;$i<scalar @node_list;$i++) {
  ok($node_list[$i] == $dom_node_list->item($i),
     "perl array element $i is the same Node as NodeList element $i");
}
