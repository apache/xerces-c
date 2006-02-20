#!/usr/bin/perl -w

package MyErrorHandler;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlErrorHandler);
sub warning {
  my $line = $_[1]->getLineNumber;
  my $column = $_[1]->getColumnNumber;
  my $message = $_[1]->getMessage;
  printf STDERR "%s:[%s]:%d:%d:%s:%s\n",
    $main::PROGRAM,$main::FILE,$line, $column, 'W', $message;
}

sub error {
  my $line = $_[1]->getLineNumber;
  my $column = $_[1]->getColumnNumber;
  my $message = $_[1]->getMessage;
  printf STDERR "%s:[%s]:%d:%d:%s:%s\n",
    $main::PROGRAM,$main::FILE,$line, $column, 'E', $message;
}

sub fatal_error {
  my $line = $_[1]->getLineNumber;
  my $column = $_[1]->getColumnNumber;
  my $message = $_[1]->getMessage;
  printf STDERR "%s:[%s]:%d:%d:%s:%s\n",
    $main::PROGRAM,$main::FILE,$line, $column, 'F', $message;
}
1;

package main;
use strict;
# use blib;
use XML::Xerces;
use IO::Handle;
use Getopt::Long;

my %OPTIONS;
$OPTIONS{count} = 1000;
$OPTIONS{dom} = 1;
my $rc = GetOptions(\%OPTIONS,
		    'file=s',
		    'help',
		    'schema',
		    'full_schema',
		    'namespace',
		    );
my $USAGE = <<"EOU";
usage: $0 [required flags]
  required flags:
    --file=file_name  : the XML file to parse

  optional parameters:
    --namespace       : enable namespace checking
    --schema          : parse a W3C XML Schema file (forces --namespace)
    --full_schema     : do full schema checking (forces --namespace and --schema)
    --help            : print this message
EOU

die "$rc\n$USAGE" unless $rc;
die $USAGE if exists $OPTIONS{help};

die "Must specify --file\n$USAGE"
  unless exists $OPTIONS{file};

# handle the optional command-line params
my $namespace = 0;
my $schema = 0;
my $full_schema = 0;
if (exists $OPTIONS{namespace}) {
  $namespace = 1;
} elsif (exists $OPTIONS{schema}) {
  $namespace = 1;
  $schema = 1;
} elsif (exists $OPTIONS{full_schema}) {
  $namespace = 1;
  $schema = 1;
  $full_schema = 1;
}

# set globals used by the error handler
$main::FILE = $OPTIONS{file};
$main::PROGRAM = $0;
$main::PROGRAM =~ s|.*/(\w+)|$1|;

my $parser = XML::Xerces::XMLReaderFactory::createXMLReader();
$parser->setErrorHandler(MyErrorHandler->new);

# print as we parse
STDERR->autoflush();

#   my $contentHandler = new XML::Xerces::PerlContentHandler() ;
#   $parser->setContentHandler($contentHandler) ;

# handle the optional features
eval {
  $parser->setFeature("$XML::Xerces::XMLUni::fgSAX2CoreNameSpaces", $namespace);
  $parser->setFeature("$XML::Xerces::XMLUni::fgXercesSchema", $schema);
  $parser->setFeature("$XML::Xerces::XMLUni::fgXercesSchemaFullChecking", $full_schema);
};
XML::Xerces::error($@) if $@;

# and the required features
eval {
  $parser->setFeature("$XML::Xerces::XMLUni::fgXercesContinueAfterFatalError", 1);
  $parser->setFeature("$XML::Xerces::XMLUni::fgXercesValidationErrorAsFatal", 0);
  $parser->setFeature("$XML::Xerces::XMLUni::fgSAX2CoreValidation", 1);
  $parser->setFeature("$XML::Xerces::XMLUni::fgXercesDynamic", 1);
};
XML::Xerces::error($@) if $@;

eval {
  my $is = XML::Xerces::LocalFileInputSource->new($OPTIONS{file});
  $parser->parse($is) ;
} ;
XML::Xerces::error($@) if $@;
exit(0);

