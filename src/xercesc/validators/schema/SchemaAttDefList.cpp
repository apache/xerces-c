/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.10  2004/01/29 11:52:31  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.9  2003/12/17 00:18:40  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.8  2003/11/13 23:20:47  peiyongz
 * initSize
 *
 * Revision 1.7  2003/11/10 21:54:51  neilg
 * implementation for new stateless means of traversing attribute definition lists
 *
 * Revision 1.6  2003/10/20 11:46:28  gareth
 * Pass in memory manager to constructors and use for creation of enumerators.
 *
 * Revision 1.5  2003/10/17 21:17:12  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.4  2003/10/10 16:25:40  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.3  2003/05/18 14:02:07  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.2  2002/11/04 14:49:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:46  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/05/11 13:27:34  tng
 * Copyright update.
 *
 * Revision 1.1  2001/02/27 18:48:22  tng
 * Schema: Add SchemaAttDef, SchemaElementDecl, SchemaAttDefList.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/SchemaAttDefList.hpp>

#include <xercesc/internal/XTemplateSerializer.hpp>
#include <xercesc/util/ArrayIndexOutOfBoundsException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  SchemaAttDefList: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaAttDefList::SchemaAttDefList(RefHash2KeysTableOf<SchemaAttDef>* const listToUse, MemoryManager* const manager)
: XMLAttDefList(manager)
,fEnum(0)
,fList(listToUse)
,fArray(0)
,fSize(0)
,fCount(0)
{
    fEnum = new (getMemoryManager()) RefHash2KeysTableOfEnumerator<SchemaAttDef>(listToUse, false, getMemoryManager());
    fArray = (SchemaAttDef **)((getMemoryManager())->allocate( sizeof(SchemaAttDef*) << 1));
    fSize = 2;
}

SchemaAttDefList::~SchemaAttDefList()
{
    delete fEnum;
    (getMemoryManager())->deallocate(fArray);
}


// ---------------------------------------------------------------------------
//  SchemaAttDefList: Implementation of the virtual interface
// ---------------------------------------------------------------------------
bool SchemaAttDefList::hasMoreElements() const
{
    return fEnum->hasMoreElements();
}


bool SchemaAttDefList::isEmpty() const
{
    return fList->isEmpty();
}


XMLAttDef* SchemaAttDefList::findAttDef(const  unsigned long   uriID
                                    , const XMLCh* const    attName)
{
   QName tempAtt(attName, uriID, XMLPlatformUtils::fgMemoryManager);
   return fList->get((void*)tempAtt.getLocalPart(), uriID);
}


const XMLAttDef*
SchemaAttDefList::findAttDef(  const   unsigned long   uriID
                            , const XMLCh* const    attName) const
{
   QName tempAtt(attName, uriID, XMLPlatformUtils::fgMemoryManager);
   return fList->get((void*)tempAtt.getLocalPart(), uriID);
}


XMLAttDef* SchemaAttDefList::findAttDef(   const   XMLCh* const
                                        , const XMLCh* const)
{
   //need numeric URI id to locate the attribute, that's how it was stored
   ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Pool_InvalidId, getMemoryManager());
   return 0;
}


const XMLAttDef*
SchemaAttDefList::findAttDef( const   XMLCh* const
                            , const XMLCh* const) const
{
   //need numeric URI id to locate the attribute, that's how it was stored
   ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Pool_InvalidId, getMemoryManager());
   return 0;
}


XMLAttDef& SchemaAttDefList::nextElement()
{
    return fEnum->nextElement();
}


void SchemaAttDefList::Reset()
{
    fEnum->Reset();
}

/**
 * return total number of attributes in this list
 */
unsigned int SchemaAttDefList::getAttDefCount() const
{
    return fCount;
}

/**
 * return attribute at the index-th position in the list.
 */
XMLAttDef &SchemaAttDefList::getAttDef(unsigned int index) 
{
    if(index >= fCount)
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::AttrList_BadIndex, getMemoryManager());
    return *(fArray[index]);
}

/**
 * return attribute at the index-th position in the list.
 */
const XMLAttDef &SchemaAttDefList::getAttDef(unsigned int index) const 
{
    if(index >= fCount)
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::AttrList_BadIndex, getMemoryManager());
    return *(fArray[index]);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(SchemaAttDefList)

void SchemaAttDefList::serialize(XSerializeEngine& serEng)
{

    XMLAttDefList::serialize(serEng);

    if (serEng.isStoring())
    {
        /***
         *
         * Serialize RefHash2KeysTableOf<SchemaAttDef>
         *
         ***/
        XTemplateSerializer::storeObject(fList, serEng);
        serEng << fCount;

        // do not serialize fEnum
    }
    else
    {
        /***
         *
         * Deserialize RefHash2KeysTableOf<SchemaAttDef>           
         *
         ***/
        XTemplateSerializer::loadObject(&fList, 29, true, serEng);

        // assume empty so we can size fArray just right
        serEng >> fSize;
        if (!fEnum && fList)
        {
            fEnum = new (getMemoryManager()) RefHash2KeysTableOfEnumerator<SchemaAttDef>(fList, false, getMemoryManager());
        }
        if(fSize) 
        {
            (getMemoryManager())->deallocate(fArray);
            fArray = (SchemaAttDef **)((getMemoryManager())->allocate( sizeof(SchemaAttDef*) * fSize));
            fCount = 0;
            while(fEnum->hasMoreElements())
            {
                fArray[fCount++] = &fEnum->nextElement();
            }
        }
    }

}

SchemaAttDefList::SchemaAttDefList(MemoryManager* const manager)
: XMLAttDefList(manager)
,fEnum(0)
,fList(0)
,fArray(0)
,fSize(0)
,fCount(0)
{
}

XERCES_CPP_NAMESPACE_END

