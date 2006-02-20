#!/usr/bin/perl -w

use blib;
use XML::Xerces;
use strict;
use Getopt::Long;
use Linux::MemInfo;
use Proc::ProcessTable;
use IO::Handle;
use vars qw(@LEAK);

my %OPTIONS;
$OPTIONS{count} = 1000;
$OPTIONS{iter} = 10;
$OPTIONS{test} = 'dom';
my $rc = GetOptions(\%OPTIONS,
		    'file=s',
		    'count=i',
		    'iter=i',
		    'test=s',
		    'help',
		    'validate',
		    'leak',
		    'schema',
		    );
my $USAGE = <<"EOU";
usage: $0 [required flags] [optional flags]
  required flags:
    --file=file_name  : the XML file to parse
    --test=name       : which test to run

  test names:
    dom               : use a DOM parser (default)
    sax               : use a SAX parser
    domcreate         : create a DOM tree
    nodemap           : check DOMNamedNodeMap for leaks
    nodelist          : check DOMNodeList for leaks
    membuf            : check MemBufInputSource
    domappend         : test DOMExceptions for leaks
    domelement        : test DOMExceptions for leaks (part 2)
    xmlexcept         : test XMLExceptions for leaks

  optional parameters:
    --count=num       : run num iterations default = $OPTIONS{count}
    --iter=num        : # iterations to print results = $OPTIONS{iter}
    --leak            : leak memory on purpose to test the script
    --validate        : turn validation on
    --schema          : parse a W3C XML Schema file (forces --validate)
    --help            : print this message
EOU

die "$rc\n$USAGE" unless $rc;
die $USAGE if exists $OPTIONS{help};

my $xml;
unless ($OPTIONS{test} eq 'domcreate'
	or $OPTIONS{test} eq 'membuf'
	or $OPTIONS{test} eq 'domelement'
	or $OPTIONS{test} eq 'domappend'
	or $OPTIONS{test} eq 'xmlexcept'
       ) {
  die "Must specify --file\n$USAGE"
    unless exists $OPTIONS{file};
  open(IN,$OPTIONS{file})
    or die "Couldn't open $OPTIONS{file} for reading";

  # slurp in the file
  {
    local $/;
    $xml = <IN>;
  }
}

my @tests = qw(dom
	       sax
	       sax2
	       nodemap
	       nodelist
	       domcreate
	       domappend
	       membuf
	       domelement
	       xmlexcept
	      );
unless (grep {$OPTIONS{test} eq $_} @tests) {
  die "Invalid test: $OPTIONS{test}, use one of: " . join(',', @tests) . "\n";
}

my $logfile = '/tmp/xerces-memtest.log';
open(LOG,">$logfile")
  or die "Couldn't open $logfile for reading";

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

$main::LEAK = 1
  if exists $OPTIONS{leak};

# we initialize the starting memory after running the first iteration
my $starting_size;

my $validate = 0;
my $schema = 0;
my $namespace = 0;
if (exists $OPTIONS{validate}) {
  if ($OPTIONS{test} eq 'dom') {
    $validate = $XML::Xerces::AbstractDOMParser::Val_Always;
  } else {
    $validate = 1;
  }
}
if (exists $OPTIONS{schema}) {
  $validate = 1;
  $schema = 1;
  $namespace = 1;
}

STDOUT->autoflush();

my $errorHandler = XML::Xerces::PerlErrorHandler->new() ;

# Just to make sure there is only one, $Parser is global but it's not used anywhere else:
my $dom = XML::Xerces::XercesDOMParser->new() ;
$dom->setErrorHandler($errorHandler) ;
$dom->setValidationScheme ($validate);
$dom->setDoNamespaces ($namespace);
$dom->setDoSchema ($schema);
$dom->setValidationSchemaFullChecking ($schema);
$dom->setValidationConstraintFatal ($validate);

package MyDocumentHandler;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlDocumentHandler);

sub start_element {
  my ($self,$name,$attrs) = @_;
  my $count = $attrs->getLength();

  if ($main::LEAK) {
    push(@main::LEAK,$attrs);
  }
}
sub end_element {}

sub characters {
}
sub ignorable_whitespace {
}

package main;

