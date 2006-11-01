 #
 # Licensed to the Apache Software Foundation (ASF) under one or more
 # contributor license agreements.  See the NOTICE file distributed with
 # this work for additional information regarding copyright ownership.
 # The ASF licenses this file to You under the Apache License, Version 2.0
 # (the "License"); you may not use this file except in compliance with
 # the License.  You may obtain a copy of the License at
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
package XML::Xerces::SAX;
use base qw(DynaLoader);
require XML::Xerces;
require XML::Xerces::IO;
bootstrap XML::Xerces::SAX;
package XML::Xerces::SAX;
############# Class : XML::Xerces::PerlDefaultHandler ##############
package XML::Xerces::PerlDefaultHandler;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::ContentHandler
	  XML::Xerces::DocumentHandler);
sub new {
  my $class = shift;

  # support copy constructor syntax
  $class = ref($class) if ref($class);

  return bless {}, $class;
}

sub startElement {
  my $self = shift;
  $self->start_element(@_);
}
sub endElement {
  my $self = shift;
  $self->end_element(@_);
}
sub startPrefixMapping {
  my $self = shift;
  $self->start_prefix_mapping(@_);
}
sub endPrefixMapping {
  my $self = shift;
  $self->end_prefix_mapping(@_);
}
sub skippedEntity {
  my $self = shift;
  $self->skipped_entity(@_);
}
sub startDocument {
  my $self = shift;
  $self->start_document();
}
sub endDocument {
  my $self = shift;
  $self->end_document();
}
sub resetDocument {
  my $self = shift;
  $self->reset_document();
}
sub characters {}
sub processingInstruction {
  my $self = shift;
  $self->processing_instruction(@_);
}
sub setDocumentLocator {
  my $self = shift;
  $self->set_document_locator(@_);
}
sub ignorableWhitespace {
  my $self = shift;
  $self->ignorable_whitespace(@_);
}

#
# support alternate Perl-friendly syntax
#
sub start_element {}
sub end_element {}
sub start_prefix_mapping {}
sub end_prefix_mapping {}
sub skipped_entity {}
sub start_document {}
sub end_document {}
sub reset_document {}
sub processing_instruction {}
sub set_document_locator {}
sub ignorable_whitespace {}


############# Class : XML::Xerces::PerlContentHandler ##############
package XML::Xerces::PerlContentHandler;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlDefaultHandler);

############# Class : XML::Xerces::PerlDocumentHandler ##############
package XML::Xerces::PerlDocumentHandler;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlDefaultHandler);

package XML::Xerces::Attributes;
sub to_hash {
  my $self = shift;
  my %hash;
  for (my $i=0; $i < $self->getLength(); $i++) {
    my $qname = $self->getQName($i);
    $hash{$qname}->{localName} = $self->getLocalName($i);
    $hash{$qname}->{URI} = $self->getURI($i);
    $hash{$qname}->{value} = $self->getValue($i);
    $hash{$qname}->{type} = $self->getType($i);
  }
  return %hash;
}

package XML::Xerces::AttributeList;
sub to_hash {
  my $self = shift;
  my %hash;
  for (my $i=0;$i<$self->getLength();$i++) {
    $hash{$self->getName($i)} = $self->getValue($i)
  }
  return %hash;
}


# ------- FUNCTION WRAPPERS --------

package XML::Xerces;


############# Class : XML::Xerces::DTDHandler ##############

package XML::Xerces::DTDHandler;
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
        XML::Xercesc::delete_DTDHandler($self);
        delete $OWNER{$self};
    }
}

*unparsedEntityDecl = *XML::Xercesc::DTDHandler_unparsedEntityDecl;
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


############# Class : XML::Xerces::DocumentHandler ##############

package XML::Xerces::DocumentHandler;
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
        XML::Xercesc::delete_DocumentHandler($self);
        delete $OWNER{$self};
    }
}

*characters = *XML::Xercesc::DocumentHandler_characters;
*processingInstruction = *XML::Xercesc::DocumentHandler_processingInstruction;
*setDocumentLocator = *XML::Xercesc::DocumentHandler_setDocumentLocator;
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


############# Class : XML::Xerces::AttributeList ##############

package XML::Xerces::AttributeList;
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
        XML::Xercesc::delete_AttributeList($self);
        delete $OWNER{$self};
    }
}

*getLength = *XML::Xercesc::AttributeList_getLength;
*getName = *XML::Xercesc::AttributeList_getName;
*getType = *XML::Xercesc::AttributeList_getType;
*getValue = *XML::Xercesc::AttributeList_getValue;
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


############# Class : XML::Xerces::HandlerBase ##############

