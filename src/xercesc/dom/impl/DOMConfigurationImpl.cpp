/*
 * The Apache Software License, Version 1.1
 *
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
 *    permission, please contact apache@apache.org.
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
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.apache.org.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#include "DOMConfigurationImpl.hpp"
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/util/Janitor.hpp>

XERCES_CPP_NAMESPACE_BEGIN

const bool DOMConfigurationImpl::fFalse = false;
const bool DOMConfigurationImpl::fTrue = true;

/* canonical-form */
const XMLCh DOMConfigurationImpl::fgCANONICAL_FORM[] = { chLatin_c, chLatin_a, chLatin_n, chLatin_o, chLatin_n, chLatin_i, chLatin_c, chLatin_a, chLatin_l, chDash, chLatin_f, chLatin_o, chLatin_r, chLatin_m, chNull };

/* cdata-sections */
const XMLCh DOMConfigurationImpl::fgCDATA_SECTIONS[] = { chLatin_c, chLatin_d, chLatin_a, chLatin_t, chLatin_a, chDash, chLatin_s, chLatin_e, chLatin_c, chLatin_t, chLatin_i, chLatin_o, chLatin_n, chLatin_s, chNull };

/* comments */
const XMLCh DOMConfigurationImpl::fgCOMMENTS[] = { chLatin_c, chLatin_o, chLatin_m, chLatin_m, chLatin_e, chLatin_n, chLatin_t, chLatin_s, chNull };

/* datatype-normalization */
const XMLCh DOMConfigurationImpl::fgDATATYPE_NORMALIZATION[] = { chLatin_d, chLatin_a, chLatin_t, chLatin_a, chLatin_t, chLatin_y, chLatin_p, chLatin_e, chDash, chLatin_n, chLatin_o, chLatin_r, chLatin_m, chLatin_a, chLatin_l, chLatin_i, chLatin_z, chLatin_a, chLatin_t, chLatin_i, chLatin_o, chLatin_n, chNull };

/* discard-default-content */
const XMLCh DOMConfigurationImpl::fgDISCARD_DEFAULT_CONTENT[] = { chLatin_d, chLatin_i, chLatin_s, chLatin_c, chLatin_a, chLatin_r, chLatin_d, chDash, chLatin_d, chLatin_e, chLatin_f, chLatin_a, chLatin_u, chLatin_l, chLatin_t, chDash, chLatin_c, chLatin_o, chLatin_n, chLatin_t, chLatin_e, chLatin_n, chLatin_t, chNull };

/* entities */
const XMLCh DOMConfigurationImpl::fgENTITIES[] = { chLatin_e, chLatin_n, chLatin_t, chLatin_i, chLatin_t, chLatin_i, chLatin_e, chLatin_s, chNull };

/* infoset */
const XMLCh DOMConfigurationImpl::fgINFOSET[] = { chLatin_i, chLatin_n, chLatin_f, chLatin_o, chLatin_s, chLatin_e, chLatin_t, chNull };

/* namespaces */
const XMLCh DOMConfigurationImpl::fgNAMESPACES[] = { chLatin_n, chLatin_a, chLatin_m, chLatin_e, chLatin_s, chLatin_p, chLatin_a, chLatin_c, chLatin_e, chLatin_s, chNull };

/* namespace-declarations */
const XMLCh DOMConfigurationImpl::fgNAMESPACE_DECLARATIONS[] = { chLatin_n, chLatin_a, chLatin_m, chLatin_e, chLatin_s, chLatin_p, chLatin_a, chLatin_c, chLatin_e, chDash, chLatin_d, chLatin_e, chLatin_c, chLatin_l, chLatin_a, chLatin_r, chLatin_a, chLatin_t, chLatin_i, chLatin_o, chLatin_n, chLatin_s, chNull };

/* normalize-characters */
const XMLCh DOMConfigurationImpl::fgNORMALIZE_CHARACTERS[] = { chLatin_n, chLatin_o, chLatin_r, chLatin_m, chLatin_a, chLatin_l, chLatin_i, chLatin_z, chLatin_e, chDash, chLatin_c, chLatin_h, chLatin_a, chLatin_r, chLatin_a, chLatin_c, chLatin_t, chLatin_e, chLatin_r, chLatin_s, chNull };

