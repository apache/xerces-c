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
 * Revision 1.14  2001/07/06 20:21:58  peiyongz
 * QNameDTV & ENTITYDTV enabled
 *
 * Revision 1.13  2001/07/05 20:15:25  peiyongz
 * NOTATIONDatatypeValidator
 *
 * Revision 1.12  2001/07/04 20:16:31  peiyongz
 * IDREFDatatypeValidator
 *
 * Revision 1.11  2001/07/04 14:38:24  peiyongz
 * IDDatatypeValidator: created
 * DatatypeValidatorFactory: IDDTV enabled
 * XMLString:isValidName(): to validate Name (XML [4][5])
 *
 * Revision 1.10  2001/05/28 21:11:17  tng
 * Schema: Various DatatypeValidator fix.  By Pei Yong Zhang
 *
 * Revision 1.9  2001/05/18 16:51:37  knoaman
 * Added circular check for complexType + more error messages.
 *
 * Revision 1.8  2001/05/17 18:11:11  knoaman
 * More constraint and attribute checking.
 *
 * Revision 1.7  2001/05/16 15:24:42  tng
 * Schema: Add Base64 and HexBin.  By Pei Yong Zhang.
 *
 * Revision 1.6  2001/05/15 21:59:19  knoaman
 * TraverseSchema: add attribute checking + some fixes + more error messages.
 * More attribute cheking to come.
 *
 * Revision 1.5  2001/05/11 21:51:13  knoaman
 * Schema updates and fixes.
 *
 * Revision 1.4  2001/05/11 13:27:28  tng
 * Copyright update.
 *
 * Revision 1.3  2001/05/09 18:43:40  tng
 * Add StringDatatypeValidator and BooleanDatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/03 19:17:47  knoaman
 * TraverseSchema Part II.
 *
 * Revision 1.1  2001/03/21 21:39:14  knoaman
 * Schema symbols and Datatype validator part I
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/datatype/DatatypeValidatorFactory.hpp>
#include <validators/schema/SchemaSymbols.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/Janitor.hpp>
#include <validators/datatype/StringDatatypeValidator.hpp>
#include <validators/datatype/BooleanDatatypeValidator.hpp>
#include <validators/datatype/DecimalDatatypeValidator.hpp>
#include <validators/datatype/HexBinaryDatatypeValidator.hpp>
#include <validators/datatype/Base64BinaryDatatypeValidator.hpp>
#include <validators/datatype/IDDatatypeValidator.hpp>
#include <validators/datatype/IDREFDatatypeValidator.hpp>
#include <validators/datatype/NOTATIONDatatypeValidator.hpp>
#include <validators/datatype/ENTITYDatatypeValidator.hpp>
#include <validators/datatype/QNameDatatypeValidator.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLDeleterFor.hpp>


// ---------------------------------------------------------------------------
//  DatatypeValidatorFactory: Local const data
// ---------------------------------------------------------------------------
const XMLCh fgTokPattern[] = 
{ 
    chBackSlash, chLatin_c, chPlus, chNull
};

//"([a-zA-Z]{2}|[iI]-[a-zA-Z]+|[xX]-[a-zA-Z]+)(-[a-zA-Z]+)*"
const XMLCh fgLangPattern[] = 
{
    chOpenParen, chOpenSquare, chLatin_a, chDash, chLatin_z, chLatin_A, chDash,
    chLatin_Z, chCloseSquare, chOpenCurly, chDigit_2, chCloseCurly, chPipe,
    chOpenSquare, chLatin_i, chLatin_I, chCloseSquare, chDash, chOpenSquare,
    chLatin_a, chDash, chLatin_z, chLatin_A, chDash, chLatin_Z, chCloseSquare,
    chPlus, chPipe, chOpenSquare, chLatin_x, chLatin_X, chCloseSquare, chDash,
    chOpenSquare, chLatin_a, chDash, chLatin_z, chLatin_A, chDash, chLatin_Z,
	chCloseSquare, chPlus, chCloseParen, chOpenParen, chDash, chOpenSquare,
    chLatin_a, chDash, chLatin_z, chLatin_A, chDash, chLatin_Z, chCloseSquare,
    chPlus, chCloseParen, chAsterisk, chNull
};

