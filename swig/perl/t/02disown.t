# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# 01disown.t'

######################### Begin module loading

# use blib;
use Test::More tests => 5;
BEGIN { use_ok(XML::Xerces::DOM) };

use strict;

######################### Begin Test

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

my $DOM = XML::Xerces::XercesDOMParser->new();
is((scalar keys %XML::Xerces::XercesDOMParser::OWNER), 1,
   'creating parser sets key in %OWNER');

$DOM = undef;
is((scalar keys %XML::Xerces::XercesDOMParser::OWNER), 0,
   'undefining parser deletes key in %OWNER');

my $valToUse = XML::Xerces::DTDValidator->new();
is((scalar keys %XML::Xerces::DTDValidator::OWNER), 1,
   'creating DTDValidator sets key in %OWNER');

$DOM = XML::Xerces::XercesDOMParser->new($valToUse);
is((scalar keys %XML::Xerces::DTDValidator::OWNER), 0,
   'passing DTDValidator to parser constructor deletes key in %OWNER');

  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
XML::Xerces::XMLPlatformUtils::Terminate();


