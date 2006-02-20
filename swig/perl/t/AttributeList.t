# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# AttributeList.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
use blib;
use XML::Xerces;
use Test::More tests => 11;
use Config;

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

package MyDocumentHandler;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlDocumentHandler);

sub start_element {
  my ($self,$name,$attrs) = @_;
  if ($name eq 'foo') {
    $self->{test} = $attrs->getLength();
  }
}
sub end_element {}

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
  <foo xmlns:$ns="$url"
   Role="manager" $ns:$local="$value">
  </foo>
</bar>];

my $SAX = XML::Xerces::SAXParser->new();
my $DOCUMENT_HANDLER = MyDocumentHandler->new();
my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
$SAX->setDocumentHandler($DOCUMENT_HANDLER);
$SAX->setErrorHandler($ERROR_HANDLER);

# test getLength
my $is = XML::Xerces::MemBufInputSource->new($document);
$DOCUMENT_HANDLER->{test} = '';
eval {$SAX->parse($is)};
XML::Xerces::error($@) if $@;

ok($DOCUMENT_HANDLER->{test} == 3,
  "getLength");
$DOCUMENT_HANDLER->{test} = '';

# we want to avoid a bunch of warnings about redefining
# the start_element method, so we turn off warnings
$^W = 0;

# test getName
*MyDocumentHandler::start_element = sub {
  my ($self,$name,$attrs) = @_;
  if ($name eq 'foo') {
    $self->{test} = $attrs->getName(2);
  }
};
$DOCUMENT_HANDLER->{test} = '';
$SAX->parse($is);
ok($DOCUMENT_HANDLER->{test} eq "$ns:$local",
  "getName");

# test getValue
*MyDocumentHandler::start_element = sub {
  my ($self,$name,$attrs) = @_;
  if ($name eq 'foo') {
    $self->{test} = $attrs->getValue("$ns:$local");
  }
};
$DOCUMENT_HANDLER->{test} = '';
$SAX->parse($is);
ok($DOCUMENT_HANDLER->{test} eq $value,
  "getValue - string");

# test overloaded getValue
*MyDocumentHandler::start_element = sub {
  my ($self,$name,$attrs) = @_;
  if ($name eq 'foo') {
    $self->{test} = $attrs->getValue(2);
  }
};
$DOCUMENT_HANDLER->{test} = '';
$SAX->parse($is);
# print STDERR "<$DOCUMENT_HANDLER->{test}>" , "\n";
# print STDERR "<$value>" , "\n";
ok($DOCUMENT_HANDLER->{test} eq $value,
  "getValue - int");

# test to_hash()
*MyDocumentHandler::start_element = sub {
  my ($self,$name,$attrs) = @_;
  if ($name eq 'foo') {
    $self->{test} = {$attrs->to_hash()};
  }
};
$DOCUMENT_HANDLER->{test} = '';
$SAX->parse($is);
my $hash_ref = $DOCUMENT_HANDLER->{test};
isa_ok($hash_ref, 'HASH',
      "to_hash()");
is(keys %{$hash_ref}, 3,
      "to_hash()");
is($hash_ref->{"$ns:$local"}, $value,
      "to_hash()");

my $document2 = qq[<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
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
  return XML::Xerces::MemBufInputSource->new('');
}

package main;
my $is2 = eval{XML::Xerces::MemBufInputSource->new($document2)};
XML::Xerces::error($@) if $@;
$SAX->setEntityResolver(MyEntityResolver->new());

# test overloaded getType
*MyDocumentHandler::start_element = sub {
  my ($self,$name,$attrs) = @_;
  if ($name eq 'foo') {
    $self->{test} = $attrs->getType(0);
  }
};
$DOCUMENT_HANDLER->{test} = '';

$SAX->parse($is2);
is($DOCUMENT_HANDLER->{test}, 'ID',
   "getType - int");

# test getType
*MyDocumentHandler::start_element = sub {
  my ($self,$name,$attrs) = @_;
  if ($name eq 'foo') {
    $self->{test} = $attrs->getType('id');
  }
};
$DOCUMENT_HANDLER->{test} = '';
$SAX->parse($is2);
is($DOCUMENT_HANDLER->{test}, 'ID',
   "getType");

my $document3 = qq[<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<!DOCTYPE bar SYSTEM "foo.dtd" [
<!ELEMENT bar (foo)>
<!ELEMENT foo EMPTY>
<!ATTLIST foo id ID #REQUIRED>
<!ATTLIST foo role CDATA #REQUIRED>
]>
<bar>
  <foo id='baz' role="manager"/>
</bar>];

$is2 = eval{XML::Xerces::MemBufInputSource->new($document3)};

TODO : {
  todo_skip "blank documents segfault the entity resolver", 1;

  $SAX->parse($is2);
  pass("blank document does not segfault the entity resolver");
}

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
