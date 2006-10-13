 #
 # Copyright 2002,2004 The Apache Software Foundation.
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

# This file was created automatically by SWIG 1.3.30.
# Don't modify this file, modify the SWIG interface instead.
package XML::Xerces::DOM;
use base qw(DynaLoader);
require XML::Xerces;
require XML::Xerces::IO;
bootstrap XML::Xerces::DOM;
package XML::Xerces::DOM;
package XML::Xerces::DOMException;
use vars qw(@CODES);
@CODES = qw(__NONEXISTENT__
	    INDEX_SIZE_ERR
	    DOMSTRING_SIZE_ERR
	    HIERARCHY_REQUEST_ERR
	    WRONG_DOCUMENT_ERR
	    INVALID_CHARACTER_ERR
	    NO_DATA_ALLOWED_ERR
	    NO_MODIFICATION_ALLOWED_ERR
	    NOT_FOUND_ERR
	    NOT_SUPPORTED_ERR
	    INUSE_ATTRIBUTE_ERR
	    INVALID_STATE_ERR
	    SYNTAX_ERR
	    INVALID_MODIFICATION_ERR
	    NAMESPACE_ERR
	    INVALID_ACCESS_ERR
	   );

package XML::Xerces::DOMNodeList;
# convert the NodeList to a perl list
sub to_list {
  my $self = shift;
  my @list;
  for (my $i=0;$i<$self->getLength();$i++) {
    push(@list,$self->item($i));
  }
  return @list;
}

package XML::Xerces::DOMStringList;
# convert the StringList to a perl list
sub to_list {
  my $self = shift;
  my @list;
  for (my $i=0;$i<$self->getLength();$i++) {
    push(@list,$self->item($i));
  }
  return @list;
}

package XML::Xerces::DOMNamedNodeMap;
# convert the NamedNodeMap to a perl hash
sub to_hash {
  my $self = shift;
  my @list;
  for (my $i=0;$i<$self->getLength();$i++) {
    my $node = $self->item($i);
    push(@list, $node->to_hash());
  }
  return @list;
}

package XML::Xerces::DOMNode;
sub to_hash {
  my $self = shift;
  return ($self->getNodeName,$self->getNodeValue);
}

sub quote_content {
  my ($self,$node_value) = @_;

  $node_value =~ s/&/&amp;/g;
  $node_value =~ s/</&lt;/g;
  $node_value =~ s/>/&gt;/g;
  $node_value =~ s/\"/&quot;/g;
  $node_value =~ s/\'/&apos;/g;

  return $node_value;
}

package XML::Xerces::DOMEntity;
sub to_hash {
  my $self = shift;
  if ($self->hasChildNodes) {
    return ($self->getNodeName(),
            $self->getFirstChild->getNodeValue());
  } else {
    return ($self->getNodeName(), '');
  }
}

package XML::Xerces::DOMText;
sub serialize {
  return $_[0]->quote_content($_[0]->getNodeValue);
}

package XML::Xerces::DOMProcessingInstruction;
sub serialize {
  my $output .= '<?' . $_[0]->getNodeName;
  if (length(my $str = $_[0]->getNodeValue)) {
    $output .= " $str"; 
  }
  $output .= '?>';
  return $output;
}

package XML::Xerces::DOMDocument;
sub serialize {
  my $output;
  my $indent = 2;
  for(my $child = $_[0]->getFirstChild() ;
     defined $child ;
     $child = $child->getNextSibling())
  {
    $output .= $child->serialize($indent);
  }
  return "$output\n";
}

package XML::Xerces::DOMElement;
sub serialize {
  my ($self,$indent) = @_;
  $indent ||= 0;
  my $output;
  ELEMENT: {
    my $node_name = $self->getNodeName;
    $output .= "<$node_name";

    my $attributes = $self->getAttributes;
    my $attribute_count = $attributes->getLength;

    for(my $ix = 0 ; $ix < $attribute_count ; ++$ix) {
      my $attribute = $attributes->item($ix);
      $output .= ' ' . $attribute->getNodeName . '="' . $self->quote_content($attribute->getNodeValue) . '"';
    }

    my $child = $self->getFirstChild();
    if (!defined $child) {
      $output .= '/>';
      last ELEMENT;
    }

    $output .= '>';
    while (defined $child) {
      $output .= $child->serialize($indent+2);
      $child = $child->getNextSibling();
    }
    $output .= "</$node_name>";
  }
  return $output;
}

package XML::Xerces::DOMEntityReference;
sub serialize {
  my ($self) = @_;
  my $output;
  for(my $child = $self->getFirstChild() ;
     defined $child;
     $child = $child->getNextSibling())
  {
    $output .= $child->serialize();
  }
  return $output;
}

package XML::Xerces::DOMCDATASection;
sub serialize {
  return '<![CDATA[' . $_[0]->getNodeValue . ']]>';
}

package XML::Xerces::DOMComment;
sub serialize {
  return '<!--' . $_[0]->getNodeValue . "-->\n";
}

package XML::Xerces::DOMDocumentType;
sub serialize {
  my $output;
  $output .= '<!DOCTYPE ' . $_[0]->getNodeName;

  my $id;
  if ($id = $_[0]->getPublicId) {
    $output .= qq[ PUBLIC "$id"];
    if ($id = $_[0]->getSystemId) {
      $output .= qq[ "$id"];
    }
  } elsif ($id = $_[0]->getSystemId) {
    $output .= qq[ SYSTEM "$id"];
  }

  if ($id = $_[0]->getInternalSubset) {
    $output .= " [$id]";
  }

  $output .= ">\n";
  return $output;
}

package XML::Xerces::DOMEntity;
sub serialize {
  my $output;
  $output .= '<!ENTITY ' . $_[0]->getNodeName;

  my $id;
  if ($id = $_[0]->getPublicId) { $output .= qq[ PUBLIC "$id"]; }
  if ($id = $_[0]->getSystemId) { $output .= qq[ SYSTEM "$id"]; }
  if ($id = $_[0]->getNotationName) { $output .= qq[ NDATA "$id"]; }

  $output .= '>';
  return $output;
}

package XML::Xerces::DOMException;
sub getMessage {
  return shift->{msg};
}

sub getCode {
  return shift->{code};
}

# in previous versions we needed to define this method
# but it is now obsolete
package XML::Xerces::DOMElement;
sub get_text {
  my $self = shift;
  warn "XML::Xerces::DOMElement::get_text is depricated, use getTextContent instead";
  return $self->getTextContent(@_);
}

############# Class : XML::Xerces::PerlNodeFilter ##############
package XML::Xerces::PerlNodeFilter;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::DOMNodeFilter);
sub new {
  my $class = shift;
  return bless {}, $class;
}

sub acceptNode {
  return undef;
}


############# Class : XML::Xerces::PerlDOMErrorHandler ##############
package XML::Xerces::PerlDOMErrorHandler;
use Carp;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::DOMErrorHandler);

sub new {
  my $class = shift;
  return bless {}, $class;
}

sub handleError {
  my $severity = $_[1]->getSeverity;
  my $msg = $_[1]->getMessage;
  croak(<<EOT);
ERROR:
SEVERITY: $severity
MESSAGE:  $msg
EOT
}

# ------- FUNCTION WRAPPERS --------

package XML::Xerces;


############# Class : XML::Xerces::DOMException ##############

package XML::Xerces::DOMException;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_DOMException(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMException($self);
        delete $OWNER{$self};
    }
}