/* split-cdata-sections */
const XMLCh DOMConfigurationImpl::fgSPLIT_CDATA_SECTIONS[] = { chLatin_s, chLatin_p, chLatin_l, chLatin_i, chLatin_t, chDash, chLatin_c, chLatin_d, chLatin_a, chLatin_t, chLatin_a, chDash, chLatin_s, chLatin_e, chLatin_c, chLatin_t, chLatin_i, chLatin_o, chLatin_n, chLatin_s, chNull };

/* validate */
const XMLCh DOMConfigurationImpl::fgVALIDATE[] = { chLatin_v, chLatin_a, chLatin_l, chLatin_i, chLatin_d, chLatin_a, chLatin_t, chLatin_e, chNull };

/* validate-if-schema */
const XMLCh DOMConfigurationImpl::fgVALIDATE_IF_SCHEMA[] = { chLatin_v, chLatin_a, chLatin_l, chLatin_i, chLatin_d, chLatin_a, chLatin_t, chLatin_e, chDash, chLatin_i, chLatin_f, chDash, chLatin_s, chLatin_c, chLatin_h, chLatin_e, chLatin_m, chLatin_a, chNull };

/* whitespace-in-element-content */
const XMLCh DOMConfigurationImpl::fgWHITESPACE_IN_ELEMENT_CONTENT[] = { chLatin_w, chLatin_h, chLatin_i, chLatin_t, chLatin_e, chLatin_s, chLatin_p, chLatin_a, chLatin_c, chLatin_e, chDash, chLatin_i, chLatin_n, chDash, chLatin_e, chLatin_l, chLatin_e, chLatin_m, chLatin_e, chLatin_n, chLatin_t, chDash, chLatin_c, chLatin_o, chLatin_n, chLatin_t, chLatin_e, chLatin_n, chLatin_t, chNull };

/* error-handler */
const XMLCh DOMConfigurationImpl::fgERROR_HANDLER[] = { chLatin_e, chLatin_r, chLatin_r, chLatin_o, chLatin_r, chDash, chLatin_h, chLatin_a, chLatin_n, chLatin_d, chLatin_l, chLatin_e, chLatin_r, chNull };

/* schema-type */
const XMLCh DOMConfigurationImpl::fgSCHEMA_TYPE[] = { chLatin_s, chLatin_c, chLatin_h, chLatin_e, chLatin_m, chLatin_a, chDash, chLatin_t, chLatin_y, chLatin_p, chLatin_e, chNull };

/* schema-location */
const XMLCh DOMConfigurationImpl::fgSCHEMA_LOCATION[] = { chLatin_s, chLatin_c, chLatin_h, chLatin_e, chLatin_m, chLatin_a, chDash, chLatin_l, chLatin_o, chLatin_c, chLatin_a, chLatin_t, chLatin_i, chLatin_o, chLatin_n, chNull };

const unsigned short DOMConfigurationImpl::fDEFAULT_VALUES = 0x2596;

DOMConfigurationImpl::DOMConfigurationImpl(MemoryManager* const manager): featureValues(fDEFAULT_VALUES),
                                              fErrorHandler(0), fSchemaType(0), fSchemaLocation(0)
, fMemoryManager(manager)
 {
}

DOMConfigurationImpl::~DOMConfigurationImpl() {
}
                                        
void DOMConfigurationImpl::setParameter(const XMLCh* name, const void* value) {

    XMLCh* lowerCaseName = XMLString::replicate(name, fMemoryManager);
    ArrayJanitor<XMLCh> janName(lowerCaseName, fMemoryManager);

    XMLString::lowerCase(lowerCaseName);

    if(!canSetParameter(lowerCaseName, value)) {
        throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
    }

    bool isBooleanParameter = true;
    DOMConfigurationFeature whichFlag;
    try {
        whichFlag = getFeatureFlag(lowerCaseName);
    } catch(DOMException e) {
        // must not be a boolean parameter
        isBooleanParameter = false;
    }

    if(isBooleanParameter) {
        if(*((bool*)value)) {
            featureValues |= whichFlag;
        } else {
            featureValues &= ~whichFlag;
        }
    } else {
        if(XMLString::equals(lowerCaseName, fgERROR_HANDLER)) {
            fErrorHandler = (DOMErrorHandler*)value;
        } else if (XMLString::equals(lowerCaseName, fgSCHEMA_TYPE)) {
            fSchemaType = (XMLCh*)value;
        } else if (XMLString::equals(lowerCaseName, fgSCHEMA_LOCATION)) {
            fSchemaLocation = (XMLCh*)value;
        } else {  // canSetParameter above should take care of this case
            throw DOMException(DOMException::NOT_FOUND_ERR, 0);
        }
    }

}

