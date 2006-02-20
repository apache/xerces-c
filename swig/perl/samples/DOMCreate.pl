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
# DOMCreate
#
# This sample creates a DOM document in memory and then outputs
# the "printed" form of the document.
#
######################################################################

use strict;
# use blib;
use XML::Xerces;

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

#
# create a document
#

my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $dt = eval{$impl->createDocumentType('contributors', '', 'contributors.dtd')};
XML::Xerces::error($@) if $@;
my $doc = eval{$impl->createDocument('contributors', 'contributors',$dt)};
XML::Xerces::error($@) if $@;

my $root = $doc->getDocumentElement();

$root->appendChild(CreatePerson(	
	$doc,
	'Mike Pogue',
	'manager',
	'mpogue@us.ibm.com'
));

$root->appendChild(CreatePerson(
	$doc,
	'Tom Watson',
	'developer',
	'rtwatson@us.ibm.com'
));

$root->appendChild(CreatePerson(
	$doc,
	'Susan Hardenbrook',
	'tech writer',
	'susanhar@us.ibm.com'
));

my $writer = $impl->createDOMWriter();
if ($writer->canSetFeature('format-pretty-print',1)) {
  $writer->setFeature('format-pretty-print',1);
}
my $target = XML::Xerces::StdOutFormatTarget->new();
$writer->writeNode($target,$doc);
exit(0);


END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}

#################################################################
# routines to create the document
# no magic here ... they just organize many DOM calls
#################################################################


sub CreatePerson {
  my ($doc, $name, $role, $email) = @_;
  my $person = $doc->createElement ("person");
  &SetName ($doc, $person, $name);
  &SetEmail ($doc, $person, $email);
  $person->setAttribute ("Role", $role);
  return $person;
}


sub SetName {
  my ($doc, $person, $nameText) = @_;
  my $nameNode = $doc->createElement ("name");
  my $nameTextNode = $doc->createTextNode ($nameText);
  $nameNode->appendChild ($nameTextNode);
  $person->appendChild ($nameNode);
}


sub SetEmail {
  my ($doc, $person, $emailText) = @_;
  my $emailNode = $doc->createElement ("email");
  my $emailTextNode = $doc->createTextNode ($emailText);
  $emailNode->appendChild ($emailTextNode);
  $person->appendChild ($emailNode);
}


