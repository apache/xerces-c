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
 * Revision 1.6  2004/09/08 13:56:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2004/01/29 11:51:21  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.4  2003/05/16 21:43:20  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.3  2003/05/15 18:48:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 14:54:58  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:37  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/05/11 13:27:14  tng
 * Copyright update.
 *
 * Revision 1.1  2001/02/27 14:48:46  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 */

#if !defined(CMANY_HPP)
#define CMANY_HPP

#include <xercesc/validators/common/CMNode.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class CMStateSet;

class CMAny : public CMNode
{
public :
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    CMAny
    (
        const   ContentSpecNode::NodeTypes type
        , const unsigned int               URI
        , const unsigned int               position
        ,       MemoryManager* const       manager = XMLPlatformUtils::fgMemoryManager
    );
    ~CMAny();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    unsigned int getURI() const;

    unsigned int getPosition() const;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setPosition(const unsigned int newPosition);

    // -----------------------------------------------------------------------
    //  Implementation of the public CMNode virtual interface
    // -----------------------------------------------------------------------
    bool isNullable() const;


protected :
    // -----------------------------------------------------------------------
    //  Implementation of the protected CMNode virtual interface
    // -----------------------------------------------------------------------
    void calcFirstPos(CMStateSet& toSet) const;
    void calcLastPos(CMStateSet& toSet) const;

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fURI;
    //  URI of the any content model. This value is set if the type is
    //  of the following:
    //  XMLContentSpec.CONTENTSPECNODE_ANY,
    //  XMLContentSpec.CONTENTSPECNODE_ANY_OTHER.
    //
	//  fPosition
    //  Part of the algorithm to convert a regex directly to a DFA
    //  numbers each leaf sequentially. If its -1, that means its an
    //  epsilon node. Zero and greater are non-epsilon positions.
    // -----------------------------------------------------------------------
    unsigned int fURI;
    unsigned int fPosition;

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    CMAny(const CMAny&);
    CMAny& operator=(const CMAny&);
};

XERCES_CPP_NAMESPACE_END

#endif
