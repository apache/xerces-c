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

/**
 * $Id$
 * $Log$
 * Revision 1.3  2004/09/08 13:55:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.2  2004/01/29 11:46:29  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.1  2003/10/10 16:23:29  peiyongz
 * Implementation of Serialization/Deserialization
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/XMLAttDefList.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_NOCREATE(XMLAttDefList)

void XMLAttDefList::serialize(XSerializeEngine&)
{
    //no data
}

XERCES_CPP_NAMESPACE_END

