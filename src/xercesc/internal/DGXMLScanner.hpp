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
 * Revision 1.16  2004/12/14 16:16:36  cargilld
 * Fix for xercesc-684: Add accessor to XMLScanner to get the current grammar type.
 *
 * Revision 1.15  2004/12/03 19:40:30  cargilld
 * Change call to resolveEntity to pass in public id so that only one call to resolveEntity is needed (a follow-on to Alberto's fix).
 *
 * Revision 1.14  2004/09/08 13:56:13  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.13  2004/04/07 14:14:08  peiyongz
 * make resolveSystemId virutal
 *
 * Revision 1.12  2004/01/29 11:46:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.11  2003/11/24 05:09:39  neilg
 * implement new, statless, method for detecting duplicate attributes
 *
 * Revision 1.10  2003/10/22 20:22:30  knoaman
 * Prepare for annotation support.
 *
 * Revision 1.9  2003/09/22 19:51:41  neilg
 * scanners should maintain their own pools of undeclared elements, rather than requiring grammars to do this.  This makes grammar objects stateless with regard to validation.
 *
 * Revision 1.8  2003/07/24 09:19:09  gareth
 * Patch for bug  #20530 - Attributes which have the same expanded name are not considered duplicates. Patch by cargilld.
 *
 * Revision 1.7  2003/07/10 19:47:23  peiyongz
 * Stateless Grammar: Initialize scanner with grammarResolver,
 *                                creating grammar through grammarPool
 *
 * Revision 1.6  2003/05/22 02:10:51  knoaman
 * Default the memory manager.
 *
 * Revision 1.5  2003/05/15 18:26:29  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2003/03/07 18:08:58  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.3  2003/01/15 15:49:49  knoaman
 * Change constant declaration name to match its value.
 *
 * Revision 1.2  2003/01/13 18:29:41  knoaman
 * Return proper scanner name.
 *
 * Revision 1.1  2002/12/09 15:45:13  knoaman
 * Initial check-in.
 *
 */


#if !defined(DGXMLSCANNER_HPP)
#define DGXMLSCANNER_HPP

#include <xercesc/internal/XMLScanner.hpp>
#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/util/NameIdPool.hpp>
#include <xercesc/validators/common/Grammar.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class DTDElementDecl;
class DTDGrammar;
class DTDValidator;

//  This is an integrated scanner class, which does DTD/XML Schema grammar
//  processing.
class XMLPARSER_EXPORT DGXMLScanner : public XMLScanner
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    DGXMLScanner
    (
          XMLValidator* const  valToAdopt
        , GrammarResolver* const grammarResolver
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    DGXMLScanner
    (
          XMLDocumentHandler* const docHandler
        , DocTypeHandler* const     docTypeHandler
        , XMLEntityHandler* const   entityHandler
        , XMLErrorReporter* const   errReporter
        , XMLValidator* const       valToAdopt
        , GrammarResolver* const    grammarResolver
        , MemoryManager* const      manager = XMLPlatformUtils::fgMemoryManager
    );
    virtual ~DGXMLScanner();

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

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DGXMLScanner();
    DGXMLScanner(const DGXMLScanner&);
    DGXMLScanner& operator=(const DGXMLScanner&);

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
    virtual void scanReset(const InputSource& src);
    virtual void sendCharData(XMLBuffer& toSend);
    virtual InputSource* resolveSystemId(const XMLCh* const sysId
                                        ,const XMLCh* const pubId); 

    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void commonInit();
    void cleanUp();

    unsigned int buildAttList
    (
        const unsigned int                attCount
        ,       XMLElementDecl*             elemDecl
        ,       RefVectorOf<XMLAttr>&       toFill
    );
    unsigned int resolvePrefix
    (
        const   XMLCh* const        prefix
        , const ElemStack::MapModes mode
    );
    void updateNSMap
    (
        const   XMLCh* const attrPrefix
        , const XMLCh* const attrLocalName
        , const XMLCh* const attrValue
    );
    void scanAttrListforNameSpaces(RefVectorOf<XMLAttr>* theAttrList, int attCount, XMLElementDecl* elemDecl);

    // -----------------------------------------------------------------------
    //  Private scanning methods
    // -----------------------------------------------------------------------
    bool scanAttValue
    (
        const   XMLAttDef* const    attDef
        , const XMLCh *const        attrName
        ,       XMLBuffer&          toFill
    );
    bool scanContent();
    void scanEndTag(bool& gotData);
    bool scanStartTag(bool& gotData);
    bool scanStartTagNS(bool& gotData);

    // -----------------------------------------------------------------------
    //  Grammar preparsing methods
    // -----------------------------------------------------------------------
    Grammar* loadDTDGrammar(const InputSource& src, const bool toCache = false);

    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fRawAttrList
    //      During the initial scan of the attributes we can only do a raw
    //      scan for key/value pairs. So this vector is used to store them
    //      until they can be processed (and put into fAttrList.)
    //
    //  fDTDValidator
    //      The DTD validator instance.
    //
    //  fElemState
    //  fElemStateSize
    //      Stores an element next state from DFA content model - used for
    //      wildcard validation
    //
    // fDTDElemNonDeclPool
    //     registry of "faulted-in" DTD element decls
    // fElemCount
    //      count of the number of start tags seen so far (starts at 1).
    //      Used for duplicate attribute detection/processing of required/defaulted attributes
    // fAttDefRegistry
    //      mapping from XMLAttDef instances to the count of the last
    //      start tag where they were utilized.
    // fUndeclaredAttrRegistry
    //      mapping of attr QNames to the count of the last start tag in which they occurred
    //
    // -----------------------------------------------------------------------
    ValueVectorOf<XMLAttr*>*    fAttrNSList;
    DTDValidator*               fDTDValidator;
    DTDGrammar*                 fDTDGrammar;
    NameIdPool<DTDElementDecl>* fDTDElemNonDeclPool;
    unsigned int                fElemCount;
    RefHashTableOf<unsigned int>* fAttDefRegistry;
    RefHashTableOf<unsigned int>* fUndeclaredAttrRegistry;
};

inline const XMLCh* DGXMLScanner::getName() const
{
    return XMLUni::fgDGXMLScanner;
}

inline Grammar::GrammarType DGXMLScanner::getCurrentGrammarType() const
{
    return Grammar::DTDGrammarType;
}

XERCES_CPP_NAMESPACE_END

#endif
