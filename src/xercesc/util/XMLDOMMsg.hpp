// This file is generated, don't edit it!!

#if !defined(XERCESC_INCLUDE_GUARD_ERRHEADER_XMLDOMMsg)
#define XERCESC_INCLUDE_GUARD_ERRHEADER_XMLDOMMsg

#include <xercesc/framework/XMLErrorReporter.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOMError.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLDOMMsg
{
public :
    enum Codes
    {
        NoError                            = 0
      , F_LowBounds                        = 1
      , DOMEXCEPTION_ERRX                  = 2
      , INDEX_SIZE_ERR                     = 3
      , HIERARCHY_REQUEST_ERR              = 4
      , WRONG_DOCUMENT_ERR                 = 5
      , INVALID_CHARACTER_ERR              = 6
      , NO_MODIFICATION_ALLOWED_ERR        = 7
      , NOT_FOUND_ERR                      = 8
      , NOT_SUPPORTED_ERR                  = 9
      , INUSE_ATTRIBUTE_ERR                = 10
      , INVALID_STATE_ERR                  = 11
      , NAMESPACE_ERR                      = 12
      , INVALID_ACCESS_ERR                 = 13
      , VALIDATION_ERR                     = 14
      , DOMRANGEEXCEPTION_ERRX             = 15
      , BAD_BOUNDARYPOINTS_ERR             = 16
      , INVALID_NODE_TYPE_ERR              = 17
      , DOMLSEXCEPTION_ERRX                = 18
      , PARSE_ERR                          = 19
      , DOMXPATHEXCEPTION_ERRX             = 20
      , INVALID_EXPRESSION_ERR             = 21
      , TYPE_ERR                           = 22
      , Writer_NestedCDATA                 = 23
      , Writer_NotRepresentChar            = 24
      , Writer_NotRecognizedType           = 25
      , LSParser_ParseInProgress           = 26
      , LSParser_ParsingAborted            = 27
      , F_HighBounds                       = 28
      , W_LowBounds                        = 29
      , W_HighBounds                       = 30
      , E_LowBounds                        = 31
      , E_HighBounds                       = 32
    };

    static bool isFatal(const XMLDOMMsg::Codes toCheck)
    {
        return ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds));
    }

    static bool isWarning(const XMLDOMMsg::Codes toCheck)
    {
        return ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds));
    }

    static bool isError(const XMLDOMMsg::Codes toCheck)
    {
        return ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds));
    }

    static XMLErrorReporter::ErrTypes errorType(const XMLDOMMsg::Codes toCheck)
    {
       if ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds))
           return XMLErrorReporter::ErrType_Warning;
       else if ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds))
            return XMLErrorReporter::ErrType_Fatal;
       else if ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds))
            return XMLErrorReporter::ErrType_Error;
       return XMLErrorReporter::ErrTypes_Unknown;
    }
    static DOMError::ErrorSeverity  DOMErrorType(const XMLDOMMsg::Codes toCheck)
    {
       if ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds))
           return DOMError::DOM_SEVERITY_WARNING;
       else if ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds))
            return DOMError::DOM_SEVERITY_FATAL_ERROR;
       else return DOMError::DOM_SEVERITY_ERROR;
    }

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLDOMMsg();
};

XERCES_CPP_NAMESPACE_END

#endif

