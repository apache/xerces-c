# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# Attributes.t'

######################### We start with some black magic to print on failure.

# Change 1..1 below to 1..last_test_to_print .
# (It may become useful if the test is moved to ./t subdirectory.)

END {ok(0) unless $loaded;}

use Carp;
use blib;
use XML::Xerces;
use Test::More tests => 19;
use Config;

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME);
use vars qw($i $loaded);
use strict;

$loaded = 1;
$i = 1;
ok($loaded, "module loaded");

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):

package MyContentHandler;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlContentHandler);

sub start_element {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = $attrs->getLength();
  }
}
sub end_element {
}
sub characters {
}
sub ignorable_whitespace {
}

package main;
my $url = 'http://www.boyscouts.org/';
my $local = 'Rank';
my $ns = 'Scout';
my $value = 'eagle scout';
my $document = qq[<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<bar>
  <foo xmlns:Scout="$url"
   Role="manager" $ns:$local="$value">
  </foo>
</bar>];

my $SAX2 = XML::Xerces::XMLReaderFactory::createXMLReader();
my $CONTENT_HANDLER = MyContentHandler->new();
my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
$SAX2->setContentHandler($CONTENT_HANDLER);
$SAX2->setErrorHandler($ERROR_HANDLER);

# test getLength
my $is = XML::Xerces::MemBufInputSource->new($document);
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is);
is($CONTENT_HANDLER->{test}, 2,
  "getLength");
$CONTENT_HANDLER->{test} = '';

# we want to avoid a bunch of warnings about redefining
# the start_element method, so we turn off warnings
$^W = 0;

# test getURI
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = $attrs->getURI(1);
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is);
is($CONTENT_HANDLER->{test}, $url,
   "getURI");

# test getLocalName
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = $attrs->getLocalName(1);
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is);
is($CONTENT_HANDLER->{test}, $local,
  "getLocalName");

# test getQName
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = $attrs->getQName(1);
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is);
is($CONTENT_HANDLER->{test}, "$ns:$local",
  "getQName");

# test getIndex
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = $attrs->getIndex("$ns:$local");
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is);
is($CONTENT_HANDLER->{test}, 1,
  "getIndex");

# test getValue
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = $attrs->getValue($url,$local);
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is);
is($CONTENT_HANDLER->{test}, $value,
  "getValue(url,localname)");

# test overloaded getValue
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = $attrs->getValue(1);
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is);
is($CONTENT_HANDLER->{test}, $value,
  "getValue(int)");

# test overloaded getValue
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = $attrs->getValue($url, $local);
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is);
is($CONTENT_HANDLER->{test}, $value,
  "getValue");

# test to_hash()
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = {$attrs->to_hash()};
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is);
my $hash_ref = $CONTENT_HANDLER->{test};
isa_ok($hash_ref, 'HASH');
is(keys %{$hash_ref}, 2,
  "found all attrs");
is($hash_ref->{"$ns:$local"}{value}, $value,
  "found attr value");
is($hash_ref->{"$ns:$local"}{URI}, $url,
  "found attr url");

$document = qq[<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<!DOCTYPE bar [
<!ELEMENT bar (foo)>
<!ELEMENT foo EMPTY>
<!ATTLIST foo id ID #REQUIRED>
<!ATTLIST foo role CDATA #REQUIRED>
]>
<bar>
  <foo id='baz' role="manager"/>
</bar>];

package MyEntityResolver;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlEntityResolver);

sub new {
  return bless {}, shift;
}

sub resolve_entity {
  my ($self,$pub,$sys) = @_;
print STDERR "resolving entity, pub: $pub, sys: $sys\n";
  return XML::Xerces::MemBufInputSource->new('');
}

package main;
my $is2 = XML::Xerces::MemBufInputSource->new($document);
$SAX2->setEntityResolver(MyEntityResolver->new());

# test overloaded getType
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = $attrs->getType(0);
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is2);
is($CONTENT_HANDLER->{test}, 'ID',
  "getType(int)");

# test overloaded getType
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = $attrs->getType('', 'id');
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is2);
is($CONTENT_HANDLER->{test}, 'ID',
  "getType(uri,local)");

# test getType
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = $attrs->getType('','id');
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is2);
is($CONTENT_HANDLER->{test}, 'ID',
  "getType");

# test type field of to_hash()
*MyContentHandler::start_element = sub {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  if ($localname eq 'foo') {
    $self->{test} = {$attrs->to_hash()};
  }
};
$CONTENT_HANDLER->{test} = '';
$SAX2->parse($is2);
$hash_ref = $CONTENT_HANDLER->{test};
isa_ok($hash_ref, 'HASH');
is(keys %{$hash_ref}, 2,
  "to_hash() found all attrs");
is($hash_ref->{id}{type}, 'ID',
  "to_hash() found correct type");

