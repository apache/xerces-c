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
 * Revision 1.1.1.1  2002/02/01 22:22:38  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/05/11 13:27:17  tng
 * Copyright update.
 *
 * Revision 1.2  2001/02/16 14:58:57  tng
 * Schema: Update Makefile, configure files, project files, and include path in
 * certain cpp files because of the move of the common Content Model files.  By Pei Yong Zhang.
 *
 * Revision 1.1  2001/02/16 14:17:29  tng
 * Schema: Move the common Content Model files that are shared by DTD
 * and schema from 'DTD' folder to 'common' folder.  By Pei Yong Zhang.
 *
 * Revision 1.4  2000/03/02 19:55:37  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:16:48  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/09 21:42:37  abagchi
 * Copyright swat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:11  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:37  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(CMUNARYOP_HPP)
#define CMUNARYOP_HPP

#include <xercesc/validators/common/CMNode.hpp>


XERCES_CPP_NAMESPACE_BEGIN

class CMStateSet;

class CMUnaryOp : public CMNode
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    CMUnaryOp
    (
          const ContentSpecNode::NodeTypes type
        ,       CMNode* const              nodeToAdopt
        ,       MemoryManager* const       manager = XMLPlatformUtils::fgMemoryManager
    );
    ~CMUnaryOp();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const CMNode* getChild() const;
    CMNode* getChild();


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
    //  fChild
    //      This is the reference to the one child that we have for this
    //      unary operation. We own it.
    // -----------------------------------------------------------------------
    CMNode*     fChild;

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    CMUnaryOp(const CMUnaryOp&);
    CMUnaryOp& operator=(const CMUnaryOp&);
};

XERCES_CPP_NAMESPACE_END

#endif
