/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 * $Log$
 * Revision 1.6  2003/12/17 00:18:34  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.5  2003/11/11 22:48:13  knoaman
 * Serialization of XSAnnotation.
 *
 * Revision 1.4  2003/11/03 22:01:27  peiyongz
 * Store/Load keys separately from SchemaElementDecl
 *
 * Revision 1.3  2003/10/31 22:15:31  peiyongz
 * fix bug in creating ElemVector
 *
 * Revision 1.2  2003/10/29 16:16:08  peiyongz
 * GrammarPool' serialization/deserialization support
 *
 * Revision 1.1  2003/10/17 21:07:49  peiyongz
 * To support Template object serialization/deserialization
 *
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/XSerializeEngine.hpp>
#include <xercesc/internal/XTemplateSerializer.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <xercesc/util/HashPtr.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/**********************************************************
 *
 * ValueVectorOf
 *
 *   SchemaElementDecl*
 *   unsigned int
 *
 ***********************************************************/   
void XTemplateSerializer::storeObject(ValueVectorOf<SchemaElementDecl*>* const objToStore
                                    , XSerializeEngine&                        serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorSize = objToStore->size();
        serEng<<vectorSize;

        for ( int i = 0; i < vectorSize; i++)
        {            
            SchemaElementDecl*& data = objToStore->elementAt(i);
            serEng<<data;
        }
    }
}

void XTemplateSerializer::loadObject(ValueVectorOf<SchemaElementDecl*>**       objToLoad
                                   , int                                       initSize
                                   , bool                                      toCallDestructor
                                   , XSerializeEngine&                         serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             ValueVectorOf<SchemaElementDecl*>(
                                                               initSize
                                                             , serEng.getMemoryManager()
                                                             , toCallDestructor
                                                             );
        }

        serEng.registerObject(*objToLoad);

        int vectorSize = 0;
        serEng>>vectorSize;

        for ( int i = 0; i < vectorSize; i++)
        {
            SchemaElementDecl* data;
            serEng>>data;
            (*objToLoad)->addElement(data);
        }
    }

}

void XTemplateSerializer::storeObject(ValueVectorOf<unsigned int>* const objToStore
                                    , XSerializeEngine&                  serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorSize = objToStore->size();
        serEng<<vectorSize;

        for ( int i = 0; i < vectorSize; i++)
        {            
            unsigned int& data = objToStore->elementAt(i);
            serEng<<data;
        }
    }
}

void XTemplateSerializer::loadObject(ValueVectorOf<unsigned int>**       objToLoad
                                   , int                                 initSize
                                   , bool                                toCallDestructor
                                   , XSerializeEngine&                   serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             ValueVectorOf<unsigned int>(
                                                         initSize
                                                       , serEng.getMemoryManager()
                                                       , toCallDestructor
                                                       );
        }

        serEng.registerObject(*objToLoad);

        int vectorSize = 0;
        serEng>>vectorSize;

        for ( int i = 0; i < vectorSize; i++)
        {
            unsigned int data;
            serEng>>data;
            (*objToLoad)->addElement(data);
        }
    }

}

/**********************************************************
 *
 * RefArrayVectorOf
 *
 *   XMLCh
 *
 ***********************************************************/

void XTemplateSerializer::storeObject(RefArrayVectorOf<XMLCh>* const objToStore
                                    , XSerializeEngine&              serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorSize = objToStore->size();
        serEng<<vectorSize;

        for ( int i = 0; i < vectorSize; i++)
        {            
            serEng.writeString(objToStore->elementAt(i));
        }
    }
    
}

void XTemplateSerializer::loadObject(RefArrayVectorOf<XMLCh>**  objToLoad
                                   , int                        initSize
                                   , bool                       toAdopt
                                   , XSerializeEngine&          serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             RefArrayVectorOf<XMLCh>(
                                                     initSize
                                                   , toAdopt
                                                   , serEng.getMemoryManager()
                                                   );
        }

        serEng.registerObject(*objToLoad);

        int vectorSize = 0;
        serEng>>vectorSize;

        for ( int i = 0; i < vectorSize; i++)
        {
            XMLCh* data;
            serEng.readString(data);
            (*objToLoad)->addElement(data);
        }
    }

}

/**********************************************************
 *
 * RefVectorOf
 *
 *   SchemaAttDef
 *   SchemaElementDecl
 *   ContentSpecNode
 *   IC_Field
 *   DatatypeValidator
 *   IdentityConstraint
 *   XMLNumber
 *   XercesLocationPath
 *   XercesStep
 *
 ***********************************************************/