*INDEX_SIZE_ERR = *XML::Xercesc::DOMException_INDEX_SIZE_ERR;
*DOMSTRING_SIZE_ERR = *XML::Xercesc::DOMException_DOMSTRING_SIZE_ERR;
*HIERARCHY_REQUEST_ERR = *XML::Xercesc::DOMException_HIERARCHY_REQUEST_ERR;
*WRONG_DOCUMENT_ERR = *XML::Xercesc::DOMException_WRONG_DOCUMENT_ERR;
*INVALID_CHARACTER_ERR = *XML::Xercesc::DOMException_INVALID_CHARACTER_ERR;
*NO_DATA_ALLOWED_ERR = *XML::Xercesc::DOMException_NO_DATA_ALLOWED_ERR;
*NO_MODIFICATION_ALLOWED_ERR = *XML::Xercesc::DOMException_NO_MODIFICATION_ALLOWED_ERR;
*NOT_FOUND_ERR = *XML::Xercesc::DOMException_NOT_FOUND_ERR;
*NOT_SUPPORTED_ERR = *XML::Xercesc::DOMException_NOT_SUPPORTED_ERR;
*INUSE_ATTRIBUTE_ERR = *XML::Xercesc::DOMException_INUSE_ATTRIBUTE_ERR;
*INVALID_STATE_ERR = *XML::Xercesc::DOMException_INVALID_STATE_ERR;
*SYNTAX_ERR = *XML::Xercesc::DOMException_SYNTAX_ERR;
*INVALID_MODIFICATION_ERR = *XML::Xercesc::DOMException_INVALID_MODIFICATION_ERR;
*NAMESPACE_ERR = *XML::Xercesc::DOMException_NAMESPACE_ERR;
*INVALID_ACCESS_ERR = *XML::Xercesc::DOMException_INVALID_ACCESS_ERR;
*VALIDATION_ERR = *XML::Xercesc::DOMException_VALIDATION_ERR;
*TYPE_MISMATCH_ERR = *XML::Xercesc::DOMException_TYPE_MISMATCH_ERR;
*getMessage = *XML::Xercesc::DOMException_getMessage;
*swig_code_get = *XML::Xercesc::DOMException_code_get;
*swig_code_set = *XML::Xercesc::DOMException_code_set;
*swig_msg_get = *XML::Xercesc::DOMException_msg_get;
*swig_msg_set = *XML::Xercesc::DOMException_msg_set;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNode ##############

package XML::Xerces::DOMNode;
use overload
    "!=" => sub { $_[0]->__ne__($_[1])},
    "==" => sub { $_[0]->__eq__($_[1])},
    "fallback" => 1;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNode($self);
        delete $OWNER{$self};
    }
}

*ELEMENT_NODE = *XML::Xercesc::DOMNode_ELEMENT_NODE;
*ATTRIBUTE_NODE = *XML::Xercesc::DOMNode_ATTRIBUTE_NODE;
*TEXT_NODE = *XML::Xercesc::DOMNode_TEXT_NODE;
*CDATA_SECTION_NODE = *XML::Xercesc::DOMNode_CDATA_SECTION_NODE;
*ENTITY_REFERENCE_NODE = *XML::Xercesc::DOMNode_ENTITY_REFERENCE_NODE;
*ENTITY_NODE = *XML::Xercesc::DOMNode_ENTITY_NODE;
*PROCESSING_INSTRUCTION_NODE = *XML::Xercesc::DOMNode_PROCESSING_INSTRUCTION_NODE;
*COMMENT_NODE = *XML::Xercesc::DOMNode_COMMENT_NODE;
*DOCUMENT_NODE = *XML::Xercesc::DOMNode_DOCUMENT_NODE;
*DOCUMENT_TYPE_NODE = *XML::Xercesc::DOMNode_DOCUMENT_TYPE_NODE;
*DOCUMENT_FRAGMENT_NODE = *XML::Xercesc::DOMNode_DOCUMENT_FRAGMENT_NODE;
*NOTATION_NODE = *XML::Xercesc::DOMNode_NOTATION_NODE;
*DOCUMENT_POSITION_DISCONNECTED = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_DISCONNECTED;
*DOCUMENT_POSITION_PRECEDING = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_PRECEDING;
*DOCUMENT_POSITION_FOLLOWING = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_FOLLOWING;
*DOCUMENT_POSITION_CONTAINS = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_CONTAINS;
*DOCUMENT_POSITION_CONTAINED_BY = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_CONTAINED_BY;
*DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC;
*getNodeName = *XML::Xercesc::DOMNode_getNodeName;
*getNodeValue = *XML::Xercesc::DOMNode_getNodeValue;
*getNodeType = *XML::Xercesc::DOMNode_getNodeType;
*getParentNode = *XML::Xercesc::DOMNode_getParentNode;
sub getChildNodes {
    my $result = XML::Xercesc::DOMNode_getChildNodes (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
*getFirstChild = *XML::Xercesc::DOMNode_getFirstChild;
*getLastChild = *XML::Xercesc::DOMNode_getLastChild;
*getPreviousSibling = *XML::Xercesc::DOMNode_getPreviousSibling;
*getNextSibling = *XML::Xercesc::DOMNode_getNextSibling;
sub getAttributes {
    my $result = XML::Xercesc::DOMNode_getAttributes (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_hash() if wantarray;
    return $result;#if *not* wantarray
}
  
*getOwnerDocument = *XML::Xercesc::DOMNode_getOwnerDocument;
*cloneNode = *XML::Xercesc::DOMNode_cloneNode;
*insertBefore = *XML::Xercesc::DOMNode_insertBefore;
*replaceChild = *XML::Xercesc::DOMNode_replaceChild;
*removeChild = *XML::Xercesc::DOMNode_removeChild;
*appendChild = *XML::Xercesc::DOMNode_appendChild;
*hasChildNodes = *XML::Xercesc::DOMNode_hasChildNodes;
*setNodeValue = *XML::Xercesc::DOMNode_setNodeValue;
*normalize = *XML::Xercesc::DOMNode_normalize;
*isSupported = *XML::Xercesc::DOMNode_isSupported;
*getNamespaceURI = *XML::Xercesc::DOMNode_getNamespaceURI;
*getLocalName = *XML::Xercesc::DOMNode_getLocalName;
*setPrefix = *XML::Xercesc::DOMNode_setPrefix;
*hasAttributes = *XML::Xercesc::DOMNode_hasAttributes;
*isSameNode = *XML::Xercesc::DOMNode_isSameNode;
*isEqualNode = *XML::Xercesc::DOMNode_isEqualNode;
*setUserData = *XML::Xercesc::DOMNode_setUserData;
*getUserData = *XML::Xercesc::DOMNode_getUserData;
*getBaseURI = *XML::Xercesc::DOMNode_getBaseURI;
*compareDocumentPosition = *XML::Xercesc::DOMNode_compareDocumentPosition;
*getTextContent = *XML::Xercesc::DOMNode_getTextContent;
*setTextContent = *XML::Xercesc::DOMNode_setTextContent;
*lookupPrefix = *XML::Xercesc::DOMNode_lookupPrefix;
*isDefaultNamespace = *XML::Xercesc::DOMNode_isDefaultNamespace;
*lookupNamespaceURI = *XML::Xercesc::DOMNode_lookupNamespaceURI;
*getFeature = *XML::Xercesc::DOMNode_getFeature;
*release = *XML::Xercesc::DOMNode_release;
*__eq__ = *XML::Xercesc::DOMNode___eq__;
*__ne__ = *XML::Xercesc::DOMNode___ne__;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMAttr ##############

package XML::Xerces::DOMAttr;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMAttr($self);
        delete $OWNER{$self};
    }
}

*getName = *XML::Xercesc::DOMAttr_getName;
*getSpecified = *XML::Xercesc::DOMAttr_getSpecified;
*getValue = *XML::Xercesc::DOMAttr_getValue;
*setValue = *XML::Xercesc::DOMAttr_setValue;
*getOwnerElement = *XML::Xercesc::DOMAttr_getOwnerElement;
*isId = *XML::Xercesc::DOMAttr_isId;
*getSchemaTypeInfo = *XML::Xercesc::DOMAttr_getSchemaTypeInfo;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMElement ##############

package XML::Xerces::DOMElement;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMElement($self);
        delete $OWNER{$self};
    }
}

