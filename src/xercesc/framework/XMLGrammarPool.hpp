/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights
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
 *    permission, please contact apache\@apache.org.
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
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.5  2003/10/10 18:36:03  neilg
 * update XMLGrammarPool interface to make expected behaviour of locked pools better specified, and to add the capability to generate XSModels
 *
 * Revision 1.4  2003/09/16 18:30:54  neilg
 * make Grammar pool be responsible for creating and owning URI string pools.  This is one more step towards having grammars be independent of the parsers involved in their creation
 *
 * Revision 1.3  2003/09/02 08:59:02  gareth
 * Added API to get enumerator of grammars.
 *
 * Revision 1.2  2003/07/31 17:02:42  peiyongz
 * Grammar embed GrammarDescription
 *
 * Revision 1.1  2003/06/20 18:37:39  peiyongz
 * Stateless Grammar Pool :: Part I
 *
 * $Id$
 *
 */

#if !defined(XMLGRAMMARPOOL_HPP)
#define XMLGRAMMARPOOL_HPP

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/RefHashTableOf.hpp>
#include <xercesc/util/XMemory.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>


XERCES_CPP_NAMESPACE_BEGIN

class Grammar;
class XMLGrammarDescription;
class DTDGrammar;
class SchemaGrammar;
class XMLDTDDescription;
class XMLSchemaDescription;
class XMLStringPool;

class XMLPARSER_EXPORT XMLGrammarPool : public XMemory
{
public :
    // -----------------------------------------------------------------------
    /** @name Virtual destructor for derived classes */
    // -----------------------------------------------------------------------
    //@{

    /**
      * virtual destructor
      *
      */
    virtual ~XMLGrammarPool(){};
    //@}

    // -----------------------------------------------------------------------
    /** @name The Grammar Pool Interface */
    // -----------------------------------------------------------------------
    //@{

    /**
      * cacheGrammar
      *
      * @param gramToCache: the Grammar to be cached in the grammar pool
	  *
      */
    virtual void           cacheGrammar(Grammar* const               gramToCache) = 0;
    
    /**
      * retrieveGrammar
      *
      * @param gramDesc: the Grammar Description used to search for grammar
	  *                  cached in the grammar pool
	  *
      */
    virtual Grammar*       retrieveGrammar(XMLGrammarDescription* const gramDesc) = 0;
    
        
    /**
      * orphanGrammar
      *
	  * grammar removed from the grammar pool and owned by the caller
      *
      * @param nameSpaceKey: Key sed to search for grammar in the grammar pool
	  *
      */
    virtual Grammar*       orphanGrammar(const XMLCh* const nameSpaceKey) = 0;  


    /**
     * Get an enumeration of the cached Grammars in the Grammar pool
     *
     * @return enumeration of the cached Grammars in Grammar pool
     */
    virtual RefHashTableOfEnumerator<Grammar> getGrammarEnumerator() const = 0;

    /**
      * clear
      *
	  * all grammars are removed from the grammar pool and deleted.
      *
      */
    virtual void           clear() = 0;
        
    /**
      * lockPool
      *
	  * When this method is called by the application, the 
      * grammar pool should stop adding new grammars to the cache.
      * This should result in the grammar pool being sharable
      * among parsers operating in different threads.
      *
      */
    virtual void           lockPool() = 0;
    
    /**
      * unlockPool
      *
	  * After this method has been called, the grammar pool implementation
      * should return to its default behaviour when cacheGrammars(...) is called.
      *
      */
    virtual void           unlockPool() = 0;
    //@}

    // -----------------------------------------------------------------------
    /** @name  Factory interface */
    // -----------------------------------------------------------------------
    //@{

    /**
      * createDTDGrammar
      *
      */
    virtual DTDGrammar*            createDTDGrammar() = 0;

    /**
      * createSchemaGrammar
      *
      */
    virtual SchemaGrammar*         createSchemaGrammar() = 0;
                    
    /**
      * createDTDDescription
      *
      */	
    virtual XMLDTDDescription*     createDTDDescription(const XMLCh* const rootName) = 0;
    /**
      * createSchemaDescription
      *
      */		
    virtual XMLSchemaDescription*  createSchemaDescription(const XMLCh* const targetNamespace) = 0;

    //@}

    // -----------------------------------------------------------------------
    /** @name  schema component model support
    // -----------------------------------------------------------------------                                                        
    //@{

    /***
      * If the grammar pool has been locked, this method returns 
      * an XSModel corresponding to the schema components represented
      * by the objects stored in the pool.  If the pool has not been 
      * locked, this must return null.  If the pool is unlocked at
      * any point, the underlying XSModel will be destroyed;
      * applications must take care that, if they wish to unlock
      * a pool, no further access is made to the XSModel
      * it produced.  The pool's XSModel will not be serialized,
      * but, if a locked pool is deserialized, its XSModel
      * will be recreated.
      */
    virtual XSModel *getXSModel() const = 0;
	
    // @}

    // -----------------------------------------------------------------------
    /** @name  Getter */
    // -----------------------------------------------------------------------                                                        
    //@{

    /**
      * getMemoryManager
      *
      */    
	inline MemoryManager* const    getMemoryManager()
    {
        return fMemMgr;
    }

    /**
      * Return an XMLStringPool for use by validation routines.  
      * Implementations should not create a string pool on each call to this
      * method, but should maintain one string pool for all grammars
      * for which this pool is responsible.
      */
    virtual XMLStringPool *getURIStringPool() = 0;
    //@}
	
protected :
    // -----------------------------------------------------------------------
    /**  Hidden Constructors */
    // -----------------------------------------------------------------------
    //@{
    XMLGrammarPool(MemoryManager* const memMgr = XMLPlatformUtils::fgMemoryManager)
    :fMemMgr(memMgr)
    {
    };
    //@}

private :
    // -----------------------------------------------------------------------
    /** name  Unimplemented copy constructor and operator= */
    // -----------------------------------------------------------------------
    //@{
    XMLGrammarPool(const XMLGrammarPool& );
    XMLGrammarPool& operator=(const XMLGrammarPool& );
    //@}

    // -----------------------------------------------------------------------
    //
    // fMemMgr: plugged-in (or defaulted-in) memory manager
    //          not owned 
    //          no reset after initialization
    //
    // -----------------------------------------------------------------------
    
    MemoryManager* const  fMemMgr;     
};

XERCES_CPP_NAMESPACE_END

#endif
