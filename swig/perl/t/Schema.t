# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# Schema.t

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 3;

use lib 't';
use TestUtils qw($PERSONAL_SCHEMA_FILE_NAME);
use vars qw($loaded $file);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

my $dom = XML::Xerces::XercesDOMParser->new();
my $handler = XML::Xerces::PerlErrorHandler->new();
my $entity_resolver = TestUtils->new();
$dom->setEntityResolver($entity_resolver);
$dom->setDoSchema(1);
$dom->setDoNamespaces(1);
$dom->setErrorHandler($handler);

# test a valid file
eval {
  $dom->parse($PERSONAL_SCHEMA_FILE_NAME);
};
ok(!$@);

# test an invalid file
open(IN,$PERSONAL_SCHEMA_FILE_NAME)
  or die "Couldn't open $PERSONAL_SCHEMA_FILE_NAME for reading";
my $buf;
while (<IN>) {
  if (m|</personnel>|) {
    s|</personnel>|</foo>|;
  }
  $buf .= $_;
}

eval {
  $dom->parse(XML::Xerces::MemBufInputSource->new($buf));
};
ok($@);
