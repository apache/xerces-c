/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2002 The Apache Software Foundation.  All rights
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
 * Revision 1.2  2002/02/05 13:20:06  tng
 * [Bug 5716] Can't parse with Validation more than one file.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:28  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/11/09 21:27:21  tng
 * Schema Regx Fix: manually add the extra specials and private use block range.
 *
 * Revision 1.5  2001/11/09 18:10:09  tng
 * Schema Regx: Block name 'X' should begin with 'IsX'.  And add missing block names and range.
 *
 * Revision 1.4  2001/06/22 16:26:27  peiyongz
 * fix: redefinition of SIZE
 *
 * Revision 1.3  2001/05/11 13:26:40  tng
 * Copyright update.
 *
 * Revision 1.2  2001/05/03 18:17:21  knoaman
 * Some design changes:
 * o Changed the TokenFactory from a single static instance, to a
 *    normal class. Each RegularExpression object will have its own
 *    instance of TokenFactory, and that instance will be passed to
 *    other classes that need to use a TokenFactory to create Token
 *    objects (with the exception of RangeTokenMap).
 * o Added a new class RangeTokenMap to map a the different ranges
 *    in a given category to a specific RangeFactory object. In the old
 *    design RangeFactory had dual functionality (act as a Map, and as
 *    a factory for creating RangeToken(s)). The RangeTokenMap will
 *    have its own copy of the TokenFactory. There will be only one
 *    instance of the RangeTokenMap class, and that instance will be
 *    lazily deleted when XPlatformUtils::Terminate is called.
 *
 * Revision 1.1  2001/03/02 19:26:41  knoaman
 * Schema: Regular expression handling part II
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/BlockRangeFactory.hpp>
#include <xercesc/util/regx/RangeToken.hpp>
#include <xercesc/util/regx/RegxDefs.hpp>
#include <xercesc/util/regx/TokenFactory.hpp>
#include <xercesc/util/regx/RangeTokenMap.hpp>
#include <xercesc/util/XMLString.hpp>


// ---------------------------------------------------------------------------
//  Local static data
// ---------------------------------------------------------------------------
const int   BLOCKNAMESIZE = 96;

