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
 * Revision 1.5  2004/09/08 13:55:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2004/01/29 11:46:29  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.3  2003/05/16 21:36:55  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.2  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:21:50  peiyongz
 * sane_include
 *
 * Revision 1.6  2000/12/14 18:49:55  tng
 * Fix API document generation warning: "Warning: end of member group without matching begin"
 *
 * Revision 1.5  2000/02/24 20:00:22  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.4  2000/02/15 23:59:06  roddey
 * More updated documentation of Framework classes.
 *
 * Revision 1.3  2000/02/15 01:21:30  roddey
 * Some initial documentation improvements. More to come...
 *
 * Revision 1.2  2000/02/06 07:47:46  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1  2000/01/12 00:13:26  roddey
 * These were moved from internal/ to framework/, which was something that should have
 * happened long ago. They are really framework type of classes.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:17  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:44  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(STDININPUTSOURCE_HPP)
#define STDININPUTSOURCE_HPP

#include <xercesc/sax/InputSource.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class BinInputStream;


/**
 *  This class is a derivative of the standard InputSource class. It provides
 *  for the parser access to data via the standard input. This input source
 *  is not commonly used, but can be useful when implementing such things
 *  as pipe based tools which exchange XML data.
 *
 *  As with all InputSource derivatives. The primary objective of an input
 *  source is to create an input stream via which the parser can spool in
 *  data from the referenced source.
 */
class XMLPARSER_EXPORT StdInInputSource : public InputSource
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------

    /** @name Constructor */
    //@{

    /**
      * Since the standard input is a canned source, the constructor is very
      * simple. It just uses local platform services to open up the standard
      * input source as file, a new handleof which it gives to each new stream
      * it creates.
      */
    StdInInputSource(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    //@}

    /** @name Destructor */
    //@{
    ~StdInInputSource();
    //@}


    // -----------------------------------------------------------------------
    //  Virtual input source interface
    // -----------------------------------------------------------------------


    /** @name Virtual methods */
    //@{

    /**
     * This method will return a binary input stream derivative that will
     * parse from the standard input of the local host.
     *
     * @return A dynamically allocated binary input stream derivative that
     *         can parse from the standardinput.
     */
    BinInputStream* makeStream() const;

    //@}

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    StdInInputSource(const StdInInputSource&);
    StdInInputSource& operator=(const StdInInputSource&);

};

inline StdInInputSource::StdInInputSource(MemoryManager* const manager) :

    InputSource("stdin", manager)
{
}

inline StdInInputSource::~StdInInputSource()
{
}

XERCES_CPP_NAMESPACE_END

#endif
