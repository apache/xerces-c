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

use XML::Xerces qw(error);
use Getopt::Long;
use Data::Dumper;
use strict;
my %OPTIONS;
my $rc = GetOptions(\%OPTIONS,
		    'help'
		   );

my $USAGE = <<"EOU";
usage: $0 xml_file
EOU

die "Bad option: $rc\n$USAGE" unless $rc;
die $USAGE if exists $OPTIONS{help};

die "Must specify input files\n$USAGE" unless scalar @ARGV;

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

package MyNodeFilter;
use strict;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlNodeFilter);
sub acceptNode {
  my ($self,$node) = @_;
  return $XML::Xerces::DOMNodeFilter::FILTER_ACCEPT;
}

package main;

my $DOM = XML::Xerces::XercesDOMParser->new();
my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
$DOM->setErrorHandler($ERROR_HANDLER);
eval{$DOM->parse($ARGV[0])};
error($@,"Couldn't parse file: $ARGV[0]")
  if $@;

my $doc = $DOM->getDocument();
my $root = $doc->getDocumentElement();

my $hash = node2hash($root);
print STDOUT Data::Dumper->Dump([$hash]);
exit(0);

sub node2hash {
  my $node = shift;
  my $return = {};

  # build a hasref that represents this element
  $return->{node_name} = $node->getNodeName();
  if ($node->hasAttributes()) {
    my %attrs = $node->getAttributes();
    $return->{attributes} = \%attrs;
  }

  # insert code to handle children
  if ($node->hasChildNodes()) {
    my $text;
    foreach my $child ($node->getChildNodes) {
      push(@{$return->{children}},node2hash($child))
	if $child->isa('XML::Xerces::DOMElement');
      $text .= $child->getNodeValue()
	if $child->isa('XML::Xerces::DOMText');
    }
    $return->{text} = $text
      if $text !~ /^\s*$/;
  }
  return $return;
}

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