void XTemplateSerializer::storeObject(RefVectorOf<SchemaAttDef>* const objToStore
                                    , XSerializeEngine&                serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorLength = objToStore->size();
        serEng<<vectorLength;

        for ( int i = 0; i < vectorLength; i++)
        {
            SchemaAttDef* data = objToStore->elementAt(i);
            serEng<<data;
        }
    }

}

void XTemplateSerializer::loadObject(RefVectorOf<SchemaAttDef>** objToLoad
                                   , int                         initSize
                                   , bool                        toAdopt
                                   , XSerializeEngine&           serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             RefVectorOf<SchemaAttDef>(
                                                       initSize
                                                     , toAdopt
                                                     , serEng.getMemoryManager()
                                                     );
        }

        serEng.registerObject(*objToLoad);

        int vectorLength = 0;
        serEng>>vectorLength;
        for ( int i = 0 ; i < vectorLength; i++)
        {            
            SchemaAttDef* data;
            serEng>>data;
            (*objToLoad)->addElement(data);
        }
    }

}

void XTemplateSerializer::storeObject(RefVectorOf<SchemaElementDecl>* const objToStore
                                    , XSerializeEngine&                     serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorLength = objToStore->size();
        serEng<<vectorLength;

        for ( int i = 0; i < vectorLength; i++)
        {
            SchemaElementDecl* data = objToStore->elementAt(i);
            serEng<<data;
        }
    }

}

void XTemplateSerializer::loadObject(RefVectorOf<SchemaElementDecl>** objToLoad
                                   , int                              initSize
                                   , bool                             toAdopt
                                   , XSerializeEngine&                serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             RefVectorOf<SchemaElementDecl>(
                                                            initSize
                                                          , toAdopt
                                                          , serEng.getMemoryManager()
                                                          );
        }

        serEng.registerObject(*objToLoad);

        int vectorLength = 0;
        serEng>>vectorLength;
        for ( int i = 0 ; i < vectorLength; i++)
        {            
            SchemaElementDecl* data;
            serEng>>data;
            (*objToLoad)->addElement(data);
        }
    }

}

void XTemplateSerializer::storeObject(RefVectorOf<ContentSpecNode>* const objToStore
                                    , XSerializeEngine&                   serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorLength = objToStore->size();
        serEng<<vectorLength;

        for ( int i = 0; i < vectorLength; i++)
        {
            ContentSpecNode* data = objToStore->elementAt(i);
            serEng<<data;
        }
    }

}

void XTemplateSerializer::loadObject(RefVectorOf<ContentSpecNode>** objToLoad
                                   , int                            initSize
                                   , bool                           toAdopt
                                   , XSerializeEngine&              serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             RefVectorOf<ContentSpecNode>(
                                                          initSize
                                                        , toAdopt
                                                        , serEng.getMemoryManager()
                                                        );
        }

        serEng.registerObject(*objToLoad);

        int vectorLength = 0;
        serEng>>vectorLength;
        for ( int i = 0 ; i < vectorLength; i++)
        {            
            ContentSpecNode* data;
            serEng>>data;
            (*objToLoad)->addElement(data);
        }
    }

}

void XTemplateSerializer::storeObject(RefVectorOf<IC_Field>* const objToStore
                                    , XSerializeEngine&            serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorLength = objToStore->size();
        serEng<<vectorLength;

        for ( int i = 0; i < vectorLength; i++)
        {
            IC_Field* data = objToStore->elementAt(i);
            serEng<<data;
        }
    }

}

void XTemplateSerializer::loadObject(RefVectorOf<IC_Field>** objToLoad
                                   , int                     initSize
                                   , bool                    toAdopt
                                   , XSerializeEngine&       serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             RefVectorOf<IC_Field>(
                                                   initSize
                                                 , toAdopt
                                                 , serEng.getMemoryManager()
                                                 );
        }

        serEng.registerObject(*objToLoad);

        int vectorLength = 0;
        serEng>>vectorLength;
        for ( int i = 0 ; i < vectorLength; i++)
        {            
            IC_Field* data;
            serEng>>data;
            (*objToLoad)->addElement(data);
        }
    }

}

