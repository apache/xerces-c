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
 * Revision 1.11  2005/01/07 15:12:10  amassari
 * Removed warnings
 *
 * Revision 1.10  2004/12/21 16:32:52  cargilld
 * Attempt to fix various apidoc problems.
 *
 * Revision 1.9  2004/12/21 16:02:51  cargilld
 * Attempt to fix various apidoc problems.
 *
 * Revision 1.8  2004/12/03 19:40:32  cargilld
 * Change call to resolveEntity to pass in public id so that only one call to resolveEntity is needed (a follow-on to Alberto's fix).
 *
 * Revision 1.7  2004/09/26 01:06:31  cargilld
 * Fix documentation generation problem.  Replace <pre> with <code>.  Patch from James Littlejohn.
 *
 * Revision 1.6  2004/09/08 13:56:24  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2004/02/15 19:37:16  amassari
 * Removed cause for warnings in VC 7.1
 *
 * Revision 1.4  2004/02/13 14:28:30  cargilld
 * Fix for bug 26900 (remove virtual on destructor)
 *
 * Revision 1.3  2004/01/29 11:48:47  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.2  2003/11/25 18:16:38  knoaman
 * Documentation update. Thanks to David Cargill.
 *
 * Revision 1.1  2003/10/30 21:37:32  knoaman
 * Enhanced Entity Resolver Support. Thanks to David Cargill.
 *
 *
 * Revision 1.1    1999/11/09 01:07:44  twl
 * Initial checkin
 *
 */

#ifndef XMLRESOURCEIDENTIFIER_HPP
#define XMLRESOURCEIDENTIFIER_HPP

XERCES_CPP_NAMESPACE_BEGIN

/**
  * <p>This class is used along with XMLEntityResolver to resolve entities.
  * Instead of passing publicId and systemId on the resolveEntity call, 
  * as is done with the SAX entity resolver, an object of type XMLResourceIdentifier
  * is passed.  By calling the getResourceIdentifierType() method the user can
  * determine which data members are available for inspection:</p>
  *
  * <table border='1'>
  * <tr>
  *  <td>ResourceIdentifierType</td>
  *  <td>Available Data Members</td>
  * </tr>
  * <tr>
  *  <td>SchemaGrammar</td>
  *  <td>schemaLocation, nameSpace & baseURI (current document)</td>
  * </tr>
  * <tr>
  *  <td>SchemaImport</td>
  *  <td>schemaLocation, nameSpace & baseURI (current document)</td>
  * </tr>
  * <tr>
  *  <td>SchemaInclude</td>
  *  <td>schemaLocation & baseURI (current document)</td>
  * </tr>
  * <tr>
  *  <td>SchemaRedefine</td>
  *  <td>schemaLocation & baseURI (current document)</td>
  * </tr>
  * <tr>
  *  <td>ExternalEntity</td>
  *  <td>systemId, publicId & baseURI (some items may be NULL)</td>
  * </tr>
  * </table>
  *
  * <p>The following resolver would provide the application
  * with a special character stream for the entity with the system
  * identifier "http://www.myhost.com/today":</p>
  *
  *<code>
  * #include <xercesc/util/XMLEntityResolver.hpp><br>
  * #include <xercesc/sax/InputSource.hpp><br>
  *<br>
  *&nbsp;class MyResolver : public XMLEntityResolver {<br>
  *&nbsp;&nbsp;public:<br>
  *&nbsp;&nbsp;&nbsp;InputSource resolveEntity (XMLResourceIdentifier* xmlri);<br>
  *&nbsp;&nbsp;&nbsp;...<br>
  *&nbsp;&nbsp;};<br>
  *<br>
  *&nbsp;&nbsp;MyResolver::resolveEntity(XMLResourceIdentifier* xmlri) {<br>
  *&nbsp;&nbsp;&nbsp;switch(xmlri->getResourceIdentifierType()) {<br>
  *&nbsp;&nbsp;&nbsp;&nbsp;case XMLResourceIdentifier::SystemId:<br>
  *&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;if (XMLString::compareString(xmlri->getSystemId(), "http://www.myhost.com/today")) {<br>
  *&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MyReader* reader = new MyReader();<br>
  *&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return new InputSource(reader);<br>
  *&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;} else {<br>
  *&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return null;<br>
  *&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br>
  *&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;break;<br>
  *&nbsp;&nbsp;&nbsp;&nbsp;default:<br>
  *&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return null;<br>
  *&nbsp;&nbsp;&nbsp;}<br>
  *&nbsp;&nbsp;}</code>
  *
  * @see SAXParser#setXMLEntityResolver
  * @see InputSource#InputSource
  */
class XMLUTIL_EXPORT XMLResourceIdentifier
{
public:

    /** @name Public Contants */
    //@{
    enum ResourceIdentifierType {
        SchemaGrammar = 0,
        SchemaImport,
        SchemaInclude,
        SchemaRedefine ,
        ExternalEntity,
        UnKnown = 255    
    };
    //@}
   
    /** @name Constructors and Destructor */
    //@{
    /** Constructor */

    XMLResourceIdentifier(const ResourceIdentifierType resourceIdentitiferType
                            , const XMLCh* const  systemId
                            , const XMLCh* const  nameSpace = 0
                            , const XMLCh* const  publicId = 0
                            , const XMLCh* const  baseURI = 0);

    /** Destructor */
    ~XMLResourceIdentifier()
    {
    }

    //@}

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    /** @name Public Methods */
    //@{
    ResourceIdentifierType getResourceIdentifierType() const;
    const XMLCh* getPublicId()          const;
    const XMLCh* getSystemId()          const;
    const XMLCh* getSchemaLocation()    const;
    const XMLCh* getBaseURI()           const;
    const XMLCh* getNameSpace()         const;

    //@}

private :

    const ResourceIdentifierType    fResourceIdentifierType;
    const XMLCh*                    fPublicId;
    const XMLCh*                    fSystemId;
    const XMLCh*                    fBaseURI;
    const XMLCh*                    fNameSpace;


    /* Unimplemented constructors and operators */

    /* Copy constructor */
    XMLResourceIdentifier(const XMLResourceIdentifier&);

    /* Assignment operator */
    XMLResourceIdentifier& operator=(const XMLResourceIdentifier&);

};

inline XMLResourceIdentifier::ResourceIdentifierType XMLResourceIdentifier::getResourceIdentifierType() const 
{
    return fResourceIdentifierType;
}

inline const XMLCh* XMLResourceIdentifier::getPublicId() const
{
    return fPublicId;
}

inline const XMLCh* XMLResourceIdentifier::getSystemId() const
{
    return fSystemId;
}

inline const XMLCh* XMLResourceIdentifier::getSchemaLocation() const
{
    return fSystemId;
}

inline const XMLCh* XMLResourceIdentifier::getBaseURI() const
{
    return fBaseURI;
}

inline const XMLCh* XMLResourceIdentifier::getNameSpace() const
{
    return fNameSpace;
}

inline XMLResourceIdentifier::XMLResourceIdentifier(const ResourceIdentifierType resourceIdentifierType
                            , const XMLCh* const  systemId
                            , const XMLCh* const  nameSpace
                            , const XMLCh* const  publicId
                            , const XMLCh* const  baseURI )
    : fResourceIdentifierType(resourceIdentifierType)
    , fPublicId(publicId)
    , fSystemId(systemId)
    , fBaseURI(baseURI)     
    , fNameSpace(nameSpace)
{
}

XERCES_CPP_NAMESPACE_END

#endif