// Block Names IsX
// only define Specials as FEFF..FEFF, missing Specials as FFF0..FFFD, add manually
// only define private use as E000..F8FF,
//    missing 2 private use (F0000..FFFFD and 100000..10FFFD), add manually
const XMLCh fgBlockNames[][BLOCKNAMESIZE] =
{
    { chLatin_I, chLatin_s, chLatin_B, chLatin_a, chLatin_s, chLatin_i, chLatin_c, chLatin_L, chLatin_a,
      chLatin_t, chLatin_i, chLatin_n,  chNull },
    { chLatin_I, chLatin_s, chLatin_L, chLatin_a, chLatin_t, chLatin_i, chLatin_n, chDash, chDigit_1,
      chLatin_S, chLatin_u, chLatin_p, chLatin_p, chLatin_l, chLatin_e, chLatin_m, chLatin_e,
      chLatin_n, chLatin_t,  chNull },
    { chLatin_I, chLatin_s, chLatin_L, chLatin_a, chLatin_t, chLatin_i, chLatin_n, chLatin_E, chLatin_x,
      chLatin_t, chLatin_e, chLatin_n, chLatin_d, chLatin_e, chLatin_d, chDash, chLatin_A,
       chNull },
    { chLatin_I, chLatin_s, chLatin_L, chLatin_a, chLatin_t, chLatin_i, chLatin_n, chLatin_E, chLatin_x,
      chLatin_t, chLatin_e, chLatin_n, chLatin_d, chLatin_e, chLatin_d, chDash, chLatin_B,
       chNull },
    { chLatin_I, chLatin_s, chLatin_I, chLatin_P, chLatin_A, chLatin_E, chLatin_x, chLatin_t, chLatin_e,
      chLatin_n, chLatin_s, chLatin_i, chLatin_o, chLatin_n, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_S, chLatin_p, chLatin_a, chLatin_c, chLatin_i, chLatin_n, chLatin_g,
      chLatin_M, chLatin_o, chLatin_d, chLatin_i, chLatin_f, chLatin_i, chLatin_e, chLatin_r,
      chLatin_L, chLatin_e, chLatin_t, chLatin_t, chLatin_e, chLatin_r, chLatin_s,
       chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_o, chLatin_m, chLatin_b, chLatin_i, chLatin_n, chLatin_i, chLatin_n,
      chLatin_g, chLatin_D, chLatin_i, chLatin_a, chLatin_c, chLatin_r, chLatin_i,
      chLatin_t, chLatin_i, chLatin_c, chLatin_a, chLatin_l, chLatin_M, chLatin_a,
      chLatin_r, chLatin_k, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_G, chLatin_r, chLatin_e, chLatin_e, chLatin_k,  chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_y, chLatin_r, chLatin_i, chLatin_l, chLatin_l, chLatin_i, chLatin_c,
       chNull },
    { chLatin_I, chLatin_s, chLatin_A, chLatin_r, chLatin_m, chLatin_e, chLatin_n, chLatin_i, chLatin_a, chLatin_n,
       chNull },
    { chLatin_I, chLatin_s, chLatin_H, chLatin_e, chLatin_b, chLatin_r, chLatin_e, chLatin_w,  chNull },
    { chLatin_I, chLatin_s, chLatin_A, chLatin_r, chLatin_a, chLatin_b, chLatin_i, chLatin_c,  chNull },
    { chLatin_I, chLatin_s, chLatin_S, chLatin_y, chLatin_r, chLatin_i, chLatin_a, chLatin_c,  chNull },
    { chLatin_I, chLatin_s, chLatin_T, chLatin_h, chLatin_a, chLatin_a, chLatin_n, chLatin_a,  chNull },
    { chLatin_I, chLatin_s, chLatin_D, chLatin_e, chLatin_v, chLatin_a, chLatin_n, chLatin_a, chLatin_g, chLatin_a,
      chLatin_r, chLatin_i,  chNull },
    { chLatin_I, chLatin_s, chLatin_B, chLatin_e, chLatin_n, chLatin_g, chLatin_a, chLatin_l, chLatin_i,  chNull },
    { chLatin_I, chLatin_s, chLatin_G, chLatin_u, chLatin_r, chLatin_m, chLatin_u, chLatin_k, chLatin_h, chLatin_i,
       chNull },
    { chLatin_I, chLatin_s, chLatin_G, chLatin_u, chLatin_j, chLatin_a, chLatin_r, chLatin_a, chLatin_t, chLatin_i,
       chNull },
    { chLatin_I, chLatin_s, chLatin_O, chLatin_r, chLatin_i, chLatin_y, chLatin_a,  chNull },
    { chLatin_I, chLatin_s, chLatin_T, chLatin_a, chLatin_m, chLatin_i, chLatin_l,  chNull },
    { chLatin_I, chLatin_s, chLatin_T, chLatin_e, chLatin_l, chLatin_u, chLatin_g, chLatin_u,  chNull },
    { chLatin_I, chLatin_s, chLatin_K, chLatin_a, chLatin_n, chLatin_n, chLatin_a, chLatin_d, chLatin_a,  chNull },
    { chLatin_I, chLatin_s, chLatin_M, chLatin_a, chLatin_l, chLatin_a, chLatin_y, chLatin_a, chLatin_l, chLatin_a,
      chLatin_m,  chNull },
    { chLatin_I, chLatin_s, chLatin_S, chLatin_i, chLatin_n, chLatin_h, chLatin_a, chLatin_l, chLatin_a,  chNull },
    { chLatin_I, chLatin_s, chLatin_T, chLatin_h, chLatin_a, chLatin_i,  chNull },
    { chLatin_I, chLatin_s, chLatin_L, chLatin_a, chLatin_o,  chNull },
    { chLatin_I, chLatin_s, chLatin_T, chLatin_i, chLatin_b, chLatin_e, chLatin_t, chLatin_a, chLatin_n,  chNull },
    { chLatin_I, chLatin_s, chLatin_M, chLatin_y, chLatin_a, chLatin_n, chLatin_m, chLatin_a, chLatin_r,  chNull },
    { chLatin_I, chLatin_s, chLatin_G, chLatin_e, chLatin_o, chLatin_r, chLatin_g, chLatin_i, chLatin_a, chLatin_n,
       chNull },
    { chLatin_I, chLatin_s, chLatin_H, chLatin_a, chLatin_n, chLatin_g, chLatin_u, chLatin_l, chLatin_J,
      chLatin_a, chLatin_m, chLatin_o,  chNull },
    { chLatin_I, chLatin_s, chLatin_E, chLatin_t, chLatin_h, chLatin_i, chLatin_o, chLatin_p, chLatin_i,  chLatin_c,
       chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_h, chLatin_e, chLatin_r, chLatin_o, chLatin_k, chLatin_e,  chLatin_e,
       chNull },
    { chLatin_I, chLatin_s, chLatin_U, chLatin_n, chLatin_i, chLatin_f, chLatin_i, chLatin_e, chLatin_d,
      chLatin_C, chLatin_a, chLatin_n, chLatin_a, chLatin_d, chLatin_i, chLatin_a, chLatin_n,
      chLatin_A, chLatin_b, chLatin_o, chLatin_r, chLatin_i, chLatin_g, chLatin_i, chLatin_n, chLatin_a, chLatin_l,
      chLatin_S, chLatin_y, chLatin_l, chLatin_l, chLatin_a, chLatin_b, chLatin_i, chLatin_c, chLatin_s, chNull },
    { chLatin_I, chLatin_s, chLatin_O, chLatin_g, chLatin_h, chLatin_a, chLatin_m, chNull },
    { chLatin_I, chLatin_s, chLatin_R, chLatin_u, chLatin_n, chLatin_i, chLatin_c, chNull },
    { chLatin_I, chLatin_s, chLatin_K, chLatin_h, chLatin_m, chLatin_e, chLatin_r, chNull },
    { chLatin_I, chLatin_s, chLatin_M, chLatin_o, chLatin_n, chLatin_g, chLatin_o, chLatin_l, chLatin_i,
      chLatin_a, chLatin_n, chNull },
    { chLatin_I, chLatin_s, chLatin_L, chLatin_a, chLatin_t, chLatin_i, chLatin_n, chLatin_E, chLatin_x,
      chLatin_t, chLatin_e, chLatin_n, chLatin_d, chLatin_e, chLatin_d, chLatin_A,
      chLatin_d, chLatin_d, chLatin_i, chLatin_t, chLatin_i, chLatin_o, chLatin_n, chLatin_a,
      chLatin_l,  chNull },
    { chLatin_I, chLatin_s, chLatin_G, chLatin_r, chLatin_e, chLatin_e, chLatin_k, chLatin_E, chLatin_x,
      chLatin_t, chLatin_e, chLatin_n, chLatin_d, chLatin_e, chLatin_d,  chNull },
    { chLatin_I, chLatin_s, chLatin_G, chLatin_e, chLatin_n, chLatin_e, chLatin_r, chLatin_a, chLatin_l,
      chLatin_P, chLatin_u, chLatin_n, chLatin_c, chLatin_t, chLatin_u, chLatin_a, chLatin_t,
      chLatin_i, chLatin_o, chLatin_n,  chNull },
    { chLatin_I, chLatin_s, chLatin_S, chLatin_u, chLatin_p, chLatin_e, chLatin_r, chLatin_s, chLatin_c, chLatin_r,
      chLatin_i, chLatin_p, chLatin_t, chLatin_s, chLatin_a, chLatin_n, chLatin_d,
      chLatin_S, chLatin_u, chLatin_b, chLatin_s, chLatin_c, chLatin_r, chLatin_i,
      chLatin_p, chLatin_t, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_u, chLatin_r, chLatin_r, chLatin_e, chLatin_n, chLatin_c, chLatin_y,
      chLatin_S, chLatin_y, chLatin_m, chLatin_b, chLatin_o, chLatin_l, chLatin_s,
       chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_o, chLatin_m, chLatin_b, chLatin_i, chLatin_n, chLatin_i, chLatin_n,
      chLatin_g, chLatin_M, chLatin_a, chLatin_r, chLatin_k, chLatin_s,
      chLatin_f, chLatin_o, chLatin_r, chLatin_S, chLatin_y, chLatin_m, chLatin_b,
      chLatin_o, chLatin_l, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_L, chLatin_e, chLatin_t, chLatin_t, chLatin_e, chLatin_r, chLatin_l, chLatin_i,
      chLatin_k, chLatin_e, chLatin_S, chLatin_y, chLatin_m, chLatin_b, chLatin_o,
      chLatin_l, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_N, chLatin_u, chLatin_m, chLatin_b, chLatin_e, chLatin_r, chLatin_F,
      chLatin_o, chLatin_r, chLatin_m, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_A, chLatin_r, chLatin_r, chLatin_o, chLatin_w, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_M, chLatin_a, chLatin_t, chLatin_h, chLatin_e, chLatin_m, chLatin_a, chLatin_t,
      chLatin_i, chLatin_c, chLatin_a, chLatin_l, chLatin_O, chLatin_p, chLatin_e,
      chLatin_r, chLatin_a, chLatin_t, chLatin_o, chLatin_r, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_M, chLatin_i, chLatin_s, chLatin_c, chLatin_e, chLatin_l, chLatin_l, chLatin_a,
      chLatin_n, chLatin_e, chLatin_o, chLatin_u, chLatin_s, chLatin_T, chLatin_e,
      chLatin_c, chLatin_h, chLatin_n, chLatin_i, chLatin_c, chLatin_a, chLatin_l,  chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_o, chLatin_n, chLatin_t, chLatin_r, chLatin_o, chLatin_l,
      chLatin_P, chLatin_i, chLatin_c, chLatin_t, chLatin_u, chLatin_r, chLatin_e, chLatin_s,
       chNull },
    { chLatin_I, chLatin_s, chLatin_O, chLatin_p, chLatin_t, chLatin_i, chLatin_c, chLatin_a, chLatin_l,
      chLatin_C, chLatin_h, chLatin_a, chLatin_r, chLatin_a, chLatin_c, chLatin_t, chLatin_e,
      chLatin_r, chLatin_R, chLatin_e, chLatin_c, chLatin_o, chLatin_g, chLatin_n,
      chLatin_i, chLatin_t, chLatin_i, chLatin_o, chLatin_n,  chNull },
    { chLatin_I, chLatin_s, chLatin_E, chLatin_n, chLatin_c, chLatin_l, chLatin_o, chLatin_s, chLatin_e, chLatin_d,
      chLatin_A, chLatin_l, chLatin_p, chLatin_h, chLatin_a, chLatin_n, chLatin_u,
      chLatin_m, chLatin_e, chLatin_r, chLatin_i, chLatin_c, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_B, chLatin_o, chLatin_x, chLatin_D, chLatin_r, chLatin_a, chLatin_w,
      chLatin_i, chLatin_n, chLatin_g,  chNull },
    { chLatin_I, chLatin_s, chLatin_B, chLatin_l, chLatin_o, chLatin_c, chLatin_k, chLatin_E, chLatin_l,
      chLatin_e, chLatin_m, chLatin_e, chLatin_n, chLatin_t, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_G, chLatin_e, chLatin_o, chLatin_m, chLatin_e, chLatin_t, chLatin_r, chLatin_i,
      chLatin_c, chLatin_S, chLatin_h, chLatin_a, chLatin_p, chLatin_e, chLatin_s,
       chNull },
    { chLatin_I, chLatin_s, chLatin_M, chLatin_i, chLatin_s, chLatin_c, chLatin_e, chLatin_l, chLatin_l, chLatin_a,
      chLatin_n, chLatin_e, chLatin_o, chLatin_u, chLatin_s, chLatin_S, chLatin_y,
      chLatin_m, chLatin_b, chLatin_o, chLatin_l, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_D, chLatin_i, chLatin_n, chLatin_g, chLatin_b, chLatin_a, chLatin_t, chLatin_s,
       chNull },
    { chLatin_I, chLatin_s, chLatin_B, chLatin_r, chLatin_a, chLatin_i, chLatin_l, chLatin_l, chLatin_e,
      chLatin_P, chLatin_a, chLatin_t, chLatin_t, chLatin_e, chLatin_r, chLatin_n, chLatin_s, chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_J, chLatin_K, chLatin_R, chLatin_a, chLatin_d, chLatin_i, chLatin_c,
      chLatin_a, chLatin_l, chLatin_s, chLatin_S, chLatin_u, chLatin_p, chLatin_p, chLatin_l, chLatin_e, chLatin_m,
      chLatin_e, chLatin_n, chLatin_t,  chNull },
    { chLatin_I, chLatin_s, chLatin_K, chLatin_a, chLatin_n, chLatin_g, chLatin_x, chLatin_i,
      chLatin_R, chLatin_a, chLatin_d, chLatin_i, chLatin_c, chLatin_a, chLatin_l, chLatin_s, chNull },
    { chLatin_I, chLatin_s, chLatin_I, chLatin_d, chLatin_e, chLatin_o, chLatin_g, chLatin_r, chLatin_a, chLatin_p,
      chLatin_h, chLatin_i, chLatin_c, chLatin_D, chLatin_e, chLatin_s, chLatin_c, chLatin_r, chLatin_i, chLatin_p,
      chLatin_t, chLatin_i, chLatin_o, chLatin_n, chLatin_C, chLatin_h, chLatin_a, chLatin_r, chLatin_a, chLatin_c,
      chLatin_t, chLatin_e, chLatin_r, chLatin_s, chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_J, chLatin_K, chLatin_S, chLatin_y, chLatin_m, chLatin_b,
      chLatin_o, chLatin_l, chLatin_s, chLatin_a, chLatin_n, chLatin_d,
      chLatin_P, chLatin_u, chLatin_n, chLatin_c, chLatin_t, chLatin_u, chLatin_a, chLatin_t,
      chLatin_i, chLatin_o, chLatin_n,  chNull },
    { chLatin_I, chLatin_s, chLatin_H, chLatin_i, chLatin_r, chLatin_a, chLatin_g, chLatin_a, chLatin_n, chLatin_a,
       chNull },
    { chLatin_I, chLatin_s, chLatin_K, chLatin_a, chLatin_t, chLatin_a, chLatin_k, chLatin_a, chLatin_n, chLatin_a,
       chNull },
    { chLatin_I, chLatin_s, chLatin_B, chLatin_o, chLatin_p, chLatin_o, chLatin_m, chLatin_o, chLatin_f, chLatin_o,
       chNull },
    { chLatin_I, chLatin_s, chLatin_H, chLatin_a, chLatin_n, chLatin_g, chLatin_u, chLatin_l, chLatin_C,
      chLatin_o, chLatin_m, chLatin_p, chLatin_a, chLatin_t, chLatin_i, chLatin_b, chLatin_i,
      chLatin_l, chLatin_i, chLatin_t, chLatin_y, chLatin_J, chLatin_a, chLatin_m,
      chLatin_o,  chNull },
    { chLatin_I, chLatin_s, chLatin_K, chLatin_a, chLatin_n, chLatin_b, chLatin_u, chLatin_n,  chNull },
    { chLatin_I, chLatin_s, chLatin_B, chLatin_o, chLatin_p, chLatin_o, chLatin_m, chLatin_o, chLatin_f, chLatin_o,
      chLatin_E, chLatin_x, chLatin_t, chLatin_e, chLatin_n, chLatin_d, chLatin_e, chLatin_d, chNull },
    { chLatin_I, chLatin_s, chLatin_E, chLatin_n, chLatin_c, chLatin_l, chLatin_o, chLatin_s, chLatin_e, chLatin_d,
      chLatin_C, chLatin_J, chLatin_K, chLatin_L, chLatin_e, chLatin_t,
      chLatin_t, chLatin_e, chLatin_r, chLatin_s, chLatin_a, chLatin_n, chLatin_d,
      chLatin_M, chLatin_o, chLatin_n, chLatin_t, chLatin_h, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_J, chLatin_K, chLatin_C, chLatin_o, chLatin_m, chLatin_p,
      chLatin_a, chLatin_t, chLatin_i, chLatin_b, chLatin_i, chLatin_l, chLatin_i, chLatin_t,
      chLatin_y,  chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_J, chLatin_K, chLatin_U, chLatin_n, chLatin_i, chLatin_f,
      chLatin_i, chLatin_e, chLatin_d, chLatin_I, chLatin_d, chLatin_e, chLatin_o,
      chLatin_g, chLatin_r, chLatin_a, chLatin_p, chLatin_h, chLatin_s,
      chLatin_E, chLatin_x, chLatin_t, chLatin_e, chLatin_n, chLatin_s, chLatin_i, chLatin_o, chLatin_n, chLatin_A,
      chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_J, chLatin_K, chLatin_U, chLatin_n, chLatin_i, chLatin_f,
      chLatin_i, chLatin_e, chLatin_d, chLatin_I, chLatin_d, chLatin_e, chLatin_o,
      chLatin_g, chLatin_r, chLatin_a, chLatin_p, chLatin_h, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_Y, chLatin_i, chLatin_S, chLatin_y, chLatin_l, chLatin_l, chLatin_a,
      chLatin_b, chLatin_l, chLatin_e, chLatin_s, chNull },
    { chLatin_I, chLatin_s, chLatin_Y, chLatin_i, chLatin_R, chLatin_a, chLatin_d, chLatin_i, chLatin_c,
      chLatin_a, chLatin_l, chLatin_s, chNull },
    { chLatin_I, chLatin_s, chLatin_H, chLatin_a, chLatin_n, chLatin_g, chLatin_u, chLatin_l, chLatin_S,
      chLatin_y, chLatin_l, chLatin_l, chLatin_a, chLatin_b, chLatin_l, chLatin_e, chLatin_s,
       chNull },
    { chLatin_I, chLatin_s, chLatin_H, chLatin_i, chLatin_g, chLatin_h, chLatin_S, chLatin_u, chLatin_r,
      chLatin_r, chLatin_o, chLatin_g, chLatin_a, chLatin_t, chLatin_e, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_H, chLatin_i, chLatin_g, chLatin_h, chLatin_P, chLatin_r, chLatin_i,
      chLatin_v, chLatin_a, chLatin_t, chLatin_e, chLatin_U, chLatin_s, chLatin_e,
      chLatin_S, chLatin_u, chLatin_r, chLatin_r, chLatin_o, chLatin_g, chLatin_a,
      chLatin_t, chLatin_e, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_L, chLatin_o, chLatin_w, chLatin_S, chLatin_u, chLatin_r, chLatin_r,
      chLatin_o, chLatin_g, chLatin_a, chLatin_t, chLatin_e, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_P, chLatin_r, chLatin_i, chLatin_v, chLatin_a, chLatin_t, chLatin_e,
      chLatin_U, chLatin_s, chLatin_e,  chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_J, chLatin_K, chLatin_C, chLatin_o, chLatin_m, chLatin_p,
      chLatin_a, chLatin_t, chLatin_i, chLatin_b, chLatin_i, chLatin_l, chLatin_i, chLatin_t,
      chLatin_y, chLatin_I, chLatin_d, chLatin_e, chLatin_o, chLatin_g, chLatin_r,
      chLatin_a, chLatin_p, chLatin_h, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_A, chLatin_l, chLatin_p, chLatin_h, chLatin_a, chLatin_b, chLatin_e, chLatin_t,
      chLatin_i, chLatin_c, chLatin_P, chLatin_r, chLatin_e, chLatin_s, chLatin_e,
      chLatin_n, chLatin_t, chLatin_a, chLatin_t, chLatin_i, chLatin_o, chLatin_n,
      chLatin_F, chLatin_o, chLatin_r, chLatin_m, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_A, chLatin_r, chLatin_a, chLatin_b, chLatin_i, chLatin_c, chLatin_P,
      chLatin_r, chLatin_e, chLatin_s, chLatin_e, chLatin_n, chLatin_t, chLatin_a, chLatin_t,
      chLatin_i, chLatin_o, chLatin_n, chLatin_F, chLatin_o, chLatin_r, chLatin_m,
      chLatin_s, chDash, chLatin_A,  chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_o, chLatin_m, chLatin_b, chLatin_i, chLatin_n, chLatin_i, chLatin_n,
      chLatin_g, chLatin_H, chLatin_a, chLatin_l, chLatin_f, chLatin_M,
      chLatin_a, chLatin_r, chLatin_k, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_J, chLatin_K, chLatin_C, chLatin_o, chLatin_m, chLatin_p,
      chLatin_a, chLatin_t, chLatin_i, chLatin_b, chLatin_i, chLatin_l, chLatin_i, chLatin_t,
      chLatin_y, chLatin_F, chLatin_o, chLatin_r, chLatin_m, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_S, chLatin_m, chLatin_a, chLatin_l, chLatin_l, chLatin_F, chLatin_o,
      chLatin_r, chLatin_m, chLatin_V, chLatin_a, chLatin_r, chLatin_i, chLatin_a,
      chLatin_n, chLatin_t, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_A, chLatin_r, chLatin_a, chLatin_b, chLatin_i, chLatin_c, chLatin_P,
      chLatin_r, chLatin_e, chLatin_s, chLatin_e, chLatin_n, chLatin_t, chLatin_a, chLatin_t,
      chLatin_i, chLatin_o, chLatin_n, chLatin_F, chLatin_o, chLatin_r, chLatin_m,
      chLatin_s, chDash, chLatin_B,  chNull },
    { chLatin_I, chLatin_s, chLatin_S, chLatin_p, chLatin_e, chLatin_c, chLatin_i, chLatin_a, chLatin_l, chLatin_s,
       chNull },
    { chLatin_I, chLatin_s, chLatin_H, chLatin_a, chLatin_l, chLatin_f, chLatin_w, chLatin_i, chLatin_d, chLatin_t,
      chLatin_h, chLatin_a, chLatin_n, chLatin_d, chLatin_F, chLatin_u,
      chLatin_l, chLatin_l, chLatin_w, chLatin_i, chLatin_d, chLatin_t, chLatin_h,
      chLatin_F, chLatin_o, chLatin_r, chLatin_m, chLatin_s,  chNull },
    { chLatin_I, chLatin_s, chLatin_O, chLatin_l, chLatin_d, chLatin_I, chLatin_t, chLatin_a, chLatin_l, chLatin_i,
      chLatin_c, chNull },
    { chLatin_I, chLatin_s, chLatin_G, chLatin_o, chLatin_t, chLatin_h, chLatin_i, chLatin_c, chNull },
    { chLatin_I, chLatin_s, chLatin_D, chLatin_e, chLatin_s, chLatin_e, chLatin_r, chLatin_e, chLatin_t, chNull },
    { chLatin_I, chLatin_s, chLatin_B, chLatin_y, chLatin_z, chLatin_a, chLatin_n, chLatin_t, chLatin_i, chLatin_n, chLatin_e,
      chLatin_M, chLatin_u, chLatin_s, chLatin_i, chLatin_c, chLatin_a, chLatin_l,
      chLatin_S, chLatin_y, chLatin_m, chLatin_b, chLatin_o, chLatin_l, chLatin_s, chNull },
    { chLatin_M, chLatin_u, chLatin_s, chLatin_i, chLatin_c, chLatin_a, chLatin_l,
      chLatin_S, chLatin_y, chLatin_m, chLatin_b, chLatin_o, chLatin_l, chLatin_s, chNull },
    { chLatin_I, chLatin_s, chLatin_M, chLatin_a, chLatin_t, chLatin_h, chLatin_e,
      chLatin_m, chLatin_a, chLatin_t, chLatin_i, chLatin_c, chLatin_a, chLatin_l,
      chLatin_A, chLatin_l, chLatin_p, chLatin_h, chLatin_a,
      chLatin_n, chLatin_u, chLatin_m, chLatin_e, chLatin_r, chLatin_i, chLatin_c,
      chLatin_S, chLatin_y, chLatin_m, chLatin_b, chLatin_o, chLatin_l, chLatin_s, chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_J, chLatin_K, chLatin_U, chLatin_n, chLatin_i, chLatin_f,
      chLatin_i, chLatin_e, chLatin_d, chLatin_I, chLatin_d, chLatin_e, chLatin_o,
      chLatin_g, chLatin_r, chLatin_a, chLatin_p, chLatin_h, chLatin_s,
      chLatin_E, chLatin_x, chLatin_t, chLatin_e, chLatin_n, chLatin_s, chLatin_i, chLatin_o, chLatin_n, chLatin_B,
      chNull },
    { chLatin_I, chLatin_s, chLatin_C, chLatin_J, chLatin_K, chLatin_C, chLatin_o, chLatin_m, chLatin_p,
      chLatin_a, chLatin_t, chLatin_i, chLatin_b, chLatin_i, chLatin_l, chLatin_i, chLatin_t, chLatin_y,
      chLatin_I, chLatin_d, chLatin_e, chLatin_o, chLatin_g, chLatin_r, chLatin_a, chLatin_p, chLatin_h, chLatin_s,
      chLatin_S, chLatin_u, chLatin_p, chLatin_p, chLatin_l, chLatin_e, chLatin_m, chLatin_e, chLatin_n, chLatin_t,  chNull },
    { chLatin_I, chLatin_s, chLatin_T, chLatin_a, chLatin_g, chLatin_s, chNull },
};