//"\\i\\c*"
const XMLCh fgNamePattern[] =
{
    chBackSlash, chLatin_i, chBackSlash, chLatin_c, chAsterisk, chNull
};

//"[\\i-[:]][\\c-[:]]*"
const XMLCh fgNCNamePattern[] =
{
    chOpenSquare, chBackSlash, chLatin_i, chDash, chOpenSquare, chColon, chCloseSquare,
    chCloseSquare, chOpenSquare, chBackSlash, chLatin_c, chDash, chOpenSquare,
    chColon, chCloseSquare, chCloseSquare, chAsterisk, chNull
};

const XMLCh fgValueZero[] = 
{
    chDigit_0, chNull
};

const XMLCh fgNegOne[] =
{
    chDash, chDigit_1, chNull
};

const XMLCh fgValueOne[] = 
{
    chDigit_1, chNull
};

//"9223372036854775807"
const XMLCh fgLongMaxInc[] =
{
    chDigit_9, chDigit_2, chDigit_2, chDigit_3, chDigit_3, chDigit_7, chDigit_2,
    chDigit_0, chDigit_3, chDigit_6, chDigit_8, chDigit_5, chDigit_4, chDigit_7,
    chDigit_7, chDigit_5, chDigit_8, chDigit_0, chDigit_7, chNull
};

//"-9223372036854775808"
const XMLCh fgLongMinInc[] =
{
    chDash, chDigit_9, chDigit_2, chDigit_2, chDigit_3, chDigit_3, chDigit_7,
    chDigit_2, chDigit_0, chDigit_3, chDigit_6, chDigit_8, chDigit_5, chDigit_4,
    chDigit_7, chDigit_7, chDigit_5, chDigit_8, chDigit_0, chDigit_8,  chNull
};

const XMLCh fgIntMaxInc[] =
{
    chDigit_2, chDigit_1, chDigit_4, chDigit_7, chDigit_4, chDigit_8,
    chDigit_3, chDigit_6, chDigit_4, chDigit_7, chNull
};

const XMLCh fgIntMinInc[] =
{
    chDash, chDigit_2, chDigit_1, chDigit_4, chDigit_7, chDigit_4,
    chDigit_8, chDigit_3, chDigit_6, chDigit_4, chDigit_8, chNull
};

const XMLCh fgShortMaxInc[] =
{
    chDigit_3, chDigit_2, chDigit_7, chDigit_6, chDigit_7, chNull
};

const XMLCh fgShortMinInc[] =
{
    chDash, chDigit_3, chDigit_2, chDigit_7, chDigit_6, chDigit_8, chNull
};

const XMLCh fgByteMaxInc[] =
{
    chDigit_1, chDigit_2, chDigit_7, chNull
};

const XMLCh fgByteMinInc[] =
{
    chDash, chDigit_1, chDigit_2, chDigit_8, chNull
};

const XMLCh fgULongMaxInc[] =
{
    chDigit_1, chDigit_8, chDigit_4, chDigit_4, chDigit_6, chDigit_7, chDigit_4,
    chDigit_4, chDigit_0, chDigit_7, chDigit_3, chDigit_7, chDigit_0, chDigit_9,
    chDigit_5, chDigit_5, chDigit_1, chDigit_6, chDigit_1, chDigit_5, chNull
};

const XMLCh fgUIntMaxInc[] =
{
    chDigit_4, chDigit_2, chDigit_9, chDigit_4, chDigit_9, chDigit_6,
    chDigit_7, chDigit_2, chDigit_9, chDigit_5, chNull
};

const XMLCh fgUShortMaxInc[] =
{
    chDigit_6, chDigit_5, chDigit_5, chDigit_3, chDigit_5, chNull
};

const XMLCh fgUByteMaxInc[] =
{
    chDigit_2, chDigit_5, chDigit_5, chNull
};

const XMLCh fgP0Y[] =
{
    chLatin_P, chDigit_0, chLatin_Y, chNull
};

