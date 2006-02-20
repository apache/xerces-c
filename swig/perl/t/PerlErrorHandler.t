# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# PerlErrorHandler.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;

# use blib;
use XML::Xerces;
use Test::More tests => 6;

use lib 't';
use TestUtils qw($PERSONAL);
use subs qw(warning error fatal_error);
use vars qw($error $loaded);

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

my $document = <<\END;
<?xml version="1.0" encoding="iso-8859-1" standalone="no"?>

<!-- @version: -->
<personnel>

  <person id="Big.Boss">
    <name><family>Boss</family> <given>Big</given></name>
    <email>chief@foo.com</email>
    <link subordinates="one.worker two.worker three.worker four.worker five.worker"/>
  </person>

  <person id="one.worker">
    <name><family>Worker</family> <given>One</given></name>
    <email>one@foo.com</email>
    <link manager="Big.Boss"/>
  </person>

  <foo id="two.worker">
    <name><family>Worker</family> <given>Two</given></name>
    <email>two@foo.com</email>
    <link manager="Big.Boss"/>
  </person>

</personnel>
END

package MyErrorHandler;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlErrorHandler);
sub warning {
  my $LINE = $_[1]->getLineNumber;
  my $COLUMN = $_[1]->getColumnNumber;
  my $MESSAGE = $_[1]->getMessage;
  $::error = <<"EOE";
WARNING:
LINE:    $LINE
COLUMN:  $COLUMN
MESSAGE: $MESSAGE
EOE
  die "\n";
}

sub error {
  my $LINE = $_[1]->getLineNumber;
  my $COLUMN = $_[1]->getColumnNumber;
  my $MESSAGE = $_[1]->getMessage;
  $::error = <<"EOE";
ERROR:
LINE:    $LINE
COLUMN:  $COLUMN
MESSAGE: $MESSAGE
EOE
  die "\n";
}

sub fatal_error {
  my $LINE = $_[1]->getLineNumber;
  my $COLUMN = $_[1]->getColumnNumber;
  my $MESSAGE = $_[1]->getMessage;
  $::error = <<"EOE";
FATAL ERROR:
LINE:    $LINE
COLUMN:  $COLUMN
MESSAGE: $MESSAGE
EOE
  die "\n";
}
1;

package main;

{
  $error = "";

  my $dom = XML::Xerces::XercesDOMParser->new();

  my $error_handler = MyErrorHandler->new();
  $dom->setErrorHandler($error_handler);

  eval {
    $dom->parse(XML::Xerces::MemBufInputSource->new($document, 'foo') );
  };

  my $expected_error = <<EOE;
FATAL ERROR:
LINE:    22
COLUMN:  5
MESSAGE: Expected end of tag 'foo'
EOE
  ok($expected_error eq $error)
    or diag("Expected error: [$expected_error]\nFound error: [$error]\n");

}

{
  $error = "";

  my $dom = XML::Xerces::XercesDOMParser->new();

  my $error_handler = MyErrorHandler->new();
  $dom->setErrorHandler($error_handler);

  $dom->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Always);
  eval {
    $dom->parse(XML::Xerces::MemBufInputSource->new($document, 'foo') );
  };

  my $expected_error = <<EOE;
ERROR:
LINE:    4
COLUMN:  11
MESSAGE: Unknown element 'personnel'
EOE
  ok($expected_error eq $error);
}

# test redefining the handler
{
  $error = "";

  my $dom = XML::Xerces::XercesDOMParser->new();

  my $error_handler = MyErrorHandler->new();
  my $error_handler2 = MyErrorHandler->new();
  my $tmp = $dom->setErrorHandler($error_handler);
  ok(!defined $tmp);

  $tmp = $dom->setErrorHandler($error_handler2);
  SKIP : {
    skip "temporarily not returning old handler", 1 unless defined($tmp);
    ok(defined $tmp);
  }

  $dom->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Always);
  eval {
    $dom->parse(XML::Xerces::MemBufInputSource->new($document, 'foo') );
  };

  my $expected_error = <<EOE;
ERROR:
LINE:    4
COLUMN:  11
MESSAGE: Unknown element 'personnel'
EOE
  ok($expected_error eq $error);
}

