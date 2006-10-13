# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# 02handler.t'

######################### Begin module loading

# use blib;
use Test::More tests => 3;

BEGIN{use_ok('XML::Xerces')};

use strict;

######################### Begin Test

for (1..2000) {
  my $ER = XML::Xerces::PerlEntityResolverHandler->new();
}
pass("deleted entity resolver");

for (1..2000) {
  my $EH = XML::Xerces::PerlErrorCallbackHandler->new();
}
pass("deleted error handler");