void XTemplateSerializer::storeObject(RefVectorOf<DatatypeValidator>* const objToStore
                                    , XSerializeEngine&                      serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorLength = objToStore->size();
        serEng<<vectorLength;

        for ( int i = 0; i < vectorLength; i++)
        {
            DatatypeValidator* data = objToStore->elementAt(i);
            DatatypeValidator::storeDV(serEng, data);
        }
    }

}

void XTemplateSerializer::loadObject(RefVectorOf<DatatypeValidator>** objToLoad
                                   , int                               initSize
                                   , bool                              toAdopt
                                   , XSerializeEngine&                 serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             RefVectorOf<DatatypeValidator>(
                                                            initSize
                                                          , toAdopt
                                                          , serEng.getMemoryManager()
                                                           );
        }

        serEng.registerObject(*objToLoad);

        int vectorLength = 0;
        serEng>>vectorLength;
        for ( int i = 0 ; i < vectorLength; i++)
        {            
            DatatypeValidator*  data;
            data = DatatypeValidator::loadDV(serEng);
            (*objToLoad)->addElement(data);
        }
    }

}

void XTemplateSerializer::storeObject(RefVectorOf<IdentityConstraint>* const objToStore
                                    , XSerializeEngine&                       serEng)
{ 

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorLength = objToStore->size();
        serEng<<vectorLength;

        for ( int i = 0; i < vectorLength; i++)
        {
            IdentityConstraint* data = objToStore->elementAt(i);
            IdentityConstraint::storeIC(serEng, data);
        }
    }

}

void XTemplateSerializer::loadObject(RefVectorOf<IdentityConstraint>** objToLoad
                                   , int                                initSize
                                   , bool                               toAdopt
                                   , XSerializeEngine&                  serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             RefVectorOf<IdentityConstraint>(
                                                             initSize
                                                           , toAdopt
                                                           , serEng.getMemoryManager()
                                                            );
        }

        serEng.registerObject(*objToLoad);

        int vectorLength = 0;
        serEng>>vectorLength;
        for ( int i = 0 ; i < vectorLength; i++)
        {            
            IdentityConstraint*  data;
            data = IdentityConstraint::loadIC(serEng);
            (*objToLoad)->addElement(data);
        }
    }

}

void XTemplateSerializer::storeObject(RefVectorOf<XMLNumber>* const objToStore
                                    , XSerializeEngine&             serEng)
{ 

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorLength = objToStore->size();
        serEng<<vectorLength;

        for ( int i = 0; i < vectorLength; i++)
        {
            XMLNumber* data = objToStore->elementAt(i);
            serEng<<data;
        }
    }

}

void XTemplateSerializer::loadObject(RefVectorOf<XMLNumber>** objToLoad
                                   , int                       initSize
                                   , bool                      toAdopt
                                   , XMLNumber::NumberType     numType
                                   , XSerializeEngine&         serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             RefVectorOf<XMLNumber>(
                                                    initSize
                                                  , toAdopt
                                                  , serEng.getMemoryManager()
                                                   );
        }

        serEng.registerObject(*objToLoad);

        int vectorLength = 0;
        serEng>>vectorLength;
        for ( int i = 0 ; i < vectorLength; i++)
        {            
            XMLNumber*  data;
            data = XMLNumber::loadNumber(numType , serEng);
            (*objToLoad)->addElement(data);
        }
    }

}

void XTemplateSerializer::storeObject(RefVectorOf<XercesLocationPath>* const objToStore
                                    , XSerializeEngine&                      serEng)
{ 

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorLength = objToStore->size();
        serEng<<vectorLength;

        for ( int i = 0; i < vectorLength; i++)
        {
            XercesLocationPath* data = objToStore->elementAt(i);
            serEng<<data;
        }
    }

}

void XTemplateSerializer::loadObject(RefVectorOf<XercesLocationPath>** objToLoad
                                   , int                               initSize
                                   , bool                              toAdopt
                                   , XSerializeEngine&                 serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             RefVectorOf<XercesLocationPath>(
                                                             initSize
                                                           , toAdopt
                                                           , serEng.getMemoryManager()
                                                            );
        }

        serEng.registerObject(*objToLoad);

        int vectorLength = 0;
        serEng>>vectorLength;
        for ( int i = 0 ; i < vectorLength; i++)
        {            
            XercesLocationPath*  data;
            serEng>>data;
            (*objToLoad)->addElement(data);
        }
    }

}

void XTemplateSerializer::storeObject(RefVectorOf<XercesStep>* const objToStore
                                    , XSerializeEngine&              serEng)
{ 

    if (serEng.needToStoreObject(objToStore))
    {
        int vectorLength = objToStore->size();
        serEng<<vectorLength;

        for ( int i = 0; i < vectorLength; i++)
        {
            XercesStep* data = objToStore->elementAt(i);
            serEng<<data;
        }
    }

}

