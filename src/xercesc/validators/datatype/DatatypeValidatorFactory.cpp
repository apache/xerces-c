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
 * Revision 1.20  2003/11/06 15:30:07  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.19  2003/11/05 16:36:15  peiyongz
 * initialize BuiltInRegistry during deserialization
 *
 * Revision 1.18  2003/10/20 15:56:48  knoaman
 * Fix multithreading problem.
 *
 * Revision 1.17  2003/10/17 21:13:05  peiyongz
 * Implement Serialization/Deserialization
 *
 * Revision 1.16  2003/05/18 14:02:07  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.15  2003/05/16 21:43:20  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.14  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.13  2003/03/18 19:42:17  knoaman
 * Schema Errata E2-18.
 *
 * Revision 1.12  2003/02/26 16:14:27  peiyongz
 * Schema Errata E2-43: disallow trailing decimal point and a new pattern added
 * to the Integer definition.
 *
 * Revision 1.11  2003/01/29 19:51:48  gareth
 * type names / uris are now added to DatatypeValidators.
 *
 * Revision 1.10  2003/01/03 16:39:45  tng
 * Performance: no need to create that many members for fBuiltInRegistry
 *
 * Revision 1.9  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.8  2002/12/02 16:01:24  gareth
 * Fix to bug #12188. NCNAME validator now has a base validator of NAME. Patch by Peter Volchek.
 *
 * Revision 1.7  2002/12/02 13:25:51  gareth
 * Fix for bug #12188. Create NMTOKEN, ID, IDREF, ENTITY, NAME, NCNAME with appropriate base types. Some reordering of creation was required where dependencies resulted.
 *
 * Revision 1.6  2002/11/21 23:43:19  peiyongz
 * Schema Errata: E2-25 language
 *
 * Revision 1.5  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/05/08 12:25:30  tng
 * Schema Fix: re-add the ID, IDREF ... datatype validators only if they were not there.
 *
 * Revision 1.3  2002/05/07 20:00:07  tng
 * Schema Fix: No need to re-add the ID, IDREF ... datatype validators if the fUserDefinedRegistry already exists.
 *
 * Revision 1.2  2002/02/25 21:28:26  tng
 * Schema Fix: Thread-safe the built-in datatype validator factory.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:40  peiyongz
 * sane_include
 *
 * Revision 1.26  2001/11/07 19:18:52  peiyongz
 * DateTime Port
 *
 * Revision 1.25  2001/10/25 15:06:49  tng
 * Thread safe the static instance.
 *
 * Revision 1.24  2001/10/23 23:14:22  peiyongz
 * [Bug#880] patch to PlatformUtils:init()/term() and related. from Mark Weaver
 *
 * Revision 1.23  2001/09/25 16:00:03  peiyongz
 * DTV Reorganization: Create native NameDTV and NCNameDTV
 *
 * Revision 1.22  2001/08/24 17:12:01  knoaman
 * Add support for anySimpleType.
 * Remove parameter 'baseValidator' from the virtual method 'newInstance'.
 *
 * Revision 1.21  2001/08/01 18:49:16  peiyongz
 * AnyRUIDatatypeValidator
 *
 * Revision 1.20  2001/07/26 20:48:02  peiyongz
 * FloatDatatypeValidator
 *
 * Revision 1.19  2001/07/24 21:23:39  tng
 * Schema: Use DatatypeValidator for ID/IDREF/ENTITY/ENTITIES/NOTATION.
 *
 * Revision 1.18  2001/07/24 13:59:03  peiyongz
 * DoubleDTV
 *
 * Revision 1.17  2001/07/19 17:46:42  tng
 * Enable those derived dataype like nonPositiveinteger, negativeInteger ... etc.
 *
 * Revision 1.16  2001/07/13 14:10:33  peiyongz
 * UnionDTV
 *
 * Revision 1.15  2001/07/11 21:37:18  peiyongz
 * ListDatatypeDTV
 *
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
#include <xercesc/validators/datatype/DatatypeValidatorFactory.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/validators/datatype/StringDatatypeValidator.hpp>
#include <xercesc/validators/datatype/BooleanDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DecimalDatatypeValidator.hpp>
#include <xercesc/validators/datatype/HexBinaryDatatypeValidator.hpp>
#include <xercesc/validators/datatype/Base64BinaryDatatypeValidator.hpp>
#include <xercesc/validators/datatype/IDDatatypeValidator.hpp>
#include <xercesc/validators/datatype/IDREFDatatypeValidator.hpp>
#include <xercesc/validators/datatype/NOTATIONDatatypeValidator.hpp>
#include <xercesc/validators/datatype/ENTITYDatatypeValidator.hpp>
#include <xercesc/validators/datatype/QNameDatatypeValidator.hpp>
#include <xercesc/validators/datatype/NameDatatypeValidator.hpp>
#include <xercesc/validators/datatype/NCNameDatatypeValidator.hpp>
#include <xercesc/validators/datatype/ListDatatypeValidator.hpp>
#include <xercesc/validators/datatype/UnionDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DoubleDatatypeValidator.hpp>
#include <xercesc/validators/datatype/FloatDatatypeValidator.hpp>
#include <xercesc/validators/datatype/AnyURIDatatypeValidator.hpp>
#include <xercesc/validators/datatype/AnySimpleTypeDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DateTimeDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DateDatatypeValidator.hpp>
#include <xercesc/validators/datatype/TimeDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DayDatatypeValidator.hpp>
#include <xercesc/validators/datatype/MonthDatatypeValidator.hpp>
#include <xercesc/validators/datatype/MonthDayDatatypeValidator.hpp>
#include <xercesc/validators/datatype/YearDatatypeValidator.hpp>
#include <xercesc/validators/datatype/YearMonthDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DurationDatatypeValidator.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>

#include <xercesc/internal/XTemplateSerializer.hpp>

XERCES_CPP_NAMESPACE_BEGIN


// ---------------------------------------------------------------------------
//  DatatypeValidatorFactory: Local const data
// ---------------------------------------------------------------------------
const XMLCh fgTokPattern[] =
{
    chBackSlash, chLatin_c, chPlus, chNull
};

// http://www.w3.org/2001/05/xmlschema-errata#Errata2
// E2-25
//"([a-zA-Z]{1,8})(-[a-zA-Z0-9]{1,8})*"
const XMLCh fgLangPattern[] =
{
    chOpenParen,   chOpenSquare,  chLatin_a, chDash, chLatin_z, chLatin_A, chDash, chLatin_Z,
    chCloseSquare, chOpenCurly, chDigit_1, chComma, chDigit_8, chCloseCurly, chCloseParen,
    chOpenParen,   chDash, chOpenSquare, chLatin_a, chDash, chLatin_z, chLatin_A, chDash, chLatin_Z,
	chDigit_0,     chDash, chDigit_9, chCloseSquare, chOpenCurly, chDigit_1, chComma, chDigit_8,
    chCloseCurly,  chCloseParen, chAsterisk, chNull
};

//E2-43
//[+\-]?[0-9]+
const XMLCh fgIntegerPattern[] =
{
    chOpenSquare, chPlus, chBackSlash, chDash, chCloseSquare, chQuestion,
    chOpenSquare, chDigit_0, chDash, chDigit_9, chCloseSquare, chPlus, chNull
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
//  Local static data
// ---------------------------------------------------------------------------
static bool               sBuiltInRegistryMutexRegistered = false;
static XMLMutex*          sBuiltInRegistryMutex = 0;
static XMLRegisterCleanup builtInRegistryCleanup;


// ---------------------------------------------------------------------------
//  DatatypeValidatorFactory: Static member data
// ---------------------------------------------------------------------------
RefHashTableOf<DatatypeValidator>* DatatypeValidatorFactory::fBuiltInRegistry = 0;

// ---------------------------------------------------------------------------
//  DatatypeValidatorFactory: Constructors and Destructor
// ---------------------------------------------------------------------------
DatatypeValidatorFactory::DatatypeValidatorFactory(MemoryManager* const manager)
    : fUserDefinedRegistry(0)
    , fMemoryManager(manager)
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

}


// -----------------------------------------------------------------------
//  Notification that lazy data has been deleted
// -----------------------------------------------------------------------
void DatatypeValidatorFactory::reinitRegistry() {

    delete fBuiltInRegistry;
    fBuiltInRegistry = 0;

    // delete local static data
    delete sBuiltInRegistryMutex;
    sBuiltInRegistryMutex = 0;
    sBuiltInRegistryMutexRegistered = false;
}

// ---------------------------------------------------------------------------
//  DatatypeValidatorFactory: Registry initialization methods
// ---------------------------------------------------------------------------
void DatatypeValidatorFactory::expandRegistryToFullSchemaSet()
{
    if (!sBuiltInRegistryMutexRegistered)
    {
        XMLMutex* tmpMutex = new XMLMutex;
        if (XMLPlatformUtils::compareAndSwap((void**)&sBuiltInRegistryMutex, tmpMutex, 0))
        {
            // Someone beat us to it, so let's clean up ours
            delete tmpMutex;
        }

        // Now lock it and try to register it
        XMLMutexLock lock(sBuiltInRegistryMutex);

        // If we got here first, then register it and set the registered flag
        if (!sBuiltInRegistryMutexRegistered)
        {
            //Initialize common Schema/DTD Datatype validator set
            fBuiltInRegistry = new RefHashTableOf<DatatypeValidator>(29);

            DatatypeValidator *dv = new StringDatatypeValidator(); 
            dv->setTypeName(SchemaSymbols::fgDT_STRING, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_STRING, dv);
            
            dv = new NOTATIONDatatypeValidator();
            dv->setTypeName(XMLUni::fgNotationString, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) XMLUni::fgNotationString, dv);

            dv = new AnySimpleTypeDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_ANYSIMPLETYPE, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_ANYSIMPLETYPE, dv);

            dv = new BooleanDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_BOOLEAN, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_BOOLEAN, dv);

            dv = new DecimalDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_DECIMAL, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_DECIMAL, dv);

            dv = new HexBinaryDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_HEXBINARY, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_HEXBINARY, dv);

            dv = new Base64BinaryDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_BASE64BINARY, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_BASE64BINARY, dv);

            dv = new DoubleDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_DOUBLE, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_DOUBLE, dv);

            dv = new FloatDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_FLOAT, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_FLOAT, dv);

            dv = new AnyURIDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_ANYURI, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_ANYURI, dv);

            dv = new QNameDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_QNAME, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_QNAME, dv);

            dv = new DateTimeDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_DATETIME, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_DATETIME, dv);

            dv = new DateDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_DATE, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_DATE, dv);

            dv = new TimeDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_TIME, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_TIME, dv);

            dv = new DayDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_DAY, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_DAY, dv);

            dv = new MonthDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_MONTH, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_MONTH, dv);

            dv = new MonthDayDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_MONTHDAY, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_MONTHDAY, dv);

            dv = new YearDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_YEAR, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_YEAR, dv);

            dv = new YearMonthDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_YEARMONTH, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_YEARMONTH, dv);

            dv = new DurationDatatypeValidator();
            dv->setTypeName(SchemaSymbols::fgDT_DURATION, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_DURATION, dv);

            // REVISIT
            // We are creating a lot of Hashtables for the facets of the different
            // validators. It's better to have some kind of a memory pool and ask
            // the pool to give us a new instance of the hashtable.

            RefHashTableOf<KVStringPair>* facets = new RefHashTableOf<KVStringPair>(3);

            // Create 'normalizedString' datatype validator
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


            dv = new NameDatatypeValidator(getDatatypeValidator(SchemaSymbols::fgDT_TOKEN), 0, 0, 0);
            dv->setTypeName(SchemaSymbols::fgDT_NAME, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_NAME, dv);


            dv = new NCNameDatatypeValidator(getDatatypeValidator(SchemaSymbols::fgDT_NAME), 0, 0, 0);
            dv->setTypeName(SchemaSymbols::fgDT_NCNAME, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
            fBuiltInRegistry->put((void*) SchemaSymbols::fgDT_NCNAME, dv);

            // Create 'NMTOKEN' datatype validator
            facets = new RefHashTableOf<KVStringPair>(3);

            facets->put((void*) SchemaSymbols::fgELT_PATTERN ,
                            new KVStringPair(SchemaSymbols::fgELT_PATTERN,fgTokPattern));
            facets->put((void*) SchemaSymbols::fgELT_WHITESPACE,
                            new KVStringPair(SchemaSymbols::fgELT_WHITESPACE, SchemaSymbols::fgWS_COLLAPSE));

            createDatatypeValidator(XMLUni::fgNmTokenString,
                            getDatatypeValidator(SchemaSymbols::fgDT_TOKEN),facets, 0, false, 0, false);

            // Create 'NMTOKENS' datatype validator
            createDatatypeValidator(XMLUni::fgNmTokensString,
        	                 getDatatypeValidator(XMLUni::fgNmTokenString), 0, 0, true, 0, false);

            // Create 'language' datatype validator
            facets = new RefHashTableOf<KVStringPair>(3);

            facets->put((void*) SchemaSymbols::fgELT_PATTERN,
                        new KVStringPair(SchemaSymbols::fgELT_PATTERN, fgLangPattern));

            createDatatypeValidator(SchemaSymbols::fgDT_LANGUAGE,
                          getDatatypeValidator(SchemaSymbols::fgDT_TOKEN),
                          facets, 0, false, 0, false);

            // Create 'integer' datatype validator
            facets = new RefHashTableOf<KVStringPair>(3);

            facets->put((void*) SchemaSymbols::fgELT_FRACTIONDIGITS,
                        new KVStringPair(SchemaSymbols::fgELT_FRACTIONDIGITS, fgValueZero));

            facets->put((void*) SchemaSymbols::fgELT_PATTERN,
                        new KVStringPair(SchemaSymbols::fgELT_PATTERN, fgIntegerPattern));

            createDatatypeValidator(SchemaSymbols::fgDT_INTEGER,
                          getDatatypeValidator(SchemaSymbols::fgDT_DECIMAL),
                          facets, 0, false, 0, false);

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

            // Create 'nonNegativeInteger' datatype validator
            facets = new RefHashTableOf<KVStringPair>(2);

            facets->put((void*) SchemaSymbols::fgELT_MININCLUSIVE,
                        new KVStringPair(SchemaSymbols::fgELT_MININCLUSIVE, fgValueZero));

            createDatatypeValidator(SchemaSymbols::fgDT_NONNEGATIVEINTEGER,
                          getDatatypeValidator(SchemaSymbols::fgDT_INTEGER),
                          facets, 0, false, 0, false);

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

            // register cleanup method
            builtInRegistryCleanup.registerCleanup(reinitRegistry);
            sBuiltInRegistryMutexRegistered = true;
        }
    }

    //todo: to move these to fBuiltInRegistry

    // ID, IDREF IDREFS, ENTITY, ENTITIES  DTV have specific data member
    //   and cannot be shared across threads
    // So instead of storing them in the static fBuiltInRegistry,
    //   store them in local data fUserDefinedRegistry
    if (fUserDefinedRegistry == 0)
        fUserDefinedRegistry = new (fMemoryManager) RefHashTableOf<DatatypeValidator>(29, fMemoryManager);

    if (!getDatatypeValidator(XMLUni::fgIDRefsString)) {

        DatatypeValidator *dv = new (fMemoryManager) IDDatatypeValidator(getDatatypeValidator(SchemaSymbols::fgDT_NCNAME), 0, 0, 0, fMemoryManager);
        dv->setTypeName(XMLUni::fgIDString, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
        fUserDefinedRegistry->put((void*) XMLUni::fgIDString, dv);

        dv = new (fMemoryManager) IDREFDatatypeValidator(getDatatypeValidator(SchemaSymbols::fgDT_NCNAME), 0, 0, 0, fMemoryManager);
        dv->setTypeName(XMLUni::fgIDRefString, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
        fUserDefinedRegistry->put((void*) XMLUni::fgIDRefString, dv);

        dv = new (fMemoryManager) ENTITYDatatypeValidator(getDatatypeValidator(SchemaSymbols::fgDT_NCNAME), 0, 0, 0, fMemoryManager);
        dv->setTypeName(XMLUni::fgEntityString, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
        fUserDefinedRegistry->put((void*) XMLUni::fgEntityString, dv);

        // Create 'IDREFS' datatype validator
    	createDatatypeValidator
        (
            XMLUni::fgIDRefsString
            , getDatatypeValidator(XMLUni::fgIDRefString)
            , 0
            , 0
            , true
            , 0
        );

        // Create 'ENTITIES' datatype validator
        createDatatypeValidator
        (
            XMLUni::fgEntitiesString
    		, getDatatypeValidator(XMLUni::fgEntityString)
            , 0
            , 0
            , true
            , 0
        );
    }

}

// ---------------------------------------------------------------------------
//  DatatypeValidatorFactory: factory methods
// ---------------------------------------------------------------------------
DatatypeValidator* DatatypeValidatorFactory::createDatatypeValidator
(
      const XMLCh* const                  typeName
	, DatatypeValidator* const            baseValidator
    , RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const bool                          isDerivedByList
    , const int                           finalSet
    , const bool                          isUserDefined
)
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
    MemoryManager* const manager = (isUserDefined)
        ? fMemoryManager : XMLPlatformUtils::fgMemoryManager;

    if (isDerivedByList) {
        datatypeValidator = new (manager) ListDatatypeValidator(baseValidator, facets, enums, finalSet, manager);

        // Set PSVI information for Ordered, Numeric, Bounded & Finite
        datatypeValidator->setOrdered(XSSimpleTypeDefinition::ORDERED_FALSE);
        datatypeValidator->setNumeric(false);
        if (facets &&
             ((facets->get(SchemaSymbols::fgELT_LENGTH) || 
              (facets->get(SchemaSymbols::fgELT_MINLENGTH) && facets->get(SchemaSymbols::fgELT_MAXLENGTH)))))
        {
            datatypeValidator->setBounded(true);
            datatypeValidator->setFinite(true);
        }
        else
        {
            datatypeValidator->setBounded(false);
            datatypeValidator->setFinite(false);
        }
    }
    else {

        if ((baseValidator->getType() != DatatypeValidator::String) && facets) {

            KVStringPair* value = facets->get(SchemaSymbols::fgELT_WHITESPACE);

            if (value != 0) {
                facets->removeKey(SchemaSymbols::fgELT_WHITESPACE);
            }
        }

        datatypeValidator = baseValidator->newInstance
        (
            facets
            , enums
            , finalSet
            , manager
        );

        // Set PSVI information for Ordered, Numeric, Bounded & Finite
        datatypeValidator->setOrdered(baseValidator->getOrdered());
        datatypeValidator->setNumeric(baseValidator->getNumeric());
        if (facets  && 
            ((facets->get(SchemaSymbols::fgELT_MININCLUSIVE) || facets->get(SchemaSymbols::fgELT_MINEXCLUSIVE)) &&
             (facets->get(SchemaSymbols::fgELT_MAXINCLUSIVE) || facets->get(SchemaSymbols::fgELT_MAXEXCLUSIVE))))
        {
            datatypeValidator->setBounded(true);
        }
        else
        {
            datatypeValidator->setBounded(false);
        }
        if (baseValidator->getFinite())
        {
            datatypeValidator->setFinite(true);
        }
        else if (!facets)
        {
            datatypeValidator->setFinite(false);
        }
        else if (facets->get(SchemaSymbols::fgELT_LENGTH) || facets->get(SchemaSymbols::fgELT_MAXLENGTH) ||
                 facets->get(SchemaSymbols::fgELT_TOTALDIGITS))
        {
            datatypeValidator->setFinite(true);
        }
        //for efficiency use this instead of rechecking...
        //else if ((facets->get(SchemaSymbols::fgELT_MININCLUSIVE) || facets->get(SchemaSymbols::fgELT_MINEXCLUSIVE)) &&
        //         (facets->get(SchemaSymbols::fgELT_MAXINCLUSIVE) || facets->get(SchemaSymbols::fgELT_MAXEXCLUSIVE)))
        else if (datatypeValidator->getBounded() ||
                 datatypeValidator->getType() == DatatypeValidator::Date      || 
                 datatypeValidator->getType() == DatatypeValidator::YearMonth ||
                 datatypeValidator->getType() == DatatypeValidator::Year      ||
                 datatypeValidator->getType() == DatatypeValidator::MonthDay  ||
                 datatypeValidator->getType() == DatatypeValidator::Day       ||
                 datatypeValidator->getType() == DatatypeValidator::Month)
        {
            if (facets->get(SchemaSymbols::fgELT_FRACTIONDIGITS))
            {
                datatypeValidator->setFinite(true);
            }
            else 
            {
                datatypeValidator->setFinite(false);
            }
        }
        else 
        {
            datatypeValidator->setFinite(false);
        }      
    }

    if (datatypeValidator != 0) {

        if (isUserDefined) {

            if (!fUserDefinedRegistry) {
                fUserDefinedRegistry = new (fMemoryManager) RefHashTableOf<DatatypeValidator>(29, fMemoryManager);
            }

            fUserDefinedRegistry->put((void *)typeName, datatypeValidator);
        }
        else {
            fBuiltInRegistry->put((void *)typeName, datatypeValidator);
        }

        datatypeValidator->setTypeName(typeName);
    }

    return datatypeValidator;
}

static DatatypeValidator::ValidatorType getPrimitiveDV(DatatypeValidator::ValidatorType validationDV)
{
    if (validationDV == DatatypeValidator::ID    || 
        validationDV == DatatypeValidator::IDREF || 
        validationDV == DatatypeValidator::ENTITY)
    {
        return DatatypeValidator::String;
    }
    return validationDV;
}

DatatypeValidator* DatatypeValidatorFactory::createDatatypeValidator
(
      const XMLCh* const                    typeName
    , RefVectorOf<DatatypeValidator>* const validators
    , const int                             finalSet
    , const bool                            userDefined
)
{
    if (validators == 0)
        return 0;

    DatatypeValidator* datatypeValidator = 0;
    MemoryManager* const manager = (userDefined)
        ? fMemoryManager : XMLPlatformUtils::fgMemoryManager;

    datatypeValidator = new (manager) UnionDatatypeValidator(validators, finalSet, manager);

    if (datatypeValidator != 0) {

        if (userDefined) {

            if (!fUserDefinedRegistry) {
                fUserDefinedRegistry = new (fMemoryManager) RefHashTableOf<DatatypeValidator>(29, fMemoryManager);
            }

            fUserDefinedRegistry->put((void *)typeName, datatypeValidator);
        }
        else {
            fBuiltInRegistry->put((void *)typeName, datatypeValidator);
        }
        datatypeValidator->setTypeName(typeName);

        // Set PSVI information for Ordered, Numeric, Bounded & Finite
        unsigned int valSize = validators->size();
        if (valSize) 
        {
            DatatypeValidator::ValidatorType ancestorId = getPrimitiveDV(validators->elementAt(0)->getType());

            // For a union the ORDERED {value} is partial unless one of the following:
            // 1. If every member of {member type definitions} is derived from a common ancestor other than the simple ur-type, then {value} is the same as that ancestor's ordered facet.
            // 2. If every member of {member type definitions} has a {value} of false for the ordered facet, then {value} is false.
            bool allOrderedFalse = true;
            bool commonAnc = ancestorId != DatatypeValidator::AnySimpleType;
            bool allNumeric = true;
            bool allBounded = true;
            bool allFinite  = true;
        
            for(unsigned int i = 0 ; (i < valSize) && (commonAnc || allOrderedFalse || allNumeric || allBounded || allFinite); i++)
            {
                // for the other member types, check whether the value is false
                // and whether they have the same ancestor as the first one
                if (commonAnc)
                    commonAnc = ancestorId == getPrimitiveDV(validators->elementAt(i)->getType());
                if (allOrderedFalse)
                    allOrderedFalse = validators->elementAt(i)->getOrdered() == XSSimpleTypeDefinition::ORDERED_FALSE;
           
                if (allNumeric && !validators->elementAt(i)->getNumeric())
                {
                    allNumeric = false;
                }
                if (allBounded && (!validators->elementAt(i)->getBounded() ||
                                   ancestorId != getPrimitiveDV(validators->elementAt(i)->getType())))
                {
                    allBounded = false;
                }
                if (allFinite && !validators->elementAt(i)->getFinite())
                {
                    allFinite = false;
                }        
            }
            if (commonAnc) 
            {
                datatypeValidator->setOrdered(validators->elementAt(0)->getOrdered());
            } 
            else if (allOrderedFalse) 
            {
                datatypeValidator->setOrdered(XSSimpleTypeDefinition::ORDERED_FALSE);
            } 
            else 
            {
                datatypeValidator->setOrdered(XSSimpleTypeDefinition::ORDERED_PARTIAL);
            }
            datatypeValidator->setNumeric(allNumeric);
            datatypeValidator->setBounded(allBounded);
            datatypeValidator->setFinite(allFinite);
        }
        else // size = 0
        {
            datatypeValidator->setOrdered(XSSimpleTypeDefinition::ORDERED_PARTIAL);
            datatypeValidator->setNumeric(true);
            datatypeValidator->setBounded(true);
            datatypeValidator->setFinite(true);
        }
    }
    return datatypeValidator;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(DatatypeValidatorFactory)

void DatatypeValidatorFactory::serialize(XSerializeEngine& serEng)
{
      
    // Need not to serialize static data member, fBuiltInRegistry

    if (serEng.isStoring())
    {
        /***
         * Serialize RefHashTableOf<DatatypeValidator>
         ***/
        XTemplateSerializer::storeObject(fUserDefinedRegistry, serEng);
    }
    else
    {
        /***
         * DV in the UserDefinedRegistry rely on the fBuiltInRegistry
         * to resolve their built-in baseValidator
         ***/
        expandRegistryToFullSchemaSet();

        /***
         * Deserialize RefHashTableOf<DatatypeValidator>
         ***/
        XTemplateSerializer::loadObject(&fUserDefinedRegistry, 29, true, serEng);
    }

}

XERCES_CPP_NAMESPACE_END

/**
  * End of file DatatypeValidatorFactory.cpp
  */

