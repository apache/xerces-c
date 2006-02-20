/*
 * Copyright 2002,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Perl/shadow.i - all modification of Xerces.pm code
 *
 */ 

/*
 * Handle Callbacks - until directors work in Perl
 */

// Define a macro to rewrite setErrorHandler() for all classes

%define SET_ERRORHANDLER(class)
%feature("shadow") XERCES_CPP_NAMESPACE::class::setErrorHandler {
    sub setErrorHandler {
      my ($self,$handler) = @_;
      my $callback = XML::Xerces::PerlErrorCallbackHandler->new($handler);
      $XML::Xerces::REMEMBER{tied(% {$self})}->{__ERROR_HANDLER} = $callback;

      return XML::Xercesc::class ##_setErrorHandler($self,$callback);
    }
}
%enddef

SET_ERRORHANDLER(Parser)
SET_ERRORHANDLER(SAX2XMLReader)
SET_ERRORHANDLER(SAXParser)
SET_ERRORHANDLER(DOMLSParser)
SET_ERRORHANDLER(DOMWriter)
SET_ERRORHANDLER(XercesDOMParser)

// Define a macro to rewrite setEntityResolver() for all classes

%define SET_ENTITYRESOLVER(class)
%feature("shadow") XERCES_CPP_NAMESPACE::class::setEntityResolver {
    sub setEntityResolver {
      my ($self,$handler) = @_;
      my $callback = XML::Xerces::PerlEntityResolverHandler->new($handler);
      $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $callback;

      return XML::Xercesc::class ##_setEntityResolver($self,$callback);
    }
}
%enddef

SET_ENTITYRESOLVER(Parser)
SET_ENTITYRESOLVER(SAX2XMLReader)
SET_ENTITYRESOLVER(SAXParser)
SET_ENTITYRESOLVER(DOMLSParser)
SET_ENTITYRESOLVER(XercesDOMParser)

// Define a macro to rewrite all methods that return a list of DOMNodes

// we need the "defined ## $result hack because defined is a cpp operator
%define DOMLIST_METHOD(class,method)
  %feature("shadow") XERCES_CPP_NAMESPACE::class::method {
sub method {
    my $result = XML::Xercesc:: ## class ## _ ## method (@_);
    unless (defined ## $result) {
      return () if wantarray;
      return undef; # if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result; # if *not* wantarray
}
  }
%enddef

DOMLIST_METHOD(DOMDocument,getElementsByTagName)
DOMLIST_METHOD(DOMDocument,getElementsByTagNameNS)
DOMLIST_METHOD(DOMElement,getElementsByTagName)
DOMLIST_METHOD(DOMElement,getElementsByTagNameNS)
DOMLIST_METHOD(DOMNode,getChildNodes)

// Define a macro to rewrite all methods that return a map of DOMNodes

// we need the "defined ## $result hack because defined is a cpp operator
%define DOMMAP_METHOD(class,method)
  %feature("shadow") XERCES_CPP_NAMESPACE::class::method {
sub method {
    my $result = XML::Xercesc:: ## class ## _ ## method (@_);
    unless (defined ## $result) {
      return () if wantarray;
      return undef; # if *not* wantarray
    }
    return $result->to_hash() if wantarray;
    return $result; # if *not* wantarray
}
  }
%enddef

DOMMAP_METHOD(DOMDocumentType,getEntities)
DOMMAP_METHOD(DOMDocumentType,getNotations)
DOMMAP_METHOD(DOMNode,getAttributes)

/*
 * have the document remember the parser, so that if the parser
 * goes out of scope before the document, the document will not
 * be freed
 */
%feature("shadow") XERCES_CPP_NAMESPACE::AbstractDOMParser::getDocument %{
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
%}

// %feature("shadowpre") MemBufInputSource %{
//     # SYSTEM ID is *optional*
//     if (scalar @_ == 1) {
//       push(@_,'FAKE_SYSTEM_ID');
//     }
// %}

%feature("shadow") MemBufInputSource %{
sub new {
  my $pkg = shift;
  # SYSTEM ID is *optional*
  if (scalar @_ == 1) {
    push(@_,'FAKE_SYSTEM_ID');
  }

  my $self = XML::Xercesc::new_MemBufInputSource(@_);
    
  bless $self, $pkg if defined($self);
}
%}

/*
 * The Node iterators defined in DOM Level2 must also be remembered
 */
%define CREATE_DOMITERATOR(method)
%feature("shadow") XERCES_CPP_NAMESPACE::DOMDocumentTraversal:: ## method {
sub method {
    my ($self,$root,$what,$filter,$expand) = @_;
    my $callback = XML::Xerces::PerlNodeFilterCallbackHandler->new($filter);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__NODE_FILTER} = $callback;

    my @args = ($self,$root,$what,$callback,$expand);
    if ($args[0]->isa('XML::Xerces::DOMDocument')) {
#      $args[0] = $args[0]->toDOMDocumentTraversal();
    }
    return XML::Xercesc::DOMDocumentTraversal_ ## method(@args);
}
}
%enddef

CREATE_DOMITERATOR(createNodeIterator)
CREATE_DOMITERATOR(createTreeWalker)

%feature("shadow") XERCES_CPP_NAMESPACE::SAXParser::setDocumentHandler %{
sub setDocumentHandler {
    my ($self,$handler) = @_;
    my $callback = XML::Xerces::PerlDocumentCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__DOCUMENT_HANDLER} = $callback;

    my @args = ($self,$callback);
    return XML::Xercesc::SAXParser_setDocumentHandler(@args);
}
%}

%feature("shadow") XERCES_CPP_NAMESPACE::SAX2XMLReader::setContentHandler %{
sub setContentHandler {
    my ($self,$handler) = @_;
    my $callback = XML::Xerces::PerlContentCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__CONTENT_HANDLER} = $callback;

    my @args = ($self,$callback);
    return XML::Xercesc::SAX2XMLReader_setContentHandler(@args);
}
%}

// %feature("shadowpre") Wrapper4InputSource %{
//     # we never handle the memory
//     # so we tell Perl to forget about the InputSource
//     $_[0]->DISOWN();
// %}
// 
// %feature("shadowpre") createDocument %{
//   if (UNIVERSAL::isa($_[3],"XML::Xerces::DOMDocumentType")) {
//     # die "createDOMDocument: expected XML::Xerces::DOMDocumentType for arg 3";
//     $_[3]->DISOWN();
//   }
// %}

%feature("shadow") Wrapper4InputSource %{
sub new {
    my $pkg = shift;
    # we never handle the memory
    # so we tell Perl to forget about the InputSource
    $_[0]->DISOWN();

    my $self = XML::Xercesc::new_Wrapper4InputSource(@_);
    
    bless $self, $pkg if defined($self);
}
%}

%feature("shadow") createDocument %{
sub createDocument {
    if (UNIVERSAL::isa($_[3],"XML::Xerces::DOMDocumentType")) {
        $_[3]->DISOWN();
    }

    my $return = XML::Xercesc::DOMImplementation_createDocument(@_);
    
    return $return;
}
%}

%feature("shadow") ~DOMDocument %{
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;

    if ($self->can('cleanup')) {
        $_[0]->cleanup();
    }

    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMDocument($self);
        delete $OWNER{$self};
    }
}
%}