void XTemplateSerializer::loadObject(RefVectorOf<XercesStep>** objToLoad
                                   , int                       initSize
                                   , bool                      toAdopt
                                   , XSerializeEngine&         serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             RefVectorOf<XercesStep>(
                                                     initSize
                                                   , toAdopt
                                                   , serEng.getMemoryManager()
                                                    );
        }

        serEng.registerObject(*objToLoad);

        int vectorLength = 0;
        serEng>>vectorLength;
        for ( int i = 0 ; i < vectorLength; i++)
        {            
            XercesStep*  data;
            serEng>>data;
            (*objToLoad)->addElement(data);
        }
    }

}

/**********************************************************
 *
 * RefHasbTableOf
 *
 *   KVStringPair
 *   XMLAttDef
 *   DTDAttDef
 *   ComplexTypeInfo
 *   XercesGroupInfo
 *   XercesAttGroupInfo
 *   XMLRefInfo
 *   DatatypeValidator
 *   Grammar
 *   XSAnnotation
 *
 ***********************************************************/
void XTemplateSerializer::storeObject(RefHashTableOf<KVStringPair>* const objToStore
                                    , XSerializeEngine&                    serEng)                                          
{

    if (serEng.needToStoreObject(objToStore))
    {
        RefHashTableOfEnumerator<KVStringPair> e(objToStore, false, objToStore->getMemoryManager());
        int itemNumber = 0;        

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLCh*     key  = (XMLCh*) e.nextElementKey();
            serEng.writeString(key);

            KVStringPair* data = objToStore->get(key);
            serEng<<data;
        }
    }
}

void XTemplateSerializer::loadObject(RefHashTableOf<KVStringPair>** objToLoad
                                   , int                            initSize
                                   , bool                           toAdopt
                                   , XSerializeEngine&              serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             RefHashTableOf<KVStringPair>(
                                                          initSize
                                                        , toAdopt
                                                        , serEng.getMemoryManager()
                                                         );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {
            XMLCh*      key;
            serEng.readString(key);

            KVStringPair*  data;
            serEng>>data;
            
            (*objToLoad)->put((void*)key, data);        
        }
    }
}

void XTemplateSerializer::storeObject(RefHashTableOf<XMLAttDef>* const objToStore
                                    , XSerializeEngine&                serEng)                                          
{

    if (serEng.needToStoreObject(objToStore))
    {
        RefHashTableOfEnumerator<XMLAttDef> e(objToStore, false, objToStore->getMemoryManager());
        int itemNumber = 0;        

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLCh*     key  = (XMLCh*) e.nextElementKey();
            serEng.writeString(key);

            XMLAttDef* data = objToStore->get(key);
            serEng<<data;
        }
    }
}

void XTemplateSerializer::loadObject(RefHashTableOf<XMLAttDef>** objToLoad
                                   , int                         initSize
                                   , bool                        toAdopt
                                   , XSerializeEngine&           serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             RefHashTableOf<XMLAttDef>(
                                                       initSize
                                                     , toAdopt
                                                     , serEng.getMemoryManager()
                                                      );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {
            XMLCh*      key;
            serEng.readString(key);

            //This is used solely by SchemaGrammar and by all means it must be
            //SchemaAttDef, ideally we may change it to RefHashTableOf<SchemaAttDef>
            //later on.
            //Affected files IGXMLScanner, SGXMLScanner, SchemaGrammar, TraverseSchema
            //XMLAttDef*  data;
            SchemaAttDef*  data;
            serEng>>data;
            
            (*objToLoad)->put((void*)key, data);        
        }
    }
}

void XTemplateSerializer::storeObject(RefHashTableOf<DTDAttDef>* const objToStore
                                    , XSerializeEngine&                serEng)                                          
{

    if (serEng.needToStoreObject(objToStore))
    {
        RefHashTableOfEnumerator<DTDAttDef> e(objToStore, false, objToStore->getMemoryManager());
        int itemNumber = 0;        

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLCh*     key  = (XMLCh*) e.nextElementKey();
            serEng.writeString(key);

            DTDAttDef* data = objToStore->get(key);
            serEng<<data;
        }
    }
}

