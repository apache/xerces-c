#
#  Licensed to the Apache Software Foundation (ASF) under one or more
#  contributor license agreements.  See the NOTICE file distributed with
#  this work for additional information regarding copyright ownership.
#  The ASF licenses this file to You under the Apache License, Version 2.0
#  (the "License"); you may not use this file except in compliance with
#  the License.  You may obtain a copy of the License at
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
# DOMPrint
#
# This sample is loosely modeled after its XML4C counterpart.
# It parses a DOM document tree and reconstructs the "printed"
# form of the document.  One important difference from the XXML4C
# example is that this one makes use of the DOMPARSE utilities only
# available in XML4P.
#
######################################################################

use strict;
# use blib;
use XML::Xerces::DOM;
use Getopt::Long;
use vars qw();

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
my $VERSION = q[$Id$];
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
my $schema = $OPTIONS{s} || 0;
my $validate = $OPTIONS{v} || 'auto';

if (uc($validate) eq 'ALWAYS') {
  $validate = $XML::Xerces::AbstractDOMParser::Val_Always;
} elsif (uc($validate) eq 'NEVER') {
  $validate = $XML::Xerces::AbstractDOMParser::Val_Never;
} elsif (uc($validate) eq 'AUTO') {
  $validate = $XML::Xerces::AbstractDOMParser::Val_Auto;
} else {
  die("Unknown value for -v: $validate\n$USAGE");
}

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();


#
# Parse and print
#

my $parser = XML::Xerces::XercesDOMParser->new();
$parser->setValidationScheme ($validate);
$parser->setDoNamespaces ($namespace);
$parser->setCreateEntityReferenceNodes(1);
$parser->setDoSchema ($schema);

my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
$parser->setErrorHandler($ERROR_HANDLER);
eval {$parser->parse ($file)};
XML::Xerces::fatal_error($@) if ($@);

my $doc = $parser->getDocument();
my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $writer = $impl->createLSSerializer();
my $config = $writer->getDomConfig();
if ($config->canSetParameter($XML::Xerces::XMLUni::fgDOMWRTFormatPrettyPrint,1)) {
  $config->setParameter($XML::Xerces::XMLUni::fgDOMWRTFormatPrettyPrint,1);
}
my $outStream = XML::Xerces::StdOutFormatTarget->new();
my $target = $impl->createLSOutput();
$target->setByteStream($outStream);
$writer->write($doc,$target);

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}

exit(0);

