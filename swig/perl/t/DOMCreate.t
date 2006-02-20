# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMCreate.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 2;
use Config;

use lib 't';
use TestUtils qw($SAMPLE_DIR);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

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

ok($document eq $output, 'document eq output');
diag("Found output\n[$output]")
  if $document ne $output;