package XML::Xerces::HandlerBase;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::EntityResolver XML::Xerces::DTDHandler XML::Xerces::DocumentHandler XML::Xerces::ErrorHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*characters = *XML::Xercesc::HandlerBase_characters;
*processingInstruction = *XML::Xercesc::HandlerBase_processingInstruction;
*setDocumentLocator = *XML::Xercesc::HandlerBase_setDocumentLocator;
*error = *XML::Xercesc::HandlerBase_error;
*fatalError = *XML::Xercesc::HandlerBase_fatalError;
*warning = *XML::Xercesc::HandlerBase_warning;
*resetErrors = *XML::Xercesc::HandlerBase_resetErrors;
*unparsedEntityDecl = *XML::Xercesc::HandlerBase_unparsedEntityDecl;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_HandlerBase(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_HandlerBase($self);
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


############# Class : XML::Xerces::Locator ##############

package XML::Xerces::Locator;
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
        XML::Xercesc::delete_Locator($self);
        delete $OWNER{$self};
    }
}

*getPublicId = *XML::Xercesc::Locator_getPublicId;
*getSystemId = *XML::Xercesc::Locator_getSystemId;
*getLineNumber = *XML::Xercesc::Locator_getLineNumber;
*getColumnNumber = *XML::Xercesc::Locator_getColumnNumber;
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


############# Class : XML::Xerces::Attributes ##############

package XML::Xerces::Attributes;
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
        XML::Xercesc::delete_Attributes($self);
        delete $OWNER{$self};
    }
}

*getLength = *XML::Xercesc::Attributes_getLength;
*getURI = *XML::Xercesc::Attributes_getURI;
*getLocalName = *XML::Xercesc::Attributes_getLocalName;
*getQName = *XML::Xercesc::Attributes_getQName;
*getIndex = *XML::Xercesc::Attributes_getIndex;
*getType = *XML::Xercesc::Attributes_getType;
*getValue = *XML::Xercesc::Attributes_getValue;
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


############# Class : XML::Xerces::ContentHandler ##############

package XML::Xerces::ContentHandler;
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
        XML::Xercesc::delete_ContentHandler($self);
        delete $OWNER{$self};
    }
}

*characters = *XML::Xercesc::ContentHandler_characters;
*processingInstruction = *XML::Xercesc::ContentHandler_processingInstruction;
*setDocumentLocator = *XML::Xercesc::ContentHandler_setDocumentLocator;
*startPrefixMapping = *XML::Xercesc::ContentHandler_startPrefixMapping;
*endPrefixMapping = *XML::Xercesc::ContentHandler_endPrefixMapping;
*skippedEntity = *XML::Xercesc::ContentHandler_skippedEntity;
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


############# Class : XML::Xerces::LexicalHandler ##############

package XML::Xerces::LexicalHandler;
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
        XML::Xercesc::delete_LexicalHandler($self);
        delete $OWNER{$self};
    }
}

*comment = *XML::Xercesc::LexicalHandler_comment;
*endCDATA = *XML::Xercesc::LexicalHandler_endCDATA;
*endDTD = *XML::Xercesc::LexicalHandler_endDTD;
*endEntity = *XML::Xercesc::LexicalHandler_endEntity;
*startCDATA = *XML::Xercesc::LexicalHandler_startCDATA;
*startDTD = *XML::Xercesc::LexicalHandler_startDTD;
*startEntity = *XML::Xercesc::LexicalHandler_startEntity;
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


############# Class : XML::Xerces::DeclHandler ##############

package XML::Xerces::DeclHandler;
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
        XML::Xercesc::delete_DeclHandler($self);
        delete $OWNER{$self};
    }
}

*attributeDecl = *XML::Xercesc::DeclHandler_attributeDecl;
*internalEntityDecl = *XML::Xercesc::DeclHandler_internalEntityDecl;
*externalEntityDecl = *XML::Xercesc::DeclHandler_externalEntityDecl;
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


############# Class : XML::Xerces::Parser ##############

package XML::Xerces::Parser;
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
        XML::Xercesc::delete_Parser($self);
        delete $OWNER{$self};
    }
}

sub setEntityResolver {
  my ($self,$handler) = @_;


  my $callback;
  if (defined$handler) {
    $callback = XML::Xerces::PerlEntityResolverHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $callback;
  }
  return XML::Xercesc::Parser_setEntityResolver($self,$callback);
}

