/*
 * Copyright 2001-2002,2004 The Apache Software Foundation.
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
 */

#if !defined(REGXDEFS_HPP)
#define REGXDEFS_HPP

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

const XMLCh fgXMLCategory[] =
{
    chLatin_X, chLatin_M, chLatin_L, chNull
};

const XMLCh fgASCIICategory[] =
{
    chLatin_A, chLatin_S, chLatin_C, chLatin_I, chLatin_I, chNull
};

const XMLCh fgUnicodeCategory[] =
{
    chLatin_U, chLatin_N, chLatin_I, chLatin_C, chLatin_O, chLatin_D,
    chLatin_E, chNull
};

const XMLCh fgBlockCategory[] =
{
    chLatin_B, chLatin_L, chLatin_O, chLatin_C, chLatin_K, chNull
};

const XMLCh fgXMLSpace[] =
{
    chLatin_x, chLatin_m, chLatin_l, chColon, chLatin_i, chLatin_s, chLatin_S,
    chLatin_p, chLatin_a, chLatin_c, chLatin_e, chNull
};

const XMLCh fgXMLDigit[] =
{
    chLatin_x, chLatin_m, chLatin_l, chColon, chLatin_i, chLatin_s, chLatin_D,
    chLatin_i, chLatin_g, chLatin_i, chLatin_t, chNull
};

const XMLCh fgXMLWord[] =
{
    chLatin_x, chLatin_m, chLatin_l, chColon, chLatin_i, chLatin_s, chLatin_W,
    chLatin_o, chLatin_r, chLatin_d, chNull
};

const XMLCh fgXMLNameChar[] =
{
    chLatin_x, chLatin_m, chLatin_l, chColon, chLatin_i, chLatin_s, chLatin_N,
    chLatin_a, chLatin_m, chLatin_e, chLatin_C, chLatin_h, chLatin_a,
	chLatin_r, chNull
};

const XMLCh fgXMLInitialNameChar[] =
{
    chLatin_x, chLatin_m, chLatin_l, chColon, chLatin_i, chLatin_s, chLatin_I,
    chLatin_n, chLatin_i, chLatin_t, chLatin_i, chLatin_a, chLatin_l,
    chLatin_N, chLatin_a, chLatin_m, chLatin_e, chLatin_C, chLatin_h,
    chLatin_a, chLatin_r, chNull
};

const XMLCh fgASCII[] =
{
    chLatin_a, chLatin_s, chLatin_c, chLatin_i, chLatin_i, chColon, chLatin_i,
    chLatin_s, chLatin_A, chLatin_s, chLatin_c, chLatin_i, chLatin_i, chNull
};

const XMLCh fgASCIIDigit[] =
{
    chLatin_a, chLatin_s, chLatin_c, chLatin_i, chLatin_i, chColon, chLatin_i,
    chLatin_s, chLatin_D, chLatin_i, chLatin_g, chLatin_i, chLatin_t, chNull
};

const XMLCh fgASCIIWord[] =
{
    chLatin_a, chLatin_s, chLatin_c, chLatin_i, chLatin_i, chColon, chLatin_i,
    chLatin_s, chLatin_W, chLatin_o, chLatin_r, chLatin_d, chNull
};

const XMLCh fgASCIISpace[] =
{
    chLatin_a, chLatin_s, chLatin_c, chLatin_i, chLatin_i, chColon, chLatin_i,
    chLatin_s, chLatin_S, chLatin_p, chLatin_a, chLatin_c, chLatin_e, chNull
};

const XMLCh fgASCIIXDigit[] =
{
    chLatin_a, chLatin_s, chLatin_c, chLatin_i, chLatin_i, chColon, chLatin_i,
    chLatin_s, chLatin_X, chLatin_D, chLatin_i, chLatin_g, chLatin_i,
    chLatin_t, chNull
};


const XMLCh fgUniAll[] =
{
    chLatin_A, chLatin_L, chLatin_L, chNull
};

const XMLCh fgUniIsAlpha[] =
{
    chLatin_I, chLatin_s, chLatin_A, chLatin_l, chLatin_p, chLatin_h,
    chLatin_a, chNull
};

const XMLCh fgUniIsAlnum[] =
{
    chLatin_I, chLatin_s, chLatin_A, chLatin_l, chLatin_n, chLatin_u,
    chLatin_m, chNull
};

const XMLCh fgUniIsWord[] =
{
    chLatin_I, chLatin_s, chLatin_W, chLatin_o, chLatin_r, chLatin_d,
    chNull
};


const XMLCh fgUniIsDigit[] =
{
    chLatin_I, chLatin_s, chLatin_D, chLatin_i, chLatin_g, chLatin_i,
    chLatin_t, chNull
};

const XMLCh fgUniIsUpper[] =
{
    chLatin_I, chLatin_s, chLatin_U, chLatin_p, chLatin_p, chLatin_e,
    chLatin_r, chNull
};

const XMLCh fgUniIsLower[] =
{
    chLatin_I, chLatin_s, chLatin_L, chLatin_o, chLatin_w, chLatin_e,
    chLatin_r, chNull
};

const XMLCh fgUniIsPunct[] =
{
    chLatin_I, chLatin_s, chLatin_P, chLatin_u, chLatin_n, chLatin_c,
    chLatin_t, chNull
};

const XMLCh fgUniIsSpace[] =
{
	chLatin_I, chLatin_s, chLatin_S, chLatin_p, chLatin_a, chLatin_c,
    chLatin_e, chNull
};

const XMLCh fgUniAssigned[] =
{
    chLatin_A, chLatin_S, chLatin_S, chLatin_I, chLatin_G, chLatin_N,
    chLatin_E, chLatin_D, chNull
};


const XMLCh fgUniDecimalDigit[] =
{
    chLatin_N, chLatin_d, chNull
};

const XMLCh fgBlockIsSpecials[] =
{
    chLatin_I, chLatin_s, chLatin_S, chLatin_p, chLatin_e, chLatin_c, chLatin_i, chLatin_a,
    chLatin_l, chLatin_s, chNull
};

const XMLCh fgBlockIsPrivateUse[] =
{
    chLatin_I, chLatin_s, chLatin_P, chLatin_r, chLatin_i, chLatin_v, chLatin_a, chLatin_t, chLatin_e,
    chLatin_U, chLatin_s, chLatin_e,  chNull
};

const XMLCh fgUniLetter[] =
{
    chLatin_L, chNull
};

const XMLCh fgUniNumber[] =
{
    chLatin_N, chNull
};

const XMLCh fgUniMark[] =
{
    chLatin_M, chNull
};

const XMLCh fgUniSeparator[] =
{
    chLatin_Z, chNull
};

const XMLCh fgUniPunctuation[] =
{
    chLatin_P, chNull
};

const XMLCh fgUniControl[] =
{
    chLatin_C, chNull
};

const XMLCh fgUniSymbol[] =
{
    chLatin_S, chNull
};

XERCES_CPP_NAMESPACE_END

#endif

/**
  * End of file RegxDefs.hpp
  */