*getTagName = *XML::Xercesc::DOMElement_getTagName;
*getAttribute = *XML::Xercesc::DOMElement_getAttribute;
*getAttributeNode = *XML::Xercesc::DOMElement_getAttributeNode;
sub getElementsByTagName {
    my $result = XML::Xercesc::DOMElement_getElementsByTagName (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
*setAttribute = *XML::Xercesc::DOMElement_setAttribute;
*setAttributeNode = *XML::Xercesc::DOMElement_setAttributeNode;
*removeAttributeNode = *XML::Xercesc::DOMElement_removeAttributeNode;
*removeAttribute = *XML::Xercesc::DOMElement_removeAttribute;
*getAttributeNS = *XML::Xercesc::DOMElement_getAttributeNS;
*setAttributeNS = *XML::Xercesc::DOMElement_setAttributeNS;
*removeAttributeNS = *XML::Xercesc::DOMElement_removeAttributeNS;
*getAttributeNodeNS = *XML::Xercesc::DOMElement_getAttributeNodeNS;
*setAttributeNodeNS = *XML::Xercesc::DOMElement_setAttributeNodeNS;
sub getElementsByTagNameNS {
    my $result = XML::Xercesc::DOMElement_getElementsByTagNameNS (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
*hasAttribute = *XML::Xercesc::DOMElement_hasAttribute;
*hasAttributeNS = *XML::Xercesc::DOMElement_hasAttributeNS;
*setIdAttribute = *XML::Xercesc::DOMElement_setIdAttribute;
*setIdAttributeNS = *XML::Xercesc::DOMElement_setIdAttributeNS;
*setIdAttributeNode = *XML::Xercesc::DOMElement_setIdAttributeNode;
*getSchemaTypeInfo = *XML::Xercesc::DOMElement_getSchemaTypeInfo;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMEntity ##############

package XML::Xerces::DOMEntity;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMEntity($self);
        delete $OWNER{$self};
    }
}

*getPublicId = *XML::Xercesc::DOMEntity_getPublicId;
*getSystemId = *XML::Xercesc::DOMEntity_getSystemId;
*getNotationName = *XML::Xercesc::DOMEntity_getNotationName;
*getInputEncoding = *XML::Xercesc::DOMEntity_getInputEncoding;
*getXmlEncoding = *XML::Xercesc::DOMEntity_getXmlEncoding;
*getXmlVersion = *XML::Xercesc::DOMEntity_getXmlVersion;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMDocumentType ##############

package XML::Xerces::DOMDocumentType;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMDocumentType($self);
        delete $OWNER{$self};
    }
}

*getName = *XML::Xercesc::DOMDocumentType_getName;
sub getEntities {
    my $result = XML::Xercesc::DOMDocumentType_getEntities (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_hash() if wantarray;
    return $result;#if *not* wantarray
}
  
sub getNotations {
    my $result = XML::Xercesc::DOMDocumentType_getNotations (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_hash() if wantarray;
    return $result;#if *not* wantarray
}
  
*getPublicId = *XML::Xercesc::DOMDocumentType_getPublicId;
*getSystemId = *XML::Xercesc::DOMDocumentType_getSystemId;
*getInternalSubset = *XML::Xercesc::DOMDocumentType_getInternalSubset;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMCharacterData ##############

package XML::Xerces::DOMCharacterData;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMCharacterData($self);
        delete $OWNER{$self};
    }
}

*getData = *XML::Xercesc::DOMCharacterData_getData;
*getLength = *XML::Xercesc::DOMCharacterData_getLength;
*substringData = *XML::Xercesc::DOMCharacterData_substringData;
*appendData = *XML::Xercesc::DOMCharacterData_appendData;
*insertData = *XML::Xercesc::DOMCharacterData_insertData;
*deleteData = *XML::Xercesc::DOMCharacterData_deleteData;
*replaceData = *XML::Xercesc::DOMCharacterData_replaceData;
*setData = *XML::Xercesc::DOMCharacterData_setData;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMComment ##############

package XML::Xerces::DOMComment;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMCharacterData XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMComment($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMText ##############

package XML::Xerces::DOMText;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMCharacterData XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMText($self);
        delete $OWNER{$self};
    }
}

*splitText = *XML::Xercesc::DOMText_splitText;
*getIsElementContentWhitespace = *XML::Xercesc::DOMText_getIsElementContentWhitespace;
*getWholeText = *XML::Xercesc::DOMText_getWholeText;
*replaceWholeText = *XML::Xercesc::DOMText_replaceWholeText;
*isIgnorableWhitespace = *XML::Xercesc::DOMText_isIgnorableWhitespace;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMCDATASection ##############

package XML::Xerces::DOMCDATASection;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMText XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMCDATASection($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNodeList ##############

package XML::Xerces::DOMNodeList;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNodeList($self);
        delete $OWNER{$self};
    }
}

*item = *XML::Xercesc::DOMNodeList_item;
*getLength = *XML::Xercesc::DOMNodeList_getLength;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNamedNodeMap ##############

package XML::Xerces::DOMNamedNodeMap;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNamedNodeMap($self);
        delete $OWNER{$self};
    }
}

*setNamedItem = *XML::Xercesc::DOMNamedNodeMap_setNamedItem;
*item = *XML::Xercesc::DOMNamedNodeMap_item;
*getNamedItem = *XML::Xercesc::DOMNamedNodeMap_getNamedItem;
*getLength = *XML::Xercesc::DOMNamedNodeMap_getLength;
*removeNamedItem = *XML::Xercesc::DOMNamedNodeMap_removeNamedItem;
*getNamedItemNS = *XML::Xercesc::DOMNamedNodeMap_getNamedItemNS;
*setNamedItemNS = *XML::Xercesc::DOMNamedNodeMap_setNamedItemNS;
*removeNamedItemNS = *XML::Xercesc::DOMNamedNodeMap_removeNamedItemNS;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMDocumentFragment ##############

package XML::Xerces::DOMDocumentFragment;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMDocumentFragment($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMEntityReference ##############

package XML::Xerces::DOMEntityReference;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMEntityReference($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNotation ##############

package XML::Xerces::DOMNotation;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNotation($self);
        delete $OWNER{$self};
    }
}

*getPublicId = *XML::Xercesc::DOMNotation_getPublicId;
*getSystemId = *XML::Xercesc::DOMNotation_getSystemId;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMProcessingInstruction ##############

package XML::Xerces::DOMProcessingInstruction;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMProcessingInstruction($self);
        delete $OWNER{$self};
    }
}

*getTarget = *XML::Xercesc::DOMProcessingInstruction_getTarget;
*getData = *XML::Xercesc::DOMProcessingInstruction_getData;
*setData = *XML::Xercesc::DOMProcessingInstruction_setData;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMTypeInfo ##############

package XML::Xerces::DOMTypeInfo;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMTypeInfo($self);
        delete $OWNER{$self};
    }
}

*DERIVATION_RESTRICTION = *XML::Xercesc::DOMTypeInfo_DERIVATION_RESTRICTION;
*DERIVATION_EXTENSION = *XML::Xercesc::DOMTypeInfo_DERIVATION_EXTENSION;
*DERIVATION_UNION = *XML::Xercesc::DOMTypeInfo_DERIVATION_UNION;
*DERIVATION_LIST = *XML::Xercesc::DOMTypeInfo_DERIVATION_LIST;
*getTypeName = *XML::Xercesc::DOMTypeInfo_getTypeName;
*getTypeNamespace = *XML::Xercesc::DOMTypeInfo_getTypeNamespace;
*isDerivedFrom = *XML::Xercesc::DOMTypeInfo_isDerivedFrom;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMPSVITypeInfo ##############

