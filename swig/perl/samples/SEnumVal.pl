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
# SEnumVal
#
# This sample is modeled after its Xerces-C counterpart.  You give it an
# XML file and it parses it and enumerates the Schema Grammar. It shows
# how to access the Schema information stored in the internal data structurs
#
######################################################################

use strict;
use blib;
use XML::Xerces qw(error);
use Getopt::Long;
use Benchmark;
use vars qw(%OPTIONS);

#
# Read and validate command line args
#

my $USAGE = <<EOU;
USAGE: $0 file
EOU
my $VERSION = q[$Id: DOMCount.pl,v 1.13 2002/08/27 19:33:19 jasons Exp $ ];

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

my $val_to_use = XML::Xerces::SchemaValidator->new();
my $parser = XML::Xerces::SAXParser->new($val_to_use);

$parser->setValidationScheme ($XML::Xerces::AbstractDOMParser::Val_Auto);
$parser->setErrorHandler(XML::Xerces::PerlErrorHandler->new());
$parser->setDoNamespaces(1);
$parser->setDoSchema(1);

my $t0 = new Benchmark;
eval {$parser->parse ($file)};
error($@) if $@;

my $count = $parser->getErrorCount();
if ($count == 0) {
  my $grammar = $val_to_use->getGrammar();
  printf STDOUT "Found Grammar: %s\n", $grammar;
  my $iterator = $grammar->getElemEnumerator();
  if ($iterator->hasMoreElements()) {
    printf STDOUT "Found Elements\n";
    while ($iterator->hasMoreElements()) {
      my $elem = $iterator->nextElement();
      printf STDOUT "Element Name: %s, Content Model: %s\n",
	$elem->getFullName(),
	$elem->getFormattedContentModel();
      if ($elem->hasAttDefs()) {
	my $attr_list = $elem->getAttDefList();
	while ($attr_list->hasMoreElements()) {
	  my $attr = $attr_list->nextElement();
	  my $type = $attr->getType();
	  my $type_name;
	  if ($type == $XML::Xerces::XMLAttDef::CData) {
	    $type_name = 'CDATA';
	  } elsif ($type == $XML::Xerces::XMLAttDef::ID) {
	    $type_name = 'ID';
	  } elsif ($type == $XML::Xerces::XMLAttDef::Notation) {
	    $type_name = 'NOTATION';
	  } elsif ($type == $XML::Xerces::XMLAttDef::Enumeration) {
	    $type_name = 'ENUMERATION';
	  } elsif ($type == $XML::Xerces::XMLAttDef::Nmtoken
		   or $type == $XML::Xerces::XMLAttDef::Nmtokens
		  ) {
	    $type_name = 'NMTOKEN(S)';
	  } elsif ($type == $XML::Xerces::XMLAttDef::IDRef
		   or $type == $XML::Xerces::XMLAttDef::IDRefs
		  ) {
	    $type_name = 'IDREF(S)';
	  } elsif ($type == $XML::Xerces::XMLAttDef::Entity
		   or $type == $XML::Xerces::XMLAttDef::Entities
		  ) {
	    $type_name = 'ENTITY(IES)';
	  } elsif ($type == $XML::Xerces::XMLAttDef::NmToken
		   or $type == $XML::Xerces::XMLAttDef::NmTokens
		  ) {
	    $type_name = 'NMTOKEN(S)';
	  }
	  printf STDOUT "\tattribute Name: %s, Type: %s\n",
	    $attr->getFullName(),
	      $type_name;
	}
      }
    }
  }
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
