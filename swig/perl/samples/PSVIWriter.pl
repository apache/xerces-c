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
# PSVIWriter
#
# This sample is modeled after its Xerces-C counterpart.  You give it an
# XML file and it parses it and writes out PSVI information.
#
######################################################################

use strict;
# use blib;
use XML::Xerces::DOM;
use XML::Xerces::SAX;
use Getopt::Long;
use Benchmark;
use vars qw(%OPTIONS);

#
# Read and validate command line args
#

my $USAGE = <<EOU;
USAGE: $0 file
EOU
my $VERSION = q[$Id$ ];

my $rc = GetOptions(\%OPTIONS,
		    'help');

die $USAGE if exists $OPTIONS{help};
die $USAGE unless scalar @ARGV;

my $file = $ARGV[0];
-f $file or die "File '$file' does not exist!\n";

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

my $TRUE = 1;
my $FALSE = 0;
my $use_schema = $FALSE;
my $unrep_flags = $XML::Xerces::XMLFormatter::UnRep_Fail;
my $encoding_name = "UTF8";

my $parser = XML::Xerces::XMLReaderFactory::createXMLReader();
$parser->setFeature($XML::Xerces::XMLUni::fgSAX2CoreNameSpaces, $TRUE);
$parser->setFeature($XML::Xerces::XMLUni::fgXercesSchema, $TRUE);
$parser->setFeature($XML::Xerces::XMLUni::fgXercesSchemaFullChecking, $use_schema);
$parser->setFeature($XML::Xerces::XMLUni::fgSAX2CoreNameSpacePrefixes, $FALSE);
$parser->setFeature($XML::Xerces::XMLUni::fgSAX2CoreValidation, $TRUE);
$parser->setFeature($XML::Xerces::XMLUni::fgXercesDynamic, $TRUE);


my $psvi_target = XML::Xerces::StdOutFormatTarget->new();
my $psvi_formatter = XML::Xerces::XMLFormatter->new($encoding_name,
						   $psvi_target,
						   $XML::Xerces::XMLFormatter::NoEscapes,
						   $unrep_flags);

my $error_target = XML::Xerces::StdOutFormatTarget->new();
my $error_formatter = XML::Xerces::XMLFormatter->new($encoding_name,
						     $error_target,
						     $XML::Xerces::XMLFormatter::NoEscapes,
						     $unrep_flags);


my $t0 = new Benchmark;
eval {$parser->parse($file)};
error($@) if $@;


my $count = $parser->getErrorCount();
if ($count == 0) {
  printf STDOUT "Successful parse\n";
} else {
  print STDERR "Errors occurred, no output available\n";
}
my $t1 = new Benchmark;
my $td = timediff($t1, $t0);

print STDOUT "$file: duration: ", timestr($td), "\n";
exit(0);

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
