# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# PSVI.t'

######################### Begin module loading

# use blib;
use Test::More tests => 5;
BEGIN {use_ok("XML::Xerces")};

use strict;

######################### Begin Test

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

# once the unicode constants were not being properly exported
# these tests guard against that happening again
is($XML::Xerces::PSVIUni::fgAbstract, 'psv:abstract',
   'PSVIUni constants exported');
is($XML::Xerces::PSVIUni::fgAg, 'ag',
   'PSVIUni constants exported');

TODO: {
  local $TODO = 'SWIG is exporting all class variables into top-level namespace';
  ok((not defined $XML::Xerces::PSVIUni_fgAbstract),
     'PSVIUni constants not in XML::Xerces namespace');
  ok((not defined $XML::Xerces::PSVIUni_fgAg),
     'PSVIUni constants not in XML::Xerces namespace');
}

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