void XTemplateSerializer::loadObject(RefHashTableOf<DTDAttDef>** objToLoad
                                   , int                         initSize
                                   , bool                        toAdopt
                                   , XSerializeEngine&           serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             RefHashTableOf<DTDAttDef>(
                                                       initSize
                                                     , toAdopt
                                                     , serEng.getMemoryManager()
                                                      );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {
            XMLCh*      key;
            serEng.readString(key);

            DTDAttDef*  data;
            serEng>>data;
            
            //key==data->getFullName()
            (*objToLoad)->put((void*)key, data);        
        }
    }
}

void XTemplateSerializer::storeObject(RefHashTableOf<ComplexTypeInfo>* const objToStore
                                    , XSerializeEngine&                      serEng)                                          
{

    if (serEng.needToStoreObject(objToStore))
    {
        RefHashTableOfEnumerator<ComplexTypeInfo> e(objToStore, false, objToStore->getMemoryManager());
        int itemNumber = 0;        

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLCh*     key  = (XMLCh*) e.nextElementKey();
            serEng.writeString(key);

            ComplexTypeInfo* data = objToStore->get(key);
            serEng<<data;
        }
    }
}

void XTemplateSerializer::loadObject(RefHashTableOf<ComplexTypeInfo>** objToLoad
                                   , int                               initSize
                                   , bool                              toAdopt
                                   , XSerializeEngine&                 serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             RefHashTableOf<ComplexTypeInfo>(
                                                             initSize
                                                           , toAdopt
                                                           , serEng.getMemoryManager()
                                                           );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {
            XMLCh*      key;
            serEng.readString(key);

            ComplexTypeInfo*  data;
            serEng>>data;

            //key==data->getTypeName()    
            (*objToLoad)->put((void*)key, data);
        }
    }
}

void XTemplateSerializer::storeObject(RefHashTableOf<XercesGroupInfo>* const objToStore
                                    , XSerializeEngine&                      serEng)                                          
{

    if (serEng.needToStoreObject(objToStore))
    {
        RefHashTableOfEnumerator<XercesGroupInfo> e(objToStore, false, objToStore->getMemoryManager());
        int itemNumber = 0;        

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLCh*     key  = (XMLCh*) e.nextElementKey();
            serEng.writeString(key);

            XercesGroupInfo* data = objToStore->get(key);
            serEng<<data;
        }
    }
}

void XTemplateSerializer::loadObject(RefHashTableOf<XercesGroupInfo>** objToLoad
                                   , int                               initSize
                                   , bool                              toAdopt
                                   , XSerializeEngine&                 serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             RefHashTableOf<XercesGroupInfo>(
                                                             initSize
                                                           , toAdopt
                                                           , serEng.getMemoryManager()
                                                           );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {
            XMLCh*      key;
            serEng.readString(key);

            XercesGroupInfo*  data;
            serEng>>data;

            (*objToLoad)->put((void*)key, data);
        }
    }
}

void XTemplateSerializer::storeObject(RefHashTableOf<XercesAttGroupInfo>* const objToStore
                                    , XSerializeEngine&                         serEng)                                          
{

    if (serEng.needToStoreObject(objToStore))
    {
        RefHashTableOfEnumerator<XercesAttGroupInfo> e(objToStore, false, objToStore->getMemoryManager());
        int itemNumber = 0;        

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLCh*     key  = (XMLCh*) e.nextElementKey();
            serEng.writeString(key);

            XercesAttGroupInfo* data = objToStore->get(key);
            serEng<<data;
        }
    }
}

void XTemplateSerializer::loadObject(RefHashTableOf<XercesAttGroupInfo>** objToLoad
                                   , int                                  initSize
                                   , bool                                 toAdopt
                                   , XSerializeEngine&                    serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             RefHashTableOf<XercesAttGroupInfo>(
                                                                initSize
                                                              , toAdopt
                                                              , serEng.getMemoryManager()
                                                              );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {
            XMLCh*      key;
            serEng.readString(key);

            XercesAttGroupInfo*  data;
            serEng>>data;

            (*objToLoad)->put((void*)key, data);
        }
    }
}

void XTemplateSerializer::storeObject(RefHashTableOf<XMLRefInfo>* const objToStore
                                    , XSerializeEngine&                 serEng)                                          
{

    if (serEng.needToStoreObject(objToStore))
    {
        RefHashTableOfEnumerator<XMLRefInfo> e(objToStore, false, objToStore->getMemoryManager());
        int itemNumber = 0;        

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLCh*     key  = (XMLCh*) e.nextElementKey();
            serEng.writeString(key);

            XMLRefInfo* data = objToStore->get(key);
            serEng<<data;
        }
    }
}