package XML::Xerces::DOMPSVITypeInfo;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*PSVI_Validity = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Validity;
*PSVI_Validitation_Attempted = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Validitation_Attempted;
*PSVI_Type_Definition_Type = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Type_Definition_Type;
*PSVI_Type_Definition_Name = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Type_Definition_Name;
*PSVI_Type_Definition_Namespace = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Type_Definition_Namespace;
*PSVI_Type_Definition_Anonymous = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Type_Definition_Anonymous;
*PSVI_Nil = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Nil;
*PSVI_Member_Type_Definition_Name = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Member_Type_Definition_Name;
*PSVI_Member_Type_Definition_Namespace = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Member_Type_Definition_Namespace;
*PSVI_Member_Type_Definition_Anonymous = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Member_Type_Definition_Anonymous;
*PSVI_Schema_Default = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Schema_Default;
*PSVI_Schema_Normalized_Value = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Schema_Normalized_Value;
*PSVI_Schema_Specified = *XML::Xercesc::DOMPSVITypeInfo_PSVI_Schema_Specified;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMPSVITypeInfo($self);
        delete $OWNER{$self};
    }
}

*getStringProperty = *XML::Xercesc::DOMPSVITypeInfo_getStringProperty;
*getNumericProperty = *XML::Xercesc::DOMPSVITypeInfo_getNumericProperty;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMDocumentRange ##############

package XML::Xerces::DOMDocumentRange;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMDocumentRange($self);
        delete $OWNER{$self};
    }
}

*createRange = *XML::Xercesc::DOMDocumentRange_createRange;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMDocumentTraversal ##############

package XML::Xerces::DOMDocumentTraversal;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMDocumentTraversal($self);
        delete $OWNER{$self};
    }
}

sub createNodeIterator {
    my ($self,$root,$what,$filter,$expand) = @_;
    my $callback = XML::Xerces::PerlNodeFilterCallbackHandler->new($filter);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__NODE_FILTER} = $callback;

    my @args = ($self,$root,$what,$callback,$expand);
    if ($args[0]->isa('XML::Xerces::DOMDocument')) {

    }
    return XML::Xercesc::DOMDocumentTraversal_createNodeIterator(@args);
}

sub createTreeWalker {
    my ($self,$root,$what,$filter,$expand) = @_;
    my $callback = XML::Xerces::PerlNodeFilterCallbackHandler->new($filter);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__NODE_FILTER} = $callback;

    my @args = ($self,$root,$what,$callback,$expand);
    if ($args[0]->isa('XML::Xerces::DOMDocument')) {

    }
    return XML::Xercesc::DOMDocumentTraversal_createTreeWalker(@args);
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNodeIterator ##############

package XML::Xerces::DOMNodeIterator;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNodeIterator($self);
        delete $OWNER{$self};
    }
}

*getRoot = *XML::Xercesc::DOMNodeIterator_getRoot;
*getWhatToShow = *XML::Xercesc::DOMNodeIterator_getWhatToShow;
*getFilter = *XML::Xercesc::DOMNodeIterator_getFilter;
*getExpandEntityReferences = *XML::Xercesc::DOMNodeIterator_getExpandEntityReferences;
*nextNode = *XML::Xercesc::DOMNodeIterator_nextNode;
*previousNode = *XML::Xercesc::DOMNodeIterator_previousNode;
*detach = *XML::Xercesc::DOMNodeIterator_detach;
*release = *XML::Xercesc::DOMNodeIterator_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNodeFilter ##############

package XML::Xerces::DOMNodeFilter;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNodeFilter($self);
        delete $OWNER{$self};
    }
}

*FILTER_ACCEPT = *XML::Xercesc::DOMNodeFilter_FILTER_ACCEPT;
*FILTER_REJECT = *XML::Xercesc::DOMNodeFilter_FILTER_REJECT;
*FILTER_SKIP = *XML::Xercesc::DOMNodeFilter_FILTER_SKIP;
*SHOW_ALL = *XML::Xercesc::DOMNodeFilter_SHOW_ALL;
*SHOW_ELEMENT = *XML::Xercesc::DOMNodeFilter_SHOW_ELEMENT;
*SHOW_ATTRIBUTE = *XML::Xercesc::DOMNodeFilter_SHOW_ATTRIBUTE;
*SHOW_TEXT = *XML::Xercesc::DOMNodeFilter_SHOW_TEXT;
*SHOW_CDATA_SECTION = *XML::Xercesc::DOMNodeFilter_SHOW_CDATA_SECTION;
*SHOW_ENTITY_REFERENCE = *XML::Xercesc::DOMNodeFilter_SHOW_ENTITY_REFERENCE;
*SHOW_ENTITY = *XML::Xercesc::DOMNodeFilter_SHOW_ENTITY;
*SHOW_PROCESSING_INSTRUCTION = *XML::Xercesc::DOMNodeFilter_SHOW_PROCESSING_INSTRUCTION;
*SHOW_COMMENT = *XML::Xercesc::DOMNodeFilter_SHOW_COMMENT;
*SHOW_DOCUMENT = *XML::Xercesc::DOMNodeFilter_SHOW_DOCUMENT;
*SHOW_DOCUMENT_TYPE = *XML::Xercesc::DOMNodeFilter_SHOW_DOCUMENT_TYPE;
*SHOW_DOCUMENT_FRAGMENT = *XML::Xercesc::DOMNodeFilter_SHOW_DOCUMENT_FRAGMENT;
*SHOW_NOTATION = *XML::Xercesc::DOMNodeFilter_SHOW_NOTATION;
*acceptNode = *XML::Xercesc::DOMNodeFilter_acceptNode;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMRange ##############

package XML::Xerces::DOMRange;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMRange($self);
        delete $OWNER{$self};
    }
}

*START_TO_START = *XML::Xercesc::DOMRange_START_TO_START;
*START_TO_END = *XML::Xercesc::DOMRange_START_TO_END;
*END_TO_END = *XML::Xercesc::DOMRange_END_TO_END;
*END_TO_START = *XML::Xercesc::DOMRange_END_TO_START;
*getStartContainer = *XML::Xercesc::DOMRange_getStartContainer;
*getStartOffset = *XML::Xercesc::DOMRange_getStartOffset;
*getEndContainer = *XML::Xercesc::DOMRange_getEndContainer;
*getEndOffset = *XML::Xercesc::DOMRange_getEndOffset;
*getCollapsed = *XML::Xercesc::DOMRange_getCollapsed;
*getCommonAncestorContainer = *XML::Xercesc::DOMRange_getCommonAncestorContainer;
*setStart = *XML::Xercesc::DOMRange_setStart;
*setEnd = *XML::Xercesc::DOMRange_setEnd;
*setStartBefore = *XML::Xercesc::DOMRange_setStartBefore;
*setStartAfter = *XML::Xercesc::DOMRange_setStartAfter;
*setEndBefore = *XML::Xercesc::DOMRange_setEndBefore;
*setEndAfter = *XML::Xercesc::DOMRange_setEndAfter;
*collapse = *XML::Xercesc::DOMRange_collapse;
*selectNode = *XML::Xercesc::DOMRange_selectNode;
*selectNodeContents = *XML::Xercesc::DOMRange_selectNodeContents;
*compareBoundaryPoints = *XML::Xercesc::DOMRange_compareBoundaryPoints;
*deleteContents = *XML::Xercesc::DOMRange_deleteContents;
*extractContents = *XML::Xercesc::DOMRange_extractContents;
*cloneContents = *XML::Xercesc::DOMRange_cloneContents;
*insertNode = *XML::Xercesc::DOMRange_insertNode;
*surroundContents = *XML::Xercesc::DOMRange_surroundContents;
*cloneRange = *XML::Xercesc::DOMRange_cloneRange;
*toString = *XML::Xercesc::DOMRange_toString;
*detach = *XML::Xercesc::DOMRange_detach;
*release = *XML::Xercesc::DOMRange_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMRangeException ##############

package XML::Xerces::DOMRangeException;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMException XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*BAD_BOUNDARYPOINTS_ERR = *XML::Xercesc::DOMRangeException_BAD_BOUNDARYPOINTS_ERR;
*INVALID_NODE_TYPE_ERR = *XML::Xercesc::DOMRangeException_INVALID_NODE_TYPE_ERR;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_DOMRangeException(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMRangeException($self);
        delete $OWNER{$self};
    }
}

