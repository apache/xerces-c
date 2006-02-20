# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# UTF8.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;

use blib;
use utf8;
use XML::Xerces;
use Test::More tests => 35119;
use Config;

use lib 't';
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

my $DOM1 = new XML::Xerces::XercesDOMParser;
my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
$DOM1->setErrorHandler($ERROR_HANDLER);

# check that an element can have a digit if a valid character comes first
eval {
  $DOM1->parse('t/letter.xml');
};
if ($@) {
  if (ref($@)) {
    if ($@->isa('XML::Xerces::XMLException')) {
      die "Couldn't open letter.xml: ", $@->getMessage();
    } elsif ($@->isa('XML::Xerces::DOMException')) {
      die "Couldn't open letter.xml: msg=<$@->{msg}>, code=$@->{code}";
    }
  }
}

my $doc1 = $DOM1->getDocument();
my ($digit_node) = $doc1->getElementsByTagName('digit');
my @digits;
foreach my $range_node ($digit_node->getElementsByTagName('range')) {
  my $low = hex($range_node->getAttribute('low'));
  my $high = hex($range_node->getAttribute('high'));
  push(@digits,$low..$high);
}
foreach my $single_node ($digit_node->getElementsByTagName('single')) {
  my $value = hex($single_node->getAttribute('value'));
  push(@digits,$value);
}
@digits = map {chr($_)} @digits;
foreach my $char (@digits) {
  eval {
    my $el = $doc1->createElement("_$char");
  };
  if ($@) {
    if (ref $@) {
      print STDERR "Error code: $@->{code}\n";
    } else {
      print STDERR $@;
    }
  }
  ok(!$@) or
    diag(sprintf("char: <0x%.4X>\n",ord($char)));
}

my ($extender_node) = $doc1->getElementsByTagName('extender');
my @extenders;
foreach my $range_node ($extender_node->getElementsByTagName('range')) {
  my $low = hex($range_node->getAttribute('low'));
  my $high = hex($range_node->getAttribute('high'));
  push(@extenders,$low..$high);
}
foreach my $single_node ($extender_node->getElementsByTagName('single')) {
  my $value = hex($single_node->getAttribute('value'));
  push(@extenders,$value);
}
@extenders = map {chr($_)} @extenders;
foreach my $char (@extenders) {
  eval {
    my $el = $doc1->createElement("_$char");
  };
  if ($@) {
    if (ref $@) {
      diag("Error code: $@->{code}\n");
    } else {
      diag($@);
    }
  }
  ok(!$@) or
    diag(sprintf("char: <0x%.4X>\n",ord($char)));
}

my ($combining_char_node) = $doc1->getElementsByTagName('combiningchar');
my @combining_chars;
foreach my $range_node ($combining_char_node->getElementsByTagName('range')) {
  my $low = hex($range_node->getAttribute('low'));
  my $high = hex($range_node->getAttribute('high'));
  push(@combining_chars,$low..$high);
}
foreach my $single_node ($combining_char_node->getElementsByTagName('single')) {
  my $value = hex($single_node->getAttribute('value'));
  push(@combining_chars,$value);
}
@combining_chars = map {chr($_)} @combining_chars;
foreach my $char (@combining_chars) {
  eval {
    my $el = $doc1->createElement("_$char");
  };
  if ($@) {
    if (ref $@) {
      diag("Error code: $@->{code}\n");
    } else {
      diag($@);
    }
  }
  ok(!$@) or
    diag(sprintf("char: <0x%.4X>\n",ord($char)));
}

my ($letter_node) = $doc1->getElementsByTagName('letter');
my @letters;
foreach my $range_node ($letter_node->getElementsByTagName('range')) {
  my $low = hex($range_node->getAttribute('low'));
  my $high = hex($range_node->getAttribute('high'));
  push(@letters,$low..$high);
}
foreach my $single_node ($letter_node->getElementsByTagName('single')) {
  my $value = hex($single_node->getAttribute('value'));
  push(@letters,$value);
}
@letters = map {chr($_)} @letters;
# $XML::Xerces::DEBUG_UTF8_IN = 1;
# $XML::Xerces::DEBUG_UTF8_OUT = 1;
foreach my $char (@letters) {
  eval {
    my $el = $doc1->createElement("$char");
  };
  if ($@) {
    if (ref $@) {
      diag("Error code: $@->{code}\n");
    } else {
      diag($@);
    }
  }
  ok(!$@) or
    diag(sprintf("char: <0x%.4X>\n",ord($char)));
}

my ($ideograph_node) = $doc1->getElementsByTagName('ideographic');
my @ideographs;
foreach my $range_node ($ideograph_node->getElementsByTagName('range')) {
  my $low = hex($range_node->getAttribute('low'));
  my $high = hex($range_node->getAttribute('high'));
  push(@ideographs,$low..$high);
}
foreach my $single_node ($ideograph_node->getElementsByTagName('single')) {
  my $value = hex($single_node->getAttribute('value'));
  push(@ideographs,$value);
}
@ideographs = map {chr($_)} @ideographs;
# $XML::Xerces::DEBUG_UTF8_IN = 1;
# $XML::Xerces::DEBUG_UTF8_OUT = 1;
foreach my $char (@ideographs) {
  eval {
    my $el = $doc1->createElement("$char");
  };
  if ($@) {
    if (ref $@) {
      diag("Error code: $@->{code}\n");
    } else {
      diag($@);
    }
  }
  ok(!$@) or
    diag(sprintf("char: <0x%.4X>\n",ord($char)));
}

# $XML::Xerces::DEBUG_UTF8_IN = 0;
# $XML::Xerces::DEBUG_UTF8_OUT = 0;

