/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
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



#if !defined(DOMCONFIGURATIONIMPL_HPP)
#define DOMCONFIGURATIONIMPL_HPP

//------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------
#include <xercesc/dom/DOMConfiguration.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class DOMDocumentImpl;

class CDOM_EXPORT DOMConfigurationImpl : public DOMConfiguration
{
private:
    //unimplemented
    DOMConfigurationImpl(const DOMConfiguration &);
    DOMConfigurationImpl & operator = (const DOMConfiguration &);


public:

    //-----------------------------------------------------------------------------------
    //  Constructor
    //-----------------------------------------------------------------------------------
    DOMConfigurationImpl(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    ~DOMConfigurationImpl();
    
    enum DOMConfigurationFeature {
        FEATURE_CANONICAL_FORM                = 0x0001,
        FEATURE_CDATA_SECTIONS                = 0x0002,
        FEATURE_COMMENTS                      = 0x0004,
        FEATURE_DATATYPE_NORMALIZATION        = 0x0008,
        FEATURE_DISCARD_DEFAULT_CONTENT       = 0x0010, 
        FEATURE_ENTITIES                      = 0x0020,  
        FEATURE_INFOSET                       = 0x0040, 
        FEATURE_NAMESPACES                    = 0x0080, 
        FEATURE_NAMESPACE_DECLARATIONS        = 0x0100, 
        FEATURE_NORMALIZE_CHARACTERS          = 0x0200, 
        FEATURE_SPLIT_CDATA_SECTIONS          = 0x0400, 
        FEATURE_VALIDATE                      = 0x0800, 
        FEATURE_VALIDATE_IF_SCHEMA            = 0x1000, 
        FEATURE_WHITESPACE_IN_ELEMENT_CONTENT = 0x2000
    };

    unsigned short featureValues;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------

    void setParameter(const XMLCh* name, const void* value);

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    
    const void* getParameter(const XMLCh* name) const;

                                        
    // -----------------------------------------------------------------------
    //  Query methods
    // -----------------------------------------------------------------------

    bool canSetParameter(const XMLCh* name, const void* value) const;


    // ---------------------------------------------------------------------------
    // Impl specific methods
    // ---------------------------------------------------------------------------
    
    /* specific get and set methods for non-boolean parameters
     * */

    DOMErrorHandler* getErrorHandler() const;

    const XMLCh* getSchemaType() const;

    const XMLCh* getSchemaLocation() const;

    void setErrorHandler(DOMErrorHandler *erHandler);

    void setSchemaType(const XMLCh* st);

    void setSchemaLocation(const XMLCh* sl);
    
    // --------------------------------------
    // static consts names 
    // --------------------------------------
    static const XMLCh fgCANONICAL_FORM[];
    static const XMLCh fgCDATA_SECTIONS[];
    static const XMLCh fgCOMMENTS[];
    static const XMLCh fgDATATYPE_NORMALIZATION[];
    static const XMLCh fgDISCARD_DEFAULT_CONTENT[];
    static const XMLCh fgENTITIES[];
    static const XMLCh fgINFOSET[];
    static const XMLCh fgNAMESPACES[];
    static const XMLCh fgNAMESPACE_DECLARATIONS[];
    static const XMLCh fgNORMALIZE_CHARACTERS[];
    static const XMLCh fgSPLIT_CDATA_SECTIONS[];
    static const XMLCh fgVALIDATE[];
    static const XMLCh fgVALIDATE_IF_SCHEMA[];
    static const XMLCh fgWHITESPACE_IN_ELEMENT_CONTENT[];

    static const XMLCh fgERROR_HANDLER[];
    static const XMLCh fgSCHEMA_TYPE[];
    static const XMLCh fgSCHEMA_LOCATION[];

    // The default values for the boolean parameters
    // from CANONICAL_FORM to WHITESPACE_IN_ELEMENT_CONTENT
    // 10010110010110 = 0x2596
    static const unsigned short fDEFAULT_VALUES;
    

private:
    // implements a simple map between the name and its enum value
    DOMConfigurationFeature getFeatureFlag(const XMLCh* name) const;

    // the error handler
    DOMErrorHandler* fErrorHandler;
    
    // the schema type
    const XMLCh* fSchemaType;
    
    // the schema location
    const XMLCh* fSchemaLocation;

    static const bool fFalse;
    static const bool fTrue;
    
    MemoryManager* fMemoryManager;
};

XERCES_CPP_NAMESPACE_END

#endif

/**
 * End of file DOMConfigurationImpl.hpp
 */
