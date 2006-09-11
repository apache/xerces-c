# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# SAXParser.t'

######################### Begin module loading

# use blib;
use Test::More tests => 12;
BEGIN { use_ok("XML::Xerces") };

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME);
use strict;

######################### Begin Test

my $document = q[<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
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

package MyDocumentHandler;
use strict;
use base qw(XML::Xerces::PerlDocumentHandler);

sub start_element {
  my ($self, $name, @attrs) = @_;
  $self->{elements}++;
  $self->{element}->{$name} = \@attrs;
}
sub characters {
  my ($self,$str,$len) = @_;
  $self->{chars} += $len;
}
sub ignorable_whitespace {
  my ($self,$str,$len) = @_;
  $self->{ws} += $len;
}

sub reset_document {
  my ($self) = @_;
  $self->{ws} = 0;
  $self->{chars} = 0;
  $self->{elements} = 0;
}

package main;
my $SAX = XML::Xerces::SAXParser->new();
my $DOCUMENT_HANDLER = MyDocumentHandler->new();
my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();

$SAX->setDocumentHandler($DOCUMENT_HANDLER);
$SAX->setErrorHandler($ERROR_HANDLER);

# reset the counts
$DOCUMENT_HANDLER->reset_document();

$SAX->parse(XML::Xerces::MemBufInputSource->new($document, 'foo'));
is($DOCUMENT_HANDLER->{elements}, 10,'elements');
is($DOCUMENT_HANDLER->{chars}, 141,'chars');
is($DOCUMENT_HANDLER->{ws}, 0,'ws');

# test the overloaded parse version
$SAX->parse($PERSONAL_FILE_NAME);
pass('successful parse');

# reset the counts
$DOCUMENT_HANDLER->reset_document();

# test the progressive parsing interface
my $token = XML::Xerces::XMLPScanToken->new();
$SAX->parseFirst($PERSONAL_FILE_NAME,$token);
while ($SAX->parseNext($token)) {
  # do nothing
}
pass('successful progressive parse');
ok($DOCUMENT_HANDLER->{elements} == 37, 'element count after progressive parse');

# reset the counts
$DOCUMENT_HANDLER->reset_document();

$token = XML::Xerces::XMLPScanToken->new();
$SAX->parseFirst($PERSONAL_FILE_NAME,$token);
while ($SAX->parseNext($token)) {
  last if $DOCUMENT_HANDLER->{elements} == 10;
}
pass('early exit from progressive parse');
ok($DOCUMENT_HANDLER->{elements} == 10, 'element count after early exit from progressive parse');

# test that we can reuse the parse again and again
$document = <<\END;
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
  die();
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
  die();
}
1;

package main;
$token = XML::Xerces::XMLPScanToken->new();
$SAX->setErrorHandler(MyErrorHandler->new());
$::error = '';
eval {
  $SAX->parseFirst(XML::Xerces::MemBufInputSource->new($document),$token);
  while ($SAX->parseNext($token)) {
    # do nothing
  }
};
ok($::error,'fatal error in progressive parse')
  or diag("Here's the error: $@");

# reset the state for a new parse
$::error = '';
$SAX->parseReset($token);
$SAX->setErrorHandler(XML::Xerces::PerlErrorHandler->new());

eval {$SAX->parse($PERSONAL_FILE_NAME)};
ok((not $@),'successful parse after fatal error');

eval {$SAX->parse(XML::Xerces::MemBufInputSource->new($document))};
ok($@,'fatal error in parse');
