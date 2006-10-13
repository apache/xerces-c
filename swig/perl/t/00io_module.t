# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# 00io_module.t'

######################### Begin module loading

# use blib;
use Test::More tests => 3;
BEGIN { use_ok(XML::Xerces::IO) };

use strict;

######################### Begin Test

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

pass("module initialized");

  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
XML::Xerces::XMLPlatformUtils::Terminate();

pass("module terminated");


