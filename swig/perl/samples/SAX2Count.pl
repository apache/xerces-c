 #
 #  Copyright 2002,2004 The Apache Software Foundation.
 #
 #  Licensed under the Apache License, Version 2.0 (the "License");
 #  you may not use this file except in compliance with the License.
 #  You may obtain a copy of the License at
 #
 #       http://www.apache.org/licenses/LICENSE-2.0
 #
 #  Unless required by applicable law or agreed to in writing, software
 #  distributed under the License is distributed on an "AS IS" BASIS,
 #  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 #  See the License for the specific language governing permissions and
 #  limitations under the License.
 #

######################################################################
#
# SAX2Count
#
# This sample is modeled after its Xerces-C counterpart.  You give it an
# XML file and it parses it with a SAX parser and counts what it sees.
#
######################################################################

use strict;
# use blib;
use XML::Xerces qw(error);
use XML::Xerces::SAX;
use Getopt::Long;
use vars qw($opt_v $opt_n);
use Benchmark;
#
# Read and validate command line args
#

my $USAGE = <<EOU;
USAGE: $0 [-v=xxx][-n] file
Options:
    -v=xxx      Validation scheme [always | never | auto*]
    -n          Enable namespace processing. Defaults to off.
    -s          Enable schema processing. Defaults to off.

  * = Default if not provided explicitly

EOU
my $VERSION = q[$Id: SAX2Count.pl,v 1.9 2002/08/27 19:33:20 jasons Exp $ ];

my %OPTIONS;
my $rc = GetOptions(\%OPTIONS,
		    'v=s',
		    'n',
		    's');

die $USAGE unless $rc;

die $USAGE unless scalar @ARGV;

my $file = $ARGV[0];
-f $file or die "File '$file' does not exist!\n";

my $namespace = $OPTIONS{n} || 0;
my $schema = $OPTIONS{'s'} || 0;
my $validate = $OPTIONS{v} || 'auto';

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

if (uc($validate) eq 'ALWAYS') {
  $validate = $XML::Xerces::SAX2XMLReader::Val_Always;
} elsif (uc($validate) eq 'NEVER') {
  $validate = $XML::Xerces::SAX2XMLReader::Val_Never;
} elsif (uc($validate) eq 'AUTO') {
  $validate = $XML::Xerces::SAX2XMLReader::Val_Auto;
} else {
  die("Unknown value for -v: $validate\n$USAGE");
}

#
# Count the nodes
#

package MyContentHandler;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlContentHandler);

sub start_element {
  my ($self,$uri,$localname,$qname,$attrs) = @_;
  $self->{elements}++;
  $self->{attrs} += $attrs->getLength;
}
sub end_element {
  my ($self,$uri,$localname,$qname) = @_;
}
sub characters {
  my ($self,$str,$len) = @_;
  $self->{chars} += $len;
}
sub ignorable_whitespace {
  my ($self,$str,$len) = @_;
  $self->{ws} += $len;
}

package main;
my $parser = XML::Xerces::XMLReaderFactory::createXMLReader();
eval {
  $parser->setFeature($XML::Xerces::XMLUni::fgSAX2CoreNameSpaces, $namespace);
  if ($validate eq $XML::Xerces::SAX2XMLReader::Val_Auto) {
    $parser->setFeature($XML::Xerces::XMLUni::fgSAX2CoreValidation, 1);
    $parser->setFeature($XML::Xerces::XMLUni::fgXercesDynamic, 1);
  } elsif ($validate eq $XML::Xerces::SAX2XMLReader::Val_Never) {
    $parser->setFeature($XML::Xerces::XMLUni::fgSAX2CoreValidation, 0);
  } elsif ($validate eq $XML::Xerces::SAX2XMLReader::Val_Always) {
    $parser->setFeature($XML::Xerces::XMLUni::fgSAX2CoreValidation, 1);
    $parser->setFeature($XML::Xerces::XMLUni::fgXercesDynamic, 0);
  }
  $parser->setFeature($XML::Xerces::XMLUni::fgXercesSchema, $schema);
};
error ($@) if $@;

my $error_handler = XML::Xerces::PerlErrorHandler->new();
$parser->setErrorHandler($error_handler);

my $CONTENT_HANDLER = MyContentHandler->new();
$parser->setContentHandler($CONTENT_HANDLER);
$CONTENT_HANDLER->{elements} = 0;
$CONTENT_HANDLER->{attrs} = 0;
$CONTENT_HANDLER->{ws} = 0;
$CONTENT_HANDLER->{chars} = 0;

my $t0 = new Benchmark;
eval {
  $parser->parse (XML::Xerces::LocalFileInputSource->new($file));
};
XML::Xerces::fatal_error($@) if ($@);

my $t1 = new Benchmark;
my $td = timediff($t1, $t0);

print "$file: duration: ", timestr($td), "\n";
print "elems: ", $CONTENT_HANDLER->{elements}, "\n"; 
print "attrs: ", $CONTENT_HANDLER->{attrs}, "\n";
print "whitespace: ", $CONTENT_HANDLER->{ws}, "\n";
print "characters: ", $CONTENT_HANDLER->{chars}, "\n";

exit(0);

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
