/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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

/**
 * $Log$
 * Revision 1.1  1999/11/09 01:08:28  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:44:35  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLUni.hpp>
#include <util/XMLString.hpp>
#include <framework/XMLAttr.hpp>



// ---------------------------------------------------------------------------
//  XMLAttr: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLAttr::XMLAttr() :

    fName(0)
    , fPrefix(0)
    , fQName(0)
    , fType(XMLAttDef::CData)
    , fValue(0)
    , fSpecified(false)
    , fURIId(0)
{
}

XMLAttr::XMLAttr(   const   unsigned int        uriId
                    , const XMLCh* const        attrName
                    , const XMLCh* const        attrPrefix
                    , const XMLCh* const        attrValue
                    , const XMLAttDef::AttTypes type
                    , const bool                specified) :

    fName(0)
    , fPrefix(0)
    , fQName(0)
    , fType(type)
    , fValue(0)
    , fSpecified(specified)
    , fURIId(0)
{
    try
    {
        set(uriId, attrName, attrPrefix, attrValue);
    }

    catch(...)
    {
        cleanUp();
    }
}


// ---------------------------------------------------------------------------
//  XMLAttr: Getter methods
// ---------------------------------------------------------------------------
const XMLCh* XMLAttr::getQName() const
{
    // Fault in the QName if not already done
    if (!fQName)
    {
        if (*fPrefix)
        {
            const unsigned int len = XMLString::stringLen(fPrefix)
                                     + XMLString::stringLen(fName)
                                     + 1;
            const XMLCh colonStr[] = { chColon, chNull };

            // We are faulting this guy in, so cast of const'ness
            ((XMLAttr*)this)->fQName = new XMLCh[len + 1];
            XMLString::copyString(fQName, fPrefix);
            XMLString::catString(fQName, colonStr);
            XMLString::catString(fQName, fName);
        }
         else
        {
            // We are faulting this guy in, so cast of const'ness
            ((XMLAttr*)this)->fQName = XMLString::replicate(fName);
        }
    }
    return fQName;
}


// ---------------------------------------------------------------------------
//  XMLAttr: Setter methods
// ---------------------------------------------------------------------------
void XMLAttr::set(  const   unsigned int        uriId
                    , const XMLCh* const        attrName
                    , const XMLCh* const        attrPrefix
                    , const XMLCh* const        attrValue
                    , const XMLAttDef::AttTypes type)
{
    // Clean up the old stuff
    delete [] fName;
    fName = 0;
    delete [] fPrefix;
    fPrefix = 0;
    delete [] fValue;
    fValue = 0;

    // And clean up the QName and leave it undone until asked for
    delete [] fQName;
    fQName = 0;

    // And now replicate the stuff into our members
    fType = type;
    fURIId = uriId;
    fName = XMLString::replicate(attrName);
    fPrefix = XMLString::replicate(attrPrefix);
    fValue = XMLString::replicate(attrValue);
}


void XMLAttr::setName(  const   unsigned int    uriId
                        , const XMLCh* const    attrName
                        , const XMLCh* const    attrPrefix)
{
    // Clean up the old stuff
    delete [] fName;
    fName = 0;
    delete [] fPrefix;
    fPrefix = 0;

    // And clean up the QName and leave it undone until asked for
    delete [] fQName;
    fQName = 0;

    // And now replicate the stuff into our members
    fURIId = uriId;
    fName = XMLString::replicate(attrName);
    fPrefix = XMLString::replicate(attrPrefix);
}


void XMLAttr::setURIId(const unsigned int uriId)
{
    fURIId = uriId;
}


void XMLAttr::setValue(const XMLCh* const newValue)
{
    delete [] fValue;
    fValue = XMLString::replicate(newValue);
}


// ---------------------------------------------------------------------------
//  XMLAttr: Private, helper methods
// ---------------------------------------------------------------------------
void XMLAttr::cleanUp()
{
    delete [] fName;
    delete [] fQName;
    delete [] fValue;
}
