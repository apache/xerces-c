/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.1  2001/05/16 15:25:38  tng
 * Schema: Add Base64 and HexBin.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/HexBin.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/XMLString.hpp>

// ---------------------------------------------------------------------------
//  constants
// ---------------------------------------------------------------------------
static const int BASELENGTH = 255;       

// ---------------------------------------------------------------------------
//  class data member
// ---------------------------------------------------------------------------
bool HexBin::hexNumberTable[BASELENGTH]; 
bool HexBin::isInitialized = false;

int HexBin::getDataLength(const XMLCh* const hexData)
{
    if (!isArrayByteHex(hexData))
        return -1;

    return XMLString::stringLen(hexData)/2;
}

bool HexBin::isArrayByteHex(const XMLCh* const hexData)
{
    if ( !isInitialized )
        init();

    if (( hexData == 0 ) || ( *hexData == 0 )) // zero length
        return false;

    int strLen = XMLString::stringLen(hexData);
    if ( strLen%2 != 0 )
        return false;

    for ( int i = 0; i < strLen; i++ )
        if( !isHex(hexData[i]) ) 
            return false;

    return true;
}

// -----------------------------------------------------------------------
//  Helper methods
// -----------------------------------------------------------------------
bool HexBin::isHex(const XMLCh& octect) 
{
    // sanity check to avoid out-of-bound index
    if (( octect >= BASELENGTH ) || ( octect < 0 ))
        return false;

    return (hexNumberTable[octect]);
}

void HexBin::init()
{
    if ( isInitialized )
        return;

    int i;
    for ( i = 0; i < BASELENGTH; i++ ) 
        hexNumberTable[i] = false; 

    for ( i = chDigit_9; i >= chDigit_0; i-- )
        hexNumberTable[i] = true;

    for ( i = chLatin_F; i >= chLatin_A; i-- ) 
        hexNumberTable[i] = true;

    for ( i = chLatin_f; i >= chLatin_a; i-- ) 
        hexNumberTable[i] = true;

    isInitialized = true;
}
