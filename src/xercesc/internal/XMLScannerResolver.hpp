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
 * $Log$
 * Revision 1.5  2004/09/08 13:56:14  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/07/10 19:47:24  peiyongz
 * Stateless Grammar: Initialize scanner with grammarResolver,
 *                                creating grammar through grammarPool
 *
 * Revision 1.3  2003/05/16 21:36:58  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.2  2003/05/15 18:26:29  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.1  2002/12/04 01:44:21  knoaman
 * Initial check-in.
 *
 */

#if !defined(XMLSCANNERRESOLVER_HPP)
#define XMLSCANNERRESOLVER_HPP

#include <xercesc/internal/XMLScanner.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLValidator;
class XMLDocumentHandler;
class XMLErrorReporter;
class DocTypeHandler;
class XMLEntityHandler;

class XMLPARSER_EXPORT XMLScannerResolver
{
public:
    // -----------------------------------------------------------------------
    //  Public class methods
    // -----------------------------------------------------------------------
    static XMLScanner* resolveScanner
    (
          const XMLCh* const   scannerName
        , XMLValidator* const  valToAdopt
        , GrammarResolver* const grammarResolver
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );

    static XMLScanner* resolveScanner
    (
          const XMLCh* const        scannerName
        , XMLDocumentHandler* const docHandler
        , DocTypeHandler* const     docTypeHandler
        , XMLEntityHandler* const   entityHandler
        , XMLErrorReporter* const   errReporter
        , XMLValidator* const       valToAdopt
        , GrammarResolver* const    grammarResolver
        , MemoryManager* const      manager = XMLPlatformUtils::fgMemoryManager
    );

    static XMLScanner* getDefaultScanner
    (
          XMLValidator* const  valToAdopt
        , GrammarResolver* const grammarResolver
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );

private :

    // -----------------------------------------------------------------------
    //  Unimplemented constructor and destructor
    // -----------------------------------------------------------------------
    XMLScannerResolver();
    ~XMLScannerResolver();
};

XERCES_CPP_NAMESPACE_END

#endif
