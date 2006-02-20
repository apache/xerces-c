# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# SAXCount.t

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 5;
use Config;

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME $SAMPLE_DIR);
use vars qw($loaded $file);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

my $perl = $Config{startperl};
$perl =~ s/^\#!//;
my @output = split(/\n/,`$perl -Mblib $SAMPLE_DIR/SAXCount.pl $PERSONAL_FILE_NAME 2>/dev/null`);
$output[1] =~ /\s(\d+)/;
ok($1 == 37);
$output[2] =~ /\b(\d+)\b/;
ok($1 == 12);
$output[3] =~ /\b(\d+)\b/;
ok($1 == 134);
$output[4] =~ /\b(\d+)\b/;
ok($1 == 134);