void XTemplateSerializer::loadObject(RefHashTableOf<XMLRefInfo>** objToLoad
                                   , int                          initSize
                                   , bool                         toAdopt
                                   , XSerializeEngine&            serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             RefHashTableOf<XMLRefInfo>(
                                                        initSize
                                                      , toAdopt
                                                      , serEng.getMemoryManager()
                                                      );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {
            XMLCh*      key;
            serEng.readString(key);

            XMLRefInfo*  data;
            serEng>>data;

            (*objToLoad)->put((void*)key, data);
        }
    }
}

void XTemplateSerializer::storeObject(RefHashTableOf<DatatypeValidator>* const objToStore
                                    , XSerializeEngine&                        serEng)                                          
{

    if (serEng.needToStoreObject(objToStore))
    {
        RefHashTableOfEnumerator<DatatypeValidator> e(objToStore, false, objToStore->getMemoryManager());
        int itemNumber = 0;        

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLCh*     key  = (XMLCh*) e.nextElementKey();
            serEng.writeString(key);

            DatatypeValidator* data = objToStore->get(key);
            DatatypeValidator::storeDV(serEng, data);
        }
    }
}

void XTemplateSerializer::loadObject(RefHashTableOf<DatatypeValidator>** objToLoad
                                   , int                                 initSize
                                   , bool                                toAdopt
                                   , XSerializeEngine&                   serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             RefHashTableOf<DatatypeValidator>(
                                                               initSize
                                                             , toAdopt
                                                             , serEng.getMemoryManager()
                                                             );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {
            XMLCh*      key;
            serEng.readString(key);

            DatatypeValidator*  data;
            data = DatatypeValidator::loadDV(serEng);

            (*objToLoad)->put((void*)key, data);
        }
    }
}

void XTemplateSerializer::storeObject(RefHashTableOf<Grammar>* const objToStore
                                    , XSerializeEngine&              serEng)                                          
{

    if (serEng.needToStoreObject(objToStore))
    {
        RefHashTableOfEnumerator<Grammar> e(objToStore, false, objToStore->getMemoryManager());
        int itemNumber = 0;        

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLCh*     key  = (XMLCh*) e.nextElementKey();
            serEng.writeString(key);

            Grammar* data = objToStore->get(key);
            Grammar::storeGrammar(serEng, data);
        }
    }
}

void XTemplateSerializer::loadObject(RefHashTableOf<Grammar>** objToLoad
                                   , int                       initSize
                                   , bool                      toAdopt
                                   , XSerializeEngine&         serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             RefHashTableOf<Grammar>(
                                                     initSize
                                                   , toAdopt
                                                   , serEng.getMemoryManager()
                                                   );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {
            XMLCh*      key;
            serEng.readString(key);

            Grammar*  data;
            data = Grammar::loadGrammar(serEng);

            (*objToLoad)->put((void*)key, data);
        }
    }
}


void XTemplateSerializer::storeObject(RefHashTableOf<XSAnnotation>* const objToStore
                                    , XSerializeEngine&              serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        RefHashTableOfEnumerator<XSAnnotation> e(objToStore, false, objToStore->getMemoryManager());
        ValueVectorOf<XSerializeEngine::XSerializedObjectId_t> ids(16, serEng.getMemoryManager());
        ValueVectorOf<void*> keys(16, serEng.getMemoryManager());

        while (e.hasMoreElements())
        {
            void* key = e.nextElementKey();
            XSerializeEngine::XSerializedObjectId_t keyId = serEng.lookupStorePool(key);

            if (keyId)
            {
                ids.addElement(keyId);
                keys.addElement(key);
            }
        }

        int itemNumber = ids.size();
        serEng<<itemNumber;

        for (int i=0; i<itemNumber; i++)
        {
            XSerializeEngine::XSerializedObjectId_t keyId = ids.elementAt(i);
            XSAnnotation* data = objToStore->get(keys.elementAt(i));
            serEng<<keyId;
            serEng<<data;
        }
    }
}

