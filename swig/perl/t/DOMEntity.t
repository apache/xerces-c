# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMEntity.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 4;

use lib 't';
use TestUtils qw($DOM);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

my $document = <<'EOT';
<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<!DOCTYPE foo [
<!ENTITY data2    "DATA">
<!ENTITY data   "DATA">
<!ENTITY bar    "BAR">
<!ELEMENT  foo        ANY>
]>
<foo>This is a test &data; of entities</foo>
EOT

$DOM->setCreateEntityReferenceNodes(1);
$DOM->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Never);
my $is = eval{XML::Xerces::MemBufInputSource->new($document)};
XML::Xerces::error($@) if $@;

eval{$DOM->parse($is)};
XML::Xerces::error($@) if $@;

my $doc = $DOM->getDocument();
my $doctype = $doc->getDoctype();

ok(defined $doctype);

  # get the single <element> node
my %ents = $doctype->getEntities();
my $fail;
ok(exists $ents{data} && $ents{data} eq 'DATA');

SKIP: {

  skip('entities still not working', 1);

  ok(exists $ents{bar} && $ents{bar} eq 'BAR');
}
