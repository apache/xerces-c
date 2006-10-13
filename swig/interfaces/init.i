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
 * Include extra verbatim C code in the initialization function
 */
%init %{
    // we create the global transcoder for UTF-8 to UTF-16
    // must initialize the Xerces-C transcoding service
    XMLPlatformUtils::Initialize();
    UTF8_TRANSCODER = Transcoder::getInstance();
    if (! UTF8_TRANSCODER) {
	croak("ERROR: XML::Xerces: INIT: Could not create UTF-8 transcoder");
    }

    XML_EXCEPTION_HANDLER = XMLExceptionHandler::getInstance();
    if (! XML_EXCEPTION_HANDLER) {
	croak("ERROR: XML::Xerces: INIT: Could not create XMLExceptionHandler");
    }
%}
