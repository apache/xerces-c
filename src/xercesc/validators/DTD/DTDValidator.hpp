/*
 * Copyright 1999-2001,2004 The Apache Software Foundation.
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
 * Revision 1.9  2004/09/08 13:56:50  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2004/04/21 18:00:13  peiyongz
 * xml1.0 3rd edition VC: no more than one attibute of notation type per element,
 * distinct token for attribute of notation/enumerated type
 *
 * Revision 1.7  2004/01/29 11:52:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.6  2002/11/07 21:58:56  tng
 * Pass elemDecl to XMLValidator::validateAttrValue so that we can include element name in error message.
 *
 * Revision 1.5  2002/11/04 14:50:40  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/09/04 18:17:49  tng
 * Do not set IDREF to used during prevalidation.
 *
 * Revision 1.3  2002/08/22 15:05:40  tng
 * Remove unused parameter variables in inline functions.
 *
 * Revision 1.2  2002/07/11 18:55:44  knoaman
 * Add a flag to the preContentValidation method to indicate whether to validate
 * default/fixed attributes or not.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:45  peiyongz
 * sane_include
 *
 * Revision 1.13  2001/11/13 13:25:28  tng
 * Deprecate function XMLValidator::checkRootElement.
 *
 * Revision 1.12  2001/06/05 16:51:20  knoaman
 * Add 'const' to getGrammar - submitted by Peter A. Volchek.
 *
 * Revision 1.11  2001/05/11 13:27:11  tng
 * Copyright update.
 *
 * Revision 1.10  2001/05/03 20:34:37  tng
 * Schema: SchemaValidator update
 *
 * Revision 1.9  2001/04/19 18:17:23  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.8  2001/03/21 21:56:21  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.7  2001/02/26 19:22:08  tng
 * Schema: add parameter prefix in findElem and findAttr.
 *
 * Revision 1.6  2000/08/09 22:08:48  jpolast
 * added const XMLCh* getURIText() as per XMLValidator.
 * allows parsers to use const URIs instead of appending
 * to a XMLBuffer.
 *
 * Revision 1.5  2000/04/06 19:00:07  roddey
 * Added a getter for the doc type handler.
 *
 * Revision 1.4  2000/02/24 20:16:49  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.3  2000/02/09 21:42:38  abagchi
 * Copyright swat
 *
 * Revision 1.2  2000/01/12 23:52:48  roddey
 * These are trivial changes required to get the C++ and Java versions
 * of error messages more into sync. Mostly it was where the Java version
 * was passing out one or more parameter than the C++ version was. In
 * some cases the change just required an extra parameter to get the
 * needed info to the place where the error was issued.
 *
 * Revision 1.1.1.1  1999/11/09 01:03:36  twl
 * Initial checkin
 *
 * Revision 1.5  1999/11/08 20:45:41  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */



#if !defined(DTDVALIDATOR_HPP)
#define DTDVALIDATOR_HPP

#include <xercesc/util/NameIdPool.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/validators/DTD/DTDGrammar.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLMsgLoader;


//
//  This is a derivative of the abstract validator interface. This class
//  implements a validator that supports standard XML 1.0 DTD semantics.
//  This class handles scanning the internal and external subsets of the
//  DTD, and provides the standard validation services against the DTD info
//  it found.
//
class VALIDATORS_EXPORT DTDValidator : public XMLValidator
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    DTDValidator(XMLErrorReporter* const errReporter = 0);
    virtual ~DTDValidator();

    // -----------------------------------------------------------------------
    //  Implementation of the XMLValidator interface
    // -----------------------------------------------------------------------
    virtual int checkContent
    (
        XMLElementDecl* const   elemDecl
        , QName** const         children
        , const unsigned int    childCount
    );

    virtual void faultInAttr
    (
                XMLAttr&    toFill
        , const XMLAttDef&  attDef
    )   const;

    virtual void preContentValidation(bool reuseGrammar,
                                      bool validateDefAttr = false);

    virtual void postParseValidation();

    virtual void reset();

    virtual bool requiresNamespaces() const;

    virtual void validateAttrValue
    (
        const   XMLAttDef*                  attDef
        , const XMLCh* const                attrValue
        , bool                              preValidation = false
        , const XMLElementDecl*             elemDecl = 0
    );
    virtual void validateElement
    (
        const   XMLElementDecl*             elemDef
    );
    virtual Grammar* getGrammar() const;
    virtual void setGrammar(Grammar* aGrammar);

    // -----------------------------------------------------------------------
    //  Virtual DTD handler interface.
    // -----------------------------------------------------------------------
    virtual bool handlesDTD() const;

    // -----------------------------------------------------------------------
    //  Virtual Schema handler interface. handlesSchema() always return false.
    // -----------------------------------------------------------------------
    virtual bool handlesSchema() const;

private:
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DTDValidator(const DTDValidator &);
    DTDValidator& operator = (const  DTDValidator&);

    // -----------------------------------------------------------------------
    //  Helper
    // -----------------------------------------------------------------------
    void   checkTokenList(const XMLAttDef&  attDef
                        ,       bool        toValidateNotation);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fDTDGrammar
    //      The DTD information stored.
    //
    // -----------------------------------------------------------------------
    DTDGrammar*                     fDTDGrammar;
};

// ---------------------------------------------------------------------------
//  Virtual interface
// ---------------------------------------------------------------------------
inline Grammar* DTDValidator::getGrammar() const {
    return fDTDGrammar;
}

inline void DTDValidator::setGrammar(Grammar* aGrammar) {
    fDTDGrammar = (DTDGrammar*) aGrammar;
}

inline void DTDValidator::validateElement (const   XMLElementDecl*) {
    // no special DTD Element validation
}

// ---------------------------------------------------------------------------
//  DTDValidator: DTD handler interface
// ---------------------------------------------------------------------------
inline bool DTDValidator::handlesDTD() const
{
    // We definitely want to handle DTD scanning
    return true;
}

// ---------------------------------------------------------------------------
//  DTDValidator: Schema handler interface
// ---------------------------------------------------------------------------
inline bool DTDValidator::handlesSchema() const
{
    // No Schema scanning
    return false;
}

XERCES_CPP_NAMESPACE_END

#endif