*swig_code_get = *XML::Xercesc::DOMRangeException_code_get;
*swig_code_set = *XML::Xercesc::DOMRangeException_code_set;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMTreeWalker ##############

package XML::Xerces::DOMTreeWalker;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMTreeWalker($self);
        delete $OWNER{$self};
    }
}

*getRoot = *XML::Xercesc::DOMTreeWalker_getRoot;
*getWhatToShow = *XML::Xercesc::DOMTreeWalker_getWhatToShow;
*getFilter = *XML::Xercesc::DOMTreeWalker_getFilter;
*getExpandEntityReferences = *XML::Xercesc::DOMTreeWalker_getExpandEntityReferences;
*getCurrentNode = *XML::Xercesc::DOMTreeWalker_getCurrentNode;
*parentNode = *XML::Xercesc::DOMTreeWalker_parentNode;
*firstChild = *XML::Xercesc::DOMTreeWalker_firstChild;
*lastChild = *XML::Xercesc::DOMTreeWalker_lastChild;
*previousSibling = *XML::Xercesc::DOMTreeWalker_previousSibling;
*nextSibling = *XML::Xercesc::DOMTreeWalker_nextSibling;
*previousNode = *XML::Xercesc::DOMTreeWalker_previousNode;
*nextNode = *XML::Xercesc::DOMTreeWalker_nextNode;
*setCurrentNode = *XML::Xercesc::DOMTreeWalker_setCurrentNode;
*release = *XML::Xercesc::DOMTreeWalker_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMStringList ##############

package XML::Xerces::DOMStringList;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMStringList($self);
        delete $OWNER{$self};
    }
}

*item = *XML::Xercesc::DOMStringList_item;
*getLength = *XML::Xercesc::DOMStringList_getLength;
*contains = *XML::Xercesc::DOMStringList_contains;
*release = *XML::Xercesc::DOMStringList_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMUserDataHandler ##############

package XML::Xerces::DOMUserDataHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMUserDataHandler($self);
        delete $OWNER{$self};
    }
}

*NODE_CLONED = *XML::Xercesc::DOMUserDataHandler_NODE_CLONED;
*NODE_IMPORTED = *XML::Xercesc::DOMUserDataHandler_NODE_IMPORTED;
*NODE_DELETED = *XML::Xercesc::DOMUserDataHandler_NODE_DELETED;
*NODE_RENAMED = *XML::Xercesc::DOMUserDataHandler_NODE_RENAMED;
*NODE_ADOPTED = *XML::Xercesc::DOMUserDataHandler_NODE_ADOPTED;
*handle = *XML::Xercesc::DOMUserDataHandler_handle;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMConfiguration ##############

package XML::Xerces::DOMConfiguration;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
sub setParameter {
  my ($self,$name,$value) = @_;

  # we allow undef to unset existing values
  if (defined $value) {
    if ($name eq $XML::Xerces::XMLUni::fgDOMErrorHandler) {
      $value = XML::Xerces::PerlDOMCallbackHandler->new($value);
      $XML::Xerces::REMEMBER{tied(% {$self})}->{__ERROR_HANDLER} = $value;
    } elsif ($name eq $XML::Xerces::XMLUni::fgDOMResourceResolver) {
      $value = XML::Xerces::PerlDOMCallbackHandler->new($value);
      $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $value;
    } elsif ($name eq $XML::Xerces::XMLUni::fgXercesEntityResolver) {
      $value = XML::Xerces::PerlEntityResolverHandler->new($value);
      $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $value;
    }
  }
  return XML::Xercesc::DOMConfiguration_setParameter($self,$name,$value);
}

*getParameter = *XML::Xercesc::DOMConfiguration_getParameter;
*canSetParameter = *XML::Xercesc::DOMConfiguration_canSetParameter;
sub getParameterNames {
    my $result = XML::Xercesc::DOMConfiguration_getParameterNames (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined$self;

    if ($self->can('cleanup')) {
        $_[0]->cleanup();
    }

    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMConfiguration($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMImplementationLS ##############

package XML::Xerces::DOMImplementationLS;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMImplementationLS($self);
        delete $OWNER{$self};
    }
}

*MODE_SYNCHRONOUS = *XML::Xercesc::DOMImplementationLS_MODE_SYNCHRONOUS;
*MODE_ASYNCHRONOUS = *XML::Xercesc::DOMImplementationLS_MODE_ASYNCHRONOUS;
*createLSParser = *XML::Xercesc::DOMImplementationLS_createLSParser;
*createLSSerializer = *XML::Xercesc::DOMImplementationLS_createLSSerializer;
*createLSInput = *XML::Xercesc::DOMImplementationLS_createLSInput;
*createLSOutput = *XML::Xercesc::DOMImplementationLS_createLSOutput;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMImplementation ##############

package XML::Xerces::DOMImplementation;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMImplementationLS XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMImplementation($self);
        delete $OWNER{$self};
    }
}

*hasFeature = *XML::Xercesc::DOMImplementation_hasFeature;
*createDocumentType = *XML::Xercesc::DOMImplementation_createDocumentType;
*getFeature = *XML::Xercesc::DOMImplementation_getFeature;
sub createDocument {
    if (UNIVERSAL::isa($_[3],"XML::Xerces::DOMDocumentType")) {
        $_[3]->DISOWN();
    }

    my $return = XML::Xercesc::DOMImplementation_createDocument(@_);
    
    return $return;
}

*getImplementation = *XML::Xercesc::DOMImplementation_getImplementation;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMImplementationList ##############

package XML::Xerces::DOMImplementationList;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMImplementationList($self);
        delete $OWNER{$self};
    }
}

*item = *XML::Xercesc::DOMImplementationList_item;
*getLength = *XML::Xercesc::DOMImplementationList_getLength;
*release = *XML::Xercesc::DOMImplementationList_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMImplementationRegistry ##############

package XML::Xerces::DOMImplementationRegistry;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*getDOMImplementation = *XML::Xercesc::DOMImplementationRegistry_getDOMImplementation;
*getDOMImplementationList = *XML::Xercesc::DOMImplementationRegistry_getDOMImplementationList;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMImplementationRegistry($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMError ##############

package XML::Xerces::DOMError;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMError($self);
        delete $OWNER{$self};
    }
}

*DOM_SEVERITY_WARNING = *XML::Xercesc::DOMError_DOM_SEVERITY_WARNING;
*DOM_SEVERITY_ERROR = *XML::Xercesc::DOMError_DOM_SEVERITY_ERROR;
*DOM_SEVERITY_FATAL_ERROR = *XML::Xercesc::DOMError_DOM_SEVERITY_FATAL_ERROR;
*getSeverity = *XML::Xercesc::DOMError_getSeverity;
*getMessage = *XML::Xercesc::DOMError_getMessage;
*getLocation = *XML::Xercesc::DOMError_getLocation;
*getRelatedException = *XML::Xercesc::DOMError_getRelatedException;
*getType = *XML::Xercesc::DOMError_getType;
*getRelatedData = *XML::Xercesc::DOMError_getRelatedData;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMErrorHandler ##############

package XML::Xerces::DOMErrorHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMErrorHandler($self);
        delete $OWNER{$self};
    }
}

*handleError = *XML::Xercesc::DOMErrorHandler_handleError;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMDocument ##############

package XML::Xerces::DOMDocument;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMDocumentRange XML::Xerces::DOMDocumentTraversal XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined$self;

    if ($self->can('cleanup')) {
        $_[0]->cleanup();
    }

    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMDocument($self);
        delete $OWNER{$self};
    }
}