const XMLCh fgP1Y[] =
{
    chLatin_P, chDigit_1, chLatin_Y, chNull
};

const XMLCh fgP100Y[] =
{
    chLatin_P, chDigit_1, chDigit_0, chDigit_0, chLatin_Y, chNull
};

const XMLCh fgPT24H[] =
{
    chLatin_P, chLatin_T, chDigit_2, chDigit_4, chLatin_H, chNull
};

const XMLCh fgP1M[] =
{
    chLatin_P, chDigit_1, chLatin_M, chNull
};


// ---------------------------------------------------------------------------
//  DatatypeValidatorFactory: Static member data
// ---------------------------------------------------------------------------
RefHashTableOf<DatatypeValidator>* DatatypeValidatorFactory::fBuiltInRegistry = 0;
int DatatypeValidatorFactory::fRegistryExpanded = 0;

// ---------------------------------------------------------------------------
//  DatatypeValidatorFactory: Constructors and Destructor
// ---------------------------------------------------------------------------
DatatypeValidatorFactory::DatatypeValidatorFactory()
    : fUserDefinedRegistry(0)
{
}

DatatypeValidatorFactory::~DatatypeValidatorFactory()
{
	cleanUp();
}


// ---------------------------------------------------------------------------
//  DatatypeValidatorFactory: Reset methods
// ---------------------------------------------------------------------------
void DatatypeValidatorFactory::resetRegistry() {

    if (fUserDefinedRegistry != 0) {
        fUserDefinedRegistry->removeAll();
    }

    // TO DO: reset some primitive datatypes static - fields (ID & IDREFS)
}


// ---------------------------------------------------------------------------
//  DatatypeValidatorFactory: Registry initialization methods
// ---------------------------------------------------------------------------
void DatatypeValidatorFactory::initializeDTDRegistry()
{
	if (fRegistryExpanded)
		return;

    if (fBuiltInRegistry == 0) {

        fBuiltInRegistry = new RefHashTableOf<DatatypeValidator>(109);
        XMLPlatformUtils::registerLazyData
        (
            new XMLDeleterFor<DVHashTable>(fBuiltInRegistry)
        );
    }


        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_STRING,
                       new StringDatatypeValidator());
        fBuiltInRegistry->put((void*) XMLUni::fgIDString,
                       new IDDatatypeValidator());
        fBuiltInRegistry->put((void*) XMLUni::fgIDRefString,
                       new IDREFDatatypeValidator());
        fBuiltInRegistry->put((void*) XMLUni::fgEntityString,
                       new ENTITYDatatypeValidator());
        fBuiltInRegistry->put((void*) XMLUni::fgNotationString,
                       new NOTATIONDatatypeValidator());


        // Create 'IDREFS' datatype validator
	    createDatatypeValidator(XMLUni::fgIDRefsString, 
                    getDatatypeValidator(XMLUni::fgIDRefString), 0, 0, true, 0, false);

        // Create 'ENTITIES' datatype validator
        createDatatypeValidator(XMLUni::fgEntitiesString,
		            getDatatypeValidator(XMLUni::fgEntityString), 0, 0, true, 0, false);

        RefHashTableOf<KVStringPair>* facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_PATTERN ,
                    new KVStringPair(SchemaSymbols::fgELT_PATTERN,fgTokPattern));
        facets->put((void*) SchemaSymbols::fgELT_WHITESPACE,
                    new KVStringPair(SchemaSymbols::fgELT_WHITESPACE, SchemaSymbols::fgWS_COLLAPSE));

        // Create 'NMTOKEN' datatype validator
        createDatatypeValidator(XMLUni::fgNmTokenString, 
                    getDatatypeValidator(SchemaSymbols::fgDT_STRING),facets, 0, false, 0, false);

        // Create 'NMTOKENS' datatype validator
        createDatatypeValidator(XMLUni::fgNmTokensString,
		            getDatatypeValidator(XMLUni::fgNmTokenString), 0, 0, true, 0, false);

        fRegistryExpanded = 1;
}


