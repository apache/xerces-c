/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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
 * Revision 1.10  2004/09/08 13:56:50  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2004/01/29 11:52:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.8  2003/12/17 00:18:40  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.7  2003/11/13 23:19:49  peiyongz
 * initSize
 *
 * Revision 1.6  2003/11/10 21:54:51  neilg
 * implementation for new stateless means of traversing attribute definition lists
 *
 * Revision 1.5  2003/10/20 11:46:28  gareth
 * Pass in memory manager to constructors and use for creation of enumerators.
 *
 * Revision 1.4  2003/10/17 21:14:30  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.3  2003/10/10 16:24:51  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.2  2002/11/04 14:50:40  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:43  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/03/02 19:55:39  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/09 21:42:37  abagchi
 * Copyright swatswat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:27  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:39  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/DTD/DTDAttDefList.hpp>
#include <xercesc/internal/XTemplateSerializer.hpp>
#include <xercesc/util/ArrayIndexOutOfBoundsException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  DTDAttDefList: Constructors and Destructor
// ---------------------------------------------------------------------------
DTDAttDefList::DTDAttDefList(RefHashTableOf<DTDAttDef>* const listToUse, MemoryManager* const manager)
: XMLAttDefList(manager)
,fEnum(0)
,fList(listToUse)
,fArray(0)
,fSize(0)
,fCount(0)
{
    fEnum = new (getMemoryManager()) RefHashTableOfEnumerator<DTDAttDef>(listToUse, false, manager);
    fArray = (DTDAttDef **)(manager->allocate( sizeof(DTDAttDef*) << 1));
    fSize = 2;
}

DTDAttDefList::~DTDAttDefList()
{
    delete fEnum;
    (getMemoryManager())->deallocate(fArray);
}


// ---------------------------------------------------------------------------
//  DTDAttDefList: Implementation of the virtual interface
// ---------------------------------------------------------------------------
bool DTDAttDefList::hasMoreElements() const
{
    return fEnum->hasMoreElements();
}


bool DTDAttDefList::isEmpty() const
{
    return fList->isEmpty();
}


XMLAttDef* DTDAttDefList::findAttDef(const  unsigned long
                                    , const XMLCh* const    attName)
{
    // We don't use the URI, so we just look up the name
    return fList->get(attName);
}


const XMLAttDef*
DTDAttDefList::findAttDef(  const   unsigned long
                            , const XMLCh* const    attName) const
{
    // We don't use the URI, so we just look up the name
    return fList->get(attName);
}


XMLAttDef* DTDAttDefList::findAttDef(   const   XMLCh* const
                                        , const XMLCh* const    attName)
{
    // We don't use the URI, so we just look up the name
    return fList->get(attName);
}


const XMLAttDef*
DTDAttDefList::findAttDef(  const   XMLCh* const
                            , const XMLCh* const    attName) const
{
    // We don't use the URI, so we just look up the name
    return fList->get(attName);
}


XMLAttDef& DTDAttDefList::nextElement()
{
    return fEnum->nextElement();
}


void DTDAttDefList::Reset()
{
    fEnum->Reset();
}

/**
 * return total number of attributes in this list
 */
unsigned int DTDAttDefList::getAttDefCount() const
{
    return fCount;
}

/**
 * return attribute at the index-th position in the list.
 */
XMLAttDef &DTDAttDefList::getAttDef(unsigned int index) 
{
    if(index >= fCount)
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::AttrList_BadIndex, getMemoryManager());
    return *(fArray[index]);
}

/**
 * return attribute at the index-th position in the list.
 */
const XMLAttDef &DTDAttDefList::getAttDef(unsigned int index) const 
{
    if(index >= fCount)
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::AttrList_BadIndex, getMemoryManager());
    return *(fArray[index]);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(DTDAttDefList)

void DTDAttDefList::serialize(XSerializeEngine& serEng)
{

    XMLAttDefList::serialize(serEng);

    if (serEng.isStoring())
    {
        /***
         *
         * Serialize RefHashTableOf<DTDAttDef>           
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
         * Deserialize RefHashTableOf<DTDAttDef>           
         *
         ***/
        XTemplateSerializer::loadObject(&fList, 29, true, serEng);
        // assume empty so we can size fArray just right
        serEng >> fSize;
        if (!fEnum && fList)
        {
             fEnum = new (getMemoryManager()) RefHashTableOfEnumerator<DTDAttDef>(fList, false, getMemoryManager());
        }
        if(fSize) 
        {
            (getMemoryManager())->deallocate(fArray);
            fArray = (DTDAttDef **)((getMemoryManager())->allocate( sizeof(DTDAttDef*) * fSize));
            fCount = 0;
            while(fEnum->hasMoreElements())
            {
                fArray[fCount++] = &fEnum->nextElement();
            }
        }
    }

}

	
DTDAttDefList::DTDAttDefList(MemoryManager* const manager)
: XMLAttDefList(manager)
,fEnum(0)
,fList(0)
,fArray(0)
,fSize(0)
,fCount(0)
{
}

XERCES_CPP_NAMESPACE_END