void XTemplateSerializer::loadObject(RefHashTableOf<XSAnnotation>** objToLoad
                                   , int                            initSize
                                   , bool                           toAdopt
                                   , XSerializeEngine&              serEng)
{
    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 29;

            *objToLoad = new (serEng.getMemoryManager())
                RefHashTableOf<XSAnnotation>(
                    initSize
                    , toAdopt
                    , new (serEng.getMemoryManager()) HashPtr()
                    , serEng.getMemoryManager()
                );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {
            XSerializeEngine::XSerializedObjectId_t keyId = 0;
            serEng>>keyId;

            void* key = serEng.lookupLoadPool(keyId);
            XSAnnotation*  data;
            serEng>>data;

            (*objToLoad)->put(key, data);
        }
    }
}

/**********************************************************
 *
 * RefHash2KeysTableOf
 *
 *   SchemaAttDef
 *   ElemVector
 *
 ***********************************************************/
void XTemplateSerializer::storeObject(RefHash2KeysTableOf<SchemaAttDef>* const objToStore
                                    , XSerializeEngine&                        serEng)
{ 

    if (serEng.needToStoreObject(objToStore))
    {
        int itemNumber = 0;

        RefHash2KeysTableOfEnumerator<SchemaAttDef> e(objToStore, false, objToStore->getMemoryManager());

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLCh*     key1;
            int        key2;
            
            e.nextElementKey((void*&)key1, key2);
            serEng.writeString(key1);
            serEng<<key2;

            SchemaAttDef* data = objToStore->get(key1, key2);
            serEng<<data;

        }

    }

}

void XTemplateSerializer::loadObject(RefHash2KeysTableOf<SchemaAttDef>** objToLoad
                                   , int                                 initSize
                                   , bool                                toAdopt
                                   , XSerializeEngine&                   serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             RefHash2KeysTableOf<SchemaAttDef>(
                                                               initSize
                                                             , toAdopt
                                                             , serEng.getMemoryManager()
                                                             );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {               
            XMLCh*      key1;
            serEng.readString(key1);

            int         key2;
            serEng>>key2;

            SchemaAttDef*  data;
            serEng>>data;

            //key1==data->getAttName()->getLocalPart()
            //key2==data->getId()
            (*objToLoad)->put((void*)key1, key2, data);        

        }

    }

}

void XTemplateSerializer::storeObject(RefHash2KeysTableOf<ElemVector>* const objToStore
                                    , XSerializeEngine&                      serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int itemNumber = 0;

        RefHash2KeysTableOfEnumerator<ElemVector> e(objToStore, false, objToStore->getMemoryManager());

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLCh*     key1;
            int        key2;
            
            e.nextElementKey((void*&)key1, key2);
            serEng.writeString(key1);
            serEng<<key2;

            ElemVector* data = objToStore->get(key1, key2);

            storeObject(data, serEng);

        }

    }

}

void XTemplateSerializer::loadObject(RefHash2KeysTableOf<ElemVector>**      objToLoad
                                   , int                                     initSize
                                   , bool                                    toAdopt
                                   , XSerializeEngine&                       serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             RefHash2KeysTableOf<ElemVector>(
                                                               initSize
                                                             , toAdopt
                                                             , serEng.getMemoryManager()
                                                             );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {               
            XMLCh*      key1;
            serEng.readString(key1);

            int         key2;
            serEng>>key2;

            ElemVector*  data = 0;

            //don't call destructor
            loadObject(&data, 8, false, serEng);

            (*objToLoad)->put((void*)key1, key2, data);        

        }

    }
}

/**********************************************************
 *
 * RefHash3KeysIdPool
 *
 *   SchemaElementDecl
 *
 ***********************************************************/
void XTemplateSerializer::storeObject(RefHash3KeysIdPool<SchemaElementDecl>* const objToStore
                                    , XSerializeEngine&                            serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int itemNumber = 0;
        RefHash3KeysIdPoolEnumerator<SchemaElementDecl> e(objToStore, false, objToStore->getMemoryManager());

        XMLCh*     key1;
        int        key2;
        int        key3;

        while (e.hasMoreKeys())
        {
            e.nextElementKey((void*&)key1, key2, key3);
            itemNumber++;
        }

        serEng<<itemNumber;
        e.resetKey();

        while (e.hasMoreKeys())
        {           
            e.nextElementKey((void*&)key1, key2, key3);
            serEng.writeString(key1);
            serEng<<key2;
            serEng<<key3;

            SchemaElementDecl* data = objToStore->getByKey(key1, key2, key3);
            serEng<<data;
        }

    }

}