void DatatypeValidatorFactory::expandRegistryToFullSchemaSet()
{
	if (fRegistryExpanded == 2)
		return;

	// Initialize common Schema/DTD Datatype validator set if not initialized
    if (fBuiltInRegistry == 0
        || fBuiltInRegistry->get(SchemaSymbols::fgDT_STRING) == 0) {
        initializeDTDRegistry();
    }


        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_BOOLEAN, 
                       new BooleanDatatypeValidator());
        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_DECIMAL,
                       new DecimalDatatypeValidator());
        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_HEXBINARY,
                       new HexBinaryDatatypeValidator());
        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_BASE64BINARY,
                       new Base64BinaryDatatypeValidator());
/*
        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_FLOAT,
                       new FloatDatatypeValidator());
        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_DOUBLE,
                       new DoubleDatatypeValidator());
        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_ANYURI,
                       new AnyURIDatatypeValidator());
*/
        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_QNAME,
                       new QNameDatatypeValidator());
/*
        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_DURATION,
                       new DurationDatatypeValidator());
        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_DAY,
                       new DayDatatypeValidator()); 
        fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_TIME,
                       new TimeDatatypeValidator());
        */

        // REVISIT
        // We are creating a lot of Hashtables for the facets of the different
        // validators. It's better to have some kind of a memory pool and ask
        // the pool to give us a new instance of the hashtable.

        // Create 'normalizedString' datatype validator
        RefHashTableOf<KVStringPair>* facets = new RefHashTableOf<KVStringPair>(3);

        facets->put((void*) SchemaSymbols::fgELT_WHITESPACE,
                    new KVStringPair(SchemaSymbols::fgELT_WHITESPACE, SchemaSymbols::fgWS_REPLACE));

        createDatatypeValidator(SchemaSymbols::fgDT_NORMALIZEDSTRING,
                    getDatatypeValidator(SchemaSymbols::fgDT_STRING),
				    facets, 0, false, 0, false);

		// Create 'token' datatype validator
        facets = new RefHashTableOf<KVStringPair>(3);

        facets->put((void*) SchemaSymbols::fgELT_WHITESPACE,
                    new KVStringPair(SchemaSymbols::fgELT_WHITESPACE, SchemaSymbols::fgWS_COLLAPSE));

        createDatatypeValidator(SchemaSymbols::fgDT_TOKEN,
                      getDatatypeValidator(SchemaSymbols::fgDT_NORMALIZEDSTRING),
                      facets, 0, false, 0, false);

        // Create 'language' datatype validator
        facets = new RefHashTableOf<KVStringPair>(3);

        facets->put((void*) SchemaSymbols::fgELT_PATTERN,
                    new KVStringPair(SchemaSymbols::fgELT_PATTERN, fgLangPattern));

        createDatatypeValidator(SchemaSymbols::fgDT_LANGUAGE, 
                      getDatatypeValidator(SchemaSymbols::fgDT_TOKEN),
                      facets, 0, false, 0, false);

        // Create 'Name' datatype validator
        facets = new RefHashTableOf<KVStringPair>(3);

        facets->put((void*) SchemaSymbols::fgELT_PATTERN,
                    new KVStringPair(SchemaSymbols::fgELT_PATTERN, fgNamePattern));

        createDatatypeValidator(SchemaSymbols::fgDT_NAME,
                      getDatatypeValidator(SchemaSymbols::fgDT_TOKEN),
                      facets, 0, false, 0, false);

        // Create a 'NCName' datatype validator
        facets = new RefHashTableOf<KVStringPair>(3);

        facets->put((void*) SchemaSymbols::fgELT_PATTERN,
                    new KVStringPair(SchemaSymbols::fgELT_PATTERN, fgNCNamePattern));

        createDatatypeValidator(SchemaSymbols::fgDT_NCNAME,
                      getDatatypeValidator(SchemaSymbols::fgDT_TOKEN),
                      facets, 0, false, 0, false);

        // Create 'integer' datatype validator
        facets = new RefHashTableOf<KVStringPair>(3);

        facets->put((void*) SchemaSymbols::fgELT_FRACTIONDIGITS,
                    new KVStringPair(SchemaSymbols::fgELT_FRACTIONDIGITS, fgValueZero));

        createDatatypeValidator(SchemaSymbols::fgDT_INTEGER,
                      getDatatypeValidator(SchemaSymbols::fgDT_DECIMAL),
                      facets, 0, false, 0, false);

