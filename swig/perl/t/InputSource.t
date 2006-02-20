# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# InputSource.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 5;

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):

my $is = XML::Xerces::LocalFileInputSource->new($PERSONAL_FILE_NAME);
ok($is->getSystemId() eq $PERSONAL_FILE_NAME);

$is->setSystemId('foo');
ok($is->getSystemId() eq 'foo');

$is->setPublicId('foo');
ok($is->getPublicId() eq 'foo');

$is->setEncoding('foo');
ok($is->getEncoding() eq 'foo');
