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
 * Revision 1.5  2000/03/28 19:43:25  roddey
 * Fixes for signed/unsigned warnings. New work for two way transcoding
 * stuff.
 *
 * Revision 1.4  2000/03/02 19:55:37  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:16:48  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/09 21:42:36  abagchi
 * Copyright swat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:05  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:36  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(CMNODE_HPP)
#define CMNODE_HPP

#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/common/CMStateSet.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class CMNode : public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructors
    // -----------------------------------------------------------------------
    CMNode
    (
        const ContentSpecNode::NodeTypes type
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    virtual ~CMNode();


    // -----------------------------------------------------------------------
    //  Virtual methods to be provided derived node classes
    // -----------------------------------------------------------------------
    virtual bool isNullable() const = 0;


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    ContentSpecNode::NodeTypes getType() const;
    const CMStateSet& getFirstPos() const;
    const CMStateSet& getLastPos() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setMaxStates(const unsigned int maxStates);


protected :
    // -----------------------------------------------------------------------
    //  Protected, abstract methods
    // -----------------------------------------------------------------------
    virtual void calcFirstPos(CMStateSet& toUpdate) const = 0;
    virtual void calcLastPos(CMStateSet& toUpdate) const = 0;

    // -----------------------------------------------------------------------
    //  Protected data members
    //
    //  fMemoryManager
    //      Pluggable memory manager for dynamic allocation/deallocation.
    // -----------------------------------------------------------------------
    MemoryManager*             fMemoryManager;


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    CMNode();
    CMNode(const CMNode&);
    CMNode& operator=(const CMNode&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fType
    //      The type of node. This indicates whether its a leaf or an
    //      operation.
    //
    //  fFirstPos
    //      The set of NFA states that represent the entry states of this
    //      node in the DFA.
    //
    //  fLastPos
    //      The set of NFA states that represent the final states of this
    //      node in the DFA.
    //
    //  fMaxStates
    //      The maximum number of states that the NFA has, which means the
    //      max number of NFA states that have to be traced in the state
    //      sets during the building of the DFA. Its unfortunate that it
    //      has to be stored redundantly, but we need to fault in the
    //      state set members and they have to be sized to this size.
    // -----------------------------------------------------------------------
    ContentSpecNode::NodeTypes fType;
    CMStateSet*                fFirstPos;
    CMStateSet*                fLastPos;
    unsigned int               fMaxStates;
};



// ---------------------------------------------------------------------------
//  CMNode: Constructors and Destructors
// ---------------------------------------------------------------------------
inline CMNode::CMNode(const ContentSpecNode::NodeTypes type,
                      MemoryManager* const manager) :

    fMemoryManager(manager)
    , fType(type)
    , fFirstPos(0)
    , fLastPos(0)
    , fMaxStates(~0)
{
}

inline CMNode::~CMNode()
{
    // Clean up any position sets that got created
    delete fFirstPos;
    delete fLastPos;
}


// ---------------------------------------------------------------------------
//  CMNode: Getter methods
// ---------------------------------------------------------------------------
inline ContentSpecNode::NodeTypes CMNode::getType() const
{
    return fType;
}

inline const CMStateSet& CMNode::getFirstPos() const
{
    //
    //  Fault in the state set if needed. Since we can't use mutable members
    //  cast off the const'ness.
    //
    if (!fFirstPos)
    {
        CMNode* unconstThis = (CMNode*)this;
        unconstThis->fFirstPos = new (fMemoryManager) CMStateSet(fMaxStates, fMemoryManager);
        unconstThis->calcFirstPos(*fFirstPos);
    }
    return *fFirstPos;
}

inline const CMStateSet& CMNode::getLastPos() const
{
    //
    //  Fault in the state set if needed. Since we can't use mutable members
    //  cast off the const'ness.
    //
    if (!fLastPos)
    {
        CMNode* unconstThis = (CMNode*)this;
        unconstThis->fLastPos = new (fMemoryManager) CMStateSet(fMaxStates, fMemoryManager);
        unconstThis->calcLastPos(*fLastPos);
    }
    return *fLastPos;
}


// ---------------------------------------------------------------------------
//  CMNode: Setter methods
// ---------------------------------------------------------------------------
inline void CMNode::setMaxStates(const unsigned int maxStates)
{
    fMaxStates = maxStates;
}

XERCES_CPP_NAMESPACE_END

#endif