my $sax = XML::Xerces::SAXParser->new() ;
$sax->setErrorHandler($errorHandler) ;
$sax->setValidationScheme ($validate);
$sax->setDoNamespaces ($namespace);
$sax->setDoSchema ($schema);
$sax->setValidationSchemaFullChecking ($schema);
$sax->setValidationConstraintFatal ($validate);
my $handler = MyDocumentHandler->new();
$sax->setDocumentHandler($handler);

if ($OPTIONS{test} eq 'nodelist' or $OPTIONS{test} eq 'nodemap') {
  $dom->parse($OPTIONS{file});
}

my ($domImpl,$dt);
my $doc;
if ($OPTIONS{test} eq 'domelement') {
  $domImpl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
  $doc = $domImpl->createDocument('myns', 'HISTORY', undef);
}

my $DOCUMENT = <<XML;
<?xml version="1.0"?>
<HISTORY xmlns="myns" xmlns:myns="myns"	
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xsi:schemaLocation="myns test.xsd">
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
	<RECORD ID="0" PRICE="1.1" VOL="10"/>
</HISTORY>
XML

for (my $count=1;$count<=$OPTIONS{count};$count++) {
  # run the loop

  if ($OPTIONS{test} eq 'dom') {
    test_dom($xml,$dom);
  } elsif ($OPTIONS{test} eq 'sax') {
    test_sax($sax);
  } elsif ($OPTIONS{test} eq 'domcreate') {
    test_dom_create();
  } elsif ($OPTIONS{test} eq 'domelement') {
    test_dom_create_element();
  } elsif ($OPTIONS{test} eq 'domappend') {
    test_dom_create_element();
  } elsif ($OPTIONS{test} eq 'membuf') {
    test_membuf();
  } elsif ($OPTIONS{test} eq 'sax2') {
    test_sax2($xml);
  } elsif ($OPTIONS{test} eq 'xmlexcept') {
    test_xmlexcept();
  } elsif ($OPTIONS{test} eq 'nodelist') {
    test_domlist($xml,$dom);
  } elsif ($OPTIONS{test} eq 'nodemap') {
    test_dommap($xml,$dom);
  } else {
    die "Unknown test $OPTIONS{test}";
  }

  if ($count % $OPTIONS{iter} == 0) {
    my ($proc) = get_proc($$);
    my $size = $proc->size();
    my $leak = $size - $starting_size;
    my $iter_leak = $leak/$count;
    my ($size_unit,$leak_unit,$iter_unit);
    ($size,$size_unit) = get_val_unit($size);
    ($leak,$leak_unit) = get_val_unit($leak);
    ($iter_leak,$iter_unit) = get_val_unit($iter_leak);
    printf STDOUT "%d: total mem: %d%s, leaked mem: %d%s, leak/iteration:%d%s",
      $count,
      $size,
      $size_unit,
      $leak,
      $leak_unit,
      $iter_leak,
      $iter_unit,
	;
    if (exists $OPTIONS{leak}) {
      printf STDOUT ", elements leaked = %d: \n", scalar @LEAK;
    } else {
      printf STDOUT "\n";
    }
  } elsif ($count == 5) {
    my ($proc) = get_proc($$);
    $starting_size = $proc->size();
    printf STDOUT "starting mem: %dk\n", int($starting_size/1024);
  }
}

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}

sub get_proc {
  my $pid = shift;
  return grep {$_->pid == $pid} @{Proc::ProcessTable->new->table};
}

sub test_dom {
  my $xml = shift ;
  my $parser = shift;

  eval {
    # my $is = XML::Xerces::LocalFileInputSource->new($OPTIONS{file});
    $parser->parse($OPTIONS{file});
  };
  XML::Xerces::error($@) if $@;

  if ($OPTIONS{leak}) {
    my $doc = $parser->getDocument();
    my $node_list = $doc->getElementsByTagName('*');
    push(@LEAK,$node_list);
  }
  $parser->resetDocumentPool();
}

