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
 * Revision 1.24  2005/04/04 15:11:38  cargilld
 * Fix a problem where illegal qualified names were not reported as errors.  Also store the colon position when searching for a qualified name to avoid looking it up again.
 *
 * Revision 1.23  2004/12/14 16:16:36  cargilld
 * Fix for xercesc-684: Add accessor to XMLScanner to get the current grammar type.
 *
 * Revision 1.22  2004/12/03 19:40:30  cargilld
 * Change call to resolveEntity to pass in public id so that only one call to resolveEntity is needed (a follow-on to Alberto's fix).
 *
 * Revision 1.21  2004/09/27 21:04:38  knoaman
 * Update SGXMLScanner to allow access of data and methods to XSAXMLScanner
 *
 * Revision 1.20  2004/09/08 13:56:13  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.19  2004/05/25 18:09:51  peiyongz
 * XML1.0 3rd: 4.2.2 ...Since escaping is not always a fully reversible process,
 * it must be performed only when absolutely necessary and as late as possible
 * in a processing chain...
 *
 * Revision 1.18  2004/04/13 16:56:58  peiyongz
 * IdentityConstraintHandler
 *
 * Revision 1.17  2004/04/07 14:14:08  peiyongz
 * make resolveSystemId virutal
 *
 * Revision 1.16  2004/01/29 11:46:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.15  2003/11/28 19:54:31  knoaman
 * PSVIElement update
 *
 * Revision 1.14  2003/11/27 22:52:37  knoaman
 * PSVIElement implementation
 *
 * Revision 1.13  2003/11/27 06:10:31  neilg
 * PSVIAttribute implementation
 *
 * Revision 1.12  2003/11/26 16:20:00  knoaman
 * Store XSModel.
 *
 * Revision 1.11  2003/11/24 05:09:38  neilg
 * implement new, statless, method for detecting duplicate attributes
 *
 * Revision 1.10  2003/10/22 20:22:30  knoaman
 * Prepare for annotation support.
 *
 * Revision 1.9  2003/09/22 19:51:41  neilg
 * scanners should maintain their own pools of undeclared elements, rather than requiring grammars to do this.  This makes grammar objects stateless with regard to validation.
 *
 * Revision 1.8  2003/07/10 19:47:23  peiyongz
 * Stateless Grammar: Initialize scanner with grammarResolver,
 *                                creating grammar through grammarPool
 *
 * Revision 1.7  2003/05/16 21:36:57  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.6  2003/05/15 18:26:29  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2003/03/07 18:08:58  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.4  2003/01/29 19:59:35  gareth
 * we now detect when elements and attributes are validated/ the result of the validation and store that information.
 *
 * Revision 1.3  2003/01/15 15:49:49  knoaman
 * Change constant declaration name to match its value.
 *
 * Revision 1.2  2003/01/13 16:30:18  knoaman
 * [Bug 14469] Validator doesn't enforce xsd:key.
 *
 * Revision 1.1  2002/12/05 16:19:27  knoaman
 * Initial check-in.
 *
 */


#if !defined(SGXMLSCANNER_HPP)
#define SGXMLSCANNER_HPP

#include <xercesc/internal/XMLScanner.hpp>
#include <xercesc/util/KVStringPair.hpp>
#include <xercesc/util/ValueHashTableOf.hpp>
#include <xercesc/util/RefHash3KeysIdPool.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <xercesc/validators/schema/SchemaElementDecl.hpp>


XERCES_CPP_NAMESPACE_BEGIN

class SchemaGrammar;
class SchemaValidator;
class IdentityConstraintHandler;
class IdentityConstraint;
class ContentLeafNameTypeVector;
class SchemaAttDef;
class XMLContentModel;
class XSModel;
class PSVIAttributeList;
class PSVIElement;

//  This is a scanner class, which process XML Schema grammar.
class XMLPARSER_EXPORT SGXMLScanner : public XMLScanner
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    SGXMLScanner
    (
        XMLValidator* const       valToAdopt
        , GrammarResolver* const grammarResolver
        , MemoryManager* const    manager = XMLPlatformUtils::fgMemoryManager
    );
    SGXMLScanner
    (
        XMLDocumentHandler* const docHandler
        , DocTypeHandler* const   docTypeHandler
        , XMLEntityHandler* const entityHandler
        , XMLErrorReporter* const errReporter
        , XMLValidator* const     valToAdopt
        , GrammarResolver* const  grammarResolver
        , MemoryManager* const    manager = XMLPlatformUtils::fgMemoryManager
    );
    virtual ~SGXMLScanner();

    // -----------------------------------------------------------------------
    //  XMLScanner public virtual methods
    // -----------------------------------------------------------------------
    virtual const XMLCh* getName() const;
    virtual NameIdPool<DTDEntityDecl>* getEntityDeclPool();
    virtual const NameIdPool<DTDEntityDecl>* getEntityDeclPool() const;
    virtual unsigned int resolveQName
    (
        const   XMLCh* const        qName
        ,       XMLBuffer&          prefixBufToFill
        , const short               mode
        ,       int&                prefixColonPos
    );
    virtual void scanDocument
    (
        const   InputSource&    src
    );
    virtual bool scanNext(XMLPScanToken& toFill);
    virtual Grammar* loadGrammar
    (
        const   InputSource&    src
        , const short           grammarType
        , const bool            toCache = false
    );
    virtual Grammar::GrammarType getCurrentGrammarType() const;

protected:
    // -----------------------------------------------------------------------
    //  XMLScanner virtual methods
    // -----------------------------------------------------------------------
    virtual void scanReset(const InputSource& src);

    // -----------------------------------------------------------------------
    //  SGXMLScanner virtual methods
    // -----------------------------------------------------------------------
    virtual bool scanStartTag(bool& gotData);
    virtual void scanEndTag(bool& gotData);

    // -----------------------------------------------------------------------
    //  Helper methods
    // -----------------------------------------------------------------------
    unsigned int buildAttList
    (
        const   RefVectorOf<KVStringPair>&  providedAttrs
        , const unsigned int                attCount
        ,       XMLElementDecl*             elemDecl
        ,       RefVectorOf<XMLAttr>&       toFill
    );
    bool laxElementValidation(QName* element, ContentLeafNameTypeVector* cv,
                              const XMLContentModel* const cm,
                              const unsigned int parentElemDepth);
    unsigned int rawAttrScan
    (
        const   XMLCh* const                elemName
        ,       RefVectorOf<KVStringPair>&  toFill
        ,       bool&                       isEmpty
    );
    void updateNSMap
    (
        const   XMLCh* const    attrName
        , const XMLCh* const    attrValue
    );
    unsigned int resolvePrefix
    (
        const   XMLCh* const        prefix
        , const ElemStack::MapModes mode
    );
    void resizeElemState();

    void updateNSMap
    (
        const   XMLCh* const    attrName
        , const XMLCh* const    attrValue
        , const int             colonPosition
    );
    void resizeRawAttrColonList();
    unsigned int resolveQNameWithColon
    (
        const   XMLCh* const        qName
        ,       XMLBuffer&          prefixBufToFill
        , const short               mode
        , const int                 prefixColonPos
    );
    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fRawAttrList
    //      During the initial scan of the attributes we can only do a raw
    //      scan for key/value pairs. So this vector is used to store them
    //      until they can be processed (and put into fAttrList.)
    //
    //  fSchemaValidator
    //      The Schema validator instance.
    //
    //  fSeeXsi
    //      This flag indicates a schema has been seen.
    //
    //  fElemState
    //  fElemStateSize
    //      Stores an element next state from DFA content model - used for
    //      wildcard validation
    //
    // fElemNonDeclPool
    //      registry for elements without decls in the grammar
    // fElemCount
    //      count of the number of start tags seen so far (starts at 1).
    //      Used for duplicate attribute detection/processing of required/defaulted attributes
    // fAttDefRegistry
    //      mapping from XMLAttDef instances to the count of the last
    //      start tag where they were utilized.
    // fUndeclaredAttrRegistryNS
    //      mapping of namespaceId/localName pairs to the count of the last
    //      start tag in which they occurred.
    //  fPSVIAttrList
    //      PSVI attribute list implementation that needs to be
    //      filled when a PSVIHandler is registered
    //
    // -----------------------------------------------------------------------
    bool                                    fSeeXsi;
    Grammar::GrammarType                    fGrammarType;
    unsigned int                            fElemStateSize;
    unsigned int*                           fElemState;
    XMLBuffer                               fContent;
    ValueHashTableOf<XMLCh>*                fEntityTable;
    RefVectorOf<KVStringPair>*              fRawAttrList;
    unsigned int                            fRawAttrColonListSize;
    int*                                    fRawAttrColonList;
    SchemaGrammar*                          fSchemaGrammar;
    SchemaValidator*                        fSchemaValidator;
    IdentityConstraintHandler*              fICHandler;
    RefHash3KeysIdPool<SchemaElementDecl>*  fElemNonDeclPool;
    unsigned int                            fElemCount;
    RefHashTableOf<unsigned int>*           fAttDefRegistry;
    RefHash2KeysTableOf<unsigned int>*      fUndeclaredAttrRegistryNS;
    PSVIAttributeList *                     fPSVIAttrList;
    XSModel*                                fModel;
    PSVIElement*                            fPSVIElement;
    ValueStackOf<bool>*                     fErrorStack;
    PSVIElemContext                         fPSVIElemContext;

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    SGXMLScanner();
    SGXMLScanner(const SGXMLScanner&);
    SGXMLScanner& operator=(const SGXMLScanner&);

    // -----------------------------------------------------------------------
    //  XMLScanner virtual methods
    // -----------------------------------------------------------------------
    virtual void scanCDSection();
    virtual void scanCharData(XMLBuffer& toToUse);
    virtual EntityExpRes scanEntityRef
    (
        const   bool    inAttVal
        ,       XMLCh&  firstCh
        ,       XMLCh&  secondCh
        ,       bool&   escaped
    );
    virtual void scanDocTypeDecl();
    virtual void sendCharData(XMLBuffer& toSend);
    virtual InputSource* resolveSystemId(const XMLCh* const sysId
                                        ,const XMLCh* const pubId);

    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void commonInit();
    void cleanUp();

    bool normalizeAttValue
    (
        const   XMLAttDef* const    attDef
        , const XMLCh* const        attrName 
        , const XMLCh* const        value
        ,       XMLBuffer&          toFill
    );
    bool normalizeAttRawValue
    (
        const   XMLCh* const        attrName
        , const XMLCh* const        value
        ,       XMLBuffer&          toFill
    );
    unsigned int resolvePrefix
    (
        const   XMLCh* const        prefix
        ,       XMLBuffer&          uriBufToFill
        , const ElemStack::MapModes mode
    );
    void scanRawAttrListforNameSpaces(int attCount);
    void parseSchemaLocation(const XMLCh* const schemaLocationStr);
    void resolveSchemaGrammar(const XMLCh* const loc, const XMLCh* const uri);
    bool switchGrammar(const XMLCh* const newGrammarNameSpace);
    bool anyAttributeValidation(SchemaAttDef* attWildCard,
                                unsigned int uriId,
                                bool& skipThisOne,
                                bool& laxThisOne);

    // -----------------------------------------------------------------------
    //  Private scanning methods
    // -----------------------------------------------------------------------
    bool basicAttrValueScan
    (
        const   XMLCh* const    attrName
        ,       XMLBuffer&      toFill
    );
    bool scanAttValue
    (
        const   XMLAttDef* const    attDef
        ,       XMLBuffer&          toFill
    );
    bool scanContent();

    // -----------------------------------------------------------------------
    //  IdentityConstraints Activation methods
    // -----------------------------------------------------------------------
    inline bool toCheckIdentityConstraint()  const;

    // -----------------------------------------------------------------------
    //  Grammar preparsing methods
    // -----------------------------------------------------------------------
    Grammar* loadXMLSchemaGrammar(const InputSource& src, const bool toCache = false);

    // -----------------------------------------------------------------------
    //  PSVI handling methods
    // -----------------------------------------------------------------------
    void endElementPSVI(SchemaElementDecl* const elemDecl,
                        DatatypeValidator* const memberDV);
    void resetPSVIElemContext();
};

inline const XMLCh* SGXMLScanner::getName() const
{
    return XMLUni::fgSGXMLScanner;
}

inline bool SGXMLScanner::toCheckIdentityConstraint()  const
{
    return fValidate && fIdentityConstraintChecking && fICHandler;
}

inline Grammar::GrammarType SGXMLScanner::getCurrentGrammarType() const
{
    return fGrammarType;
}

XERCES_CPP_NAMESPACE_END

#endif
