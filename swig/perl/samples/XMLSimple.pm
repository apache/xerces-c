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

package XML::Xerces::XMLSimple;

# From: Brian Faull <bfaull@mitre.org>
# Subject: Re: XML::Simple in Xerces (was Re: Xerces-perl for Win32)
# To: "Jason E. Stewart" <jason@openinformatics.com>
# CC: xerces-p-dev@xml.apache.org
# Date: Tue, 11 Nov 2003 23:29:26 -0500

# Construct the ridiculous data structure used by XML::Simple
# Each node (except toplevel!) is an arrayref.
# For text nodes, the hashref in the first element of the arrayref includes a key
#   'content' whose value is an arrayref of all of the text nodes within parent node.
# For attributes of a node, the first element of the arrayref includes a key having
#   the same name as the attribute name, and the value is an arrayref. If there exists
#   one or more child nodes with the same name as an attribute name, the first element
#   of the arrayref is the value of the attribute, and the remaining elements are
#   hashrefs containing one key 'content' whose value is the enclosed text.
# For children of the node, there is a key that points to an arrayref, as above.
# If there is more than one node of the same type, these are represented as individual
#   hashrefs within the enclosing arrayref.
# This is perhaps an oversimplification. Further information in XML::Simple

sub node2hash {
  my $node = shift;
  my $return = [{}];

  # build a hashref that represents the attributes of this element
  my $nodename = $node->getNodeName();
  if ($node->hasAttributes()) {
    my %attrs = $node->getAttributes();
    foreach my $attr (keys %attrs) {
      $return->[0]->{$attr} = $attrs{$attr};
    }
  }

  # recursively create structures for child nodes...
  # sorry, this gets kinda ugly...
  if ($node->hasChildNodes()) {
    my @text = ();
    foreach my $child ($node->getChildNodes()) {
      my $childname = $child->getNodeName();
      if ($child->isa('XML::Xerces::DOMElement')) {
        if (exists $return->[0]->{$childname}) {
          if (UNIVERSAL::isa($return->[0]->{$childname}, 'ARRAY')) {
            push @{$return->[0]->{$childname}}, node2hash($child)->[0];
          }
          else {
            $return->[0]->{$childname} = [$return->[0]->{$childname}, node2hash($child)->[0]];
          }
        }
        else { $return->[0]->{$childname} = node2hash($child); }
      }
      elsif ($child->isa('XML::Xerces::DOMText')) {
        my $val = $child->getNodeValue();
        push @text, $val
          if $val =~ m/\S+/;
      }
    }

    # Add text nodes as key 'content'
    if (exists $return->[0]->{'content'}) {
      if (UNIVERSAL::isa($return->[0]->{'content'}, 'ARRAY')) {
        unshift @{$return->[0]->{'content'}}, @text;
      }
      else { warn "content element should be array"; }
    }
    else {
      if (scalar @text == 1) { $return->[0]->{'content'} = $text[0]; }
      if (scalar @text >  1) { $return->[0]->{'content'} = \@text; }
    }
  }
  return $return;
}

