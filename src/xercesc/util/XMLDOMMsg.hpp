// This file is generated, don't edit it!!

#if !defined(ERRHEADER_XMLDOMMsg)
#define ERRHEADER_XMLDOMMsg

#include <xercesc/framework/XMLErrorReporter.hpp>
#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLDOMMsg
{
public :
    enum Codes
    {
        NoError                            = 0
      , F_LowBounds                        = 1
      , INDEX_SIZE_ERR                     = 2
      , DOMSTRING_SIZE_ERR                 = 3
      , HIERARCHY_REQUEST_ERR              = 4
      , WRONG_DOCUMENT_ERR                 = 5
      , INVALID_CHARACTER_ERR              = 6
      , NO_DATA_ALLOWED_ERR                = 7
      , NO_MODIFICATION_ALLOWED_ERR        = 8
      , NOT_FOUND_ERR                      = 9
      , NOT_SUPPORTED_ERR                  = 10
      , INUSE_ATTRIBUTE_ERR                = 11
      , INVALID_STATE_ERR                  = 12
      , SYNTAX_ERR                         = 13
      , INVALID_MODIFICATION_ERR           = 14
      , NAMESPACE_ERR                      = 15
      , INVALID_ACCESS_ERR                 = 16
      , VALIDATION_ERR                     = 17
      , INVALID_NODE_TYPE_ERR              = 18
      , BAD_BOUNDARYPOINTS_ERR             = 19
      , F_HighBounds                       = 20
      , W_LowBounds                        = 21
      , W_HighBounds                       = 22
      , E_LowBounds                        = 23
      , E_HighBounds                       = 24
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
};

XERCES_CPP_NAMESPACE_END

#endif