// --------------------------------------
// Getter Methods
// --------------------------------------

const void* DOMConfigurationImpl::getParameter(const XMLCh* name) const {

    XMLCh* lowerCaseName = XMLString::replicate(name, fMemoryManager);
    ArrayJanitor<XMLCh> janName(lowerCaseName, fMemoryManager);

    XMLString::lowerCase(lowerCaseName);

    bool isBooleanParameter = true;
    DOMConfigurationFeature whichFlag;
    try {
        whichFlag = getFeatureFlag(lowerCaseName);
    } catch (DOMException e) {
        // must not be a boolean parameter
        isBooleanParameter = false;
    }

    if(isBooleanParameter){
        if(featureValues & whichFlag) {
            return &fTrue;
        } else {
            return &fFalse;
        }
    } else {
        if(XMLString::equals(lowerCaseName, fgERROR_HANDLER)) {
            return fErrorHandler;
        } else if (XMLString::equals(lowerCaseName, fgSCHEMA_TYPE)) {
            return fSchemaType;
        } else if (XMLString::equals(lowerCaseName, fgSCHEMA_LOCATION)) {
            return fSchemaLocation;
        } else {
            throw DOMException(DOMException::NOT_FOUND_ERR, 0);
        }
    }

}

// -----------------------------------------
// Query Methods
// -----------------------------------------

bool DOMConfigurationImpl::canSetParameter(const XMLCh* name, const void* value) const {

    /**
     * canSetParameter(name, value) returns false in two conditions:
     *  1) if a [required] feature has no supporting code, then return false in 
     *     both the true and false outcomes (This is in order to be either fully 
     *     spec compliant, or not at all)
     *  2) if an [optional] feature has no supporting code, then return false
     **/ 
    
    // if value is null, return true
    if(value == 0) return true;

    XMLCh* lowerCaseName = XMLString::replicate(name, fMemoryManager);
    ArrayJanitor<XMLCh> janName(lowerCaseName, fMemoryManager);
    
    XMLString::lowerCase(lowerCaseName);
    
    bool isBooleanParameter = true;
    bool booleanValue = false;
    DOMConfigurationFeature whichFlag;
    try {
        whichFlag = getFeatureFlag(lowerCaseName);
        booleanValue = *((bool*)value);
    } catch (DOMException e) {
        // must not be a boolean parameter
        isBooleanParameter = false;
    }
    
    if(isBooleanParameter) {
        switch (whichFlag) {
            case FEATURE_CANONICAL_FORM: 
                if(booleanValue) return false;      // optional //
                else             return true;       // required // 
            case FEATURE_CDATA_SECTIONS: 
                return true;
            case FEATURE_COMMENTS:  
                return true;
            case FEATURE_DATATYPE_NORMALIZATION:  
                if(booleanValue) return false;       // required //
                else             return true;        // required //
            case FEATURE_DISCARD_DEFAULT_CONTENT:  
                if(booleanValue) return false;       // required //
                else             return true;        // required //
            case FEATURE_ENTITIES:  
                if(booleanValue) return true;       // required //
                else             return true;       // required //
            case FEATURE_INFOSET:  
                if(booleanValue) return false;       // required //
                else             return true;       // no effect//
            case FEATURE_NAMESPACES:  
                return true;       
            case FEATURE_NAMESPACE_DECLARATIONS:  
                if(booleanValue) return true;      // optional //
                else             return false;       // required //
            case FEATURE_NORMALIZE_CHARACTERS:  
                if(booleanValue) return false;      // optional //
                else             return true;       // required //
            case FEATURE_SPLIT_CDATA_SECTIONS:  
                //we dont report an error in the false case so we cant claim we do it
                if(booleanValue) return false;       // required //
                else             return false;       // required //
            case FEATURE_VALIDATE:  
                if(booleanValue) return false;      // optional //
                else             return true;       // required //
            case FEATURE_VALIDATE_IF_SCHEMA:  
                if(booleanValue) return false;      // optional //
                else             return true;       // required //
              
            case FEATURE_WHITESPACE_IN_ELEMENT_CONTENT:  
                if(booleanValue) return true;       // required //
                else             return false;      // optional //
            default: return false; // should never be here
        }
    } else {
        if(XMLString::equals(lowerCaseName, fgERROR_HANDLER)) {
            return true;                               // required //
        } else if (XMLString::equals(lowerCaseName, fgSCHEMA_TYPE)) {
            return false;                            // optional //
        } else if (XMLString::equals(lowerCaseName, fgSCHEMA_LOCATION)) {
            return false;                            // optional //
        } 
    }
    return false;
}

