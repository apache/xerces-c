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
 * Revision 1.1  2002/02/01 22:22:51  peiyongz
 * Initial revision
 *
 * Revision 1.1  2001/11/02 14:08:40  knoaman
 * Add support for identity constraints.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/validators/schema/identity/XPathSymbols.hpp>

// ---------------------------------------------------------------------------
//  SchemaSymbols: Static data
// ---------------------------------------------------------------------------
const XMLCh XPathSymbols::fgSYMBOL_AND[] =
{
    chLatin_a, chLatin_n, chLatin_d, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_OR[] =
{
    chLatin_o, chLatin_r, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_MOD[] =
{
    chLatin_m, chLatin_o, chLatin_d, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_DIV[] =
{
    chLatin_d, chLatin_i, chLatin_v, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_COMMENT[] =
{
    chLatin_c, chLatin_o, chLatin_m, chLatin_m, chLatin_e, chLatin_n, chLatin_t, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_TEXT[] =
{
    chLatin_t, chLatin_e, chLatin_x, chLatin_t, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_PI[] =
{
    chLatin_p, chLatin_r, chLatin_o, chLatin_c, chLatin_e, chLatin_s, chLatin_s,
    chLatin_i, chLatin_n, chLatin_g, chDash, chLatin_i, chLatin_n, chLatin_s, chLatin_t,
    chLatin_r, chLatin_u, chLatin_c, chLatin_t, chLatin_i, chLatin_o, chLatin_n, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_NODE[] =
{
    chLatin_n, chLatin_o, chLatin_d, chLatin_e, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_ANCESTOR[] =
{
    chLatin_a, chLatin_n, chLatin_c, chLatin_e, chLatin_s, chLatin_t, chLatin_o,
    chLatin_r, chNull
};


const XMLCh XPathSymbols::fgSYMBOL_ANCESTOR_OR_SELF[] =
{
    chLatin_a, chLatin_n, chLatin_c, chLatin_e, chLatin_s, chLatin_t, chLatin_o,
    chLatin_r, chDash, chLatin_o, chLatin_r, chDash, chLatin_s, chLatin_e,
    chLatin_l, chLatin_f, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_ATTRIBUTE[] =
{
    chLatin_a, chLatin_t, chLatin_t, chLatin_r, chLatin_i, chLatin_b, chLatin_u,
    chLatin_t, chLatin_e, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_CHILD[] =
{
    chLatin_c, chLatin_h, chLatin_i, chLatin_l, chLatin_d, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_DESCENDANT[] =
{
    chLatin_d, chLatin_e, chLatin_s, chLatin_c, chLatin_e, chLatin_n, chLatin_d,
    chLatin_a, chLatin_n, chLatin_t, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_DESCENDANT_OR_SELF[] =
{
    chLatin_d, chLatin_e, chLatin_s, chLatin_c, chLatin_e, chLatin_n, chLatin_d,
    chLatin_a, chLatin_n, chLatin_t, chDash, chLatin_o, chLatin_r, chDash, chLatin_s,
	chLatin_e, chLatin_l, chLatin_f, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_FOLLOWING[] =
{
    chLatin_f, chLatin_o, chLatin_l, chLatin_l, chLatin_o, chLatin_w, chLatin_i,
    chLatin_n, chLatin_g, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_FOLLOWING_SIBLING[] =
{
    chLatin_f, chLatin_o, chLatin_l, chLatin_l, chLatin_o, chLatin_w, chLatin_i,
    chLatin_n, chLatin_g, chDash, chLatin_s, chLatin_i, chLatin_b, chLatin_l, chLatin_i,
    chLatin_n, chLatin_g, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_NAMESPACE[] =
{
    chLatin_n, chLatin_a, chLatin_m, chLatin_e, chLatin_s, chLatin_p, chLatin_a,
    chLatin_c, chLatin_e, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_PARENT[] =
{
    chLatin_p, chLatin_a, chLatin_r, chLatin_e, chLatin_n, chLatin_t, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_PRECEDING[] =
{
    chLatin_p, chLatin_r, chLatin_e, chLatin_c, chLatin_e, chLatin_d, chLatin_i,
    chLatin_n, chLatin_g, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_PRECEDING_SIBLING[] =
{
    chLatin_p, chLatin_r, chLatin_e, chLatin_c, chLatin_e, chLatin_d, chLatin_i,
    chLatin_n, chLatin_g, chDash, chLatin_s, chLatin_i, chLatin_b, chLatin_l, chLatin_i,
    chLatin_n, chLatin_g, chNull
};

const XMLCh XPathSymbols::fgSYMBOL_SELF[] =
{
    chLatin_s, chLatin_e, chLatin_l, chLatin_f, chNull
};

/**
  * End of file XPathSymbols.cpp
  */

