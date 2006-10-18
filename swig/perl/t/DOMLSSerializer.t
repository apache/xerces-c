# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# DOMLSserializer.t'

######################### Begin module loading

# use blib;
use Test::More tests => 37;
BEGIN { use_ok("XML::Xerces::DOM") };

use strict;

######################### Begin Test

# Create a couple of identical test documents
my $document = q[<?xml version="1.0" encoding="UTF-8" standalone="no" ?><contributors>
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

my $dom = XML::Xerces::XercesDOMParser->new();
my $handler = XML::Xerces::PerlErrorHandler->new();
$dom->setErrorHandler($handler);
eval{$dom->parse(XML::Xerces::MemBufInputSource->new($document))};
XML::Xerces::error($@) if $@;

my $doc = $dom->getDocument();
my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $writer = $impl->createLSSerializer();
my $config = $writer->getDomConfig();
isa_ok($config, "XML::Xerces::DOMConfiguration",
       'getDomConfig()');

ok((not defined $writer->getNewLine()),
   "default EOL is undef")
  or diag("Found bad EOL: " . $writer->getNewLine());

foreach my $eol_ch ('CR', 'CR-LF', 'LF') {
  $writer->setNewLine($eol_ch);
  is($writer->getNewLine(), $eol_ch,
     "setting EOL: $eol_ch");
}

$writer->setNewLine(undef);
ok((not defined $writer->getNewLine()),
   "re-setting EOL using undef")
  or diag("Found bad EOL: " . $writer->getNewLine());

TODO: {
  local $TODO = 'setting of bad newline value not checked';
  eval {$writer->setNewLine('BAD VALUE')};
  ok($@,
     "exception setting EOL to bogus value")
    or diag("Found bad EOL: " . $writer->getNewLine());
}

my $output = $writer->writeToString($doc);

# change the encoding string
$output =~ s/UTF-16/UTF-8/;

ok($output, "output written");
is($output, $document, "got expected document")
  or diag("Found: $output");

my @parameters = ($XML::Xerces::XMLUni::fgDOMWRTCanonicalForm,
		  $XML::Xerces::XMLUni::fgDOMWRTDiscardDefaultContent,
		  $XML::Xerces::XMLUni::fgDOMWRTEntities,
		  $XML::Xerces::XMLUni::fgDOMWRTFormatPrettyPrint,
		  $XML::Xerces::XMLUni::fgDOMWRTNormalizeCharacters,
		  $XML::Xerces::XMLUni::fgDOMWRTSplitCdataSections,
		  $XML::Xerces::XMLUni::fgDOMWRTValidation,
		  $XML::Xerces::XMLUni::fgDOMWRTWhitespaceInElementContent,
		  $XML::Xerces::XMLUni::fgDOMWRTBOM,
		  $XML::Xerces::XMLUni::fgDOMXMLDeclaration,
		  $XML::Xerces::XMLUni::fgDOMWRTXercesPrettyPrint,
		 );
my %not_supported_true = ($XML::Xerces::XMLUni::fgDOMWRTCanonicalForm=>1,
		       $XML::Xerces::XMLUni::fgDOMWRTNormalizeCharacters=>1,
		       $XML::Xerces::XMLUni::fgDOMWRTValidation=>1,
		      );
my %not_supported_false = ($XML::Xerces::XMLUni::fgDOMWRTWhitespaceInElementContent=>1);

foreach my $param (@parameters) {
  if ($not_supported_true{$param}) {
  TODO: {
      local $TODO = "setting $param TRUE not supported";
      ok(!$config->canSetParameter($param, 1),
	 "canSetParameter: $param, TRUE");
    }
  } else {
    ok($config->canSetParameter($param, 1),
       "canSetParameter: $param, TRUE");
  }
  if ($not_supported_false{$param}) {
  TODO: {
      local $TODO = "setting $param FALSE not supported";
      ok(!$config->canSetParameter($param, 0),
	 "canSetParameter: $param, FALSE");
    }
  } else {
    ok($config->canSetParameter($param, 0),
       "canSetParameter: $param, FALSE");
  }
}

like($output, qr/encoding/,
       'xml declaration turned on');

$config->setParameter($XML::Xerces::XMLUni::fgDOMXMLDeclaration,0);
$output = $writer->writeToString($doc);
unlike($output, qr/encoding/,
       'xml declaration turned off');
$config->setParameter($XML::Xerces::XMLUni::fgDOMXMLDeclaration,1);

my $file = 'foo.xml';
my $rc = eval {$writer->writeToURI($doc, $file)};
XML::Xerces::error($@) if $@;

ok($rc,
   'writeToURI');

SKIP: {
  skip "Couldn't open $file for reading", 2, unless open(TEST, $file);

# slurp the whole file in
  local $/;
  $output = <TEST>;

  # change the encoding string
  $output =~ s/UTF-16/UTF-8/;

  ok($output, "output written");
  is($output, $document, "got expected document")
    or diag("Found: $output");

  unlink($file);
}