/*
        // Create 'nonPositiveInteger' datatype validator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MAXINCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MAXINCLUSIVE, fgValueZero));

        createDatatypeValidator(SchemaSymbols::fgDT_NONPOSITIVEINTEGER, 
                      getDatatypeValidator(SchemaSymbols::fgDT_INTEGER),
                      facets, 0, false, 0, false);

        // Create 'negativeInteger' datatype validator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MAXINCLUSIVE,
			        new KVStringPair(SchemaSymbols::fgELT_MAXINCLUSIVE,fgNegOne));

        createDatatypeValidator(SchemaSymbols::fgDT_NEGATIVEINTEGER, 
                      getDatatypeValidator(SchemaSymbols::fgDT_NONPOSITIVEINTEGER),
                      facets, 0, false, 0, false);

        // Create 'long' datatype validator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MAXINCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MAXINCLUSIVE,fgLongMaxInc));
        facets->put((void*) SchemaSymbols::fgELT_MININCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MININCLUSIVE,fgLongMinInc));

        createDatatypeValidator(SchemaSymbols::fgDT_LONG,
                      getDatatypeValidator(SchemaSymbols::fgDT_INTEGER),
                      facets, 0, false, 0, false);

        // Create 'int' datatype validator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MAXINCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MAXINCLUSIVE, fgIntMaxInc));
        facets->put((void*) SchemaSymbols::fgELT_MININCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MININCLUSIVE, fgIntMinInc));

        createDatatypeValidator(SchemaSymbols::fgDT_INT,
                      getDatatypeValidator(SchemaSymbols::fgDT_LONG),
                      facets, 0, false, 0, false);

        // Create 'short' datatype validator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MAXINCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MAXINCLUSIVE, fgShortMaxInc));
        facets->put((void*) SchemaSymbols::fgELT_MININCLUSIVE, 
                    new KVStringPair(SchemaSymbols::fgELT_MININCLUSIVE, fgShortMinInc));

        createDatatypeValidator(SchemaSymbols::fgDT_SHORT,
                      getDatatypeValidator(SchemaSymbols::fgDT_INT),                                
                      facets, 0, false, 0 ,false);

        // Create 'byte' datatype validator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MAXINCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MAXINCLUSIVE, fgByteMaxInc));
        facets->put((void*) SchemaSymbols::fgELT_MININCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MININCLUSIVE, fgByteMinInc));

        createDatatypeValidator(SchemaSymbols::fgDT_BYTE,
                      getDatatypeValidator(SchemaSymbols::fgDT_SHORT),
                      facets, 0, false, 0, false);
*/

        // Create 'nonNegativeInteger' datatype validator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MININCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MININCLUSIVE, fgValueZero));

        createDatatypeValidator(SchemaSymbols::fgDT_NONNEGATIVEINTEGER,
                      getDatatypeValidator(SchemaSymbols::fgDT_INTEGER),
                      facets, 0, false, 0, false);

