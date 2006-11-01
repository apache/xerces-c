/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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
 * Perl/sax-shadow.i - modification of Xerces/SAX.pm code
 *
 */ 

/*
 * Handle Callbacks - until directors work in Perl
 */

%feature("shadow") XERCES_CPP_NAMESPACE::SAXParser::setDocumentHandler %{
sub setDocumentHandler {
    my ($self,$handler) = @_;
    my $callback = XML::Xerces::PerlSAXCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__DOCUMENT_HANDLER} = $callback;

    my @args = ($self,$callback);
    return XML::Xercesc::SAXParser_setDocumentHandler(@args);
}
%}

%feature("shadow") XERCES_CPP_NAMESPACE::SAX2XMLReader::setContentHandler %{
sub setContentHandler {
    my ($self,$handler) = @_;
    my $callback = XML::Xerces::PerlSAXCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__CONTENT_HANDLER} = $callback;

    my @args = ($self,$callback);
    return XML::Xercesc::SAX2XMLReader_setContentHandler(@args);
}
%}
