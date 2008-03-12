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

// DOMConfiguration::canSetParameter() must handle certain args specially
%extend XERCES_CPP_NAMESPACE::DOMConfiguration {
%typemap(in) const void* value (void *argp, int res) {
  if (XMLString::compareIStringASCII(arg2, XMLUni::fgDOMErrorHandler) == 0) {
    res = SWIG_ConvertPtr($input, &argp, $descriptor(DOMErrorHandler*), 0 |  0 );
    if (!SWIG_IsOK(res)) {
      SWIG_exception_fail(SWIG_ArgError(res), "in method '" "$symname" "', argument " "$argnum"" of type '" "DOMErrorHandler const *""'"); 
    }
    $1 = reinterpret_cast< DOMErrorHandler * >(argp);
    
  } else if (XMLString::compareIStringASCII(arg2, XMLUni::fgXercesEntityResolver) == 0) {
    res = SWIG_ConvertPtr($input, &argp, $descriptor(XMLEntityResolver*), 0 |  0 );
    if (!SWIG_IsOK(res)) {
      SWIG_exception_fail(SWIG_ArgError(res), "in method '" "$symname" "', argument " "$argnum"" of type '" "XMLEntityResolver const *""'"); 
    }
    $1 = reinterpret_cast< XMLEntityResolver * >(argp);
    
  } else if (XMLString::compareIStringASCII(arg2, XMLUni::fgDOMResourceResolver) == 0) {
    res = SWIG_ConvertPtr($input, &argp, $descriptor(DOMLSResourceResolver*), 0 |  0 );
    if (!SWIG_IsOK(res)) {
      SWIG_exception_fail(SWIG_ArgError(res), "in method '" "$symname" "', argument " "$argnum"" of type '" "DOMLSResourceResolver const *""'"); 
    }
    $1 = reinterpret_cast< DOMLSResourceResolver * >(argp);
    
  } else {
    $1 = UTF8_TRANSCODER->Local2XMLString($input);
  }
}

%typemap(out) const void* {
  if (XMLString::compareIStringASCII(arg2, XMLUni::fgDOMErrorHandler) == 0) {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), $descriptor(DOMErrorHandler*), 0 | SWIG_SHADOW); 
  } else if (XMLString::compareIStringASCII(arg2, XMLUni::fgXercesEntityResolver) == 0) {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), $descriptor(XMLEntityResolver*), 0 | SWIG_SHADOW); 
  } else if (XMLString::compareIStringASCII(arg2, XMLUni::fgDOMResourceResolver) == 0) {
    $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), $descriptor(DOMLSResourceResolver*), 0 | SWIG_SHADOW); 
  } else if (isDOMConfigXMLChParameter(arg2)) {
    $result = UTF8_TRANSCODER->XMLString2Local((XMLCh*)$1);
  } else {
    $result = SWIG_From_bool  SWIG_PERL_CALL_ARGS_1(static_cast< bool >($1));
  }
  argvi++; 
}

%typemap(freearg) void * %{
  if (isDOMConfigXMLChParameter(arg2)) {
    delete (XMLCh*) $1;
  }
%}

%{

bool
isDOMConfigObjectParameter(const XMLCh* name) {
    if (XMLString::compareIStringASCII(name, XMLUni::fgXercesEntityResolver) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSecurityManager) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMResourceResolver) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMErrorHandler) == 0) 
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
isDOMConfigBoolParameter(const XMLCh* name) {
    if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCharsetOverridesXMLEncoding) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMDisallowDoctype) == 0) 
    {
        // TODO
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMIgnoreUnknownCharacterDenormalization) == 0) 
    {
        // TODO
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMNamespaces) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSupportedMediatypesOnly) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMValidate) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMValidateIfSchema) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMWellFormed) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCanonicalForm) == 0 ) 
    {
        // TODO
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCDATASections) == 0 ) 
    {
        // TODO
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCheckCharacterNormalization) == 0 ) 
    {
        // TODO
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMComments) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMDatatypeNormalization) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMElementContentWhitespace) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMEntities) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMNamespaceDeclarations) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMNormalizeCharacters) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSchemaLocation) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSchemaType) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSplitCDATASections) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMInfoset) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchema) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaFullChecking) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIdentityConstraintChecking) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesLoadExternalDTD) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesContinueAfterFatalError) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesValidationErrorAsFatal) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesCacheGrammarFromParse) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesUseCachedGrammarInParse) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesCalculateSrcOfs) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesStandardUriConformant) == 0)
    {
        return true;
    }
    else if(XMLString::compareIStringASCII(name, XMLUni::fgXercesUserAdoptsDOMDocument) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesDOMHasPSVIInfo) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesGenerateSyntheticAnnotations) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesValidateAnnotations) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIgnoreCachedDTD) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIgnoreAnnotations) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesDisableDefaultEntityResolution) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSkipDTDValidation) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
isDOMConfigXMLChParameter(const XMLCh* name) {
    if (XMLString::compareIStringASCII(name, XMLUni::fgXercesScannerName) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesParserUseDocumentFromImplementation) == 0)
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalSchemaLocation) == 0) 
    {
        return true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation) == 0) 
    {
        return true;
    }
    else
    {
        return false;
    }
}

%}

%typemap(typecheck,precedence=SWIG_TYPECHECK_VOIDPTR,noblock=1) void*, const void* {
        int res = -1;
	XMLCh *name = UTF8_TRANSCODER->Local2XMLString(ST(1));
        if (XMLString::compareIStringASCII(name, XMLUni::fgDOMErrorHandler) == 0) {
          res = 1;
        } else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesEntityResolver) == 0) {
	  res = 1;
        } else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSchemaLocation) == 0) {
	  res = 1;
        } else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSchemaType) == 0) {
	  res = 1;
        } else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMResourceResolver) == 0) {
	  res = 1;
        } else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSecurityManager) == 0) {
	  res = 1;
        } else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesScannerName) == 0) {
	  res = 1;
        } else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSchemaType) == 0) {
	  res = 1;
        } else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalSchemaLocation) == 0) {
	  res = 1;
        } else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation) == 0) {
	  res = 1;
        } else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesParserUseDocumentFromImplementation) == 0) {
	  res = 1;
        }
        _v = SWIG_CheckState(res);
}
}


%typemap(out) XERCES_CPP_NAMESPACE::XMLFormatTarget * = SWIGTYPE *DYNAMIC;

DYNAMIC_CAST(SWIGTYPE_p_XERCES_CPP_NAMESPACE__XMLFormatTarget, XMLFormatTarget_dynamic_cast);

%{
static swig_type_info *
XMLFormatTarget_dynamic_cast(void **ptr) {
   XMLFormatTarget **nptr = (XMLFormatTarget **) ptr;
   if (*nptr == NULL) {
       return NULL;
   }
   if (dynamic_cast< MemBufFormatTarget * >(*nptr)) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__MemBufFormatTarget;
   }
   if (dynamic_cast< LocalFileFormatTarget * >(*nptr)) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__LocalFileFormatTarget;
   }
   if (dynamic_cast< StdOutFormatTarget * >(*nptr)) {
      return SWIGTYPE_p_XERCES_CPP_NAMESPACE__StdOutFormatTarget;
   }
   return NULL;
}
%}

