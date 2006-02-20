use blib;
use XML::Xerces;
use strict;
use Getopt::Long;
use Linux::MemInfo;
use Proc::ProcessTable;
use IO::Handle;

my %OPTIONS;
$OPTIONS{count} = 1000;
$OPTIONS{test} = 'dom';
my $rc = GetOptions(\%OPTIONS,
		    'file=s',
		    'count=i',
		    'test=s',
		    'help',
		    'validate',
		    'schema',
		    );
my $USAGE = <<"EOU";
usage: $0 [required flags] [optional flags]
  required flags:
    --file=file_name  : the XML file to parse
    --test=name       : which test to run

  test names:
    dom               : use a DOM parser (default)
    builder           : use a DOMBuilder
    writer            : test a DOMWriter
    sax2              : use a SAX2XMLReader
    exception         : use a DOM parser to test exception handler leakage

  optional parameters:
    --count=num       : run num iterations default = $OPTIONS{count}
    --validate        : turn validation on
    --schema          : parse a W3C XML Schema file (forces --validate)
    --help            : print this message
EOU

die "$rc\n$USAGE" unless $rc;
die $USAGE if exists $OPTIONS{help};

die "Must specify --file\n$USAGE"
  unless exists $OPTIONS{file};

my @tests = qw(dom sax2 builder writer exception);
unless (grep {$OPTIONS{test} eq $_} @tests) {
  die "Invalid test: $OPTIONS{test}, use one of: " . join(',', @tests) . "\n";
}

open(IN,$OPTIONS{file})
  or die "Couldn't open $OPTIONS{file} for reading";
my $logfile = '/tmp/xerces-memtest.log';
open(LOG,">$logfile")
  or die "Couldn't open $logfile for reading";

# slurp in the file
my $xml;
{
  local $/;
  $xml = <IN>;
}

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

STDERR->autoflush();

my $errorHandler = XML::Xerces::PerlErrorHandler->new() ;
my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
# Just to make sure there is only one, $Parser is global but it's not used anywhere else:
my $dom = XML::Xerces::XercesDOMParser->new() ;
$dom->setErrorHandler($errorHandler) ;
$dom->setValidationScheme ($validate);
$dom->setDoNamespaces ($namespace);
$dom->setDoSchema ($schema);
$dom->setValidationSchemaFullChecking ($schema);
$dom->setValidationConstraintFatal ($validate);

my $builder = $impl->createDOMBuilder($XML::Xerces::DOMImplementationLS::MODE_SYNCHRONOUS,'');
my $writer = $impl->createDOMWriter();
my $doc;
if ($OPTIONS{test} eq 'writer') {
  if ($writer->canSetFeature('format-pretty-print',1)) {
    $writer->setFeature('format-pretty-print',1);
  }
  eval {$dom->parse($OPTIONS{file})};
  XML::Xerces::error($@) if $@;
  $doc = $dom->getDocument();
}

for (my $count=1;$count<=$OPTIONS{count};$count++) {
  # run the loop
  if ($OPTIONS{test} eq 'dom') {
    test_dom($xml,$dom);
  } elsif ($OPTIONS{test} eq 'exception') {
    test_exception($xml,$dom);
  } elsif ($OPTIONS{test} eq 'sax2') {
    test_sax2($xml);
  } elsif ($OPTIONS{test} eq 'builder') {
    test_builder($xml,$impl);
  } elsif ($OPTIONS{test} eq 'writer') {
    test_writer($writer,$doc);
  }
  if ($count % 100 == 0) {
    my ($proc) = get_proc($$);
    my $size = $proc->size();
    my $leak = $size - $starting_size;
    my $iter_leak = $leak/$count;
    my ($size_unit,$leak_unit,$iter_unit);
    ($size,$size_unit) = get_val_unit($size);
    ($leak,$leak_unit) = get_val_unit($leak);
    ($iter_leak,$iter_unit) = get_val_unit($iter_leak);
    printf STDERR "%d: total mem: %d%s, leaked mem: %d%s, leak/iteration:%d%s\n",
      $count,
      $size,
      $size_unit,
      $leak,
      $leak_unit,
      $iter_leak,
      $iter_unit,
	;
  } elsif ($count == 1) {
    my ($proc) = get_proc($$);
    $starting_size = $proc->size();
    printf STDERR "starting mem: %dk\n", int($starting_size/1024);
  }
}

