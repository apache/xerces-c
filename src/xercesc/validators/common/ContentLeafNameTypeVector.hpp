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
 * Revision 1.7  2005/03/25 10:54:22  amassari
 * On Windows, export the class from the DLL
 *
 * Revision 1.6  2004/09/08 13:56:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/05/16 21:43:20  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.4  2003/05/15 18:48:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2003/03/07 18:16:57  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.2  2002/11/04 14:54:58  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:38  peiyongz
 * sane_include
 *
 * Revision 1.4  2001/08/07 15:21:20  knoaman
 * The value of 'fLeafCount' was not set.
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


#if !defined(CONTENTLEAFNAMETYPEVECTOR_HPP)
#define CONTENTLEAFNAMETYPEVECTOR_HPP

#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/framework/MemoryManager.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLPARSER_EXPORT ContentLeafNameTypeVector : public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Class specific types
    // -----------------------------------------------------------------------


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    ContentLeafNameTypeVector
    (
        MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    ContentLeafNameTypeVector
    (
        QName** const                     qName
      , ContentSpecNode::NodeTypes* const types
      , const unsigned int                count
      , MemoryManager* const              manager = XMLPlatformUtils::fgMemoryManager
    );

    ~ContentLeafNameTypeVector();

    ContentLeafNameTypeVector(const ContentLeafNameTypeVector&);

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    QName* getLeafNameAt(const unsigned int pos) const;

    const ContentSpecNode::NodeTypes getLeafTypeAt(const unsigned int pos) const;
    const unsigned int getLeafCount() const;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setValues
    (
        QName** const                      qName
      , ContentSpecNode::NodeTypes* const  types
      , const unsigned int                       count
    );

    // -----------------------------------------------------------------------
    //  Miscellaneous
    // -----------------------------------------------------------------------

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    ContentLeafNameTypeVector& operator=(const ContentLeafNameTypeVector&);

    // -----------------------------------------------------------------------
    //  helper methods
    // -----------------------------------------------------------------------
    void cleanUp();
    void init(const unsigned int);

    // -----------------------------------------------------------------------
    //  Private Data Members
    //
    // -----------------------------------------------------------------------
    MemoryManager*                fMemoryManager;
    QName**                       fLeafNames;
    ContentSpecNode::NodeTypes   *fLeafTypes;
    unsigned int                  fLeafCount;
};

inline void ContentLeafNameTypeVector::cleanUp()
{
	fMemoryManager->deallocate(fLeafNames); //delete [] fLeafNames;
	fMemoryManager->deallocate(fLeafTypes); //delete [] fLeafTypes;
}

inline void ContentLeafNameTypeVector::init(const unsigned int size)
{
    fLeafNames = (QName**) fMemoryManager->allocate(size * sizeof(QName*));//new QName*[size];
    fLeafTypes = (ContentSpecNode::NodeTypes *) fMemoryManager->allocate
    (
        size * sizeof(ContentSpecNode::NodeTypes)
    ); //new ContentSpecNode::NodeTypes [size];
    fLeafCount = size;
}

XERCES_CPP_NAMESPACE_END

#endif