*setDTDHandler = *XML::Xercesc::Parser_setDTDHandler;
*setDocumentHandler = *XML::Xercesc::Parser_setDocumentHandler;
sub setErrorHandler {
  my ($self,$handler) = @_;


  my $callback;
  if (defined$handler) {
    $callback = XML::Xerces::PerlErrorCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__ERROR_HANDLER} = $callback;
  }
  return XML::Xercesc::Parser_setErrorHandler($self,$callback);
}

*parse = *XML::Xercesc::Parser_parse;
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


############# Class : XML::Xerces::SAXParser ##############

package XML::Xerces::SAXParser;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::Parser XML::Xerces::XMLDocumentHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*Val_Never = *XML::Xercesc::SAXParser_Val_Never;
*Val_Always = *XML::Xercesc::SAXParser_Val_Always;
*Val_Auto = *XML::Xercesc::SAXParser_Val_Auto;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_SAXParser(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SAXParser($self);
        delete $OWNER{$self};
    }
}

*getDocumentHandler = *XML::Xercesc::SAXParser_getDocumentHandler;
*getEntityResolver = *XML::Xercesc::SAXParser_getEntityResolver;
*getXMLEntityResolver = *XML::Xercesc::SAXParser_getXMLEntityResolver;
*getErrorHandler = *XML::Xercesc::SAXParser_getErrorHandler;
*getPSVIHandler = *XML::Xercesc::SAXParser_getPSVIHandler;
*getValidator = *XML::Xercesc::SAXParser_getValidator;
*getValidationScheme = *XML::Xercesc::SAXParser_getValidationScheme;
*getDoSchema = *XML::Xercesc::SAXParser_getDoSchema;
*getValidationSchemaFullChecking = *XML::Xercesc::SAXParser_getValidationSchemaFullChecking;
*getIdentityConstraintChecking = *XML::Xercesc::SAXParser_getIdentityConstraintChecking;
*getErrorCount = *XML::Xercesc::SAXParser_getErrorCount;
*getDoNamespaces = *XML::Xercesc::SAXParser_getDoNamespaces;
*getExitOnFirstFatalError = *XML::Xercesc::SAXParser_getExitOnFirstFatalError;
*getValidationConstraintFatal = *XML::Xercesc::SAXParser_getValidationConstraintFatal;
*getExternalSchemaLocation = *XML::Xercesc::SAXParser_getExternalSchemaLocation;
*getExternalNoNamespaceSchemaLocation = *XML::Xercesc::SAXParser_getExternalNoNamespaceSchemaLocation;
*getSecurityManager = *XML::Xercesc::SAXParser_getSecurityManager;
*getLoadExternalDTD = *XML::Xercesc::SAXParser_getLoadExternalDTD;
*isCachingGrammarFromParse = *XML::Xercesc::SAXParser_isCachingGrammarFromParse;
*isUsingCachedGrammarInParse = *XML::Xercesc::SAXParser_isUsingCachedGrammarInParse;
*getCalculateSrcOfs = *XML::Xercesc::SAXParser_getCalculateSrcOfs;
*getStandardUriConformant = *XML::Xercesc::SAXParser_getStandardUriConformant;
*getGrammar = *XML::Xercesc::SAXParser_getGrammar;
*getRootGrammar = *XML::Xercesc::SAXParser_getRootGrammar;
*getURIText = *XML::Xercesc::SAXParser_getURIText;
*getSrcOffset = *XML::Xercesc::SAXParser_getSrcOffset;
*getGenerateSyntheticAnnotations = *XML::Xercesc::SAXParser_getGenerateSyntheticAnnotations;
*getValidateAnnotations = *XML::Xercesc::SAXParser_getValidateAnnotations;
*getIgnoreCachedDTD = *XML::Xercesc::SAXParser_getIgnoreCachedDTD;
*getIgnoreAnnotations = *XML::Xercesc::SAXParser_getIgnoreAnnotations;
*getDisableDefaultEntityResolution = *XML::Xercesc::SAXParser_getDisableDefaultEntityResolution;
*getSkipDTDValidation = *XML::Xercesc::SAXParser_getSkipDTDValidation;
*setGenerateSyntheticAnnotations = *XML::Xercesc::SAXParser_setGenerateSyntheticAnnotations;
*setValidateAnnotations = *XML::Xercesc::SAXParser_setValidateAnnotations;
*setDoNamespaces = *XML::Xercesc::SAXParser_setDoNamespaces;
*setValidationScheme = *XML::Xercesc::SAXParser_setValidationScheme;
*setDoSchema = *XML::Xercesc::SAXParser_setDoSchema;
*setValidationSchemaFullChecking = *XML::Xercesc::SAXParser_setValidationSchemaFullChecking;
*setIdentityConstraintChecking = *XML::Xercesc::SAXParser_setIdentityConstraintChecking;
*setExitOnFirstFatalError = *XML::Xercesc::SAXParser_setExitOnFirstFatalError;
*setValidationConstraintFatal = *XML::Xercesc::SAXParser_setValidationConstraintFatal;
*setExternalSchemaLocation = *XML::Xercesc::SAXParser_setExternalSchemaLocation;
*setExternalNoNamespaceSchemaLocation = *XML::Xercesc::SAXParser_setExternalNoNamespaceSchemaLocation;
*setSecurityManager = *XML::Xercesc::SAXParser_setSecurityManager;
*setLoadExternalDTD = *XML::Xercesc::SAXParser_setLoadExternalDTD;
*cacheGrammarFromParse = *XML::Xercesc::SAXParser_cacheGrammarFromParse;
*useCachedGrammarInParse = *XML::Xercesc::SAXParser_useCachedGrammarInParse;
*setCalculateSrcOfs = *XML::Xercesc::SAXParser_setCalculateSrcOfs;
*setStandardUriConformant = *XML::Xercesc::SAXParser_setStandardUriConformant;
*useScanner = *XML::Xercesc::SAXParser_useScanner;
*setInputBufferSize = *XML::Xercesc::SAXParser_setInputBufferSize;
*setIgnoreCachedDTD = *XML::Xercesc::SAXParser_setIgnoreCachedDTD;
*setIgnoreAnnotations = *XML::Xercesc::SAXParser_setIgnoreAnnotations;
*setDisableDefaultEntityResolution = *XML::Xercesc::SAXParser_setDisableDefaultEntityResolution;
*setSkipDTDValidation = *XML::Xercesc::SAXParser_setSkipDTDValidation;
*installAdvDocHandler = *XML::Xercesc::SAXParser_installAdvDocHandler;
*removeAdvDocHandler = *XML::Xercesc::SAXParser_removeAdvDocHandler;
*parseFirst = *XML::Xercesc::SAXParser_parseFirst;
*parseNext = *XML::Xercesc::SAXParser_parseNext;
*parseReset = *XML::Xercesc::SAXParser_parseReset;
*loadGrammar = *XML::Xercesc::SAXParser_loadGrammar;
*resetCachedGrammarPool = *XML::Xercesc::SAXParser_resetCachedGrammarPool;
*parse = *XML::Xercesc::SAXParser_parse;
sub setDocumentHandler {
    my ($self,$handler) = @_;
    my $callback = XML::Xerces::PerlSAXCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__DOCUMENT_HANDLER} = $callback;

    my @args = ($self,$callback);
    return XML::Xercesc::SAXParser_setDocumentHandler(@args);
}

