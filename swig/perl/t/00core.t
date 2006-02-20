# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# XercesDOMParser.t'

######################### We start with some black magic to print on failure.

END {fail("module loaded") unless $loaded;}

use Carp;
# use blib;
use Test::More tests => 2;
use XML::Xerces;

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME $PERSONAL_NO_DOCTYPE $PERSONAL $DOM);
use vars qw($loaded);
use strict;

$loaded = 1;
pass("module loaded");

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

######################### End of black magic.

my $document = q[<?xml version="1.0" encoding="UTF-8"?>
<contributors>
	<person Role="manager">
		<name>Mike Pogue</name>
		<email>mpogue@us.ibm.com</email>
	</person>
	<person Role="developer">
		<name>Tom Watson</name>
		<email>rtwatson@us.ibm.com</email>
	</person>
	<person Role="tech writer">
		<name>Susan Hardenbrook</name>
		<email>susanhar@us.ibm.com</email>
	</person>
</contributors>];

my $parser = XML::Xerces::XercesDOMParser->new();
isa_ok($parser,"XML::Xerces::AbstractDOMParser");

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