*createElement = *XML::Xercesc::DOMDocument_createElement;
*createDocumentFragment = *XML::Xercesc::DOMDocument_createDocumentFragment;
*createTextNode = *XML::Xercesc::DOMDocument_createTextNode;
*createComment = *XML::Xercesc::DOMDocument_createComment;
*createCDATASection = *XML::Xercesc::DOMDocument_createCDATASection;
*createProcessingInstruction = *XML::Xercesc::DOMDocument_createProcessingInstruction;
*createAttribute = *XML::Xercesc::DOMDocument_createAttribute;
*createEntityReference = *XML::Xercesc::DOMDocument_createEntityReference;
*getDoctype = *XML::Xercesc::DOMDocument_getDoctype;
*getImplementation = *XML::Xercesc::DOMDocument_getImplementation;
*getDocumentElement = *XML::Xercesc::DOMDocument_getDocumentElement;
sub getElementsByTagName {
    my $result = XML::Xercesc::DOMDocument_getElementsByTagName (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
*importNode = *XML::Xercesc::DOMDocument_importNode;
*createAttributeNS = *XML::Xercesc::DOMDocument_createAttributeNS;
sub getElementsByTagNameNS {
    my $result = XML::Xercesc::DOMDocument_getElementsByTagNameNS (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
*getElementById = *XML::Xercesc::DOMDocument_getElementById;
*getInputEncoding = *XML::Xercesc::DOMDocument_getInputEncoding;
*getXmlEncoding = *XML::Xercesc::DOMDocument_getXmlEncoding;
*getXmlStandalone = *XML::Xercesc::DOMDocument_getXmlStandalone;
*setXmlStandalone = *XML::Xercesc::DOMDocument_setXmlStandalone;
*getXmlVersion = *XML::Xercesc::DOMDocument_getXmlVersion;
*setXmlVersion = *XML::Xercesc::DOMDocument_setXmlVersion;
*getDocumentURI = *XML::Xercesc::DOMDocument_getDocumentURI;
*setDocumentURI = *XML::Xercesc::DOMDocument_setDocumentURI;
*getStrictErrorChecking = *XML::Xercesc::DOMDocument_getStrictErrorChecking;
*setStrictErrorChecking = *XML::Xercesc::DOMDocument_setStrictErrorChecking;
*renameNode = *XML::Xercesc::DOMDocument_renameNode;
*adoptNode = *XML::Xercesc::DOMDocument_adoptNode;
*normalizeDocument = *XML::Xercesc::DOMDocument_normalizeDocument;
*getDOMConfig = *XML::Xercesc::DOMDocument_getDOMConfig;
*createEntity = *XML::Xercesc::DOMDocument_createEntity;
*createDocumentType = *XML::Xercesc::DOMDocument_createDocumentType;
*createNotation = *XML::Xercesc::DOMDocument_createNotation;
*createElementNS = *XML::Xercesc::DOMDocument_createElementNS;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLocator ##############

package XML::Xerces::DOMLocator;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLocator($self);
        delete $OWNER{$self};
    }
}

*getLineNumber = *XML::Xercesc::DOMLocator_getLineNumber;
*getColumnNumber = *XML::Xercesc::DOMLocator_getColumnNumber;
*getByteOffset = *XML::Xercesc::DOMLocator_getByteOffset;
*getUtf16Offset = *XML::Xercesc::DOMLocator_getUtf16Offset;
*getRelatedNode = *XML::Xercesc::DOMLocator_getRelatedNode;
*getURI = *XML::Xercesc::DOMLocator_getURI;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSResourceResolver ##############

package XML::Xerces::DOMLSResourceResolver;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSResourceResolver($self);
        delete $OWNER{$self};
    }
}

*resolveResource = *XML::Xercesc::DOMLSResourceResolver_resolveResource;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSOutput ##############

package XML::Xerces::DOMLSOutput;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSOutput($self);
        delete $OWNER{$self};
    }
}

*getByteStream = *XML::Xercesc::DOMLSOutput_getByteStream;
*getEncoding = *XML::Xercesc::DOMLSOutput_getEncoding;
*getSystemId = *XML::Xercesc::DOMLSOutput_getSystemId;
*setByteStream = *XML::Xercesc::DOMLSOutput_setByteStream;
*setEncoding = *XML::Xercesc::DOMLSOutput_setEncoding;
*setSystemId = *XML::Xercesc::DOMLSOutput_setSystemId;
*release = *XML::Xercesc::DOMLSOutput_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSException ##############

package XML::Xerces::DOMLSException;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMException XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*PARSE_ERR = *XML::Xercesc::DOMLSException_PARSE_ERR;
*SERIALIZE_ERR = *XML::Xercesc::DOMLSException_SERIALIZE_ERR;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_DOMLSException(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSException($self);
        delete $OWNER{$self};
    }
}

*getMessage = *XML::Xercesc::DOMLSException_getMessage;
*swig_code_get = *XML::Xercesc::DOMLSException_code_get;
*swig_code_set = *XML::Xercesc::DOMLSException_code_set;
*swig_msg_get = *XML::Xercesc::DOMLSException_msg_get;
*swig_msg_set = *XML::Xercesc::DOMLSException_msg_set;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSSerializerFilter ##############

package XML::Xerces::DOMLSSerializerFilter;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNodeFilter XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSSerializerFilter($self);
        delete $OWNER{$self};
    }
}

*acceptNode = *XML::Xercesc::DOMLSSerializerFilter_acceptNode;
*getWhatToShow = *XML::Xercesc::DOMLSSerializerFilter_getWhatToShow;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSSerializer ##############

package XML::Xerces::DOMLSSerializer;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSSerializer($self);
        delete $OWNER{$self};
    }
}

*getDomConfig = *XML::Xercesc::DOMLSSerializer_getDomConfig;
*setNewLine = *XML::Xercesc::DOMLSSerializer_setNewLine;
*setFilter = *XML::Xercesc::DOMLSSerializer_setFilter;
*getNewLine = *XML::Xercesc::DOMLSSerializer_getNewLine;
*write = *XML::Xercesc::DOMLSSerializer_write;
*writeToURI = *XML::Xercesc::DOMLSSerializer_writeToURI;
*writeToString = *XML::Xercesc::DOMLSSerializer_writeToString;
*release = *XML::Xercesc::DOMLSSerializer_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::AbstractDOMParser ##############

package XML::Xerces::AbstractDOMParser;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLDocumentHandler XML::Xerces::PSVIHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*Val_Never = *XML::Xercesc::AbstractDOMParser_Val_Never;
*Val_Always = *XML::Xercesc::AbstractDOMParser_Val_Always;
*Val_Auto = *XML::Xercesc::AbstractDOMParser_Val_Auto;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_AbstractDOMParser($self);
        delete $OWNER{$self};
    }
}

*reset = *XML::Xercesc::AbstractDOMParser_reset;
*adoptDocument = *XML::Xercesc::AbstractDOMParser_adoptDocument;
# hold a reference to the parser internally, so that the
# document can exist after the parser has gone out of scope
sub getDocument {
  my ($self) = @_;
  my $result = XML::Xercesc::AbstractDOMParser_getDocument($self);

  # the DOMDocument destructor will automatically delete this entry if we
  # store it using the underlying tied() object
  $XML::Xerces::REMEMBER{tied(% {$result})}->{__PARSER} = $self;
  return $result;
}