*setDTDHandler = *XML::Xercesc::SAXParser_setDTDHandler;
sub setErrorHandler {
  my ($self,$handler) = @_;


  my $callback;
  if (defined$handler) {
    $callback = XML::Xerces::PerlErrorCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__ERROR_HANDLER} = $callback;
  }
  return XML::Xercesc::SAXParser_setErrorHandler($self,$callback);
}

*setPSVIHandler = *XML::Xercesc::SAXParser_setPSVIHandler;
sub setEntityResolver {
  my ($self,$handler) = @_;


  my $callback;
  if (defined$handler) {
    $callback = XML::Xerces::PerlEntityResolverHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $callback;
  }
  return XML::Xercesc::SAXParser_setEntityResolver($self,$callback);
}

*setXMLEntityResolver = *XML::Xercesc::SAXParser_setXMLEntityResolver;
*error = *XML::Xercesc::SAXParser_error;
*resetErrors = *XML::Xercesc::SAXParser_resetErrors;
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


############# Class : XML::Xerces::SAX2XMLReader ##############

package XML::Xerces::SAX2XMLReader;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*Val_Never = *XML::Xercesc::SAX2XMLReader_Val_Never;
*Val_Always = *XML::Xercesc::SAX2XMLReader_Val_Always;
*Val_Auto = *XML::Xercesc::SAX2XMLReader_Val_Auto;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SAX2XMLReader($self);
        delete $OWNER{$self};
    }
}

*getContentHandler = *XML::Xercesc::SAX2XMLReader_getContentHandler;
*getDTDHandler = *XML::Xercesc::SAX2XMLReader_getDTDHandler;
*getFeature = *XML::Xercesc::SAX2XMLReader_getFeature;
*getProperty = *XML::Xercesc::SAX2XMLReader_getProperty;
sub setContentHandler {
    my ($self,$handler) = @_;
    my $callback = XML::Xerces::PerlSAXCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__CONTENT_HANDLER} = $callback;

    my @args = ($self,$callback);
    return XML::Xercesc::SAX2XMLReader_setContentHandler(@args);
}

