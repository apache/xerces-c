/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
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
 * $Id$
 */


#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include "SimpleHashPtr.hpp"
#include <xercesc/dom/DOMBuilder.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOMError.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/dom/deprecated/DOMParser.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include "SimpleValueHashTableOf.hpp"

XERCES_CPP_NAMESPACE_USE

/**
  * Configurable memory manager
  *
  * <p>This class is a memory manager implementation that keeps track of all 
  * allocations/deallocations to ensure that all memory that it allocated is 
  * deallocated.
  * </p>
  */

class MemoryMonitor : public MemoryManager
{
public:

    /** @name Constructor */
    //@{

    /**
      * Default constructor
      */
    MemoryMonitor()
    { 
        fHashType = new SimpleHashPtr();
        fHashTable = new SimpleValueHashTableOf<unsigned int>(1013, fHashType);
    }
    //@}


    /** @name Destructor */
    //@{

    /**
      * Default destructor
      */
    virtual ~MemoryMonitor()
    {
        delete fHashTable;
    }
    //@}

    /** @name The virtual methods in MemoryManager */
    //@{

    /**
      * This method allocates requested memory.
      *
      * @param size The requested memory size
      *
      * @return A pointer to the allocated memory
      */
    virtual void* allocate(size_t size);

    /**
      * This method deallocates memory
      *
      * @param p The pointer to the allocated memory to be deleted
      */
    virtual void deallocate(void* p);

    //@}
    
    // Print out amount of currently allocated memory
    unsigned int getTotalMemory();

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    MemoryMonitor(const MemoryMonitor &);
    MemoryMonitor& operator=(const MemoryMonitor &);
    SimpleValueHashTableOf<unsigned int>* fHashTable;
    SimpleHashPtr* fHashType;

};


