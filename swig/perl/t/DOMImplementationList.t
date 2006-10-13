# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMImplementationList.t'

######################### Begin module loading

# use blib;
use Test::More tests => 6;
BEGIN { use_ok("XML::Xerces::DOM") };

use strict;

######################### Begin Test

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

my $list = XML::Xerces::DOMImplementationRegistry::getDOMImplementationList('');
isa_ok($list,"XML::Xerces::DOMImplementationList");
ok(($list->getLength() > 0),
   'testing getLength()');
my $domImpl = $list->item(0);
ok((defined $domImpl),
   'testing item()');
isa_ok($domImpl,"XML::Xerces::DOMImplementation");
isa_ok($domImpl,"XML::Xerces::DOMImplementationLS");

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
