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
 * Revision 1.4  2001/05/11 13:27:17  tng
 * Copyright update.
 *
 * Revision 1.3  2001/02/27 14:48:47  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 * Revision 1.2  2001/02/16 14:58:57  tng
 * Schema: Update Makefile, configure files, project files, and include path in
 * certain cpp files because of the move of the common Content Model files.  By Pei Yong Zhang.
 *
 * Revision 1.1  2001/02/16 14:17:29  tng
 * Schema: Move the common Content Model files that are shared by DTD
 * and schema from 'DTD' folder to 'common' folder.  By Pei Yong Zhang.
 *
 * Revision 1.3  2000/03/02 19:55:37  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/09 21:42:37  abagchi
 * Copyright swatswat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:08  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:37  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/validators/common/CMStateSet.hpp>
#include <xercesc/validators/common/CMUnaryOp.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  CMUnaryOp: Constructors and Destructor
// ---------------------------------------------------------------------------
CMUnaryOp::CMUnaryOp( const ContentSpecNode::NodeTypes type
                    ,       CMNode* const              nodeToAdopt
                    ,       MemoryManager* const       manager) :
    CMNode(type, manager)
    , fChild(nodeToAdopt)
{
    // Insure that its one of the types we require
    if ((type != ContentSpecNode::ZeroOrOne)
    &&  (type != ContentSpecNode::ZeroOrMore)
    &&  (type != ContentSpecNode::OneOrMore))
    {
        ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::CM_UnaryOpHadBinType, manager);
    }
}

CMUnaryOp::~CMUnaryOp()
{
    delete fChild;
}


// ---------------------------------------------------------------------------
//  CMUnaryOp: Getter methods
// ---------------------------------------------------------------------------
const CMNode* CMUnaryOp::getChild() const
{
    return fChild;
}

CMNode* CMUnaryOp::getChild()
{
    return fChild;
}


// ---------------------------------------------------------------------------
//  CMUnaryOp: Implementation of the public CMNode virtual interface
// ---------------------------------------------------------------------------
bool CMUnaryOp::isNullable() const
{
    if (getType() == ContentSpecNode::OneOrMore)
        return fChild->isNullable();
    else
        return true;
}


// ---------------------------------------------------------------------------
//  CMUnaryOp: Implementation of the protected CMNode virtual interface
// ---------------------------------------------------------------------------
void CMUnaryOp::calcFirstPos(CMStateSet& toSet) const
{
    // Its just based on our child node's first pos
    toSet = fChild->getFirstPos();
}

void CMUnaryOp::calcLastPos(CMStateSet& toSet) const
{
    // Its just based on our child node's last pos
    toSet = fChild->getLastPos();
}

XERCES_CPP_NAMESPACE_END
