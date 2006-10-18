# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# SAX2Count.t`

######################### Begin module loading

# use blib;
use Test::More tests => 5;
BEGIN { use_ok("XML::Xerces::SAX") };

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME $SAMPLE_DIR);
use strict;
use Config;

######################### Begin Test

my $perl = $Config{startperl};
$perl =~ s/^\#!//;
my @output;
if ($^O eq 'MSWin32') {
  @output = split(/\n/,`$perl $SAMPLE_DIR/SAX2Count.pl $PERSONAL_FILE_NAME 2>NUL`);
} else {
  @output = split(/\n/,`$perl $SAMPLE_DIR/SAX2Count.pl $PERSONAL_FILE_NAME 2>/dev/null`);
}
$output[1] =~ /\s(\d+)/;
is($1, 37,
   'elems');
$output[2] =~ /\b(\d+)\b/;
is($1, 12,
  'attrs');
$output[3] =~ /\b(\d+)\b/;
is($1, 134,
   'whitespace');
$output[4] =~ /\b(\d+)\b/;
is($1, 134,
   'characters');
