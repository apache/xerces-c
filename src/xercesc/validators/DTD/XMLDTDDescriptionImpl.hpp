/*
 * Copyright 1999-2004 The Apache Software Foundation.
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
 * Revision 1.4  2004/09/08 13:56:50  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2004/04/07 14:10:04  peiyongz
 * systemId (to replace rootElemName) as DTDGrammar Key
 *
 * Revision 1.2  2003/10/14 15:20:42  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.1  2003/06/20 18:39:33  peiyongz
 * Stateless Grammar Pool :: Part I
 *
 * $Id$
 *
 */

#if !defined(XMLDTDDescriptionImplIMPL_HPP)
#define XMLDTDDescriptionImplIMPL_HPP

#include <xercesc/framework/XMLDTDDescription.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLPARSER_EXPORT XMLDTDDescriptionImpl : public XMLDTDDescription
{
public :
    // -----------------------------------------------------------------------
    /** @name constructor and destructor */
    // -----------------------------------------------------------------------
    //@{
    XMLDTDDescriptionImpl(
                          const XMLCh* const   systemId 
                        , MemoryManager* const memMgr
                          );

    ~XMLDTDDescriptionImpl();
    //@}

    // -----------------------------------------------------------------------
    /** @name Implementation of GrammarDescription Interface */
    // -----------------------------------------------------------------------
    //@{
    /**
      * getGrammarKey
      *
      */
    virtual const XMLCh*           getGrammarKey() const ;
    //@}

    // -----------------------------------------------------------------------
    /** @name Implementation of DTDDescription Interface */
    // -----------------------------------------------------------------------
    //@{
    /**
      * Getter
      *
      */
    virtual const XMLCh*          getRootName() const;
    virtual const XMLCh*          getSystemId() const;

    /**
      * Setter
      *
      */
    virtual void                  setRootName(const XMLCh* const);
    virtual void                  setSystemId(const XMLCh* const);
    //@}
    
    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(XMLDTDDescriptionImpl)

    XMLDTDDescriptionImpl(MemoryManager* const memMgr = XMLPlatformUtils::fgMemoryManager);

private :
    // -----------------------------------------------------------------------
    /** name  Unimplemented copy constructor and operator= */
    // -----------------------------------------------------------------------
    //@{
    XMLDTDDescriptionImpl(const XMLDTDDescriptionImpl& );
    XMLDTDDescriptionImpl& operator=(const XMLDTDDescriptionImpl& );
    //@}

    // -----------------------------------------------------------------------
    //
    // fSystemId:
    //     SYSTEM ID of the grammar
    //
    // fRootName: 
    //      root name of the grammar
    //
    // -----------------------------------------------------------------------

    const XMLCh*      fSystemId;
    const XMLCh*      fRootName;    

};


XERCES_CPP_NAMESPACE_END

#endif