/*
        // Create 'unsignedLong' datatype validator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MAXINCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MAXINCLUSIVE, fgULongMaxInc));

        createDatatypeValidator(SchemaSymbols::fgDT_ULONG,
                      getDatatypeValidator(SchemaSymbols::fgDT_NONNEGATIVEINTEGER),
                      facets, 0, false, 0, false);

        // Create 'unsignedInt' datatype validator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MAXINCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MAXINCLUSIVE, fgUIntMaxInc));

        createDatatypeValidator(SchemaSymbols::fgDT_UINT,
                      getDatatypeValidator(SchemaSymbols::fgDT_ULONG),
                      facets, 0, false, 0, false);

        // Create 'unsignedShort' datatypeValidator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MAXINCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MAXINCLUSIVE, fgUShortMaxInc));

        createDatatypeValidator(SchemaSymbols::fgDT_USHORT, 
                      getDatatypeValidator(SchemaSymbols::fgDT_UINT),
                      facets, 0, false, 0, false);

        // Create 'unsignedByte' datatype validator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MAXINCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MAXINCLUSIVE, fgUByteMaxInc));

        createDatatypeValidator(SchemaSymbols::fgDT_UBYTE,
                      getDatatypeValidator(SchemaSymbols::fgDT_USHORT),
                      facets, 0, false, 0, false);

        // Create 'positiveInteger' datatype validator
        facets = new RefHashTableOf<KVStringPair>(2);

        facets->put((void*) SchemaSymbols::fgELT_MININCLUSIVE,
                    new KVStringPair(SchemaSymbols::fgELT_MININCLUSIVE, fgValueOne));

        createDatatypeValidator(SchemaSymbols::fgDT_POSITIVEINTEGER,
                      getDatatypeValidator(SchemaSymbols::fgDT_NONNEGATIVEINTEGER),
                      facets, 0, false, 0, false);

        // REVISIT - Add the remaining datatype validators
        // Create 'dateTime' datatype validator

        // Create 'date' datatype validator

        // Create 'gMonthDay' datatype validator

        // Create 'gYearMonth' datatype validator

        // Create 'gYear' datatype validator

        // Create 'gMonth' datatype validator
*/

        fRegistryExpanded = 2;
}

// ---------------------------------------------------------------------------
//  DatatypeValidatorFactory: factory methods
// ---------------------------------------------------------------------------
DatatypeValidator* 
DatatypeValidatorFactory::createDatatypeValidator(const XMLCh* const typeName, 
		                                          DatatypeValidator* const baseValidator,
                                                  RefHashTableOf<KVStringPair>* const facets,
                                                  RefVectorOf<XMLCh>* const enums,
                                                  const bool derivedByList,
                                                  const int finalSet,
                                                  const bool userDefined)
{
	if (baseValidator == 0) {

        if (facets) {
            Janitor<KVStringPairHashTable> janFacets(facets);
        }

        if (enums) {
            Janitor<XMLChRefVector> janEnums(enums);
        }

        return 0;
    }

	DatatypeValidator* datatypeValidator = 0;

    if (derivedByList) {

        //TO DO
    }
    else {

        if ((baseValidator->getType() != DatatypeValidator::String) && facets) {

            KVStringPair* value = facets->get(SchemaSymbols::fgELT_WHITESPACE);

            if (value != 0) {
                facets->removeKey(SchemaSymbols::fgELT_WHITESPACE);
            }
        } 

        datatypeValidator = baseValidator->newInstance(baseValidator, facets, enums, finalSet);
    }

    if (datatypeValidator != 0) {

        if (userDefined) {

            if (!fUserDefinedRegistry) {
                fUserDefinedRegistry = new RefHashTableOf<DatatypeValidator>(29);
            }

            fUserDefinedRegistry->put((void *)typeName, datatypeValidator);
        }
        else {
            fBuiltInRegistry->put((void *)typeName, datatypeValidator);
        }
    }

    return datatypeValidator;
}


DatatypeValidator*
DatatypeValidatorFactory::createDatatypeValidator(const XMLCh* const typeName,
                                                  RefVectorOf<DatatypeValidator>* const validators,
                                                  const int finalSet,
                                                  const bool userDefined)
{
    if (validators == 0)
        return 0;

    DatatypeValidator* datatypeValidator = 0;

    //datatypeValidator = new UnionDatatypeValidator(validators, finalSet);

    if (datatypeValidator != 0) {

        if (userDefined) {

            if (!fUserDefinedRegistry) {
                fUserDefinedRegistry = new RefHashTableOf<DatatypeValidator>(29);
            }

            fUserDefinedRegistry->put((void *)typeName, datatypeValidator);
        }
        else {
            fBuiltInRegistry->put((void *)typeName, datatypeValidator);
        }
    }

    return datatypeValidator;
}

/**
  * End of file DatatypeValidatorFactory.cpp
  */

