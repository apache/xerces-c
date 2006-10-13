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

  # allow undef to unset current value
  my $callback;
  if (defined ## $handler) {
    $callback = XML::Xerces::PerlErrorCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__ERROR_HANDLER} = $callback;
  }
  return XML::Xercesc::class ##_setErrorHandler($self,$callback);
}
}
%enddef

SET_ERRORHANDLER(Parser)
SET_ERRORHANDLER(SAX2XMLReader)
SET_ERRORHANDLER(SAXParser)
SET_ERRORHANDLER(XercesDOMParser)

// Define a macro to rewrite setEntityResolver() for all classes

%define SET_ENTITYRESOLVER(class)
%feature("shadow") XERCES_CPP_NAMESPACE::class::setEntityResolver {
sub setEntityResolver {
  my ($self,$handler) = @_;

  # allow undef to unset current value
  my $callback;
  if (defined ## $handler) {
    $callback = XML::Xerces::PerlEntityResolverHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $callback;
  }
  return XML::Xercesc::class ##_setEntityResolver($self,$callback);
}
}
%enddef

SET_ENTITYRESOLVER(Parser)
SET_ENTITYRESOLVER(SAX2XMLReader)
SET_ENTITYRESOLVER(SAXParser)
SET_ENTITYRESOLVER(XercesDOMParser)

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

// Define a macro to rewrite all methods that return a list 

// we need the "defined ## $result hack because defined is a cpp operator
%define LIST_METHOD(class,method)
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

// Define a macro to rewrite all methods that return a map 

// we need the "defined ## $result hack because defined is a cpp operator
%define MAP_METHOD(class,method)
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

LIST_METHOD(XMLElementDecl,getAttDefList)
LIST_METHOD(XMLSchemaDescription,getLocationHints)
// MAP_METHOD(XMLElementDecl,getAttDefList)