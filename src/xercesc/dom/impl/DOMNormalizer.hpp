#ifndef DOMNormalizer_HEADER_GUARD_
#define DOMNormalizer_HEADER_GUARD_

/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2003 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */


//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <xercesc/dom/DOM.hpp> for the entire
//  DOM API, or xercesc/dom/DOM*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/RefHashTableOf.hpp>
#include <xercesc/util/RefVectorOf.hpp>
#include <xercesc/framework/XMLErrorCodes.hpp>


XERCES_CPP_NAMESPACE_BEGIN

class DOMConfigurationImpl;
class DOMDocumentImpl;
class DOMNode;
class DOMElementImpl;
class DOMAttr;
class DOMNamedNodeMap;

class DOMNormalizer : public XMemory {

    //the following are the data structures maintain the stack of namespace information 
    class InScopeNamespaces : public XMemory {
        class Scope : public XMemory {
        public:
            Scope(Scope *baseScopeWithBindings);
            ~Scope();
            void addOrChangeBinding(const XMLCh *prefix, const XMLCh *uri,
                                    MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
            const XMLCh* getUri(const XMLCh *prefix) const;
            const XMLCh* getPrefix(const XMLCh* uri) const;
            Scope *fBaseScopeWithBindings;

        private:
            RefHashTableOf<XMLCh> *fPrefixHash;
            RefHashTableOf<XMLCh> *fUriHash;
            // unimplemented
            Scope ( const Scope& toCopy);
            Scope& operator= (const Scope& other);
        };

    public:    
        InScopeNamespaces(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
        ~InScopeNamespaces();
        void addOrChangeBinding(const XMLCh *prefix, const XMLCh *uri,
                                MemoryManager* const manager  = XMLPlatformUtils::fgMemoryManager);
        void addScope(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
        void removeScope();
        bool isValidBinding(const XMLCh* prefix, const XMLCh* uri) const;
        const XMLCh* getOrDeclarePrefix(const XMLCh* uri);
        const XMLCh* getPrefix(const XMLCh* uri) const;
        const XMLCh* getUri(const XMLCh* prefix) const;
        int size();

    private:
        RefVectorOf<Scope> *fScopes;
        Scope *lastScopeWithBindings;
        // unimplemented
        InScopeNamespaces ( const InScopeNamespaces& toCopy);
        InScopeNamespaces& operator= (const InScopeNamespaces& other);
    };

public:
    DOMNormalizer(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    ~DOMNormalizer();

    /**
     * Main entry method to normalize a document
     */
    void normalizeDocument(DOMDocumentImpl *doc);

    // -----------------------------------------------------------------------
    //  Notification that lazy data has been deleted
    // -----------------------------------------------------------------------
	static void reinitNormalizerMutex();
	static void reinitMsgLoader();

private:
    // unimplemented
    DOMNormalizer ( const DOMNormalizer& toCopy);
    DOMNormalizer& operator= (const DOMNormalizer& other);

    /**
     * Recursively normalizes a node
     */
    DOMNode * normalizeNode(DOMNode *node) const;

    /**
     * Helper method that fixes up the namespace declarations according to the
     * DOM Level 3 psydocode
     */
    void namespaceFixUp(DOMElementImpl *ele) const;

    /**
     * Converts an integer to an XMLCh - max 15 digits long. 
     */
    const XMLCh * integerToXMLCh(unsigned int i) const;

    /**
     * Adds a namespace attribute or replaces the value of existing namespace
     * attribute with the given prefix and value for URI.
     * In case prefix is empty will add/update default namespace declaration.
     */
    void addOrChangeNamespaceDecl(const XMLCh* prefix, const XMLCh* uri, DOMElementImpl *element) const;
    
    /**
     * Adds a custom namespace in the form "NSx" where x is an integer that 
     * has not yet used in the document
     */
    const XMLCh* addCustomNamespaceDecl(const XMLCh* uri, DOMElementImpl *element) const;


    /**
     * Report an error 
     */
    void error(const XMLErrs::Codes code, const DOMNode *node) const;

    //
    // fDocument - the document we are operating on
    //
    // fDOMConfiguration - the configuration from the document
    //
    // fErrorHandler - the errorhandler to be used when reporting errors during normalization
    //
    // fNSScope - the data stucture that holds the prefix-uri information
    //
    // fNewNamespaceCount - the number of custom namespace declarations we have created
    //
    DOMDocumentImpl *fDocument;
    DOMConfigurationImpl *fConfiguration;
    DOMErrorHandler *fErrorHandler;
    InScopeNamespaces *fNSScope;
    unsigned int fNewNamespaceCount;
    MemoryManager* fMemoryManager;
};



XERCES_CPP_NAMESPACE_END

#endif
