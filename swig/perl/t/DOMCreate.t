# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMCreate.t'

######################### Begin module loading

use Carp;
# use blib;
use Test::More tests => 1;
use Config;

use lib 't';
use TestUtils qw($SAMPLE_DIR);
use strict;

######################### Begin Test

my $document = q[<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<!DOCTYPE contributors SYSTEM "contributors.dtd">
<contributors xmlns="contributors">

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


my $perl = $Config{startperl};
$perl =~ s/^\#!//;
my $cmd = "$perl -Mblib $SAMPLE_DIR/DOMCreate.pl 2>/dev/null";
# print STDERR "Running: $cmd\n";
my $output = `$cmd`;

is($document, $output,
   'document eq output') or diag("Found output\n[$output]");
