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
 * Revision 1.6  2004/09/26 01:06:30  cargilld
 * Fix documentation generation problem.  Replace <pre> with <code>.  Patch from James Littlejohn.
 *
 * Revision 1.5  2004/09/08 13:56:19  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/03/07 18:10:06  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.3  2002/11/04 14:56:25  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/06/06 20:39:16  tng
 * Document Fix: document that the returned object from resolveEntity is owned by the parser
 *
 * Revision 1.1.1.1  2002/02/01 22:22:08  peiyongz
 * sane_include
 *
 * Revision 1.6  2000/03/02 19:54:34  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
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
 * Revision 1.1.1.1  1999/11/09 01:07:44  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:56  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#ifndef ENTITYRESOLVER_HPP
#define ENTITYRESOLVER_HPP

#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class InputSource;

/**
  * Basic interface for resolving entities.
  *
  * <p>If a SAX application needs to implement customized handling
  * for external entities, it must implement this interface and
  * register an instance with the SAX parser using the parser's
  * setEntityResolver method.</p>
  *
  * <p>The parser will then allow the application to intercept any
  * external entities (including the external DTD subset and external
  * parameter entities, if any) before including them.</p>
  *
  * <p>Many SAX applications will not need to implement this interface,
  * but it will be especially useful for applications that build
  * XML documents from databases or other specialised input sources,
  * or for applications that use URI types other than URLs.</p>
  *
  * <p>The following resolver would provide the application
  * with a special character stream for the entity with the system
  * identifier "http://www.myhost.com/today":</p>
  *
  *<code>
  *#include <xercesc/sax/EntityResolver.hpp><br>
  *#include <xercesc/sax/InputSource.hpp><br>
  *<br>
  *class MyResolver : public EntityResolver {<br>
  *  public:<br>&nbsp;
  *    InputSource resolveEntity (const XMLCh* const publicId, const XMLCh* const systemId);<br>&nbsp;&nbsp;
  *    <br>
  *   ...<br>&nbsp;&nbsp;
  *   };<br>&nbsp;
  *<br>
  *&nbsp;MyResolver::resolveEntity {<br>
  *&nbsp;&nbsp;if (XMLString::compareString(systemId, "http://www.myhost.com/today")) {<br>
  *&nbsp;&nbsp;&nbsp;MyReader* reader = new MyReader();<br>
  *&nbsp;&nbsp;&nbsp;return new InputSource(reader);<br>
  *&nbsp;&nbsp;} else {<br>
  *&nbsp;&nbsp;&nbsp;return null;<br>
  *&nbsp;&nbsp;}<br>
  *&nbsp;}<br>
  *<br>
  *</code>
  *
  * <p>The application can also use this interface to redirect system
  * identifiers to local URIs or to look up replacements in a catalog
  * (possibly by using the public identifier).</p>
  *
  * <p>The HandlerBase class implements the default behaviour for
  * this interface, which is simply always to return null (to request
  * that the parser use the default system identifier).</p>
  *
  * @see Parser#setEntityResolver
  * @see InputSource#InputSource
  * @see HandlerBase#HandlerBase
  */
class SAX_EXPORT EntityResolver
{
public:
    /** @name Constructors and Destructor */
    //@{

    /** Default Constructor */
    EntityResolver()
    {
    }

    /** Destructor */
    virtual ~EntityResolver()
    {
    }

    //@}

    /** @name The EntityResolver interface */
    //@{

  /**
    * Allow the application to resolve external entities.
    *
    * <p>The Parser will call this method before opening any external
    * entity except the top-level document entity (including the
    * external DTD subset, external entities referenced within the
    * DTD, and external entities referenced within the document
    * element): the application may request that the parser resolve
    * the entity itself, that it use an alternative URI, or that it
    * use an entirely different input source.</p>
    *
    * <p>Application writers can use this method to redirect external
    * system identifiers to secure and/or local URIs, to look up
    * public identifiers in a catalogue, or to read an entity from a
    * database or other input source (including, for example, a dialog
    * box).</p>
    *
    * <p>If the system identifier is a URL, the SAX parser must
    * resolve it fully before reporting it to the application.</p>
    *
    * @param publicId The public identifier of the external entity
    *        being referenced, or null if none was supplied.
    * @param systemId The system identifier of the external entity
    *        being referenced.
    * @return An InputSource object describing the new input source,
    *         or null to request that the parser open a regular
    *         URI connection to the system identifier.
    *         The returned InputSource is owned by the parser which is
    *         responsible to clean up the memory.
    * @exception SAXException Any SAX exception, possibly
    *            wrapping another exception.
    * @exception IOException An IO exception,
    *            possibly the result of creating a new InputStream
    *            or Reader for the InputSource.
    * @see InputSource#InputSource
    */
    virtual InputSource* resolveEntity
    (
        const   XMLCh* const    publicId
        , const XMLCh* const    systemId
    ) = 0;

    //@}

private :
    /* Unimplemented constructors and operators */


    /* Copy constructor */
    EntityResolver(const EntityResolver&);

    /* Assignment operator */
    EntityResolver& operator=(const EntityResolver&);

};

XERCES_CPP_NAMESPACE_END

#endif
