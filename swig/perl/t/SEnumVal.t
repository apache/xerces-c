# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# SEnumVal.t'

######################### Begin module loading

# use blib;
use Test::More tests => 12;
BEGIN { use_ok(XML::Xerces::DOM) };
use Config;

use lib 't';
use TestUtils qw($PERSONAL_SCHEMA_FILE_NAME $SAMPLE_DIR);

use strict;

######################### Begin Test

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

my $perl = $Config{startperl};
$perl =~ s/^\#!//;
my $output;
if ($^O eq 'MSWin32') {
  $output = `$perl -Mblib $SAMPLE_DIR/SEnumVal.pl $PERSONAL_SCHEMA_FILE_NAME 2>NUL`;
} else {
  $output = `$perl -Mblib $SAMPLE_DIR/SEnumVal.pl $PERSONAL_SCHEMA_FILE_NAME 2>/dev/null`;
}

my @lines = split("\n", $output);

my %elements;
my %attributes;
my $curr_elem;
foreach my $line (@lines) {
  if ($line =~ /^Element Name: (\w+), Content Model: (.*)$/) {
    $curr_elem = $1;
    $elements{$curr_elem} = $2;
  } elsif ($line =~ /^\s+attribute Name: (\w+), Type: (.*)$/) {
    $attributes{$curr_elem}->{$1} = $2;
  }
}
# diag("Found elements: " . scalar @elements);
is((scalar keys %elements), 8,
   'number of elements');

ok(exists $elements{name},
  "found 'name'")
  && is($elements{name},'(family,given)',
	"'name' has correct content-model");

ok(exists $elements{person},
  "found 'person'")
  && is($elements{person},'((name,email,url),link)',
	"'person' has correct content-model");

ok(exists $elements{url},
  "found 'url'")
  && is($elements{url},'EMPTY',
	"'url' has correct content-model");

is((scalar keys %attributes), 3,
   'found correct elements with attributes');

is((scalar keys %{$attributes{person}}), 4,
   'person has correct number of attributes');

is((scalar keys %{$attributes{url}}), 1,
   'url has correct number of attributes');

is((scalar keys %{$attributes{link}}), 2,
   'link has correct number of attributes');

  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
XML::Xerces::XMLPlatformUtils::Terminate();


