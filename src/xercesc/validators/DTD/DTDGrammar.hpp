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
 * Revision 1.17  2004/10/28 20:18:26  peiyongz
 * Data member reshuffle
 *
 * Revision 1.16  2004/10/20 15:19:07  knoaman
 * Allow option of initializing static data in XMLPlatformUtils::Initialize
 *
 * Revision 1.15  2004/09/08 13:56:50  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.14  2004/01/29 11:52:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.13  2003/12/22 15:22:58  gareth
 * made getRootElemID const. Bug #25699
 *
 * Revision 1.12  2003/12/17 00:18:40  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.11  2003/10/14 15:20:42  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.10  2003/09/22 19:49:02  neilg
 * implement change to Grammar::putElem(XMLElementDecl, bool).  If Grammars are used only to hold declared objects, there will be no need for the fElemNonDeclPool tables; make Grammar implementations lazily create them only if the application requires them (which good cpplications should not.)
 *
 * Revision 1.9  2003/08/14 03:00:46  knoaman
 * Code refactoring to improve performance of validation.
 *
 * Revision 1.8  2003/07/31 17:09:59  peiyongz
 * Grammar embed grammar description
 *
 * Revision 1.7  2003/05/16 21:43:19  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.6  2003/05/15 18:54:50  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2002/11/04 14:50:40  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/08/22 15:05:39  tng
 * Remove unused parameter variables in inline functions.
 *
 * Revision 1.3  2002/07/11 18:19:28  knoaman
 * Grammar caching/preparsing - initial implementation.
 *
 * Revision 1.2  2002/07/05 17:08:10  tng
 * [Bug 10119] Grammar::getGrammarType need a const modifier
 *
 * Revision 1.1.1.1  2002/02/01 22:22:43  peiyongz
 * sane_include
 *
 * Revision 1.5  2001/09/14 14:50:22  tng
 * Schema: Fix some wildcard bugs, and some retrieving qualified/unqualified element decl problems.
 *
 * Revision 1.4  2001/05/28 20:56:17  tng
 * Schema: Move getTargetNamespace as virtual function in base class Grammar
 *
 * Revision 1.3  2001/05/11 13:27:09  tng
 * Copyright update.
 *
 * Revision 1.2  2001/04/19 18:17:21  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.1  2001/03/21 21:56:20  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 */



#if !defined(DTDGRAMMAR_HPP)
#define DTDGRAMMAR_HPP

#include <xercesc/util/RefHashTableOf.hpp>
#include <xercesc/util/NameIdPool.hpp>
#include <xercesc/util/StringPool.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <xercesc/validators/DTD/DTDElementDecl.hpp>
#include <xercesc/validators/DTD/DTDEntityDecl.hpp>
#include <xercesc/framework/XMLDTDDescription.hpp>


XERCES_CPP_NAMESPACE_BEGIN

//
// This class stores the DTD information
//  NOTE: DTDs are not namespace aware, so we just use regular NameIdPool
//  data structures to store element and attribute decls. They are all set
//  to be in the global namespace and the full QName is used as the base name
//  of the decl. This means that all the URI parameters below are expected
//  to be null pointers (and anything else will cause an exception.)
//