sub test_sax2 {
  my $parser = XML::Xerces::XMLReaderFactory::createXMLReader() ;
  $parser->setErrorHandler($errorHandler) ;

#   my $contentHandler = new XML::Xerces::PerlContentHandler() ;
#   $parser->setContentHandler($contentHandler) ;

  eval {
    $parser->setFeature($XML::Xerces::XMLUni::fgSAX2CoreNameSpaces, $namespace);
    $parser->setFeature($XML::Xerces::XMLUni::fgXercesSchema, $schema);
    $parser->setFeature($XML::Xerces::XMLUni::fgXercesSchemaFullChecking, $schema);
    $parser->setFeature($XML::Xerces::XMLUni::fgXercesValidationErrorAsFatal, $validate);
  };
  XML::Xerces::error($@) if $@;


  eval {
  $parser->setFeature($XML::Xerces::XMLUni::fgSAX2CoreValidation, $validate);
  $parser->setFeature($XML::Xerces::XMLUni::fgXercesDynamic, 0);
  };
  XML::Xerces::error($@) if $@;

  eval {
    # my $is = XML::Xerces::MemBufInputSource->new($xml);
    my $is = XML::Xerces::LocalFileInputSource->new($OPTIONS{file});
    $parser->parse($is) ;
  } ;
  XML::Xerces::error($@) if $@;
}

sub test_sax {
  # my $parser = XML::Xerces::SAXParser->new();
  my $parser = shift;

  eval {
    # my $is = XML::Xerces::LocalFileInputSource->new($OPTIONS{file});
    $parser->parse($OPTIONS{file});
  };
  XML::Xerces::error($@) if $@;
}

sub test_xmlexcept {
  my $uri = eval{XML::Xerces::XMLUri->new(undef,"")};
  die "no error" unless $@;
}

sub test_domlist {
  my $xml = shift ;
  my $parser = shift;

  my $doc = $parser->getDocument();
  my $node_list = $doc->getElementsByTagName('*');
  if ($OPTIONS{leak}) {
    push(@LEAK,$node_list);
  }
}

sub test_membuf {
  my $is = eval {XML::Xerces::MemBufInputSource->new($DOCUMENT,"foo")};
  XML::Xerces::error($@) if $@;
  if ($OPTIONS{leak}) {
    push(@LEAK,$is);
  }
}

sub test_dommap {
  my $xml = shift ;
  my $parser = shift;

  my $doc = $parser->getDocument();
  my $root = $doc->getDocumentElement();
  my $map = $root->getAttributes();
  if ($OPTIONS{leak}) {
    push(@LEAK,$map);
  }
}

sub test_dom_create {
  my $domImpl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
#  my $dt  = eval{$domImpl->createDocumentType('x', '', '')};
#  XML::Xerces::error($@) if $@;
  my $document = $domImpl->createDocument('myns', 'HISTORY', undef);

  # Default prefix
  $document->getDocumentElement->setAttributeNS("http://www.w3.org/2000/xmlns/", "xmlns:myns", 'myns');

  for (my $i = 0; $i < 10; $i++) {
    my $record = $document->createElement('RECORD');
    $document->getDocumentElement->appendChild($record);
    $record->setAttribute('ID', '0');
    $record->setAttribute('PRICE', '1.1');
    $record->setAttribute('VOl', '10');
  }
  $document->release();
}


sub test_dom_create_element {
  # my $document = shift;

  my $domImpl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
  # my $dt  = eval{$domImpl->createDocumentType('x', '', '')};
  # XML::Xerces::error($@) if $@;
  my $document = $domImpl->createDocument('myns', 'HISTORY', undef);

  my $element = eval {$document->createElement('9')};
  die "No exception raised"
    unless $@;

  $document->release();
}

sub test_dom_append_child {
  my $document = q[<?xml version="1.0" encoding="UTF-8"?>
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

  my $DOM1 = new XML::Xerces::XercesDOMParser;
  my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
  $DOM1->setErrorHandler($ERROR_HANDLER);
  my $is = eval{XML::Xerces::MemBufInputSource->new($document)};
  XML::Xerces::error($@) if $@;
  eval{$DOM1->parse($is)};
  XML::Xerces::error($@) if $@;

  my $DOM2 = new XML::Xerces::XercesDOMParser;
  $DOM2->setErrorHandler($ERROR_HANDLER);
  eval {$DOM2->parse($is)};
  XML::Xerces::error($@) if $@;

  my $doc1 = $DOM1->getDocument();
  my $doc2 = $DOM2->getDocument();
  my $root2 = $doc2->getDocumentElement();

  # Trying to append to a DOMDocument node gives a hierarchy error
  eval {
    $doc1->appendChild($root2);
  };
  die "No exception raised"
    unless $@;
}


sub get_val_unit {
  my $val = shift;
  my $unit;
  if ($val > 2 * 1024) {
    $unit = 'k';
    $val = int($val/1024);
  } else {
    $unit = 'b';
  }
  return ($val,$unit)
}

