# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMPrint.t

######################### Begin module loading

# use blib;
use Test::More tests => 1;
use Config;

use lib 't';
use TestUtils qw($SAMPLE_DIR);
use vars qw($file);
use strict;

######################### Begin Test

my $document = q[<?xml version="1.0" encoding="iso-8859-1" standalone="yes" ?>
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

</contributors>
];

$file = '.domprint.xml';
open(OUT,">$file") or die "Couldn't open $file from writing";
print OUT $document;
close(OUT);

my $perl = $Config{startperl};
$perl =~ s/^\#!//;
my $output;
if ($^O eq 'MSWin32') {
  $output = `$perl $SAMPLE_DIR/DOMPrint.pl $file 2>NUL`;
} else {
  $output = `$perl $SAMPLE_DIR/DOMPrint.pl $file 2>/dev/null`;
}

ok($document eq $output)
  or diag("Expected: [$document]\nFound: [$output]\n");

END {unlink $file;}
