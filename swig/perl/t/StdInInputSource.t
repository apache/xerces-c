# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# StdInInputSource.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 3;

use lib 't';
use TestUtils qw($DOM
		 $PERSONAL_NO_DOCTYPE
		 $PERSONAL_NO_DOCTYPE_FILE_NAME);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

open(STDIN, $PERSONAL_NO_DOCTYPE_FILE_NAME)
  or die "Couldn't open $PERSONAL_NO_DOCTYPE_FILE_NAME for reading";
my $is = XML::Xerces::StdInInputSource->new();
ok(UNIVERSAL::isa($is,'XML::Xerces::InputSource')
   && $is->isa('XML::Xerces::StdInInputSource')
  );

$DOM->parse($is);
my $serialize = $DOM->getDocument->serialize;
ok($serialize eq $PERSONAL_NO_DOCTYPE);
