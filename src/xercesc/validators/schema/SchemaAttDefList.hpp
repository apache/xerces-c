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
 * Revision 1.10  2004/12/16 03:21:24  cargilld
 * Performance fix from David Bertoni.  Remove virtual so function gets inlined.
 *
 * Revision 1.9  2004/09/10 18:42:06  cargilld
 * Performance improvement fix to more efficiently findattdef.  Fix from Dave Bertoni.
 *
 * Revision 1.8  2004/09/08 13:56:56  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2004/01/29 11:52:31  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.6  2003/11/10 21:54:51  neilg
 * implementation for new stateless means of traversing attribute definition lists
 *
 * Revision 1.5  2003/10/20 11:46:28  gareth
 * Pass in memory manager to constructors and use for creation of enumerators.
 *
 * Revision 1.4  2003/10/10 16:25:40  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.3  2002/12/06 13:27:14  tng
 * [Bug 9083] Make some classes be exportable.
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


#if !defined(SCHEMAATTDEFLIST_HPP)
#define SCHEMAATTDEFLIST_HPP

#include <xercesc/util/RefHash2KeysTableOf.hpp>
#include <xercesc/validators/schema/SchemaElementDecl.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
//  This is a derivative of the framework abstract class which defines the
//  interface to a list of attribute defs that belong to a particular
//  element. The scanner needs to be able to get a list of the attributes
//  that an element supports, for use during the validation process and for
//  fixed/default attribute processing.
//
//  For us, we just wrap the RefHash2KeysTableOf collection that the SchemaElementDecl
//  class uses to store the attributes that belong to it.
//
//  This class does not adopt the hash table, it just references it. The
//  hash table is owned by the element decl it is a member of.
//
class VALIDATORS_EXPORT SchemaAttDefList : public XMLAttDefList
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    SchemaAttDefList
    (
         RefHash2KeysTableOf<SchemaAttDef>* const    listToUse,
         MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );

    ~SchemaAttDefList();


    // -----------------------------------------------------------------------
    //  Implementation of the virtual interface
    // -----------------------------------------------------------------------

    /** 
     * @deprecated This method is not thread-safe.
     */
    virtual bool hasMoreElements() const;

    virtual bool isEmpty() const;
    virtual XMLAttDef* findAttDef
    (
        const   unsigned long       uriID
        , const XMLCh* const        attName
    );
    virtual const XMLAttDef* findAttDef
    (
        const   unsigned long       uriID
        , const XMLCh* const        attName
    )   const;
    virtual XMLAttDef* findAttDef
    (
        const   XMLCh* const        attURI
        , const XMLCh* const        attName
    );
    virtual const XMLAttDef* findAttDef
    (
        const   XMLCh* const        attURI
        , const XMLCh* const        attName
    )   const;

    XMLAttDef* findAttDefLocalPart
    (
        const   unsigned long       uriID
        , const XMLCh* const        attLocalPart
    );

    const XMLAttDef* findAttDefLocalPart
    (
        const   unsigned long       uriID
        , const XMLCh* const        attLocalPart
    )   const;

    /** 
     * @deprecated This method is not thread-safe.
     */
    virtual XMLAttDef& nextElement();

    /** 
     * @deprecated This method is not thread-safe.
     */
    virtual void Reset();

    /**
     * return total number of attributes in this list
     */
    virtual unsigned int getAttDefCount() const ;

    /**
     * return attribute at the index-th position in the list.
     */
    virtual XMLAttDef &getAttDef(unsigned int index) ;

    /**
     * return attribute at the index-th position in the list.
     */
    virtual const XMLAttDef &getAttDef(unsigned int index) const ;

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(SchemaAttDefList)

	SchemaAttDefList(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    SchemaAttDefList(const SchemaAttDefList&);
    SchemaAttDefList& operator=(const SchemaAttDefList&);

    void addAttDef(SchemaAttDef *toAdd);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fEnum
    //      This is an enumerator for the list that we use to do the enumerator
    //      type methods of this class.
    //
    //  fList
    //      The list of SchemaAttDef objects that represent the attributes that
    //      a particular element supports.
    //  fArray
    //      vector of pointers to the DTDAttDef objects contained in this list
    //  fSize
    //      size of fArray
    //  fCount
    //      number of DTDAttDef objects currently stored in this list
    // -----------------------------------------------------------------------
    RefHash2KeysTableOfEnumerator<SchemaAttDef>*    fEnum;
    RefHash2KeysTableOf<SchemaAttDef>*              fList;
    SchemaAttDef**                          fArray;
    unsigned int                            fSize;
    unsigned int                            fCount;

    friend class ComplexTypeInfo;
};

inline void SchemaAttDefList::addAttDef(SchemaAttDef *toAdd)
{
    if(fCount == fSize)
    {
        // need to grow fArray
        fSize <<= 1;
        SchemaAttDef** newArray = (SchemaAttDef **)((getMemoryManager())->allocate( sizeof(SchemaAttDef*) * fSize ));
        memcpy(newArray, fArray, fCount * sizeof(SchemaAttDef *));
        (getMemoryManager())->deallocate(fArray);
        fArray = newArray;
    }
    fArray[fCount++] = toAdd;
}

inline XMLAttDef* SchemaAttDefList::findAttDefLocalPart(const   unsigned long       uriID
                                                      , const XMLCh* const        attLocalPart)
{
    return fList->get((void*)attLocalPart, uriID);
}

inline const XMLAttDef* SchemaAttDefList::findAttDefLocalPart(const   unsigned long       uriID
                                                            , const XMLCh* const        attLocalPart)   const
{
    return fList->get((void*)attLocalPart, uriID);
}

XERCES_CPP_NAMESPACE_END

#endif