*setDTDHandler = *XML::Xercesc::SAX2XMLReader_setDTDHandler;
sub setEntityResolver {
  my ($self,$handler) = @_;


  my $callback;
  if (defined$handler) {
    $callback = XML::Xerces::PerlEntityResolverHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $callback;
  }
  return XML::Xercesc::SAX2XMLReader_setEntityResolver($self,$callback);
}

sub setErrorHandler {
  my ($self,$handler) = @_;


  my $callback;
  if (defined$handler) {
    $callback = XML::Xerces::PerlErrorCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__ERROR_HANDLER} = $callback;
  }
  return XML::Xercesc::SAX2XMLReader_setErrorHandler($self,$callback);
}

*setFeature = *XML::Xercesc::SAX2XMLReader_setFeature;
*setProperty = *XML::Xercesc::SAX2XMLReader_setProperty;
*parse = *XML::Xercesc::SAX2XMLReader_parse;
*getDeclarationHandler = *XML::Xercesc::SAX2XMLReader_getDeclarationHandler;
*getLexicalHandler = *XML::Xercesc::SAX2XMLReader_getLexicalHandler;
*setDeclarationHandler = *XML::Xercesc::SAX2XMLReader_setDeclarationHandler;
*setLexicalHandler = *XML::Xercesc::SAX2XMLReader_setLexicalHandler;
*getValidator = *XML::Xercesc::SAX2XMLReader_getValidator;
*getErrorCount = *XML::Xercesc::SAX2XMLReader_getErrorCount;
*getExitOnFirstFatalError = *XML::Xercesc::SAX2XMLReader_getExitOnFirstFatalError;
*getValidationConstraintFatal = *XML::Xercesc::SAX2XMLReader_getValidationConstraintFatal;
*getGrammar = *XML::Xercesc::SAX2XMLReader_getGrammar;
*getRootGrammar = *XML::Xercesc::SAX2XMLReader_getRootGrammar;
*getURIText = *XML::Xercesc::SAX2XMLReader_getURIText;
*getSrcOffset = *XML::Xercesc::SAX2XMLReader_getSrcOffset;
*setValidator = *XML::Xercesc::SAX2XMLReader_setValidator;
*setExitOnFirstFatalError = *XML::Xercesc::SAX2XMLReader_setExitOnFirstFatalError;
*setValidationConstraintFatal = *XML::Xercesc::SAX2XMLReader_setValidationConstraintFatal;
*parseFirst = *XML::Xercesc::SAX2XMLReader_parseFirst;
*parseNext = *XML::Xercesc::SAX2XMLReader_parseNext;
*parseReset = *XML::Xercesc::SAX2XMLReader_parseReset;
*loadGrammar = *XML::Xercesc::SAX2XMLReader_loadGrammar;
*resetCachedGrammarPool = *XML::Xercesc::SAX2XMLReader_resetCachedGrammarPool;
*setInputBufferSize = *XML::Xercesc::SAX2XMLReader_setInputBufferSize;
*installAdvDocHandler = *XML::Xercesc::SAX2XMLReader_installAdvDocHandler;
*removeAdvDocHandler = *XML::Xercesc::SAX2XMLReader_removeAdvDocHandler;
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


############# Class : XML::Xerces::XMLReaderFactory ##############

package XML::Xerces::XMLReaderFactory;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
*createXMLReader = *XML::Xercesc::XMLReaderFactory_createXMLReader;
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


############# Class : XML::Xerces::PerlSAXCallbackHandler ##############

package XML::Xerces::PerlSAXCallbackHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::ContentHandler XML::Xerces::DocumentHandler XML::Xerces::PerlCallbackHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PerlSAXCallbackHandler(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PerlSAXCallbackHandler($self);
        delete $OWNER{$self};
    }
}

*type = *XML::Xercesc::PerlSAXCallbackHandler_type;
*characters = *XML::Xercesc::PerlSAXCallbackHandler_characters;
*processingInstruction = *XML::Xercesc::PerlSAXCallbackHandler_processingInstruction;
*setDocumentLocator = *XML::Xercesc::PerlSAXCallbackHandler_setDocumentLocator;
*startPrefixMapping = *XML::Xercesc::PerlSAXCallbackHandler_startPrefixMapping;
*endPrefixMapping = *XML::Xercesc::PerlSAXCallbackHandler_endPrefixMapping;
*skippedEntity = *XML::Xercesc::PerlSAXCallbackHandler_skippedEntity;
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