class VALIDATORS_EXPORT DTDGrammar : public Grammar
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    DTDGrammar(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    virtual ~DTDGrammar();

    // -----------------------------------------------------------------------
    //  Implementation of Virtual Interface
    // -----------------------------------------------------------------------
    virtual Grammar::GrammarType getGrammarType() const;
    virtual const XMLCh* getTargetNamespace() const;

    // this method should only be used while the grammar is being
    // constructed, not while it is being used
    // in a validation episode!
    virtual XMLElementDecl* findOrAddElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefixName
        , const XMLCh* const    qName
        , unsigned int          scope
        ,       bool&           wasAdded
    ) ;

    virtual unsigned int getElemId
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
        , unsigned int          scope
    )   const ;

    virtual const XMLElementDecl* getElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
        , unsigned int          scope
    )   const ;

    virtual XMLElementDecl* getElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    qName
        , unsigned int          scope
    );

    virtual const XMLElementDecl* getElemDecl
    (
        const   unsigned int    elemId
    )   const;

    virtual XMLElementDecl* getElemDecl
    (
        const   unsigned int    elemId
    );

    virtual const XMLNotationDecl* getNotationDecl
    (
        const   XMLCh* const    notName
    )   const;

    virtual XMLNotationDecl* getNotationDecl
    (
        const   XMLCh* const    notName
    );

    virtual bool getValidated() const;

    virtual XMLElementDecl* putElemDecl
    (
        const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefixName
        , const XMLCh* const    qName
        , unsigned int          scope
        , const bool            notDeclared = false
    );

    virtual unsigned int putElemDecl
    (
        XMLElementDecl* const elemDecl
        , const bool          notDeclared = false
    )   ;

    virtual unsigned int putNotationDecl
    (
        XMLNotationDecl* const notationDecl
    )   const;

    virtual void setValidated(const bool newState);

    virtual void reset();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    
    // deprecated.  returns the ID of the root element; not
    // useable in multithreaded environments!
    unsigned int getRootElemId() const;
    const DTDEntityDecl* getEntityDecl(const XMLCh* const entName) const;
    DTDEntityDecl* getEntityDecl(const XMLCh* const entName);
    NameIdPool<DTDEntityDecl>* getEntityDeclPool();
    const NameIdPool<DTDEntityDecl>* getEntityDeclPool() const;
    NameIdPoolEnumerator<DTDElementDecl> getElemEnumerator() const;
    NameIdPoolEnumerator<DTDEntityDecl> getEntityEnumerator() const;
    NameIdPoolEnumerator<XMLNotationDecl> getNotationEnumerator() const;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------

    // deprecated.  Not usable in multithreaded environments
    void setRootElemId(unsigned int rootElemId);

    virtual void                    setGrammarDescription( XMLGrammarDescription*);
    virtual XMLGrammarDescription*  getGrammarDescription() const;

    // -----------------------------------------------------------------------
    //  Content management methods
    // -----------------------------------------------------------------------
    unsigned int putEntityDecl(DTDEntityDecl* const entityDecl) const;


    // -----------------------------------------------------------------------
    //  Notification that lazy data has been deleted
    // -----------------------------------------------------------------------
    static void reinitDfltEntities();

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(DTDGrammar)

private:
    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void resetEntityDeclPool();

    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DTDGrammar(const DTDGrammar &);
    DTDGrammar& operator = (const  DTDGrammar&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fElemDeclPool
    //      This is the element decl pool. It contains all of the elements
    //      declared in the DTD (and their associated attributes.)
    //
    //  fElemNonDeclPool
    //      This is the element decl pool that is is populated as new elements
    //      are seen in the XML document (not declared in the DTD), and they
    //      are given default characteristics.
    //
    //  fEntityDeclPool
    //      This is a pool of EntityDecl objects, which contains all of the
    //      general entities that are declared in the DTD subsets, plus the
    //      default entities (such as &gt; &lt; ...) defined by the XML Standard.
    //
    //  fNotationDeclPool
    //      This is a pool of NotationDecl objects, which contains all of the
    //      notations declared in the DTD subsets.
    //
    //  fRootElemId
    //      The id of the root element that we found in the DOCTYPE statement.
    //      Its initialized to ContentModel::fgInvalidElemId, so that its
    //      invalid unless we have a DOCTYPE.
    //
    //  fValidated
    //      Indicates if the content of the Grammar has been pre-validated
    //      or not. When using a cached grammar, no need for pre content
    //      validation.
    //
    //  fGramDesc: adopted
    //
    // -----------------------------------------------------------------------
    static NameIdPool<DTDEntityDecl>* fDefaultEntities;
    MemoryManager*                    fMemoryManager;
    NameIdPool<DTDElementDecl>*       fElemDeclPool;
    NameIdPool<DTDElementDecl>*       fElemNonDeclPool;
    NameIdPool<DTDEntityDecl>*        fEntityDeclPool;
    NameIdPool<XMLNotationDecl>*      fNotationDeclPool;
    XMLDTDDescription*                fGramDesc;

    unsigned int                      fRootElemId;
    bool                              fValidated;

    friend class XMLInitializer;
};


// ---------------------------------------------------------------------------
//  DTDGrammar: Getter methods
// ---------------------------------------------------------------------------
inline unsigned int DTDGrammar::getRootElemId() const
{
    return fRootElemId;
}

// ---------------------------------------------------------------------------
//  DTDGrammar: Getter methods
// ---------------------------------------------------------------------------
inline NameIdPoolEnumerator<DTDElementDecl>
DTDGrammar::getElemEnumerator() const
{
    return NameIdPoolEnumerator<DTDElementDecl>(fElemDeclPool, fMemoryManager);
}

inline NameIdPoolEnumerator<DTDEntityDecl>
DTDGrammar::getEntityEnumerator() const
{
    return NameIdPoolEnumerator<DTDEntityDecl>(fEntityDeclPool, fMemoryManager);
}

inline NameIdPoolEnumerator<XMLNotationDecl>
DTDGrammar::getNotationEnumerator() const
{
    return NameIdPoolEnumerator<XMLNotationDecl>(fNotationDeclPool, fMemoryManager);
}

