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

/**
 * $Log$
 * Revision 1.3  2004/09/08 13:55:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.2  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:21:51  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/05/28 20:53:35  tng
 * Schema: move static data gInvalidTrans, gEOCFakeId, gEpsilonFakeId to XMLContentModel for others to access
 *
 * Revision 1.2  2000/02/06 07:47:47  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:30  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:37  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/XMLContentModel.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  public static data
// ---------------------------------------------------------------------------
const unsigned int   XMLContentModel::gInvalidTrans   = 0xFFFFFFFF;
const unsigned int   XMLContentModel::gEOCFakeId      = 0xFFFFFFF1;
const unsigned int   XMLContentModel::gEpsilonFakeId  = 0xFFFFFFF2;

XERCES_CPP_NAMESPACE_END

