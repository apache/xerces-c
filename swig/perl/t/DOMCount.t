# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMCount.t

######################### We start with some black magic to print on failure.

# Change 1..1 below to 1..last_test_to_print .
# (It may become useful if the test is moved to ./t subdirectory.)

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 2;
use Config;

use lib 't';
use TestUtils qw($SAMPLE_DIR);
use vars qw($loaded $file);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

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

$file = '.domprint.xml';

open(OUT,">$file") or die "Couldn't open $file from writing";
print OUT $document;
close(OUT);

my $perl = $Config{startperl};
$perl =~ s/^\#!//;
my $output = `$perl -Mblib $SAMPLE_DIR/DOMCount.pl $file 2>/dev/null`;
# print STDERR "Out <$output>\n";
$output =~ /(\d+) elems/;
ok($1 == 10, 'element count');

END {unlink $file;}
