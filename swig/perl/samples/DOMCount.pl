#
# Copyright 2001,2004 The Apache Software Foundation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
######################################################################
#
# DOMCount
#
# This sample is modeled after its XML4C counterpart.  You give it an
# XML file and it parses it into a DOM tree and counts the nodes.
#
######################################################################

use strict;
use XML::Xerces::DOM;
use Getopt::Long;
use Benchmark;
use vars qw(%OPTIONS);

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
my $VERSION = q[$Id: DOMCount.pl,v 1.13 2002/08/27 19:33:19 jasons Exp $ ];

my $rc = GetOptions(\%OPTIONS,
		    'v=s',
		    'n',
		    's');

die $USAGE unless $rc;

die $USAGE unless scalar @ARGV;

my $file = $ARGV[0];
-f $file or die "File '$file' does not exist!\n";

my $namespace = $OPTIONS{n} || 0;
my $schema = $OPTIONS{"s"} || 0;
my $validate = $OPTIONS{v} || 'auto';

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();


if (uc($validate) eq 'ALWAYS') {
  $validate = $XML::Xerces::AbstractDOMParser::Val_Always;
} elsif (uc($validate) eq 'NEVER') {
  $validate = $XML::Xerces::AbstractDOMParser::Val_Never;
} elsif (uc($validate) eq 'AUTO') {
  $validate = $XML::Xerces::AbstractDOMParser::Val_Auto;
} else {
  die("Unknown value for -v: $validate\n$USAGE");
}


#
# Count the nodes
#

my $parser = XML::Xerces::XercesDOMParser->new();
$parser->setValidationScheme($validate);
$parser->setDoNamespaces($namespace);
$parser->setCreateEntityReferenceNodes(1);
$parser->setCreateSchemaInfo(1);
$parser->setDoSchema($schema);

my $error_handler = XML::Xerces::PerlErrorHandler->new();
$parser->setErrorHandler($error_handler);

my $t0 = new Benchmark;
eval {
  $parser->parse ($file);
};
XML::Xerces::fatal_error($@) if ($@);

my $doc = $parser->getDocument ();
my $element_count = $doc->getElementsByTagName("*")->getLength();
my $t1 = new Benchmark;
my $td = timediff($t1, $t0);

print STDOUT "$file: duration: ", timestr($td), "\n";
print STDOUT "\t($element_count elems)\n";

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}

exit(0);