const XMLInt32 blockRanges[] =
{
    0x0000,0x007F,0x0080,0x00FF,0x0100,0x017F,0x0180,0x024F,0x0250,0x02AF,0x02B0,0x02FF,
    0x0300,0x036F,0x0370,0x03FF,0x0400,0x04FF,0x0530,0x058F,0x0590,0x05FF,0x0600,0x06FF,
    0x0700,0x074F,0x0780,0x07BF,0x0900,0x097F,0x0980,0x09FF,0x0A00,0x0A7F,0x0A80,0x0AFF,
    0x0B00,0x0B7F,0x0B80,0x0BFF,0x0C00,0x0C7F,0x0C80,0x0CFF,0x0D00,0x0D7F,0x0D80,0x0DFF,
    0x0E00,0x0E7F,0x0E80,0x0EFF,0x0F00,0x0FFF,0x1000,0x109F,0x10A0,0x10FF,0x1100,0x11FF,
    0x1200,0x137F,0x13A0,0x13FF,0x1400,0x167F,0x1680,0x169F,0x16A0,0x16FF,0x1780,0x17FF,
    0x1800,0x18AF,0x1E00,0x1EFF,0x1F00,0x1FFF,0x2000,0x206F,0x2070,0x209F,0x20A0,0x20CF,
    0x20D0,0x20FF,0x2100,0x214F,0x2150,0x218F,0x2190,0x21FF,0x2200,0x22FF,0x2300,0x23FF,
    0x2400,0x243F,0x2440,0x245F,0x2460,0x24FF,0x2500,0x257F,0x2580,0x259F,0x25A0,0x25FF,
    0x2600,0x26FF,0x2700,0x27BF,0x2800,0x28FF,0x2E80,0x2EFF,0x2F00,0x2FDF,0x2FF0,0x2FFF,
    0x3000,0x303F,0x3040,0x309F,0x30A0,0x30FF,0x3100,0x312F,0x3130,0x318F,0x3190,0x319F,
    0x31A0,0x31BF,0x3200,0x32FF,0x3300,0x33FF,0x3400,0x4DB5,0x4E00,0x9FFF,0xA000,0xA48F,
    0xA490,0xA4CF,0xAC00,0xD7A3,0xD800,0xDB7F,0xDB80,0xDBFF,0xDC00,0xDFFF,0xE000,0xF8FF,
    0xF900,0xFAFF,0xFB00,0xFB4F,0xFB50,0xFDFF,0xFE20,0xFE2F,0xFE30,0xFE4F,0xFE50,0xFE6F,
    0xFE70,0xFEFE,0xFEFF,0xFEFF,0xFF00,0xFFEF,0x10300,0x1032F,0x10330,0x1034F,
    0x10400,0x1044F,0x1D000,0x1D0FF,0x1D100,0x1D1FF,0x1D400,0x1D7FF,0x20000,0x2A6D6,0x2F800,0x2FA1F,
    0xE0000,0xE007F, chNull
};

