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
 * Revision 1.1  2003/06/20 18:38:39  peiyongz
 * Stateless Grammar Pool :: Part I
 *
 * $Id$
 *
 */

#if !defined(XMLGrammarPoolImplIMPL_HPP)
#define XMLGrammarPoolImplIMPL_HPP

#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/util/RefHashTableOf.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class GrammarEntry;

class XMLUTIL_EXPORT XMLGrammarPoolImpl : public XMLGrammarPool
{
public :
    // -----------------------------------------------------------------------
    /** @name constructor and destructor */
    // -----------------------------------------------------------------------
    //@{

    XMLGrammarPoolImpl(MemoryManager* memMgr);

    ~XMLGrammarPoolImpl();
    //@}

    // -----------------------------------------------------------------------
    /** @name Implementation of Grammar Pool Interface */
    // -----------------------------------------------------------------------
    //@{
   
    /**
      * cacheGrammar
      *
      * @param gramDesc:    the grammar description
      * @param gramToCache: the Grammar to be cached in the grammar pool
	  *
      */
    virtual void           cacheGrammar(XMLGrammarDescription* const gramDesc 
                                      , Grammar* const               gramToCache);
    

    /**
      * retrieveGrammar
      *
      * @param gramDesc: the Grammar Description used to search for grammar
	  *                  cached in the grammar pool
	  *
      */
    virtual Grammar*       retrieveGrammar(XMLGrammarDescription* const gramDesc);
    
        
    /**
      * orphanGrammar
      *
	  * grammar removed from the grammar pool and owned by the caller
      *
      * @param gramDesc: the Grammar Description used to search for grammar
	  *                  cached in the grammar pool
	  *
      */
    virtual Grammar*       orphanGrammar(XMLGrammarDescription* const);  

    /**
      * clear
      *
	  * all grammars are removed from the grammar pool and deleted.
      *
      */
    virtual void           clear();
        
    /**
      * lookPool
      *
	  * The grammar pool is looked and accessed exclusively by the looking thread
      *
      */
    virtual void           lockPool();
    
    /**
      * lookPool
      *
	  * The grammar pool is accessible by multi threads
      *
      */
    virtual void           unlockPool();
    //@}

    // -----------------------------------------------------------------------
    /** @name  Implementation of Factory interface */
    // -----------------------------------------------------------------------
    //@{

    /**
      * createDTDGrammar
      *
      */
    virtual DTDGrammar*            createDTDGrammar();

    /**
      * createSchemaGrammar
      *
      */
    virtual SchemaGrammar*         createSchemaGrammar();
                    
    /**
      * createDTDDescription
      *
      */	
    virtual XMLDTDDescription*     createDTDDescription(const XMLCh* const rootName);
    /**
      * createSchemaDescription
      *
      */		
    virtual XMLSchemaDescription*  createSchemaDescription(const XMLCh* const targetNamespace);
    //@}
	
private:
    // -----------------------------------------------------------------------
    /** name  Unimplemented copy constructor and operator= */
    // -----------------------------------------------------------------------
    //@{
    XMLGrammarPoolImpl(const XMLGrammarPoolImpl& );
    XMLGrammarPoolImpl& operator=(const XMLGrammarPoolImpl& );
    //@}

    // -----------------------------------------------------------------------
    //
    // fGrammarRegistry: 
    //
	//    container
    //
    // -----------------------------------------------------------------------
    RefHashTableOf<GrammarEntry>* fGrammarRegistry; 

};

/**
 * Helper class, ought to be nested class
 */
class GrammarEntry : public XMemory
{
public:
    GrammarEntry(XMLGrammarDescription* const  gramDesc
               , Grammar* const                grammar);

    ~GrammarEntry();

    inline XMLGrammarDescription*      getDescription() const;

    inline Grammar*                    getGrammar() const;

    inline void                        nullGrammar();

    inline void                        nullDescription();

private:

    XMLGrammarDescription*     fDescription;
    Grammar*                   fGrammar;

};

inline XMLGrammarDescription* GrammarEntry::getDescription() const
{
    return fDescription;
}

inline Grammar* GrammarEntry::getGrammar() const
{
    return fGrammar;
}

inline void GrammarEntry::nullGrammar()
{
    fGrammar = 0;
}

inline void GrammarEntry::nullDescription()
{
    fDescription = 0;
}

XERCES_CPP_NAMESPACE_END

#endif
