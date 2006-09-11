# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMEntity.t'

######################### Begin module loading

# use blib;
use Test::More tests => 4;
BEGIN { use_ok("XML::Xerces::DOM") };

use lib 't';
use TestUtils qw($DOM);
use strict;

######################### Begin Test

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
