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
 * Revision 1.4  2004/09/08 13:56:13  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2004/01/29 11:46:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.2  2002/11/04 14:58:18  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:21:58  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/02/24 20:18:07  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/06 07:47:53  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:07  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:42  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(ENDOFENTITYEXCEPTION_HPP)
#define ENDOFENTITYEXCEPTION_HPP

#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLEntityDecl;

//
//  This class is only used internally. Its thrown by the ReaderMgr class,
//  when an entity ends, and is caught in the scanner. This tells the scanner
//  that an entity has ended, and allows it to do the right thing according
//  to what was going on when the entity ended.
//
//  Since its internal, it does not bother implementing XMLException.
//
class XMLPARSER_EXPORT EndOfEntityException
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    EndOfEntityException(       XMLEntityDecl*  entityThatEnded
                        , const unsigned int    readerNum) :

        fEntity(entityThatEnded)
        , fReaderNum(readerNum)
    {
    }

    EndOfEntityException(const EndOfEntityException& toCopy) :

        fEntity(toCopy.fEntity)
        , fReaderNum(toCopy.fReaderNum)
    {
    }

    ~EndOfEntityException()
    {
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    XMLEntityDecl& getEntity();
    const XMLEntityDecl& getEntity() const;
    unsigned int getReaderNum() const;


private :
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    EndOfEntityException& operator = (const  EndOfEntityException&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fEntity
    //      This is a reference to the entity that ended, causing this
    //      exception.
    //
    //  fReaderNum
    //      The unique reader number of the reader that was handling this
    //      entity. This is used to know whether a particular entity has
    //      ended.
    // -----------------------------------------------------------------------
    XMLEntityDecl*  fEntity;
    unsigned int    fReaderNum;
};


// ---------------------------------------------------------------------------
//  EndOfEntityException: Getter methods
// ---------------------------------------------------------------------------
inline XMLEntityDecl& EndOfEntityException::getEntity()
{
    return *fEntity;
}

inline const XMLEntityDecl& EndOfEntityException::getEntity() const
{
    return *fEntity;
}

inline unsigned int EndOfEntityException::getReaderNum() const
{
    return fReaderNum;
}

XERCES_CPP_NAMESPACE_END

#endif
