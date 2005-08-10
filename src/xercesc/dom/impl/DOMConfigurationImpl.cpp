/*
 * Copyright 2003,2004 The Apache Software Foundation.
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

#include "DOMConfigurationImpl.hpp"
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/util/Janitor.hpp>

XERCES_CPP_NAMESPACE_BEGIN

const unsigned short DOMConfigurationImpl::fDEFAULT_VALUES = 0x2596;

DOMConfigurationImpl::DOMConfigurationImpl(MemoryManager* const manager): featureValues(fDEFAULT_VALUES),
                                              fErrorHandler(0), fSchemaType(0), fSchemaLocation(0),
                                              fSupportedParameters(0), fMemoryManager(manager)
{
    fSupportedParameters=new RefVectorOf<XMLCh>(17, false, manager);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMErrorHandler);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMSchemaType);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMSchemaLocation);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMCanonicalForm);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMCDATASections);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMComments);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMDatatypeNormalization);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMWRTDiscardDefaultContent);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMEntities);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMInfoset);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMNamespaces);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMNamespaceDeclarations);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMNormalizeCharacters);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMSplitCDATASections);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMValidate);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMValidateIfSchema);
    fSupportedParameters->addElement((XMLCh*)XMLUni::fgDOMElementContentWhitespace);
}

DOMConfigurationImpl::~DOMConfigurationImpl() {
    delete fSupportedParameters;
}
                                        
void DOMConfigurationImpl::setParameter(const XMLCh* name, const void* value) {

    XMLCh* lowerCaseName = XMLString::replicate(name, fMemoryManager);
    ArrayJanitor<XMLCh> janName(lowerCaseName, fMemoryManager);

    XMLString::lowerCaseASCII(lowerCaseName);

    if(!canSetParameter(lowerCaseName, value)) {
        throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0, fMemoryManager);
    }

    if(XMLString::equals(lowerCaseName, XMLUni::fgDOMErrorHandler)) {
        fErrorHandler = (DOMErrorHandler*)value;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMSchemaType)) {
        fSchemaType = (XMLCh*)value;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMSchemaLocation)) {
        fSchemaLocation = (XMLCh*)value;
    } else {  // canSetParameter above should take care of this case
        throw DOMException(DOMException::NOT_FOUND_ERR, 0, fMemoryManager);
    }

}

void DOMConfigurationImpl::setParameter(const XMLCh* name, bool value) {
    XMLCh* lowerCaseName = XMLString::replicate(name, fMemoryManager);
    ArrayJanitor<XMLCh> janName(lowerCaseName, fMemoryManager);

    XMLString::lowerCaseASCII(lowerCaseName);

    if(!canSetParameter(lowerCaseName, value)) {
        throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0, fMemoryManager);
    }

    DOMConfigurationFeature whichFlag = getFeatureFlag(lowerCaseName);
    if(value) {
        featureValues |= whichFlag;
    } else {
        featureValues &= ~whichFlag;
    }

}

// --------------------------------------
// Getter Methods
// --------------------------------------

const void* DOMConfigurationImpl::getParameter(const XMLCh* name) const {

    XMLCh* lowerCaseName = XMLString::replicate(name, fMemoryManager);
    ArrayJanitor<XMLCh> janName(lowerCaseName, fMemoryManager);

    XMLString::lowerCaseASCII(lowerCaseName);

    DOMConfigurationFeature whichFlag;
    try {
        whichFlag = getFeatureFlag(lowerCaseName);
        if(featureValues & whichFlag) {
            return (void*)true;
        } else {
            return (void*)false;
        }
   } catch (DOMException&) {
        // must not be a boolean parameter
        if(XMLString::equals(lowerCaseName, XMLUni::fgDOMErrorHandler)) {
            return fErrorHandler;
        } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMSchemaType)) {
            return fSchemaType;
        } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMSchemaLocation)) {
            return fSchemaLocation;
        } else {
            throw DOMException(DOMException::NOT_FOUND_ERR, 0, fMemoryManager);
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
    
    XMLCh* lowerCaseName = XMLString::replicate(name, fMemoryManager);
    ArrayJanitor<XMLCh> janName(lowerCaseName, fMemoryManager);
    
    XMLString::lowerCaseASCII(lowerCaseName);
    
    if(XMLString::equals(lowerCaseName, XMLUni::fgDOMErrorHandler)) {
        return true;                               // required //
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMSchemaType)) {
        return false;                            // optional //
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMSchemaLocation)) {
        return false;                            // optional //
    } 
    return false;
}

bool DOMConfigurationImpl::canSetParameter(const XMLCh* name, bool booleanValue) const {
    /**
     * canSetParameter(name, value) returns false in two conditions:
     *  1) if a [required] feature has no supporting code, then return false in 
     *     both the true and false outcomes (This is in order to be either fully 
     *     spec compliant, or not at all)
     *  2) if an [optional] feature has no supporting code, then return false
     **/ 
    
    XMLCh* lowerCaseName = XMLString::replicate(name, fMemoryManager);
    ArrayJanitor<XMLCh> janName(lowerCaseName, fMemoryManager);
    
    XMLString::lowerCaseASCII(lowerCaseName);
    
    DOMConfigurationFeature whichFlag = getFeatureFlag(lowerCaseName);
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
          
        case FEATURE_ELEMENT_CONTENT_WHITESPACE:  
            if(booleanValue) return true;       // required //
            else             return false;      // optional //
        default: return false; // should never be here
    }
    return false;
}

const RefVectorOf<XMLCh>* DOMConfigurationImpl::getParameterNames() const
{
    return fSupportedParameters;
}

// -------------------------------------------
// Impl methods
// -------------------------------------------

DOMConfigurationImpl::DOMConfigurationFeature DOMConfigurationImpl::getFeatureFlag(const XMLCh* name) const {
    XMLCh* lowerCaseName = XMLString::replicate(name, fMemoryManager);
    ArrayJanitor<XMLCh> janName(lowerCaseName, fMemoryManager);
    
    XMLString::lowerCaseASCII(lowerCaseName);
  
    if(XMLString::equals(lowerCaseName, XMLUni::fgDOMCanonicalForm)) {
        return FEATURE_CANONICAL_FORM;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMCDATASections )) {
        return FEATURE_CDATA_SECTIONS;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMComments)) {
        return FEATURE_COMMENTS;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMDatatypeNormalization))  {
        return FEATURE_DATATYPE_NORMALIZATION;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMWRTDiscardDefaultContent)) {
        return FEATURE_DISCARD_DEFAULT_CONTENT;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMEntities)) {
        return FEATURE_ENTITIES;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMInfoset))  {
        return FEATURE_INFOSET;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMNamespaces)) {
        return FEATURE_NAMESPACES;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMNamespaceDeclarations)) {
        return FEATURE_NAMESPACE_DECLARATIONS;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMNormalizeCharacters)) {
        return FEATURE_NORMALIZE_CHARACTERS;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMSplitCDATASections)) {
        return FEATURE_SPLIT_CDATA_SECTIONS;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMValidate)) {
        return FEATURE_VALIDATE;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMValidateIfSchema)) {
        return FEATURE_VALIDATE_IF_SCHEMA;
    } else if (XMLString::equals(lowerCaseName, XMLUni::fgDOMElementContentWhitespace)) {
        return FEATURE_ELEMENT_CONTENT_WHITESPACE;
    } else {
        throw DOMException(DOMException::NOT_FOUND_ERR, 0, fMemoryManager);
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