*getValidator = *XML::Xercesc::AbstractDOMParser_getValidator;
*getValidationScheme = *XML::Xercesc::AbstractDOMParser_getValidationScheme;
*getDoSchema = *XML::Xercesc::AbstractDOMParser_getDoSchema;
*getValidationSchemaFullChecking = *XML::Xercesc::AbstractDOMParser_getValidationSchemaFullChecking;
*getIdentityConstraintChecking = *XML::Xercesc::AbstractDOMParser_getIdentityConstraintChecking;
*getErrorCount = *XML::Xercesc::AbstractDOMParser_getErrorCount;
*getDoNamespaces = *XML::Xercesc::AbstractDOMParser_getDoNamespaces;
*getExitOnFirstFatalError = *XML::Xercesc::AbstractDOMParser_getExitOnFirstFatalError;
*getValidationConstraintFatal = *XML::Xercesc::AbstractDOMParser_getValidationConstraintFatal;
*getCreateEntityReferenceNodes = *XML::Xercesc::AbstractDOMParser_getCreateEntityReferenceNodes;
*getIncludeIgnorableWhitespace = *XML::Xercesc::AbstractDOMParser_getIncludeIgnorableWhitespace;
*getExternalSchemaLocation = *XML::Xercesc::AbstractDOMParser_getExternalSchemaLocation;
*getExternalNoNamespaceSchemaLocation = *XML::Xercesc::AbstractDOMParser_getExternalNoNamespaceSchemaLocation;
*getSecurityManager = *XML::Xercesc::AbstractDOMParser_getSecurityManager;
*getLoadExternalDTD = *XML::Xercesc::AbstractDOMParser_getLoadExternalDTD;
*getCreateCommentNodes = *XML::Xercesc::AbstractDOMParser_getCreateCommentNodes;
*getCalculateSrcOfs = *XML::Xercesc::AbstractDOMParser_getCalculateSrcOfs;
*getStandardUriConformant = *XML::Xercesc::AbstractDOMParser_getStandardUriConformant;
*getPSVIHandler = *XML::Xercesc::AbstractDOMParser_getPSVIHandler;
*getCreateSchemaInfo = *XML::Xercesc::AbstractDOMParser_getCreateSchemaInfo;
*getGenerateSyntheticAnnotations = *XML::Xercesc::AbstractDOMParser_getGenerateSyntheticAnnotations;
*getValidateAnnotations = *XML::Xercesc::AbstractDOMParser_getValidateAnnotations;
*getIgnoreAnnotations = *XML::Xercesc::AbstractDOMParser_getIgnoreAnnotations;
*getDisableDefaultEntityResolution = *XML::Xercesc::AbstractDOMParser_getDisableDefaultEntityResolution;
*getSkipDTDValidation = *XML::Xercesc::AbstractDOMParser_getSkipDTDValidation;
*setGenerateSyntheticAnnotations = *XML::Xercesc::AbstractDOMParser_setGenerateSyntheticAnnotations;
*setValidateAnnotations = *XML::Xercesc::AbstractDOMParser_setValidateAnnotations;
*setDoNamespaces = *XML::Xercesc::AbstractDOMParser_setDoNamespaces;
*setExitOnFirstFatalError = *XML::Xercesc::AbstractDOMParser_setExitOnFirstFatalError;
*setValidationConstraintFatal = *XML::Xercesc::AbstractDOMParser_setValidationConstraintFatal;
*setCreateEntityReferenceNodes = *XML::Xercesc::AbstractDOMParser_setCreateEntityReferenceNodes;
*setIncludeIgnorableWhitespace = *XML::Xercesc::AbstractDOMParser_setIncludeIgnorableWhitespace;
*setValidationScheme = *XML::Xercesc::AbstractDOMParser_setValidationScheme;
*setDoSchema = *XML::Xercesc::AbstractDOMParser_setDoSchema;
*setValidationSchemaFullChecking = *XML::Xercesc::AbstractDOMParser_setValidationSchemaFullChecking;
*setIdentityConstraintChecking = *XML::Xercesc::AbstractDOMParser_setIdentityConstraintChecking;
*setExternalSchemaLocation = *XML::Xercesc::AbstractDOMParser_setExternalSchemaLocation;
*setExternalNoNamespaceSchemaLocation = *XML::Xercesc::AbstractDOMParser_setExternalNoNamespaceSchemaLocation;
*setSecurityManager = *XML::Xercesc::AbstractDOMParser_setSecurityManager;
*setLoadExternalDTD = *XML::Xercesc::AbstractDOMParser_setLoadExternalDTD;
*setCreateCommentNodes = *XML::Xercesc::AbstractDOMParser_setCreateCommentNodes;
*setCalculateSrcOfs = *XML::Xercesc::AbstractDOMParser_setCalculateSrcOfs;
*setStandardUriConformant = *XML::Xercesc::AbstractDOMParser_setStandardUriConformant;
*useScanner = *XML::Xercesc::AbstractDOMParser_useScanner;
*useImplementation = *XML::Xercesc::AbstractDOMParser_useImplementation;
*setPSVIHandler = *XML::Xercesc::AbstractDOMParser_setPSVIHandler;
*setCreateSchemaInfo = *XML::Xercesc::AbstractDOMParser_setCreateSchemaInfo;
*setIgnoreAnnotations = *XML::Xercesc::AbstractDOMParser_setIgnoreAnnotations;
*setDisableDefaultEntityResolution = *XML::Xercesc::AbstractDOMParser_setDisableDefaultEntityResolution;
*setSkipDTDValidation = *XML::Xercesc::AbstractDOMParser_setSkipDTDValidation;
*parse = *XML::Xercesc::AbstractDOMParser_parse;
*parseFirst = *XML::Xercesc::AbstractDOMParser_parseFirst;
*parseNext = *XML::Xercesc::AbstractDOMParser_parseNext;
*parseReset = *XML::Xercesc::AbstractDOMParser_parseReset;
*handleElementPSVI = *XML::Xercesc::AbstractDOMParser_handleElementPSVI;
*handlePartialElementPSVI = *XML::Xercesc::AbstractDOMParser_handlePartialElementPSVI;
*handleAttributesPSVI = *XML::Xercesc::AbstractDOMParser_handleAttributesPSVI;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSInput ##############

package XML::Xerces::DOMLSInput;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSInput($self);
        delete $OWNER{$self};
    }
}

*getStringData = *XML::Xercesc::DOMLSInput_getStringData;
*getByteStream = *XML::Xercesc::DOMLSInput_getByteStream;
*getEncoding = *XML::Xercesc::DOMLSInput_getEncoding;
*getPublicId = *XML::Xercesc::DOMLSInput_getPublicId;
*getSystemId = *XML::Xercesc::DOMLSInput_getSystemId;
*getBaseURI = *XML::Xercesc::DOMLSInput_getBaseURI;
*setStringData = *XML::Xercesc::DOMLSInput_setStringData;
*setByteStream = *XML::Xercesc::DOMLSInput_setByteStream;
*setEncoding = *XML::Xercesc::DOMLSInput_setEncoding;
*setPublicId = *XML::Xercesc::DOMLSInput_setPublicId;
*setSystemId = *XML::Xercesc::DOMLSInput_setSystemId;
*setBaseURI = *XML::Xercesc::DOMLSInput_setBaseURI;
*setIssueFatalErrorIfNotFound = *XML::Xercesc::DOMLSInput_setIssueFatalErrorIfNotFound;
*getIssueFatalErrorIfNotFound = *XML::Xercesc::DOMLSInput_getIssueFatalErrorIfNotFound;
*release = *XML::Xercesc::DOMLSInput_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::Wrapper4InputSource ##############

package XML::Xerces::Wrapper4InputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMLSInput XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    # we never handle the memory
    # so we tell Perl to forget about the InputSource
    $_[0]->DISOWN();

    my $self = XML::Xercesc::new_Wrapper4InputSource(@_);
    
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_Wrapper4InputSource($self);
        delete $OWNER{$self};
    }
}

*getStringData = *XML::Xercesc::Wrapper4InputSource_getStringData;
*getByteStream = *XML::Xercesc::Wrapper4InputSource_getByteStream;
*getEncoding = *XML::Xercesc::Wrapper4InputSource_getEncoding;
*getPublicId = *XML::Xercesc::Wrapper4InputSource_getPublicId;
*getSystemId = *XML::Xercesc::Wrapper4InputSource_getSystemId;
*getBaseURI = *XML::Xercesc::Wrapper4InputSource_getBaseURI;
*getIssueFatalErrorIfNotFound = *XML::Xercesc::Wrapper4InputSource_getIssueFatalErrorIfNotFound;
*setEncoding = *XML::Xercesc::Wrapper4InputSource_setEncoding;
*setPublicId = *XML::Xercesc::Wrapper4InputSource_setPublicId;
*setSystemId = *XML::Xercesc::Wrapper4InputSource_setSystemId;
*setBaseURI = *XML::Xercesc::Wrapper4InputSource_setBaseURI;
*setIssueFatalErrorIfNotFound = *XML::Xercesc::Wrapper4InputSource_setIssueFatalErrorIfNotFound;
*release = *XML::Xercesc::Wrapper4InputSource_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::Wrapper4DOMLSInput ##############