// ---------------------------------------------------------------------------
//  BlockRangeFactory: Constructors and Destructor
// ---------------------------------------------------------------------------
BlockRangeFactory::BlockRangeFactory() :
   fRangesCreated(false)
 , fKeywordsInitialized(false)
{

}

BlockRangeFactory::~BlockRangeFactory() {

}

// ---------------------------------------------------------------------------
//  BlockRangeFactory: Range creation methods
// ---------------------------------------------------------------------------
void BlockRangeFactory::buildRanges() {

    if (fRangesCreated)
        return;

    if (!fKeywordsInitialized) {
        initializeKeywordMap();
    }

    RangeTokenMap* rangeTokMap = RangeTokenMap::instance();
    TokenFactory* tokFactory = rangeTokMap->getTokenFactory();

    //for performance, once the desired specials and private use are found
    //don't need to compareString anymore
    bool foundSpecial = false;
    bool foundPrivate = false;

    for (int i=0; i < BLOCKNAMESIZE; i++) {
        RangeToken* tok = tokFactory->createRange();
        tok->addRange(blockRanges[i*2], blockRanges[(i*2)+1]);

        if (!foundSpecial && !XMLString::compareString((XMLCh*)fgBlockNames[i] , (XMLCh*) fgBlockIsSpecials)) {
            tok->addRange(0xFFF0, 0xFFFD);
            foundSpecial = true;
        }
        if (!foundPrivate && !XMLString::compareString((XMLCh*)fgBlockNames[i] , (XMLCh*) fgBlockIsPrivateUse)) {
            tok->addRange(0xF0000, 0xFFFFD);
            tok->addRange(0x100000, 0x10FFFD);
            foundPrivate = true;
        }
        rangeTokMap->setRangeToken(fgBlockNames[i], tok);
    }

    fRangesCreated = true;
}

// ---------------------------------------------------------------------------
//  BlockRangeFactory: Range creation methods
// ---------------------------------------------------------------------------
void BlockRangeFactory::initializeKeywordMap() {

    if (fKeywordsInitialized)
        return;

	RangeTokenMap* rangeTokMap = RangeTokenMap::instance();

	for (int i=0; i< BLOCKNAMESIZE; i++) {
        rangeTokMap->addKeywordMap(fgBlockNames[i], fgBlockCategory);
    }

    fKeywordsInitialized = true;
}

/**
  * End of file BlockRangeFactory.cpp
  */