// -------------------------------------------
// Impl methods
// -------------------------------------------

DOMConfigurationImpl::DOMConfigurationFeature DOMConfigurationImpl::getFeatureFlag(const XMLCh* name) const {
    XMLCh* lowerCaseName = XMLString::replicate(name, fMemoryManager);
    ArrayJanitor<XMLCh> janName(lowerCaseName, fMemoryManager);
    
    XMLString::lowerCase(lowerCaseName);
  
    if(XMLString::equals(lowerCaseName, fgCANONICAL_FORM)) {
        return FEATURE_CANONICAL_FORM;
    } else if (XMLString::equals(lowerCaseName, fgCDATA_SECTIONS )) {
        return FEATURE_CDATA_SECTIONS;
    } else if (XMLString::equals(lowerCaseName, fgCOMMENTS)) {
        return FEATURE_COMMENTS;
    } else if (XMLString::equals(lowerCaseName, fgDATATYPE_NORMALIZATION))  {
        return FEATURE_DATATYPE_NORMALIZATION;
    } else if (XMLString::equals(lowerCaseName, fgDISCARD_DEFAULT_CONTENT)) {
        return FEATURE_DISCARD_DEFAULT_CONTENT;
    } else if (XMLString::equals(lowerCaseName, fgENTITIES)) {
        return FEATURE_ENTITIES;
    } else if (XMLString::equals(lowerCaseName, fgINFOSET))  {
        return FEATURE_INFOSET;
    } else if (XMLString::equals(lowerCaseName, fgNAMESPACES)) {
        return FEATURE_NAMESPACES;
    } else if (XMLString::equals(lowerCaseName, fgNAMESPACE_DECLARATIONS)) {
        return FEATURE_NAMESPACE_DECLARATIONS;
    } else if (XMLString::equals(lowerCaseName, fgNORMALIZE_CHARACTERS)) {
        return FEATURE_NORMALIZE_CHARACTERS;
    } else if (XMLString::equals(lowerCaseName, fgSPLIT_CDATA_SECTIONS)) {
        return FEATURE_SPLIT_CDATA_SECTIONS;
    } else if (XMLString::equals(lowerCaseName, fgVALIDATE)) {
        return FEATURE_VALIDATE;
    } else if (XMLString::equals(lowerCaseName, fgVALIDATE_IF_SCHEMA)) {
        return FEATURE_VALIDATE_IF_SCHEMA;
    } else if (XMLString::equals(lowerCaseName, fgWHITESPACE_IN_ELEMENT_CONTENT)) {
        return FEATURE_WHITESPACE_IN_ELEMENT_CONTENT;
    } else {
        throw DOMException(DOMException::NOT_FOUND_ERR, 0);
    }
        
}

DOMErrorHandler* DOMConfigurationImpl::getErrorHandler() const {
    return fErrorHandler;
}

const XMLCh* DOMConfigurationImpl::getSchemaType() const {
    return fSchemaType;
}

const XMLCh* DOMConfigurationImpl::getSchemaLocation() const {
    return fSchemaLocation;
}

void DOMConfigurationImpl::setErrorHandler(DOMErrorHandler *erHandler) {
    fErrorHandler = erHandler;
}

void DOMConfigurationImpl::setSchemaType(const XMLCh* st) {
    fSchemaType = st;
}

void DOMConfigurationImpl::setSchemaLocation(const XMLCh* sl) {
    fSchemaLocation = sl;
}


XERCES_CPP_NAMESPACE_END


/**
 * End of file DOMConfigurationImpl.cpp
 */
