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
 * Revision 1.5  2005/01/07 15:22:13  amassari
 * Removed warnings
 *
 * Revision 1.4  2004/09/08 13:55:59  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2003/05/15 18:26:07  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:21:52  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/02/24 20:00:23  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.3  2000/02/15 01:21:31  roddey
 * Some initial documentation improvements. More to come...
 *
 * Revision 1.2  2000/02/06 07:47:48  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:36  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:39  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(XMLPSCANTOKEN_HPP)
#define XMLPSCANTOKEN_HPP

#include <xercesc/util/XMemory.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLScanner;

/**
 *  This simple class is used as a sanity check when the scanner is used to
 *  do progressive parsing. It insures that things are not done out of
 *  sequence and that sequences of scan calls are made correctly to the
 *  right scanner instances.
 *
 *  To client code, it is just a magic cookie which is obtained when a
 *  progressive parse is begun, and which is passed back in on each subsequent
 *  call of the progressive parse.
 */
class XMLPARSER_EXPORT XMLPScanToken : public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    /** @name Constructor */
    //@{
    XMLPScanToken();
    XMLPScanToken(const XMLPScanToken& toCopy);
    //@}

    /** @name Destructor */
    //@{
    ~XMLPScanToken();
    //@}


    // -----------------------------------------------------------------------
    //  Public operators
    // -----------------------------------------------------------------------
    XMLPScanToken& operator=(const XMLPScanToken& toCopy);


protected :
    // -----------------------------------------------------------------------
    //  XMLScanner is our friend, can you say friend? Sure...
    // -----------------------------------------------------------------------
    friend class XMLScanner;


    // -----------------------------------------------------------------------
    //  Hidden methods for use by XMLScanner
    // -----------------------------------------------------------------------
    void set
    (
        const   XMLUInt32   scannerId
        , const XMLUInt32   sequenceId
    );


private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fScannerId
    //      This field is set to the id of the scanner, to catch problems
    //      where a token is gotten from one scanner and passed to another.
    //      Each scanner is assigned an incrementing id.
    //
    //  fSequenceId
    //      In order to avoid problems such as calling scanNext() without
    //      a call to scanFirst() and such, this value is set when scanFirst()
    //      is called and matches this token to the current sequence id of
    //      the scanner.
    // -----------------------------------------------------------------------
    XMLUInt32   fScannerId;
    XMLUInt32   fSequenceId;
};


// ---------------------------------------------------------------------------
//  XMLPScanToken: Constructors and Operators
// ---------------------------------------------------------------------------
inline XMLPScanToken::XMLPScanToken() :

    fScannerId(0)
    , fSequenceId(0)
{
}

inline XMLPScanToken::XMLPScanToken(const XMLPScanToken& toCopy) :
    XMemory(toCopy)
    , fScannerId(toCopy.fScannerId)
    , fSequenceId(toCopy.fSequenceId)
{
}

inline XMLPScanToken::~XMLPScanToken()
{
}


// ---------------------------------------------------------------------------
//  XMLPScanToken: Public operators
// ---------------------------------------------------------------------------
inline XMLPScanToken& XMLPScanToken::operator=(const XMLPScanToken& toCopy)
{
    if (this == &toCopy)
        return *this;

    fScannerId = toCopy.fScannerId;
    fSequenceId = toCopy.fSequenceId;

    return *this;
}


// ---------------------------------------------------------------------------
//  XMLPScanToken: Hidden methods
// ---------------------------------------------------------------------------
inline void XMLPScanToken::set( const   XMLUInt32   scannerId
                                , const XMLUInt32   sequenceId)
{
    fScannerId = scannerId;
    fSequenceId = sequenceId;
}

XERCES_CPP_NAMESPACE_END

#endif
