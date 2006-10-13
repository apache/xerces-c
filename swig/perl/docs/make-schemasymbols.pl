#!/usr/bin/perl -w
use strict;

use blib;
use XML::Xerces;

my $max_len = 0;
foreach my $varname (keys %XML::Xerces::SchemaSymbols::) {
  next unless $varname =~ /^fg/;
  $max_len = (length  $varname > $max_len) ? length $varname : $max_len;
}

foreach my $varname (sort keys %XML::Xerces::SchemaSymbols::) {
  next unless $varname =~ /^fg/;
  my $len = length  $varname;
  my $diff = $max_len - $len;
  my $spacer = ' ' x $diff;
  my $val = ${$XML::Xerces::SchemaSymbols::{$varname}};
  if (ref $val eq 'ARRAY') {
    my $tmp_val = join(':', @{$val});
    $val = $tmp_val;
  }
  print STDOUT "$varname $spacer '$val'\n";
}

print STDERR "Finished\n";
