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
 * Revision 1.6  2004/09/08 13:56:19  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/12/01 23:23:26  neilg
 * fix for bug 25118; thanks to Jeroen Witmond
 *
 * Revision 1.4  2003/05/30 16:11:44  gareth
 * Fixes so we compile under VC7.1. Patch by Alberto Massari.
 *
 * Revision 1.3  2003/03/07 18:10:06  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.2  2002/11/04 14:56:25  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:08  peiyongz
 * sane_include
 *
 * Revision 1.6  2000/04/27 19:33:15  rahulj
 * Included <util/XercesDefs.hpp> as suggested by David N Bertoni.
 *
 * Revision 1.5  2000/02/24 20:12:55  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.4  2000/02/12 03:31:55  rahulj
 * Removed duplicate CVS Log entries.
 *
 * Revision 1.3  2000/02/12 01:27:19  aruna1
 * Documentation updated
 *
 * Revision 1.2  2000/02/06 07:47:57  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:45  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:00  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class SAXParseException;


/**
  * Basic interface for SAX error handlers.
  *
  * <p>If a SAX application needs to implement customized error
  * handling, it must implement this interface and then register an
  * instance with the SAX parser using the parser's setErrorHandler
  * method.  The parser will then report all errors and warnings
  * through this interface.</p>
  *
  * <p> The parser shall use this interface instead of throwing an
  * exception: it is up to the application whether to throw an
  * exception for different types of errors and warnings.  Note,
  * however, that there is no requirement that the parser continue to
  * provide useful information after a call to fatalError (in other
  * words, a SAX driver class could catch an exception and report a
  * fatalError).</p>
  *
  * <p>The HandlerBase class provides a default implementation of this
  * interface, ignoring warnings and recoverable errors and throwing a
  * SAXParseException for fatal errors.  An application may extend
  * that class rather than implementing the complete interface
  * itself.</p>
  *
  * @see Parser#setErrorHandler
  * @see SAXParseException#SAXParseException
  * @see HandlerBase#HandlerBase
  */

class SAX_EXPORT ErrorHandler
{
public:
    /** @name Constructors and Destructor */
    //@{
    /** Default constructor */
    ErrorHandler()
    {
    }

    /** Desctructor */
    virtual ~ErrorHandler()
    {
    }
    //@}

    /** @name The error handler interface */
    //@{
   /**
    * Receive notification of a warning.
    *
    * <p>SAX parsers will use this method to report conditions that
    * are not errors or fatal errors as defined by the XML 1.0
    * recommendation.  The default behaviour is to take no action.</p>
    *
    * <p>The SAX parser must continue to provide normal parsing events
    * after invoking this method: it should still be possible for the
    * application to process the document through to the end.</p>
    *
    * @param exc The warning information encapsulated in a
    *            SAX parse exception.
    * @exception SAXException Any SAX exception, possibly
    *            wrapping another exception.
    * @see SAXParseException#SAXParseException
    */
    virtual void warning(const SAXParseException& exc) = 0;

  /**
    * Receive notification of a recoverable error.
    *
    * <p>This corresponds to the definition of "error" in section 1.2
    * of the W3C XML 1.0 Recommendation.  For example, a validating
    * parser would use this callback to report the violation of a
    * validity constraint.  The default behaviour is to take no
    * action.</p>
    *
    * <p>The SAX parser must continue to provide normal parsing events
    * after invoking this method: it should still be possible for the
    * application to process the document through to the end.  If the
    * application cannot do so, then the parser should report a fatal
    * error even if the XML 1.0 recommendation does not require it to
    * do so.</p>
    *
    * @param exc The error information encapsulated in a
    *            SAX parse exception.
    * @exception SAXException Any SAX exception, possibly
    *            wrapping another exception.
    * @see SAXParseException#SAXParseException
    */
    virtual void error(const SAXParseException& exc) = 0;

  /**
    * Receive notification of a non-recoverable error.
    *
    * <p>This corresponds to the definition of "fatal error" in
    * section 1.2 of the W3C XML 1.0 Recommendation.  For example, a
    * parser would use this callback to report the violation of a
    * well-formedness constraint.</p>
    *
    * <p>The application must assume that the document is unusable
    * after the parser has invoked this method, and should continue
    * (if at all) only for the sake of collecting addition error
    * messages: in fact, SAX parsers are free to stop reporting any
    * other events once this method has been invoked.</p>
    *
    * @param exc The error information encapsulated in a
    *            SAX parse exception.
    * @exception SAXException Any SAX exception, possibly
    *            wrapping another exception.
    * @see SAXParseException#SAXParseException
    */
    virtual void fatalError(const SAXParseException& exc) = 0;

    /**
    * Reset the Error handler object on its reuse
    *
    * <p>This method helps in reseting the Error handler object
    * implementational defaults each time the Error handler is begun.</p>
    *
    */
    virtual void resetErrors() = 0;


    //@}

private :
    /* Unimplemented constructors and operators */

    /* Copy constructor */
    ErrorHandler(const ErrorHandler&);

    /* Assignment operator */
    ErrorHandler& operator=(const ErrorHandler&);

};

XERCES_CPP_NAMESPACE_END

#endif