package XML::Xerces::Wrapper4DOMLSInput;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::InputSource XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_Wrapper4DOMLSInput(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_Wrapper4DOMLSInput($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::Wrapper4DOMLSInput_makeStream;
*getEncoding = *XML::Xercesc::Wrapper4DOMLSInput_getEncoding;
*getPublicId = *XML::Xercesc::Wrapper4DOMLSInput_getPublicId;
*getSystemId = *XML::Xercesc::Wrapper4DOMLSInput_getSystemId;
*getIssueFatalErrorIfNotFound = *XML::Xercesc::Wrapper4DOMLSInput_getIssueFatalErrorIfNotFound;
*setEncoding = *XML::Xercesc::Wrapper4DOMLSInput_setEncoding;
*setPublicId = *XML::Xercesc::Wrapper4DOMLSInput_setPublicId;
*setSystemId = *XML::Xercesc::Wrapper4DOMLSInput_setSystemId;
*setIssueFatalErrorIfNotFound = *XML::Xercesc::Wrapper4DOMLSInput_setIssueFatalErrorIfNotFound;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSParserFilter ##############

package XML::Xerces::DOMLSParserFilter;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSParserFilter($self);
        delete $OWNER{$self};
    }
}

*FILTER_ACCEPT = *XML::Xercesc::DOMLSParserFilter_FILTER_ACCEPT;
*FILTER_REJECT = *XML::Xercesc::DOMLSParserFilter_FILTER_REJECT;
*FILTER_SKIP = *XML::Xercesc::DOMLSParserFilter_FILTER_SKIP;
*FILTER_INTERRUPT = *XML::Xercesc::DOMLSParserFilter_FILTER_INTERRUPT;
*acceptNode = *XML::Xercesc::DOMLSParserFilter_acceptNode;
*getWhatToShow = *XML::Xercesc::DOMLSParserFilter_getWhatToShow;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSParser ##############

package XML::Xerces::DOMLSParser;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSParser($self);
        delete $OWNER{$self};
    }
}

*ACTION_APPEND_AS_CHILDREN = *XML::Xercesc::DOMLSParser_ACTION_APPEND_AS_CHILDREN;
*ACTION_REPLACE_CHILDREN = *XML::Xercesc::DOMLSParser_ACTION_REPLACE_CHILDREN;
*ACTION_INSERT_BEFORE = *XML::Xercesc::DOMLSParser_ACTION_INSERT_BEFORE;
*ACTION_INSERT_AFTER = *XML::Xercesc::DOMLSParser_ACTION_INSERT_AFTER;
*ACTION_REPLACE = *XML::Xercesc::DOMLSParser_ACTION_REPLACE;
*getDomConfig = *XML::Xercesc::DOMLSParser_getDomConfig;
*getAsync = *XML::Xercesc::DOMLSParser_getAsync;
*getBusy = *XML::Xercesc::DOMLSParser_getBusy;
*setFilter = *XML::Xercesc::DOMLSParser_setFilter;
*parse = *XML::Xercesc::DOMLSParser_parse;
*parseURI = *XML::Xercesc::DOMLSParser_parseURI;
*parseWithContext = *XML::Xercesc::DOMLSParser_parseWithContext;
*abort = *XML::Xercesc::DOMLSParser_abort;
*release = *XML::Xercesc::DOMLSParser_release;
*resetDocumentPool = *XML::Xercesc::DOMLSParser_resetDocumentPool;
*loadGrammar = *XML::Xercesc::DOMLSParser_loadGrammar;
*getGrammar = *XML::Xercesc::DOMLSParser_getGrammar;
*getRootGrammar = *XML::Xercesc::DOMLSParser_getRootGrammar;
*getURIText = *XML::Xercesc::DOMLSParser_getURIText;
*resetCachedGrammarPool = *XML::Xercesc::DOMLSParser_resetCachedGrammarPool;
*getSrcOffset = *XML::Xercesc::DOMLSParser_getSrcOffset;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XercesDOMParser ##############

package XML::Xerces::XercesDOMParser;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::AbstractDOMParser XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_XercesDOMParser(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XercesDOMParser($self);
        delete $OWNER{$self};
    }
}

*getErrorHandler = *XML::Xercesc::XercesDOMParser_getErrorHandler;
*getEntityResolver = *XML::Xercesc::XercesDOMParser_getEntityResolver;
*getXMLEntityResolver = *XML::Xercesc::XercesDOMParser_getXMLEntityResolver;
*isCachingGrammarFromParse = *XML::Xercesc::XercesDOMParser_isCachingGrammarFromParse;
*isUsingCachedGrammarInParse = *XML::Xercesc::XercesDOMParser_isUsingCachedGrammarInParse;
*getGrammar = *XML::Xercesc::XercesDOMParser_getGrammar;
*getRootGrammar = *XML::Xercesc::XercesDOMParser_getRootGrammar;
*getURIText = *XML::Xercesc::XercesDOMParser_getURIText;
*getSrcOffset = *XML::Xercesc::XercesDOMParser_getSrcOffset;
*getIgnoreCachedDTD = *XML::Xercesc::XercesDOMParser_getIgnoreCachedDTD;
sub setErrorHandler {
  my ($self,$handler) = @_;


  my $callback;
  if (defined$handler) {
    $callback = XML::Xerces::PerlErrorCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__ERROR_HANDLER} = $callback;
  }
  return XML::Xercesc::XercesDOMParser_setErrorHandler($self,$callback);
}

sub setEntityResolver {
  my ($self,$handler) = @_;


  my $callback;
  if (defined$handler) {
    $callback = XML::Xerces::PerlEntityResolverHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $callback;
  }
  return XML::Xercesc::XercesDOMParser_setEntityResolver($self,$callback);
}

*setXMLEntityResolver = *XML::Xercesc::XercesDOMParser_setXMLEntityResolver;
*cacheGrammarFromParse = *XML::Xercesc::XercesDOMParser_cacheGrammarFromParse;
*useCachedGrammarInParse = *XML::Xercesc::XercesDOMParser_useCachedGrammarInParse;
*setIgnoreCachedDTD = *XML::Xercesc::XercesDOMParser_setIgnoreCachedDTD;
*resetDocumentPool = *XML::Xercesc::XercesDOMParser_resetDocumentPool;
*error = *XML::Xercesc::XercesDOMParser_error;
*resetErrors = *XML::Xercesc::XercesDOMParser_resetErrors;
*loadGrammar = *XML::Xercesc::XercesDOMParser_loadGrammar;
*resetCachedGrammarPool = *XML::Xercesc::XercesDOMParser_resetCachedGrammarPool;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::PerlNodeFilterCallbackHandler ##############

package XML::Xerces::PerlNodeFilterCallbackHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNodeFilter XML::Xerces::PerlCallbackHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PerlNodeFilterCallbackHandler(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PerlNodeFilterCallbackHandler($self);
        delete $OWNER{$self};
    }
}

*type = *XML::Xercesc::PerlNodeFilterCallbackHandler_type;
*acceptNode = *XML::Xercesc::PerlNodeFilterCallbackHandler_acceptNode;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::PerlDOMCallbackHandler ##############

package XML::Xerces::PerlDOMCallbackHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMErrorHandler XML::Xerces::DOMLSResourceResolver XML::Xerces::PerlCallbackHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PerlDOMCallbackHandler(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PerlDOMCallbackHandler($self);
        delete $OWNER{$self};
    }
}

*type = *XML::Xercesc::PerlDOMCallbackHandler_type;
*handleError = *XML::Xercesc::PerlDOMCallbackHandler_handleError;
*resolveResource = *XML::Xercesc::PerlDOMCallbackHandler_resolveResource;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


# ------- VARIABLE STUBS --------

package XML::Xerces;

1;
