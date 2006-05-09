# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# SAX2XMLReader.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces qw(error);
use Test::More tests => 26;
use Config;

use lib 't';
use TestUtils qw($PERSONAL_FILE_NAME $SCHEMA_FILE_NAME $PERSONAL_SCHEMA_INVALID_FILE_NAME);
use vars qw($loaded $error);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

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

package MyContentHandler;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlContentHandler);

sub start_element {
  my $self = shift;
  $self->{elements}++;
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
my $SAX = XML::Xerces::XMLReaderFactory::createXMLReader();
my $CONTENT_HANDLER = MyContentHandler->new();
$SAX->setContentHandler($CONTENT_HANDLER);
$SAX->setErrorHandler(XML::Xerces::PerlErrorHandler->new());
eval {
    $SAX->setFeature($XML::Xerces::XMLUni::fgSAX2CoreValidation, 1);
    $SAX->setFeature($XML::Xerces::XMLUni::fgXercesDynamic, 0);
};
ok((not $@),'setting validate=>yes, dynamic=>no')
  or diag("Here's the error: $@");

my $is = XML::Xerces::MemBufInputSource->new($document, 'foo');
eval{$SAX->parse($is)};
ok($@,'no dtd');

$SAX = XML::Xerces::XMLReaderFactory::createXMLReader();
$SAX->setContentHandler($CONTENT_HANDLER);
$SAX->setErrorHandler(XML::Xerces::PerlErrorHandler->new());
eval {
    $SAX->setFeature($XML::Xerces::XMLUni::fgSAX2CoreValidation, 1);
    $SAX->setFeature($XML::Xerces::XMLUni::fgXercesDynamic, 1);
};
ok((not $@),'setting dynamic=>yes')
  or diag("Here's the error: $@");

# reset the counts
$CONTENT_HANDLER->reset_document();

eval{$SAX->parse(XML::Xerces::MemBufInputSource->new($document, 'foo'))};
ok((not $@),'membuf parse');

is($CONTENT_HANDLER->{elements}, 10,'elements');
is($CONTENT_HANDLER->{chars}, 141,'chars');
is($CONTENT_HANDLER->{ws}, 0,'ws');

# test the overloaded parse version
$SAX->parse($PERSONAL_FILE_NAME);
pass('parse filename');

# reset the counts
$CONTENT_HANDLER->reset_document();

# test the progressive parsing interface
my $token = XML::Xerces::XMLPScanToken->new();
$SAX->parseFirst($PERSONAL_FILE_NAME,$token);
while ($SAX->parseNext($token)) {
  # do nothing
}
pass('progressive parse');
ok($CONTENT_HANDLER->{elements} == 37, 'element count after progressive parse');


# reset the counts, and parser for new parse
$CONTENT_HANDLER->reset_document();
$SAX->parseReset($token);

# test the progressive parsing interface
$token = XML::Xerces::XMLPScanToken->new();
$SAX->parseFirst($PERSONAL_FILE_NAME,$token);
while ($SAX->parseNext($token)) {
  last if $CONTENT_HANDLER->{elements} == 10;
}
pass('early exit from progressive parse');
ok($CONTENT_HANDLER->{elements} == 10, 'element count after early exit from progressive parse');

# reset the parser for new parse
$SAX->parseReset($token);

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
  my $LINE = $_[1]->getLineNumber;
  my $COLUMN = $_[1]->getColumnNumber;
  my $MESSAGE = $_[1]->getMessage;
  $::error = <<"EOE";
WARNING:
LINE:    $LINE
COLUMN:  $COLUMN
MESSAGE: $MESSAGE
EOE
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

#
# Test setProperty()
#
$SAX = XML::Xerces::XMLReaderFactory::createXMLReader();
$SAX->setErrorHandler(XML::Xerces::PerlErrorHandler->new());

# setProperty(): fgXercesSchemaExternalSchemaLocation
eval {
    $SAX->setProperty($XML::Xerces::XMLUni::fgXercesSchemaExternalSchemaLocation, "nowhere");
};
ok((not $@),'setProperty(): fgXercesSchemaExternalSchemaLocation')
  or diag("Here's the error: $@");

# setProperty(): fgXercesSchemaExternalNoNameSpaceSchemaLocation
eval {
    $SAX->setProperty($XML::Xerces::XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, "nowhere");
};
ok((not $@),'setProperty(): fgXercesSchemaExternalNoNameSpaceSchemaLocation')
  or diag("Here's the error: $@");

# setProperty(): fgXercesScannerName
eval {
    $SAX->setProperty($XML::Xerces::XMLUni::fgXercesScannerName, "BADNAME");
};
ok((not $@),'setProperty(): fgXercesScannerName')
  or diag("Here's the error: $@");

# setProperty(): fgXercesSecurityManager
eval {
    $SAX->setProperty($XML::Xerces::XMLUni::fgXercesSecurityManager, "BADNAME");
};
$error = $@;
ok($error,'setProperty(): fgXercesSecurityManager - throws exception');
isa_ok($@,'XML::Xerces::SAXNotSupportedException');

# setProperty(): bogus
eval {
    $SAX->setProperty("bogus", "BADNAME");
};
$error = $@;
ok($@,'setProperty(): bogus - throws exception');
isa_ok($@,'XML::Xerces::SAXNotRecognizedException');

$document = q[<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<contributors  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
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

$SAX = XML::Xerces::XMLReaderFactory::createXMLReader();

$CONTENT_HANDLER = MyContentHandler->new();
$SAX->setContentHandler($CONTENT_HANDLER);

$SAX->setFeature($XML::Xerces::XMLUni::fgSAX2CoreNameSpaces, 1);
$SAX->setFeature($XML::Xerces::XMLUni::fgXercesSchema, 1);
$SAX->setFeature($XML::Xerces::XMLUni::fgSAX2CoreValidation, 1);
$SAX->setFeature($XML::Xerces::XMLUni::fgXercesDynamic, 1);

$SAX->setProperty($XML::Xerces::XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, 'this_file_does_not_exist');

$SAX->setErrorHandler(MyErrorHandler->new());
$::error = '';

$is = eval{XML::Xerces::MemBufInputSource->new($document)};
eval {$SAX->parse($is)};
ok($::error, "parse with fgXercesSchemaExternalNoNameSpaceSchemaLocation set to invalid file throws warning");

$SAX->setProperty($XML::Xerces::XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, $SCHEMA_FILE_NAME);
$SAX->setErrorHandler(XML::Xerces::PerlErrorHandler->new());

# $is = eval{XML::Xerces::LocalFileInputSource->new($PERSONAL_SCHEMA_INVALID_FILE_NAME)};
eval {$SAX->parse($is)};
ok($@, "parse invalid doc with fgXercesSchemaExternalNoNameSpaceSchemaLocation set");
like($@, qr/Unknown element 'contributors'/, 'invalid document error');

