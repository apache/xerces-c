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
 *    nor may "Apache" appear in their featName, without prior written
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
 * Revision 1.1  2002/05/28 22:38:55  peiyongz
 * DOM3 Save Interface: DOMWriter/DOMWriterFilter
 *
 */

#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

//canonical-form
const XMLCh    DOMWriter::CanonicalForm[] =
{
	chLatin_c, chLatin_a, chLatin_n, chLatin_o, chLatin_n, chLatin_i,
    chLatin_c, chLatin_a, chLatin_l, chDash,    chLatin_f, chLatin_o,
	chLatin_r, chLatin_m, chNull
};

//discard-default-content
const XMLCh    DOMWriter::DiscardDefaultContent[] =
{
	chLatin_d, chLatin_i, chLatin_s, chLatin_c, chLatin_a, chLatin_r,
    chLatin_d, chDash,    chLatin_d, chLatin_e, chLatin_f, chLatin_a,
	chLatin_u, chLatin_l, chLatin_l, chLatin_t, chDash,    chLatin_c,
	chLatin_o, chLatin_n, chLatin_t, chLatin_e, chLatin_n, chLatin_t,
	chNull
};

//entities
const XMLCh    DOMWriter::Entities[] =
{
	chLatin_e, chLatin_n, chLatin_t, chLatin_i, chLatin_t, chLatin_i,
	chLatin_e, chLatin_s, chNull
};

//format-pretty-print
const XMLCh    DOMWriter::FormatPrettyPrint[] =
{
	chLatin_f, chLatin_o, chLatin_r, chLatin_m, chLatin_a, chLatin_t,
    chDash,    chLatin_p, chLatin_r, chLatin_e, chLatin_t, chLatin_t,
	chLatin_y, chDash,    chLatin_p, chLatin_r, chLatin_i, chLatin_n,
	chLatin_t, chNull
};

//normalize-characters
const XMLCh    DOMWriter::NormalizeCharacters[] =
{
	chLatin_n, chLatin_o, chLatin_r, chLatin_m, chLatin_a, chLatin_l,
    chLatin_i, chLatin_z, chLatin_e, chDash,    chLatin_c, chLatin_h,
	chLatin_a, chLatin_r, chLatin_a, chLatin_c, chLatin_t, chLatin_e, 
	chLatin_r, chLatin_s, chNull
};

//split-cdata-sections
const XMLCh    DOMWriter::SplitCdataSections[] =
{
	chLatin_s, chLatin_p, chLatin_l, chLatin_i, chLatin_t, chDash,
    chLatin_c, chLatin_d, chLatin_a, chLatin_t, chLatin_a, chDash,
	chLatin_s, chLatin_e, chLatin_c, chLatin_t, chLatin_i, chLatin_o, 
	chLatin_n, chLatin_s, chNull
};

//validation
const XMLCh    DOMWriter::Validation[] =
{
	chLatin_v, chLatin_a, chLatin_l, chLatin_i, chLatin_d, chLatin_a, 
	chLatin_t, chLatin_i, chLatin_o, chLatin_n, chNull
};

//whitespace-in-element-content
const XMLCh    DOMWriter::WhitespaceInElementContent[] =
{
	chLatin_w, chLatin_h, chLatin_i, chLatin_t, chLatin_e, chLatin_s,
    chLatin_p, chLatin_a, chLatin_c, chLatin_e, chDash,    chLatin_i,
	chLatin_n, chDash,    chLatin_e, chLatin_l, chLatin_e, chLatin_m, 
	chLatin_e, chLatin_n, chLatin_t, chDash,    chLatin_c, chLatin_o, 
	chLatin_n, chLatin_t, chLatin_e, chLatin_n, chLatin_t, chNull
};