inline const DTDEntityDecl*
DTDGrammar::getEntityDecl(const XMLCh* const entName) const
{
    DTDEntityDecl* decl = fDefaultEntities->getByKey(entName);

    if (!decl)
        return fEntityDeclPool->getByKey(entName);

    return decl;
}

inline DTDEntityDecl* DTDGrammar::getEntityDecl(const XMLCh* const entName)
{
    DTDEntityDecl* decl = fDefaultEntities->getByKey(entName);

    if (!decl)
        return fEntityDeclPool->getByKey(entName);

    return decl;
}


inline NameIdPool<DTDEntityDecl>* DTDGrammar::getEntityDeclPool()
{
    return fEntityDeclPool;
}

inline const NameIdPool<DTDEntityDecl>* DTDGrammar::getEntityDeclPool() const
{
    return fEntityDeclPool;
}

// -----------------------------------------------------------------------
//  Setter methods
// -----------------------------------------------------------------------
inline void DTDGrammar::setRootElemId(unsigned int rootElemId) {
    fRootElemId = rootElemId;
}

inline unsigned int DTDGrammar::putEntityDecl(DTDEntityDecl* const entityDecl)   const
{
    return fEntityDeclPool->put(entityDecl);
}


// ---------------------------------------------------------------------------
//  DTDGrammar: Virtual methods
// ---------------------------------------------------------------------------
inline Grammar::GrammarType DTDGrammar::getGrammarType() const {
    return Grammar::DTDGrammarType;
}

inline const XMLCh* DTDGrammar::getTargetNamespace() const {
    return XMLUni::fgZeroLenString;
}

// Element Decl
inline unsigned int DTDGrammar::getElemId (const   unsigned int
                                              , const XMLCh* const
                                              , const XMLCh* const    qName
                                              , unsigned int) const
{
    //
    //  In this case, we don't return zero to mean 'not found', so we have to
    //  map it to the official not found value if we don't find it.
    //
    const DTDElementDecl* decl = fElemDeclPool->getByKey(qName);
    if (!decl)
        return XMLElementDecl::fgInvalidElemId;
    return decl->getId();
}

inline const XMLElementDecl* DTDGrammar::getElemDecl( const   unsigned int
                                              , const XMLCh* const
                                              , const XMLCh* const    qName
                                              , unsigned int)   const
{
    const XMLElementDecl* elemDecl = fElemDeclPool->getByKey(qName);

    if (!elemDecl && fElemNonDeclPool)
        elemDecl = fElemNonDeclPool->getByKey(qName);

    return elemDecl;
}

inline XMLElementDecl* DTDGrammar::getElemDecl (const   unsigned int
                                              , const XMLCh* const
                                              , const XMLCh* const    qName
                                              , unsigned int)
{
    XMLElementDecl* elemDecl = fElemDeclPool->getByKey(qName);

    if (!elemDecl && fElemNonDeclPool)
        elemDecl = fElemNonDeclPool->getByKey(qName);

    return elemDecl;
}

inline const XMLElementDecl* DTDGrammar::getElemDecl(const unsigned int elemId) const
{
    // Look up this element decl by id
    return fElemDeclPool->getById(elemId);
}

inline XMLElementDecl* DTDGrammar::getElemDecl(const unsigned int elemId)
{
    // Look up this element decl by id
    return fElemDeclPool->getById(elemId);
}

inline unsigned int
DTDGrammar::putElemDecl(XMLElementDecl* const elemDecl,
                        const bool notDeclared) 
{
    if (notDeclared)
    {
        if(!fElemNonDeclPool)
            fElemNonDeclPool = new (fMemoryManager) NameIdPool<DTDElementDecl>(29, 128, fMemoryManager);
        return fElemNonDeclPool->put((DTDElementDecl*) elemDecl);
    }

    return fElemDeclPool->put((DTDElementDecl*) elemDecl);
}

// Notation Decl
inline const XMLNotationDecl* DTDGrammar::getNotationDecl(const XMLCh* const notName) const
{
    return fNotationDeclPool->getByKey(notName);
}

inline XMLNotationDecl* DTDGrammar::getNotationDecl(const XMLCh* const notName)
{
    return fNotationDeclPool->getByKey(notName);
}

inline unsigned int DTDGrammar::putNotationDecl(XMLNotationDecl* const notationDecl)   const
{
    return fNotationDeclPool->put(notationDecl);
}

inline bool DTDGrammar::getValidated() const
{
    return fValidated;
}

inline void DTDGrammar::setValidated(const bool newState)
{
    fValidated = newState;
}

XERCES_CPP_NAMESPACE_END

#endif