void XTemplateSerializer::loadObject(RefHash3KeysIdPool<SchemaElementDecl>** objToLoad
                                   , int                                     initSize
                                   , bool                                    toAdopt
                                   , int                                     initSize2
                                   , XSerializeEngine&                       serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             RefHash3KeysIdPool<SchemaElementDecl>(
                                                                   initSize
                                                                 , toAdopt
                                                                 , initSize2
                                                                 , serEng.getMemoryManager());
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        XMLCh*     key1;
        int        key2;
        int        key3;
        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {                       
            serEng.readString(key1);
            serEng>>key2;
            serEng>>key3;

            SchemaElementDecl*  elemDecl;
            serEng>>elemDecl;

            (*objToLoad)->put(key1
                            , key2
                            , key3
                            , elemDecl);

        }
   
    }

}

/**********************************************************
 *
 * NameIdPool             
 *    no NameIdPool::nextElementKey()
 *
 *   DTDElementDecl
 *   DTDEntityDecl
 *   XMLNotationDecl
 *
 ***********************************************************/
void XTemplateSerializer::storeObject(NameIdPool<DTDElementDecl>* const objToStore
                                    , XSerializeEngine&                 serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int itemNumber = 0;
        NameIdPoolEnumerator<DTDElementDecl> e(objToStore, objToStore->getMemoryManager());

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            DTDElementDecl& data = e.nextElement();
            data.serialize(serEng);
        }

    }

}

void XTemplateSerializer::loadObject(NameIdPool<DTDElementDecl>** objToLoad
                                   , int                          initSize
                                   , int                          initSize2
                                   , XSerializeEngine&            serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager())
                             NameIdPool<DTDElementDecl>(
                                                        initSize
                                                      , initSize2
                                                      , serEng.getMemoryManager()
                                                      );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {               
            DTDElementDecl*  data = new (serEng.getMemoryManager())
                                    DTDElementDecl(serEng.getMemoryManager());
            data->serialize(serEng);
            (*objToLoad)->put(data);
        }
   
    }

}

void XTemplateSerializer::storeObject(NameIdPool<DTDEntityDecl>* const objToStore
                                    , XSerializeEngine&                 serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int itemNumber = 0;
        NameIdPoolEnumerator<DTDEntityDecl> e(objToStore, objToStore->getMemoryManager());

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            DTDEntityDecl& data = e.nextElement();
            data.serialize(serEng);
        }

    }

}

void XTemplateSerializer::loadObject(NameIdPool<DTDEntityDecl>** objToLoad
                                   , int                          initSize
                                   , int                          initSize2
                                   , XSerializeEngine&            serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             NameIdPool<DTDEntityDecl>(
                                                       initSize
                                                     , initSize2
                                                     , serEng.getMemoryManager()
                                                     );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {               
            DTDEntityDecl*  data = new (serEng.getMemoryManager()) 
                                   DTDEntityDecl(serEng.getMemoryManager());
            data->serialize(serEng);
            (*objToLoad)->put(data);
        }
   
    }

}

void XTemplateSerializer::storeObject(NameIdPool<XMLNotationDecl>* const objToStore
                                    , XSerializeEngine&                  serEng)
{

    if (serEng.needToStoreObject(objToStore))
    {
        int itemNumber = 0;
        NameIdPoolEnumerator<XMLNotationDecl> e(objToStore, objToStore->getMemoryManager());

        while (e.hasMoreElements())
        {
            e.nextElement();
            itemNumber++;
        }

        serEng<<itemNumber;
        e.Reset();

        while (e.hasMoreElements())
        {
            XMLNotationDecl& data = e.nextElement();
            data.serialize(serEng);
        }

    }

}

void XTemplateSerializer::loadObject(NameIdPool<XMLNotationDecl>** objToLoad
                                   , int                          initSize
                                   , int                          initSize2
                                   , XSerializeEngine&            serEng)
{

    if (serEng.needToLoadObject((void**)objToLoad))
    {
        if (!*objToLoad)
        {
            if (initSize < 0)
                initSize = 16;

            *objToLoad = new (serEng.getMemoryManager()) 
                             NameIdPool<XMLNotationDecl>(
                                                         initSize
                                                       , initSize2
                                                       , serEng.getMemoryManager()
                                                       );
        }

        serEng.registerObject(*objToLoad);

        int itemNumber = 0;
        serEng>>itemNumber;

        for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
        {               
            XMLNotationDecl*  data = new (serEng.getMemoryManager()) 
                                     XMLNotationDecl(serEng.getMemoryManager());
            data->serialize(serEng);
            (*objToLoad)->put(data);
        }
   
    }

}

XERCES_CPP_NAMESPACE_END

