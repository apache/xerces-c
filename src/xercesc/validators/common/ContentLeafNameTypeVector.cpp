/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * Revision 1.6  2005/01/07 15:28:49  amassari
 * Removed warnings
 *
 * Revision 1.5  2004/09/08 13:56:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.3  2003/05/15 18:48:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 14:54:58  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:38  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/05/11 13:27:17  tng
 * Copyright update.
 *
 * Revision 1.2  2001/04/19 18:17:28  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.1  2001/02/27 14:48:49  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/common/ContentLeafNameTypeVector.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  ContentLeafNameTypeVector: Constructors and Destructor
// ---------------------------------------------------------------------------
ContentLeafNameTypeVector::ContentLeafNameTypeVector
(
    MemoryManager* const manager
)
: fMemoryManager(manager)
, fLeafNames(0)
, fLeafTypes(0)
, fLeafCount(0)
{
}

ContentLeafNameTypeVector::ContentLeafNameTypeVector
(
      QName** const                     names
    , ContentSpecNode::NodeTypes* const types
    , const unsigned int                count
    , MemoryManager* const              manager
)
: fMemoryManager(manager)
, fLeafNames(0)
, fLeafTypes(0)
, fLeafCount(0)
{
    setValues(names, types, count);
}

/***
copy ctor
***/
ContentLeafNameTypeVector::ContentLeafNameTypeVector
(
    const ContentLeafNameTypeVector& toCopy
)
: XMemory(toCopy)
, fMemoryManager(toCopy.fMemoryManager)
, fLeafNames(0)
, fLeafTypes(0)
, fLeafCount(0)
{
    fLeafCount=toCopy.getLeafCount();
    init(fLeafCount);

    for (unsigned int i=0; i<this->fLeafCount; i++)
    {
        fLeafNames[i] = toCopy.getLeafNameAt(i);
        fLeafTypes[i] = toCopy.getLeafTypeAt(i);
    }
}

ContentLeafNameTypeVector::~ContentLeafNameTypeVector()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  ContentSpecType: Setter methods
// ---------------------------------------------------------------------------
void ContentLeafNameTypeVector::setValues
    (
         QName** const                      names
       , ContentSpecNode::NodeTypes* const  types
       , const unsigned int                 count
    )
{
    cleanUp();
    init(count);

    for (unsigned int i=0; i<count; i++)
    {
        fLeafNames[i] = names[i];
        fLeafTypes[i] = types[i];
    }
}

// ---------------------------------------------------------------------------
//  ContentLeafNameTypeVector: Getter methods
// ---------------------------------------------------------------------------
QName* ContentLeafNameTypeVector::getLeafNameAt(const unsigned int pos) const
{
    if (pos >= fLeafCount)
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex, fMemoryManager);

    return fLeafNames[pos];
}

const ContentSpecNode::NodeTypes ContentLeafNameTypeVector::getLeafTypeAt
       (const unsigned int pos) const
{
    if (pos >= fLeafCount)
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex, fMemoryManager);

	return fLeafTypes[pos];
}

const unsigned int ContentLeafNameTypeVector::getLeafCount() const
{
	return fLeafCount;
}

XERCES_CPP_NAMESPACE_END
