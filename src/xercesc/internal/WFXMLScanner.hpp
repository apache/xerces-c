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
 * Revision 1.12  2004/12/03 19:40:30  cargilld
 * Change call to resolveEntity to pass in public id so that only one call to resolveEntity is needed (a follow-on to Alberto's fix).
 *
 * Revision 1.11  2004/09/08 13:56:13  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.10  2004/04/07 14:14:08  peiyongz
 * make resolveSystemId virutal
 *
 * Revision 1.9  2004/01/29 11:46:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.8  2003/10/22 20:22:30  knoaman
 * Prepare for annotation support.
 *
 * Revision 1.7  2003/07/10 19:47:24  peiyongz
 * Stateless Grammar: Initialize scanner with grammarResolver,
 *                                creating grammar through grammarPool
 *
 * Revision 1.6  2003/05/16 21:36:57  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
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
 * Revision 1.2  2003/01/02 16:29:05  knoaman
 * Modified the way we handle element tags.
 *
 * Revision 1.1  2002/12/04 02:01:29  knoaman
 * Initial checkin.
 *
 */


#if !defined(WFXMLSCANNER_HPP)
#define WFXMLSCANNER_HPP

#include <xercesc/internal/XMLScanner.hpp>
#include <xercesc/util/ValueHashTableOf.hpp>
#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/validators/DTD/DTDElementDecl.hpp>

XERCES_CPP_NAMESPACE_BEGIN


//  This is a a non-validating scanner. No DOCTYPE or XML Schema processing
//  will take place.
class XMLPARSER_EXPORT WFXMLScanner : public XMLScanner
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    WFXMLScanner
    (
        XMLValidator* const       valToAdopt
        , GrammarResolver* const  grammarResolver
        , MemoryManager* const    manager = XMLPlatformUtils::fgMemoryManager
    );
    WFXMLScanner
    (
        XMLDocumentHandler* const docHandler
        , DocTypeHandler* const   docTypeHandler
        , XMLEntityHandler* const entityHandler
        , XMLErrorReporter* const errReporter
        , XMLValidator* const     valToAdopt
        , GrammarResolver* const  grammarResolver
        , MemoryManager* const    manager = XMLPlatformUtils::fgMemoryManager
    );
    virtual ~WFXMLScanner();

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

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    WFXMLScanner();
    WFXMLScanner(const WFXMLScanner&);
    WFXMLScanner& operator=(const WFXMLScanner&);

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
    unsigned int resolvePrefix
    (
        const   XMLCh* const        prefix
        , const ElemStack::MapModes mode
    );

    // -----------------------------------------------------------------------
    //  Private scanning methods
    // -----------------------------------------------------------------------
    bool scanAttValue
    (
        const   XMLCh* const    attrName
        ,       XMLBuffer&      toFill
    );
    bool scanContent();
    void scanEndTag(bool& gotData);
    bool scanStartTag(bool& gotData);
    bool scanStartTagNS(bool& gotData);

    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fEntityTable
    //      This the table that contains the default entity entries.
    //
    //  fAttrNameHashList
    //      This contains the hash value for attribute names. It's used when
    //      checking for duplicate attributes.
    //
    //  fAttrNSList
    //      This contains XMLAttr objects that we need to map their prefixes
    //      to URIs when namespace is enabled.
    //
    // -----------------------------------------------------------------------
    unsigned int                       fElementIndex;
    RefVectorOf<XMLElementDecl>*       fElements;
    ValueHashTableOf<XMLCh>*           fEntityTable;
    ValueVectorOf<unsigned int>*       fAttrNameHashList;
    ValueVectorOf<XMLAttr*>*           fAttrNSList;
    RefHashTableOf<XMLElementDecl>*    fElementLookup;  
};

inline const XMLCh* WFXMLScanner::getName() const
{
    return XMLUni::fgWFXMLScanner;
}


XERCES_CPP_NAMESPACE_END

#endif
