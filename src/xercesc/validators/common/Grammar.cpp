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
 * $Id$
 * $Log$
 * Revision 1.4  2004/09/08 13:56:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2004/01/29 11:51:21  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.2  2003/10/29 16:19:47  peiyongz
 * storeGrammar()/loadGrammar added
 *
 * Revision 1.1  2003/10/14 15:19:24  peiyongz
 * Implementation of Serialization/Deserialization
 *
 */

#include <xercesc/validators/common/Grammar.hpp>

//since we need to dynamically created each and every derivatives 
//during deserialization by XSerializeEngine>>Derivative, we got
//to include all hpp

#include <xercesc/validators/DTD/DTDGrammar.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_NOCREATE(Grammar)

void Grammar::serialize(XSerializeEngine&)
{
    //no data
}

void Grammar::storeGrammar(XSerializeEngine&   serEng
                         , Grammar*  const     grammar)
{
    if (grammar)
    {
        serEng<<(int) grammar->getGrammarType();
        serEng<<grammar;
    }
    else
    {
        serEng<<(int) UnKnown;
    }

}

Grammar* Grammar::loadGrammar(XSerializeEngine& serEng)
{

    int type;
    serEng>>type;

    switch((GrammarType)type)
    {
    case DTDGrammarType: 
        DTDGrammar* dtdGrammar;
        serEng>>dtdGrammar;
        return dtdGrammar;
    case SchemaGrammarType:
        SchemaGrammar* schemaGrammar;
        serEng>>schemaGrammar;
        return schemaGrammar;        
    case UnKnown:
        return 0;        
    default: //we treat this same as UnKnown
        return 0;        
    }

}

XERCES_CPP_NAMESPACE_END