sub get_proc {
  my $pid = shift;
  return grep {$_->pid == $pid} @{Proc::ProcessTable->new->table};
}

sub test_sax2 {
  my $xml = shift ;
  # Just to make sure there is only one, $Parser is global but it's not used anywhere else:
  my $parser = XML::Xerces::XMLReaderFactory::createXMLReader() ;
  $parser->setErrorHandler($errorHandler) ;

#   my $contentHandler = new XML::Xerces::PerlContentHandler() ;
#   $parser->setContentHandler($contentHandler) ;

  eval {
    $parser->setFeature("$XML::Xerces::XMLUni::fgSAX2CoreNameSpaces", $namespace);
    $parser->setFeature("$XML::Xerces::XMLUni::fgXercesSchema", $schema);
    $parser->setFeature("$XML::Xerces::XMLUni::fgXercesSchemaFullChecking", $schema);
    $parser->setFeature("$XML::Xerces::XMLUni::fgXercesValidationErrorAsFatal", $validate);
  };
  XML::Xerces::error($@) if $@;


  eval {
  $parser->setFeature("$XML::Xerces::XMLUni::fgSAX2CoreValidation", $validate);
  $parser->setFeature("$XML::Xerces::XMLUni::fgXercesDynamic", 0);
  };
  XML::Xerces::error($@) if $@;

  eval {
    # my $is = XML::Xerces::MemBufInputSource->new($xml);
    my $is = XML::Xerces::LocalFileInputSource->new($OPTIONS{file});
    $parser->parse($is) ;
  } ;
  XML::Xerces::error($@) if $@;
}

sub test_builder {
  my $xml = shift ;
  my $impl = shift;

  my $parser = $impl->createDOMBuilder($XML::Xerces::DOMImplementationLS::MODE_SYNCHRONOUS,'');

  eval {
    $parser->setFeature("$XML::Xerces::XMLUni::fgDOMNamespaces", $namespace) ;
    $parser->setFeature("$XML::Xerces::XMLUni::fgXercesSchema", $schema) ;
    $parser->setFeature("$XML::Xerces::XMLUni::fgXercesSchemaFullChecking", $schema) ;
  };
  XML::Xerces::error($@) if $@;


  eval {
    $parser->setFeature("$XML::Xerces::XMLUni::fgDOMValidation", $validate) ;
    $parser->setFeature("$XML::Xerces::XMLUni::fgXercesValidationErrorAsFatal", $validate) ;
    $parser->setFeature("$XML::Xerces::XMLUni::fgXercesContinueAfterFatalError", not $validate) ;
#    $parser->setFeature("$XML::Xerces::XMLUni::fgDOMValidateIfSchema", 1) ;
  };
  XML::Xerces::error($@) if $@;

  eval {
    # my $is = XML::Xerces::MemBufInputSource->new($xml);
    # my $is = XML::Xerces::LocalFileInputSource->new($OPTIONS{file});
    $parser->parseURI($OPTIONS{file}) ;
  } ;
  XML::Xerces::error($@) if $@;
  $parser->resetDocumentPool();
  $parser->release();
}

sub test_dom {
  my $xml = shift ;
  my $parser = shift;

  eval {
    # my $is = XML::Xerces::MemBufInputSource->new($xml);
    my $is = XML::Xerces::LocalFileInputSource->new($OPTIONS{file});
    $parser->parse($is) ;
  };
  XML::Xerces::error($@) if $@;
  $parser->resetDocumentPool();
}

sub test_exception {
  my $xml = shift ;
  my $parser = shift;

  eval {
    # my $is = XML::Xerces::MemBufInputSource->new($xml);
    my $is = XML::Xerces::LocalFileInputSource->new($OPTIONS{file});
    $parser->parse($is) ;
  };
  # if we run this test with a file that raises an exception
  # and we leave this line commented out, we will check if exceptions
  # are leaking or not
  # XML::Xerces::error($@) if $@;
  $parser->resetDocumentPool();
}

sub test_writer {
  my $writer = shift;
  my $doc = shift;

  my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
  $writer = $impl->createDOMWriter();

  return;

  my $target = XML::Xerces::MemBufFormatTarget->new();
  eval{$writer->writeNode($target, $doc)};
  XML::Xerces::error($@) if $@;

  my $xml = $target->getRawBuffer();
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
