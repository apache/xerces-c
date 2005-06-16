/*
 * Copyright 2002-2004 The Apache Software Foundation.
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
 * $Log$
 * Revision 1.58  2005/04/21 09:05:57  cargilld
 * Add detection for UCS-2 and treat as another UTF16 alias.
 *
 * Revision 1.57  2005/03/20 19:02:45  cargilld
 * Implement versions of uppercase and compareIstring that only check a to z, instead of all characters, and don't rely on functionality provided in the transcoders.
 *
 * Revision 1.56  2005/03/08 09:33:32  amassari
 * Redeclare a namespace binding if the prefix is already bound to a different namespace (jira# 1371)
 *
 * Revision 1.55  2004/09/08 13:55:52  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.54  2004/07/06 15:16:57  amassari
 * Fix for jira#1238: DOMWriter was creating an XMLFormatter without propagating the memory manager
 *
 * Revision 1.53  2004/05/10 08:03:25  amassari
 * Performance: push a new map on the namespace stack only when an element has a xmlns attribute
 *
 * Revision 1.52  2004/04/01 22:05:32  peiyongz
 * invoke DOMException with Memory Manager
 *
 * Revision 1.51  2004/01/29 11:44:27  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.50  2004/01/20 23:23:57  peiyongz
 * patch to Bug#25751
 *
 * Revision 1.49  2004/01/13 20:47:42  knoaman
 * Remove unnecessary local static data
 *
 * Revision 1.48  2004/01/13 16:34:20  cargilld
 * Misc memory management changes.
 *
 * Revision 1.47  2003/12/17 00:18:33  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.46  2003/11/24 19:52:22  neilg
 * fix a typo
 *
 * Revision 1.45  2003/11/24 12:27:57  gareth
 * added in support for xml-declaration feature.
 *
 * Revision 1.44  2003/11/24 11:10:58  gareth
 * Fix for bug 22917. Patch by Adam Heinz .
 *
 * Revision 1.43  2003/11/10 17:52:58  amassari
 * Fixed memory leak
 *
 * Revision 1.42  2003/10/01 16:32:37  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.41  2003/08/14 16:31:13  gareth
 * Method added to allow serilization of custom nodes from derived classes.
 *
 * Revision 1.40  2003/08/12 12:46:57  gareth
 * Added serialization for attribute nodes. Patch by Caroline Rioux.
 *
 * Revision 1.39  2003/05/18 14:02:03  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.38  2003/05/16 21:36:55  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.37  2003/05/15 18:25:54  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.36  2003/05/14 16:20:13  gareth
 * Fix to problem with multiple default namespace attributes being serialized. Patch by Alberto Massari.
 *
 * Revision 1.35  2003/05/12 16:08:11  gareth
 * fix to #18832. Corrected serilization with regards to namespace nodes. Patch by Alby Massari.
 *
 * Revision 1.34  2003/05/06 13:48:35  neilg
 * fix GCC compilation problem and incorrect #include
 *
 * Revision 1.33  2003/05/05 21:23:21  neilg
 * use of the new DOMNodeSPtr typedef and its friends to enable reference counting in
 * the DOMWriter implementation for applications that require it.
 *
 * Revision 1.32  2003/04/02 03:14:42  peiyongz
 * Bug#18594: DOMWriter does not recognize Document Fragment
 *
 * Revision 1.31  2003/03/16 05:42:04  peiyongz
 * Bug#17983 Formatter does not escape control characters
 *
 * Revision 1.30  2003/02/25 16:07:37  tng
 * [Bug 13493] Use const on static data in DOMWriterImpl.cpp.
 *
 * Revision 1.29  2003/01/28 18:31:47  peiyongz
 * Bug#13694: Allow Xerces to write the BOM to XML files
 *
 * Revision 1.28  2003/01/24 20:21:46  tng
 * DOMWriter: Call XMLFormatTarget::flush when done.
 *
 * Revision 1.27  2003/01/20 16:50:13  tng
 * DOMWriter fix:
 * 1. wrong wrong nested cdata message
 * 2. pretty format the cdata section
 * 3. do not increment error count if warning was issued
 *
 * Revision 1.26  2003/01/09 19:53:45  tng
 * [Bug 15372] DOMBuilder::parseFromURI ignores result of handleErrors.
 *
 * Revision 1.25  2002/12/10 21:01:32  tng
 * NLS: DOMWriter should use message loader to load message instead of using hardcoded static stirng
 *
 * Revision 1.24  2002/12/10 18:59:14  tng
 * pretty format print: consistent newline
 *
 * Revision 1.23  2002/12/10 13:34:07  tng
 * Pretty-format print: also indent PI/comment that appear inside the root element.
 *
 * Revision 1.22  2002/12/09 11:46:08  gareth
 * More pretty pretty print feature. Patch by Kevin King. Closes bug #13840.
 *
 * Revision 1.21  2002/12/02 23:08:09  peiyongz
 * fix to bug#14528: output n+1 cdatasection
 *
 * Revision 1.20  2002/11/13 21:51:22  peiyongz
 * fix to Bug#14528
 *
 * Revision 1.19  2002/11/04 15:07:35  tng
 * C++ Namespace Support.
 *
 * Revision 1.18  2002/10/03 18:13:38  peiyongz
 * Bug#12560 Use const in DOMWriter - patch from Duncan Stodart
 *                                                              (Duncan_Stodart@insession.com )
 *
 * Revision 1.17  2002/09/24 20:19:14  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 * and check for null string directly isntead of calling XMLString::stringLen
 *
 * Revision 1.16  2002/09/09 15:42:14  peiyongz
 * Patch to Bug#12369: invalid output from DOMWriter using MemBufFormatTarget
 *
 * Revision 1.15  2002/08/07 18:10:19  peiyongz
 * Fix to Bug#11534: Wrong CDATA Terminator in DOMWriterImpl
 *
 * Revision 1.14  2002/07/22 23:24:01  tng
 * DOM L3: writeToString should use the fFormatter to do the transcoding
 *
 * Revision 1.13  2002/07/16 15:19:42  peiyongz
 * check lenght of getEncoding()/getActualEncoding()
 *
 * Revision 1.12  2002/06/25 16:17:16  tng
 * DOM L3: add release()
 *
 * Revision 1.11  2002/06/21 19:33:12  peiyongz
 * support for feature split_cdata_section and entities revised.
 *
 * Revision 1.10  2002/06/18 15:35:25  peiyongz
 * Bug#9950: Compilation error on MSVC5, patch from PeterV@ti.com.od.ua (Peter A. Volchek)
 *
 * Revision 1.9  2002/06/17 19:45:58  peiyongz
 * optimization on fFeatures and featureId introduced
 *
 * Revision 1.8  2002/06/11 19:45:45  peiyongz
 * Notify application of the XMLFormatter creation failure
 *
 * Revision 1.7  2002/06/10 16:02:21  peiyongz
 * format-pretty-print partially supported
 * resolve encoding from DOMDocument Interface
 *
 * Revision 1.6  2002/06/06 20:58:37  tng
 * [Bug 9639] enum_mem in DOMError clashes with constant.
 *
 * Revision 1.5  2002/06/05 16:03:03  peiyongz
 * delete[] used.
 *
 * Revision 1.4  2002/06/03 22:35:54  peiyongz
 * constants changed
 *
 * Revision 1.3  2002/05/31 21:01:06  peiyongz
 * move printing of XMLDecl into the processNode().
 *
 * Revision 1.2  2002/05/29 21:31:50  knoaman
 * DOM L3 LS: DOMInputSource, DOMEntityResolver, DOMImplementationLS and DOMBuilder
 *
 * Revision 1.1  2002/05/28 22:39:39  peiyongz
 * DOM3 Save Interface: DOMWriter/DOMWriterFilter
 *
 */

#include "DOMWriterImpl.hpp"
#include "DOMErrorImpl.hpp"
#include "DOMLocatorImpl.hpp"
#include "DOMImplementationImpl.hpp"

#include <xercesc/framework/MemBufFormatTarget.hpp>

#include <xercesc/util/TransService.hpp>
#include <xercesc/util/TranscodingException.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/dom/StDOMNode.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN


// ---------------------------------------------------------------------------
//  Local const data
//
// ---------------------------------------------------------------------------

static const int INVALID_FEATURE_ID               = -1;
static const int CANONICAL_FORM_ID                = 0x0;
static const int DISCARD_DEFAULT_CONTENT_ID       = 0x1;
static const int ENTITIES_ID                      = 0x2;
static const int FORMAT_PRETTY_PRINT_ID           = 0x3;
static const int NORMALIZE_CHARACTERS_ID          = 0x4;
static const int SPLIT_CDATA_SECTIONS_ID          = 0x5;
static const int VALIDATION_ID                    = 0x6;
static const int WHITESPACE_IN_ELEMENT_CONTENT_ID = 0x7;
static const int BYTE_ORDER_MARK_ID               = 0x8;
static const int XML_DECLARATION                 = 0x9;

//    feature                      true                       false
// ================================================================================
//canonical-form                 [optional] Not Supported     [required] (default)
//discard-default-content        [required] (default)         [required]
//entity                         [required] (default)         [optional]
//format-pretty-print            [optional] Partially Supported [required] (default)
//normalize-characters           [optional] Not Supported     [required] (default)
//split-cdata-sections           [required] (default)         [required]
//validation                     [optional] Not Supported     [required] (default)
//whitespace-in-element-content  [requierd] (default)         [optional] Not Supported
//

//
// Each feature has 2 entries in this array,
// the first for "true",
// the second for "false".
//
static const bool  featuresSupported[] = {
    false, true,  // canonical-form
    true,  true,  // discard-default-content
    true,  true,  // entity
    true,  true,  // format-pretty-print
    false, true,  // normalize-characters
    true,  true,  // split-cdata-sections
    false, true,  // validation
    true,  false, // whitespace-in-element-content
    true,  true,   // byte-order-mark
    true,  true   // xml-declaration
};

// default end-of-line sequence
static const XMLCh  gEOLSeq[] =
{
    chLF, chNull
};

//UTF-8
static const XMLCh  gUTF8[] =
{
    chLatin_U, chLatin_T, chLatin_F, chDash, chDigit_8, chNull
};

//</
static const XMLCh  gEndElement[] =
{
    chOpenAngle, chForwardSlash, chNull
};

//?>
static const XMLCh  gEndPI[] =
{
    chQuestion, chCloseAngle, chNull
};

//<?
static const XMLCh  gStartPI[] =
{
    chOpenAngle, chQuestion, chNull
};

//<?xml version="
static const XMLCh  gXMLDecl_VersionInfo[] =
{
    chOpenAngle, chQuestion, chLatin_x,     chLatin_m,  chLatin_l,  chSpace,
    chLatin_v,   chLatin_e,  chLatin_r,     chLatin_s,  chLatin_i,  chLatin_o,
    chLatin_n,   chEqual,    chDoubleQuote, chNull
};

static const XMLCh gXMLDecl_ver10[] =
{
    chDigit_1, chPeriod, chDigit_0, chNull
};

//encoding="
static const XMLCh  gXMLDecl_EncodingDecl[] =
{
    chLatin_e,  chLatin_n,  chLatin_c,  chLatin_o,      chLatin_d, chLatin_i,
    chLatin_n,  chLatin_g,  chEqual,    chDoubleQuote,  chNull
};

//" standalone="
static const XMLCh  gXMLDecl_SDDecl[] =
{
    chLatin_s, chLatin_t, chLatin_a,   chLatin_n,    chLatin_d,   chLatin_a,
    chLatin_l, chLatin_o, chLatin_n,   chLatin_e,    chEqual,     chDoubleQuote,
    chNull
};

//"
static const XMLCh  gXMLDecl_separator[] =
{
    chDoubleQuote, chSpace, chNull
};

//?>
static const XMLCh  gXMLDecl_endtag[] =
{
    chQuestion, chCloseAngle,  chNull
};

//<![CDATA[
static const XMLCh  gStartCDATA[] =
{
    chOpenAngle, chBang,    chOpenSquare, chLatin_C, chLatin_D,
    chLatin_A,   chLatin_T, chLatin_A,    chOpenSquare, chNull
};

//]]>
static const XMLCh  gEndCDATA[] =
{
//    chCloseSquare, chCloseAngle, chCloseAngle, chNull  // test only: ]>>
      chCloseSquare, chCloseSquare, chCloseAngle, chNull
};

static const int offset = XMLString::stringLen(gEndCDATA);

//<!--
static const XMLCh  gStartComment[] =
{
    chOpenAngle, chBang, chDash, chDash, chNull
};

//-->
static const XMLCh  gEndComment[] =
{
    chDash, chDash, chCloseAngle, chNull
};

//<!DOCTYPE
static const XMLCh  gStartDoctype[] =
{
    chOpenAngle, chBang,    chLatin_D, chLatin_O, chLatin_C, chLatin_T,
    chLatin_Y,   chLatin_P, chLatin_E, chSpace,   chNull
};

//PUBLIC "
static const XMLCh  gPublic[] =
{
    chLatin_P, chLatin_U, chLatin_B,     chLatin_L, chLatin_I,
    chLatin_C, chSpace,   chDoubleQuote, chNull
};

//SYSTEM "
static const XMLCh  gSystem[] =
{
    chLatin_S, chLatin_Y, chLatin_S,     chLatin_T, chLatin_E,
    chLatin_M, chSpace,   chDoubleQuote, chNull
};

//<!ENTITY
static const XMLCh  gStartEntity[] =
{
    chOpenAngle, chBang,    chLatin_E, chLatin_N, chLatin_T, chLatin_I,
    chLatin_T,   chLatin_Y, chSpace,   chNull
};

//NDATA "
static const XMLCh  gNotation[] =
{
    chLatin_N, chLatin_D,     chLatin_A, chLatin_T, chLatin_A,
    chSpace,   chDoubleQuote, chNull
};

//Feature
static const XMLCh  gFeature[] =
{
    chLatin_F, chLatin_e, chLatin_a, chLatin_t, chLatin_u, chLatin_r,
    chLatin_e, chSpace,   chNull
};

// Can not be set to
static const XMLCh  gCantSet[] =
{
    chSpace,   chLatin_C, chLatin_a, chLatin_n, chSpace, chLatin_n, chLatin_o,
    chLatin_t, chSpace,   chLatin_b, chLatin_e, chSpace, chLatin_s,
    chLatin_e, chLatin_t, chSpace,   chLatin_t, chLatin_o, chSpace, chNull
};

static const XMLCh  gTrue[] =
{
    chSingleQuote, chLatin_t, chLatin_r, chLatin_u, chLatin_e,
    chSingleQuote, chLF,      chNull
};

static const XMLCh  gFalse[] =
{
    chSingleQuote, chLatin_f, chLatin_a, chLatin_l, chLatin_s,
    chLatin_e,     chSingleQuote, chLF, chNull
};

static const XMLByte  BOM_utf16be[] = {(XMLByte)0xFE, (XMLByte)0xFF, (XMLByte) 0};
static const XMLByte  BOM_utf16le[] = {(XMLByte)0xFF, (XMLByte)0xFE, (XMLByte) 0};
static const XMLByte  BOM_ucs4be[]  = {(XMLByte)0x00, (XMLByte)0x00, (XMLByte)0xFE, (XMLByte)0xFF, (XMLByte) 0};
static const XMLByte  BOM_ucs4le[]  = {(XMLByte)0xFF, (XMLByte)0xFE, (XMLByte)0x00, (XMLByte)0x00, (XMLByte) 0};

static bool lineFeedInTextNodePrinted = false;
static int  lastWhiteSpaceInTextNode = 0;

//
// Notification of the error though error handler
//
// The application may instruct the engine to abort serialization
// by returning "false".
//
// REVISIT: update the locator ctor once the line#, col#, uri and offset
// are available from DOM3 core
//
// REVISIT: use throwing exception to abort serialization is an interesting
// thing here, since the serializer is a recusive function, we
// can't use return, obviously. However we may have multiple try/catch
// along its way going back to writeNode(). So far we don't come up with a
// "short-cut" to go "directly" back.
//
#define  TRY_CATCH_THROW(action, forceToRethrow)                     \
fFormatter->setUnRepFlags(XMLFormatter::UnRep_Fail);                 \
try                                                                  \
{                                                                    \
    action;                                                          \
}                                                                    \
catch(TranscodingException const &e)                                 \
{                                                                    \
    if ( !reportError(nodeToWrite                                    \
                    , DOMError::DOM_SEVERITY_FATAL_ERROR             \
                    , e.getMessage())                       ||       \
          forceToRethrow)                                            \
        throw e;                                                       \
}

DOMWriterImpl::~DOMWriterImpl()
{
    fMemoryManager->deallocate(fEncoding);//delete [] fEncoding;
    fMemoryManager->deallocate(fNewLine);//delete [] fNewLine;
    delete fNamespaceStack;
    // we don't own/adopt error handler and filter
}

DOMWriterImpl::DOMWriterImpl(MemoryManager* const manager)
:fFeatures(0)
,fEncoding(0)
,fNewLine(0)
,fErrorHandler(0)
,fFilter(0)
,fDocumentVersion(XMLUni::fgVersion1_0)
,fEncodingUsed(0)
,fNewLineUsed(0)
,fFormatter(0)
,fErrorCount(0)
,fCurrentLine(0)
,fNamespaceStack(0)
,fMemoryManager(manager)
{
    fNamespaceStack=new (fMemoryManager) RefVectorOf< RefHashTableOf<XMLCh> >(0,true, fMemoryManager);

    //
    // set features to default setting
    //
    setFeature(CANONICAL_FORM_ID,                false);
    setFeature(DISCARD_DEFAULT_CONTENT_ID,       true );
    setFeature(ENTITIES_ID,                      true );
    setFeature(FORMAT_PRETTY_PRINT_ID,           false);
    setFeature(NORMALIZE_CHARACTERS_ID,          false);
    setFeature(SPLIT_CDATA_SECTIONS_ID,          true );
    setFeature(VALIDATION_ID,                    false);
    setFeature(WHITESPACE_IN_ELEMENT_CONTENT_ID, true );
    setFeature(BYTE_ORDER_MARK_ID,               false);
    setFeature(XML_DECLARATION,                 true );

}

bool DOMWriterImpl::canSetFeature(const XMLCh* const featName
                                  , bool               state) const
{
    int featureId = INVALID_FEATURE_ID;
    return checkFeature(featName, false, featureId) ? canSetFeature(featureId, state) : false;
}

void DOMWriterImpl::setFeature(const XMLCh* const featName
                             , bool               state)
{
    int featureId = INVALID_FEATURE_ID;
    checkFeature(featName, true, featureId);

    if (!canSetFeature(featureId, state))
    {
        XMLCh  tmpbuf[256];
        unsigned int strLen = XMLString::stringLen(gFeature) +
                              XMLString::stringLen(featName) +
                              XMLString::stringLen(gCantSet) +
                              XMLString::stringLen(gFalse);

        XMLString::copyString(tmpbuf, gFeature);
        if (strLen < 256)
        {
            XMLString::catString(tmpbuf, featName);
        }
        else
        {
            // truncate the featurename to fit into the 256 buffer
            XMLString::copyNString(tmpbuf+XMLString::stringLen(gFeature),
                                   featName, 200);
        }
        XMLString::catString(tmpbuf, gCantSet);
        XMLString::catString(tmpbuf, state? gTrue : gFalse);
        throw DOMException(DOMException::NOT_SUPPORTED_ERR, tmpbuf, fMemoryManager);
    }
    else
        setFeature(featureId, state);

    //
    // canonical-form and format-pretty-print can not be both set to true
    // meaning set canonical-form true will automatically set
    // format-pretty-print to false and vise versa.
    //
    if ((featureId == CANONICAL_FORM_ID) && state)
        setFeature(FORMAT_PRETTY_PRINT_ID, false);

    if ((featureId == FORMAT_PRETTY_PRINT_ID) && state)
        setFeature(CANONICAL_FORM_ID, false);

    return;
}

bool DOMWriterImpl::getFeature(const XMLCh* const featName) const
{
    int featureId = INVALID_FEATURE_ID;
    checkFeature(featName, true, featureId);
    return getFeature(featureId);
}

// we don't check the validity of the encoding set
void DOMWriterImpl::setEncoding(const XMLCh* const encoding)
{
    fMemoryManager->deallocate(fEncoding);//delete [] fEncoding;
    fEncoding = XMLString::replicate(encoding, fMemoryManager);
}

const XMLCh* DOMWriterImpl::getEncoding() const
{
    return fEncoding;
}

void DOMWriterImpl::setNewLine(const XMLCh* const newLine)
{
    fMemoryManager->deallocate(fNewLine);//delete [] fNewLine;
    fNewLine = XMLString::replicate(newLine, fMemoryManager);
}

const XMLCh* DOMWriterImpl::getNewLine() const
{
    return fNewLine;
}

void DOMWriterImpl::setErrorHandler(DOMErrorHandler *errorHandler)
{
    fErrorHandler = errorHandler;
}

DOMErrorHandler* DOMWriterImpl::getErrorHandler() const
{
    return fErrorHandler;
}

void DOMWriterImpl::setFilter(DOMWriterFilter *filter)
{
    fFilter = filter;
}

DOMWriterFilter* DOMWriterImpl::getFilter() const
{
    return fFilter;
}

//
//
//
bool DOMWriterImpl::writeNode(XMLFormatTarget* const destination
                            , const DOMNode         &nodeToWrite)
{
    //init session vars
    initSession(&nodeToWrite);

    try
    {
        fFormatter = new (fMemoryManager) XMLFormatter(fEncodingUsed
                                     ,fDocumentVersion
                                     ,destination
                                     ,XMLFormatter::NoEscapes
                                     ,XMLFormatter::UnRep_CharRef
                                     ,fMemoryManager);
    }
    catch (const TranscodingException& e)
    {
        reportError(&nodeToWrite, DOMError::DOM_SEVERITY_FATAL_ERROR, e.getMessage());
        return false;
    }

    try
    {
        Janitor<XMLFormatter> janName(fFormatter);
        processNode(&nodeToWrite);
        destination->flush();
    }

    //
    // The serialize engine (processNode) throws an exception to abort
    // serialization if
    //
    //   . A fatal error occurs which renters the output ill-formed, or
    //   . Instructed by the application's error handler
    //
    catch (const TranscodingException&)
    {
        destination->flush();
        return false;
    }

    catch (const XMLDOMMsg::Codes)
    {
        destination->flush();
        return false;
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    //
    // DOMSystemException
    // This exception will be raised in response to any sort of IO or system
    // error that occurs while writing to the destination. It may wrap an
    // underlying system exception.
    //
    //catch (RuntimeException const &)
    catch (...)
    {
        // REVISIT generate a DOMSystemException wrapping the underlying
        //         exception.
        destination->flush();
        throw;
    }

    //
    // true if node was successfully serialized and
    // false in case a failure occured and the
    // failure wasn't canceled by the error handler.
    //
    return ((fErrorCount == 0)? true : false);
}

//
// We don't throw DOMSTRING_SIZE_ERR since we are no longer
// using DOMString.
//
XMLCh* DOMWriterImpl::writeToString(const DOMNode &nodeToWrite)
{
    MemBufFormatTarget  destination(1023, fMemoryManager);
    bool retVal;

    // XMLCh is unicode, assume fEncoding as UTF-16
    XMLCh* tempEncoding = fEncoding;
    fEncoding = (XMLCh*) XMLUni::fgUTF16EncodingString;

    try
    {
        retVal = writeNode(&destination, nodeToWrite);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch (...)
    {
        //
        // there is a possibility that memeory allocation
        // exception thrown in XMLBuffer class
        //
        fEncoding = tempEncoding;
        return 0;
    }

    fEncoding = tempEncoding;
    return (retVal ? XMLString::replicate((XMLCh*) destination.getRawBuffer(), fMemoryManager) : 0);
}

void DOMWriterImpl::initSession(const DOMNode* const nodeToWrite)
{

/**
 * The encoding to use when writing is determined as follows:
 *     If the encoding attribute has been set, that value will be used.
 *     If the encoding attribute is null or empty,
 *            but the item to be written, or
 *            the owner document specified encoding (ie. the "actualEncoding"
 *            from the document) that value will be used.
 *     If neither of the above provides an encoding name, a default encoding of
 *            "UTF-8" will be used.
 */
    fEncodingUsed = gUTF8;

    if (fEncoding && *fEncoding)
    {
        fEncodingUsed = fEncoding;
    }
    else
    {
        const DOMDocument *docu = (nodeToWrite->getNodeType() == DOMNode::DOCUMENT_NODE)?
                            (const DOMDocument*)nodeToWrite : nodeToWrite->getOwnerDocument();
        if (docu)
        {
            const XMLCh* tmpEncoding = docu->getEncoding();

            if ( tmpEncoding && *tmpEncoding)
            {
                fEncodingUsed = tmpEncoding;
            }
            else
            {
                tmpEncoding = docu->getActualEncoding();

                if ( tmpEncoding && *tmpEncoding)
                {
                    fEncodingUsed = tmpEncoding;
                }
            }
        }
    }

    /**
     *  The end-of-line sequence of characters to be used in the XML being
     *  written out. The only permitted values are these:
     *     . null
     *
     *  Use a default end-of-line sequence. DOM implementations should choose
     * the default to match the usual convention for text files in the
     * environment being used. Implementations must choose a default
     * sequence that matches one of those allowed by  2.11 "End-of-Line
     * Handling".
     *
     *    CR    The carriage-return character (#xD)
     *    CR-LF The carriage-return and line-feed characters (#xD #xA)
     *    LF    The line-feed character (#xA)
     *
     *  The default value for this attribute is null
     */
    fNewLineUsed = (fNewLine && *fNewLine)? fNewLine : gEOLSeq;

    /**
     *  get Document Version
     */
    const DOMDocument *docu = (nodeToWrite->getNodeType() == DOMNode::DOCUMENT_NODE)?
                              (const DOMDocument*)nodeToWrite : nodeToWrite->getOwnerDocument();
    if (docu)
    {
        fDocumentVersion = docu->getVersion();
    }

    fErrorCount = 0;
}

//
// Characters not representable in output encoding,
//
// 1. CHARACTER DATA (outside of markup)                --- no error
//    ordinary character  -> numeric character reference
//    '<' and '&'         -> &lt; and &amp;
//
// 2. Within MARKUP, but outside of attributes
//    reported as an error                                 --- ERROR
//    markup:
//           start tag                                  done
//           end tag                                    done
//           empty element tag                          done
//           entity references                          done
//           character references    // REVISIT
//           comments                                   done
//           CDATA section delimiters                   done, done
//           document type declarartions                done
//           processing instructions (PI)               done
//
// 3. With in ATTRIBUTE
//    -> numeric character reference
//    no quotes                        -> in quotes
//    with quotes, no apostrophe       -> in apostrophe
//    with quotes and apostrophe       -> in quotes and &quot;
//
// 4. CDATA sections
//    "split_cdata_section"  true                      --- char ref
//                           false                     ---      ERROR
//
// ---------------------------------------------------------------------------
//  Stream out a DOM node, and, recursively, all of its children. This
//  function is the heart of writing a DOM tree out as XML source. Give it
//  a document node and it will do the whole thing.
// ---------------------------------------------------------------------------

void DOMWriterImpl::processNode(const DOMNode* const nodeToWrite, int level)
{

    // Get the name and value out for convenience
    const XMLCh*    nodeName = nodeToWrite->getNodeName();
    const XMLCh*    nodeValue = nodeToWrite->getNodeValue();
    unsigned long   lent = XMLString::stringLen(nodeValue);

    switch (nodeToWrite->getNodeType())
    {
    case DOMNode::TEXT_NODE:
        {
            if (checkFilter(nodeToWrite) != DOMNodeFilter::FILTER_ACCEPT)
                break;

            if (getFeature(FORMAT_PRETTY_PRINT_ID))
            {
                lineFeedInTextNodePrinted = false;
                lastWhiteSpaceInTextNode = 0;

                if(XMLString::isAllWhiteSpace(nodeValue))
                {
                    // skips whitespace-only text nodes unless whitespace-in-element is set.
                    if (!getFeature(WHITESPACE_IN_ELEMENT_CONTENT_ID))
                    {
                        break;
                    }
                    else        
                    {
                        //
                        // we need to trace if newline(s) have been printed out
                        // to avoid generate extra newline for pretty printing,
                        // as well as the number of whitespaces after the last
                        // newline character to do indentation properly.
                        //
                        int pos = XMLString::lastIndexOf(nodeValue, chLF);
                        if (-1 != pos)
                        {
                            lineFeedInTextNodePrinted = true;
                            lastWhiteSpaceInTextNode = lent - pos;
                        }
                        else
                        {
                            // for those platforms using chCR alone as
                            // a newline character
                            pos = XMLString::lastIndexOf(nodeValue, chCR);
                            if (-1 != pos)
                            {
                                lineFeedInTextNodePrinted = true;
                                lastWhiteSpaceInTextNode = lent - pos;
                            }
                        }
                    }
                }
            }

            setURCharRef();      // character data
            fFormatter->formatBuf(nodeValue, lent, XMLFormatter::CharEscapes);
            break;
        }

    case DOMNode::PROCESSING_INSTRUCTION_NODE:
        {
            if (checkFilter(nodeToWrite) != DOMNodeFilter::FILTER_ACCEPT)
                break;

            if(level == 1)
                printNewLine();

            printNewLine();
            printIndent(level);

            TRY_CATCH_THROW
            (
                *fFormatter << XMLFormatter::NoEscapes << gStartPI << nodeName;
                if (lent > 0)
                {
                    *fFormatter << chSpace << nodeValue;
                }
                *fFormatter << gEndPI;
                ,true
            )
            break;
        }

    case DOMNode::DOCUMENT_NODE: // Not to be shown to Filter
        {

            // output BOM if needed
            processBOM();          
            
            setURCharRef();
            const DOMDocument *docu = (const DOMDocument*)nodeToWrite;
        
            //[23] XMLDecl      ::= '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
            //[24] VersionInfo  ::= S 'version' Eq ("'" VersionNum "'" | '"' VersionNum '"')
            //[80] EncodingDecl ::= S 'encoding' Eq ('"' EncName '"' | "'" EncName
            //[32] SDDecl       ::= S 'standalone' Eq (("'" ('yes' | 'no') "'") | ('"' ('yes' | 'no') '"'))
            //

            if (getFeature(XML_DECLARATION)) {
                const XMLCh* versionNo = (docu->getVersion()) ? docu->getVersion() : gXMLDecl_ver10;
                *fFormatter << gXMLDecl_VersionInfo << versionNo << gXMLDecl_separator;

                // use the encoding resolved in initSession()
                *fFormatter << gXMLDecl_EncodingDecl << fEncodingUsed << gXMLDecl_separator;

                const XMLCh* st = (docu->getStandalone())? XMLUni::fgYesString : XMLUni::fgNoString;
                *fFormatter << gXMLDecl_SDDecl << st << gXMLDecl_separator;
                
                *fFormatter << gXMLDecl_endtag;
            }

            DOMNodeSPtr child = nodeToWrite->getFirstChild();
            while( child != 0)
            {
                processNode(child, level);
                child = child->getNextSibling();
            }
            printNewLine();
            break;
        }

    case DOMNode::DOCUMENT_FRAGMENT_NODE:
        {
           
            setURCharRef();

            DOMNode *child = nodeToWrite->getFirstChild();
            while( child != 0)
            {
                processNode(child, level);
                child = child->getNextSibling();
            }
            printNewLine();
            break;
        }

    case DOMNode::ELEMENT_NODE:
        {
            DOMNodeFilter::FilterAction filterAction = checkFilter(nodeToWrite);

            if ( filterAction == DOMNodeFilter::FILTER_REJECT)
                break;

            if (!lineFeedInTextNodePrinted)
            {
                if(level == 1)
                    printNewLine();

                printNewLine();
            }
            else
            {
                lineFeedInTextNodePrinted = false;
            }

            printIndent(level);

            //track the line number the current node begins on
            int nodeLine = fCurrentLine;

            // add an entry in the namespace stack
            RefHashTableOf<XMLCh>* namespaceMap=NULL;

            if ( filterAction == DOMNodeFilter::FILTER_ACCEPT)
            {
                //           this element    attributes   child elements
                // accept        yes             yes           yes
                // skip          no              no            yes
                //
                TRY_CATCH_THROW
                (
                // The name has to be representable without any escapes
                    *fFormatter  << XMLFormatter::NoEscapes
                                 << chOpenAngle << nodeName;
                    ,true
                )

                // Output any attributes on this element
                setURCharRef();
                DOMNamedNodeMap *attributes = nodeToWrite->getAttributes();
                int attrCount = attributes->getLength();

                // check if the namespace for the current node is already defined
                const XMLCh* prefix = nodeToWrite->getPrefix();
                const XMLCh* uri = nodeToWrite->getNamespaceURI();
                if(uri && uri[0])
                {
                    if(prefix==0 || prefix[0]==0)
                        prefix=XMLUni::fgZeroLenString;
                    bool bPrefixDeclared=false;
                    for(int i=fNamespaceStack->size()-1;i>=0;i--)
                    {
                        RefHashTableOf<XMLCh>* curNamespaceMap=fNamespaceStack->elementAt(i);
                        const XMLCh* thisUri=curNamespaceMap->get((void*)prefix);
                        if(thisUri)
                        {
                            // the prefix has been declared: check if it binds to the correct namespace, otherwise, redeclare it
                            if(XMLString::equals(thisUri,nodeToWrite->getNamespaceURI()))
                                bPrefixDeclared=true;
                            break;
                        }
                    }
                    if(!bPrefixDeclared)
                    {
                        if(namespaceMap==NULL)
                        {
                            namespaceMap=new (fMemoryManager) RefHashTableOf<XMLCh>(12, false, fMemoryManager);
                            fNamespaceStack->addElement(namespaceMap);
                        }
                        namespaceMap->put((void*)prefix,(XMLCh*)nodeToWrite->getNamespaceURI());
                        *fFormatter  << XMLFormatter::NoEscapes
                                     << chSpace << XMLUni::fgXMLNSString;
                        if(!XMLString::equals(prefix,XMLUni::fgZeroLenString))
                            *fFormatter  << chColon << prefix;
                        *fFormatter  << chEqual << chDoubleQuote
                                     << XMLFormatter::AttrEscapes
                                     << nodeToWrite->getNamespaceURI()
                                     << XMLFormatter::NoEscapes
                                     << chDoubleQuote;
                    }
                }

                bool discard = getFeature(DISCARD_DEFAULT_CONTENT_ID);
                for (int i = 0; i < attrCount; i++)
                {
                    DOMAttrSPtr  attribute = (DOMAttr*)attributes->item(i);

                    // Not to be shown to Filter

                    //
                    //"discard-default-content"
                    //    true
                    //    [required] (default)
                    //    Use whatever information available to the implementation
                    //  (i.e. XML schema, DTD, the specified flag on Attr nodes,
                    //  and so on) to decide what attributes and content should be
                    //  discarded or not.
                    //  Note that the specified flag on Attr nodes in itself is
                    //  not always reliable, it is only reliable when it is set
                    //  to false since the only case where it can be set to false
                    //  is if the attribute was created by the implementation.
                    //  The default content won't be removed if an implementation
                    //  does not have any information available.
                    //    false
                    //    [required]
                    //    Keep all attributes and all content.
                    //
                    if (discard && !((DOMAttr*)attribute )->getSpecified())
                        continue;
                    //
                    //  Again the name has to be completely representable. But the
                    //  attribute can have refs and requires the attribute style
                    //  escaping.
                    //

                    // if this attribute is a namespace declaration, add it to the namespace map for the current level
                    const XMLCh* ns = attribute->getNamespaceURI();
                    if (ns != 0 )
                    {
                        if(XMLString::equals(ns, XMLUni::fgXMLNSURIName)) 
                        {
                            if(namespaceMap==NULL)
                            {
                                namespaceMap=new (fMemoryManager) RefHashTableOf<XMLCh>(12, false, fMemoryManager);
                                fNamespaceStack->addElement(namespaceMap);
                            }
			                const XMLCh* nsPrefix = attribute->getLocalName();
                            if(XMLString::equals(attribute->getNodeName(),XMLUni::fgXMLNSString))
								nsPrefix = XMLUni::fgZeroLenString;
							if(namespaceMap->containsKey((void*)nsPrefix))
								continue;
                            namespaceMap->put((void*)attribute->getLocalName(),(XMLCh*)attribute->getNodeValue());
                        }
                        else if(!XMLString::equals(ns, XMLUni::fgXMLURIName)) 
                        {
                            // check if the namespace for the current node is already defined
                            const XMLCh* prefix = attribute->getPrefix();
                            if(prefix && prefix[0])
                            {
                                bool bPrefixDeclared=false;
                                for(int i=fNamespaceStack->size()-1;i>=0;i--)
                                {
                                    RefHashTableOf<XMLCh>* curNamespaceMap=fNamespaceStack->elementAt(i);
                                    const XMLCh* thisUri=curNamespaceMap->get((void*)prefix);
                                    if(thisUri)
                                    {
                                        // the prefix has been declared: check if it binds to the correct namespace, otherwise, redeclare it
                                        if(XMLString::equals(thisUri,attribute->getNamespaceURI()))
                                            bPrefixDeclared=true;
                                        break;
                                    }
                                }
                                if(!bPrefixDeclared)
                                {
                                    if(namespaceMap==NULL)
                                    {
                                        namespaceMap=new (fMemoryManager) RefHashTableOf<XMLCh>(12, false, fMemoryManager);
                                        fNamespaceStack->addElement(namespaceMap);
                                    }
                                    namespaceMap->put((void*)prefix,(XMLCh*)attribute->getNamespaceURI());
                                    *fFormatter  << XMLFormatter::NoEscapes
                                                 << chSpace << XMLUni::fgXMLNSString << chColon << prefix
                                                 << chEqual << chDoubleQuote
                                                 << XMLFormatter::AttrEscapes
                                                 << attribute->getNamespaceURI()
                                                 << XMLFormatter::NoEscapes
                                                 << chDoubleQuote;
                                }
                            }
                        }
                    }
                    *fFormatter  << XMLFormatter::NoEscapes
                                 << chSpace << attribute->getNodeName()
                                 << chEqual << chDoubleQuote
                                 << XMLFormatter::AttrEscapes
                                 << attribute->getNodeValue()
                                 << XMLFormatter::NoEscapes
                                 << chDoubleQuote;
                } // end of for
            } // end of FILTER_ACCEPT

            level++;

            // FILTER_SKIP may start from here

            //
            //  Test for the presence of children, which includes both
            //  text content and nested elements.
            //
            DOMNodeSPtr child = nodeToWrite->getFirstChild();
            if (child != 0)
            {
                // There are children. Close start-tag, and output children.
                // No escapes are legal here
                if (filterAction == DOMNodeFilter::FILTER_ACCEPT)
                    *fFormatter << XMLFormatter::NoEscapes << chCloseAngle;

                while( child != 0)
                {
                    processNode(child, level);
                    child = child->getNextSibling();
                }

                level--;

                if (filterAction == DOMNodeFilter::FILTER_ACCEPT)
                {
                    //if we are not on the same line as when we started
                    //this node then print a new line and indent
                    if(nodeLine != fCurrentLine)
                    {
                        if (!lineFeedInTextNodePrinted)
                        {
                            printNewLine();
                        }
                        else
                        {
                            lineFeedInTextNodePrinted = false;
                        }

                        if(nodeLine != fCurrentLine && level == 0)
                            printNewLine();

                        printIndent(level);
                    }
                    TRY_CATCH_THROW
                    (
                         *fFormatter << XMLFormatter::NoEscapes << gEndElement
                                     << nodeName << chCloseAngle;
                        ,true
                    )

                }
            }
            else
            {
                level--;

                //
                //  There were no children. Output the short form close of
                //  the element start tag, making it an empty-element tag.
                //
                if (filterAction == DOMNodeFilter::FILTER_ACCEPT)
                {
                    TRY_CATCH_THROW
                    (
                        *fFormatter << XMLFormatter::NoEscapes << chForwardSlash << chCloseAngle;
                       , true
                    )
                }
            }

            // remove the namespace map at this level
            if(namespaceMap!=NULL)
                fNamespaceStack->removeLastElement();

            break;
        }
    case DOMNode::ATTRIBUTE_NODE:
        {
            if (checkFilter(nodeToWrite) != DOMNodeFilter::FILTER_ACCEPT)
                break;

            const XMLCh* localName = nodeToWrite->getLocalName();

            // check if this is a DOM Level 1 Node
            if(localName == 0) {
                *fFormatter  << XMLFormatter::NoEscapes
                             << nodeToWrite->getNodeName()
                             << chEqual << chDoubleQuote
                             << XMLFormatter::AttrEscapes
                             << nodeToWrite->getNodeValue()
                             << XMLFormatter::NoEscapes
                             << chDoubleQuote;
            } else {
                *fFormatter  << XMLFormatter::NoEscapes
                             << chOpenCurly << nodeToWrite->getNamespaceURI() 
                             << chCloseCurly << localName
                             << chEqual << chDoubleQuote
                             << XMLFormatter::AttrEscapes
                             << nodeToWrite->getNodeValue()
                             << XMLFormatter::NoEscapes
                             << chDoubleQuote;
            }

            break;
        }
    case DOMNode::ENTITY_REFERENCE_NODE:
        {
            //"entities"
            //true
            //[required] (default)
            //Keep EntityReference and Entity nodes in the document.

            //false
            //[optional]
            //Remove all EntityReference and Entity nodes from the document,
            //       putting the entity expansions directly in their place.
            //       Text nodes are into "normal" form.
            //Only EntityReference nodes to non-defined entities are kept in the document.

            if (checkFilter(nodeToWrite) != DOMNodeFilter::FILTER_ACCEPT)
                break;

            if (getFeature(ENTITIES_ID))
            {
                TRY_CATCH_THROW
                (
                    *fFormatter << XMLFormatter::NoEscapes << chAmpersand
                                << nodeName << chSemiColon;
                    , true
                )
            }
            else
            {
                // check if the referenced entity is defined or not
                if (nodeToWrite->getOwnerDocument()->getDoctype()->getEntities()->getNamedItem(nodeName))
                {
                    DOMNodeSPtr child;
                    for (child = nodeToWrite->getFirstChild();
                    child != 0;
                    child = child->getNextSibling())
                    {
                        processNode(child, level);
                    }
                }
                else
                {
                    TRY_CATCH_THROW
                   (
                        *fFormatter<<XMLFormatter::NoEscapes<<chAmpersand<<nodeName<<chSemiColon;
                        , true
                    )
                }
            }
            break;
        }

        //
        //  feature:split_cdata_sections     occurence of ]]>   unrep-char
        //  ===============================================================
        //          true                        split            split
        //          false                       fails            fails
        //
    case DOMNode::CDATA_SECTION_NODE:
        {
            if (checkFilter(nodeToWrite) != DOMNodeFilter::FILTER_ACCEPT)
                break;

            if(level == 1)
                printNewLine();

            printNewLine();
            printIndent(level);

            if (getFeature(SPLIT_CDATA_SECTIONS_ID))
            {
                // it is fairly complicated and we process this
                // in a separate function.
                procCdataSection(nodeValue, nodeToWrite, level);
            }
            else
            {
                // search for "]]>", the node value is not supposed to have this
                if (XMLString::patternMatch((XMLCh* const) nodeValue, gEndCDATA) != -1)
                {
                    reportError(nodeToWrite, DOMError::DOM_SEVERITY_FATAL_ERROR, XMLDOMMsg::Writer_NestedCDATA);
                }

                TRY_CATCH_THROW
                (
                    // transcoder throws exception for unrep chars
                    *fFormatter << XMLFormatter::NoEscapes << gStartCDATA << nodeValue << gEndCDATA;
                   , true
                )
            }

            break;
        }

    case DOMNode::COMMENT_NODE:
        {
            if (checkFilter(nodeToWrite) != DOMNodeFilter::FILTER_ACCEPT)
                break;

            if(level == 1)
                printNewLine();

            printNewLine();
            printIndent(level);

            TRY_CATCH_THROW
            (
                *fFormatter << XMLFormatter::NoEscapes << gStartComment
                << nodeValue << gEndComment;
                , true
            )
            break;
        }

    case DOMNode::DOCUMENT_TYPE_NODE:  // Not to be shown to Filter
        {
            const DOMDocumentType *doctype = (const DOMDocumentType *)nodeToWrite;

            fFormatter->setEscapeFlags(XMLFormatter::NoEscapes);

            printNewLine();
            printIndent(level);

            TRY_CATCH_THROW
            (
                *fFormatter << gStartDoctype << nodeName;

                const XMLCh  *id = doctype->getPublicId();
                if (id && *id)
                {
                    *fFormatter << chSpace << gPublic << id << chDoubleQuote;

                    id = doctype->getSystemId();
                    if (id && *id)
                    {
                        *fFormatter << chSpace << chDoubleQuote << id << chDoubleQuote;
                    }
                    else
                    {
                        //
                        // 4.2.2 External Entities
                        // [Definition: If the entity is not internal,
                        //           it is an external entity, declared as follows:]
                        // External Entity Declaration
                        // [75] ExternalID ::= 'SYSTEM' S SystemLiteral
                        //                   | 'PUBLIC' S PubidLiteral S SystemLiteral
                        //
                        reportError(nodeToWrite, DOMError::DOM_SEVERITY_FATAL_ERROR, XMLDOMMsg::Writer_NotRecognizedType);
                        // systemLiteral not found
                    }
                }
                else
                {
                    id = doctype->getSystemId();
                    if (id && *id)
                    {
                        *fFormatter << chSpace << gSystem << id << chDoubleQuote;
                    }
                }

                id = doctype->getInternalSubset();
                if (id && *id)
                {
                    *fFormatter << chSpace << chOpenSquare << id << chCloseSquare;
                }

                *fFormatter << chCloseAngle;
                , true

            ) // end of TRY_CATCH_THROW

            break;
        }

    case DOMNode::ENTITY_NODE:  // Not to be shown to Filter
        {
            //
            // REVISIT: how does the feature "entities" impact
            // entity node?
            //
            printNewLine();
            printIndent(level);

            fFormatter->setEscapeFlags(XMLFormatter::NoEscapes);
            *fFormatter << gStartEntity    << nodeName;

            const XMLCh * id = ((const DOMEntity*)nodeToWrite)->getPublicId();
            if (id)
                *fFormatter << gPublic << id << chDoubleQuote;

            id = ((const DOMEntity*)nodeToWrite)->getSystemId();
            if (id)
                *fFormatter << gSystem << id << chDoubleQuote;

            id = ((const DOMEntity*)nodeToWrite)->getNotationName();
            if (id)
                *fFormatter << gNotation << id << chDoubleQuote;

            *fFormatter << chCloseAngle;

            break;
        }

    default:
        /***
            This is an implementation specific behaviour, we abort if a user derived class has not dealt with
            this node type.
         ***/
        {
            if(!customNodeSerialize(nodeToWrite, level)) {
                reportError(nodeToWrite, DOMError::DOM_SEVERITY_FATAL_ERROR, XMLDOMMsg::Writer_NotRecognizedType);
                // UnreognizedNodeType;
            }
        }

        break;
    }

}

bool DOMWriterImpl::customNodeSerialize(const DOMNode* const, int) {
    return false;
}

//
//
DOMNodeFilter::FilterAction DOMWriterImpl::checkFilter(const DOMNode* const node) const
{
    if (!fFilter ||
        ((fFilter->getWhatToShow() & (1 << (node->getNodeType() - 1))) == 0))
        return DOMNodeFilter::FILTER_ACCEPT;

    //
    // if and only if there is a filter, and it is interested
    // in the node type, then we pass the node to the filter
    // for examination
    //
    return (DOMNodeFilter::FilterAction) fFilter->acceptNode(node);
}


bool DOMWriterImpl::checkFeature(const XMLCh* const featName
                               , bool               toThrow
                               , int&               featureId) const
{
    // check for null and/or empty feature name
    if (!featName || !*featName)
    {
        if (toThrow)
            throw DOMException(DOMException::NOT_FOUND_ERR, 0, fMemoryManager);

        return false;
    }

    featureId = INVALID_FEATURE_ID;

    if (XMLString::equals(featName, XMLUni::fgDOMWRTCanonicalForm))
        featureId = CANONICAL_FORM_ID;
    else if (XMLString::equals(featName, XMLUni::fgDOMWRTDiscardDefaultContent))
        featureId = DISCARD_DEFAULT_CONTENT_ID;
    else if (XMLString::equals(featName, XMLUni::fgDOMWRTEntities))
        featureId = ENTITIES_ID;
    else if (XMLString::equals(featName, XMLUni::fgDOMWRTFormatPrettyPrint))
        featureId = FORMAT_PRETTY_PRINT_ID;
    else if (XMLString::equals(featName, XMLUni::fgDOMWRTNormalizeCharacters))
        featureId = NORMALIZE_CHARACTERS_ID;
    else if (XMLString::equals(featName, XMLUni::fgDOMWRTSplitCdataSections))
        featureId = SPLIT_CDATA_SECTIONS_ID;
    else if (XMLString::equals(featName, XMLUni::fgDOMWRTValidation))
        featureId = VALIDATION_ID;
    else if (XMLString::equals(featName, XMLUni::fgDOMWRTWhitespaceInElementContent))
        featureId = WHITESPACE_IN_ELEMENT_CONTENT_ID;
    else if (XMLString::equals(featName, XMLUni::fgDOMWRTBOM))
        featureId = BYTE_ORDER_MARK_ID;
    else if (XMLString::equals(featName, XMLUni::fgDOMXMLDeclaration))
        featureId = XML_DECLARATION;


    //feature name not resolvable
    if (featureId == INVALID_FEATURE_ID)
    {
        if (toThrow)
            throw DOMException(DOMException::NOT_FOUND_ERR, featName, fMemoryManager);

        return false;
    }

    return true;
}

bool DOMWriterImpl::reportError(const DOMNode* const    errorNode
                              , DOMError::ErrorSeverity errorType
                              , const XMLCh*   const    errorMsg)
{
    bool toContinueProcess = true;   // default value for no error handler

    if (fErrorHandler)
    {
        DOMLocatorImpl  locator(0, 0, (DOMNode* const) errorNode, 0, 0);
        DOMErrorImpl    domError(errorType , errorMsg, &locator);
        toContinueProcess = fErrorHandler->handleError(domError);
    }

    if (errorType != DOMError::DOM_SEVERITY_WARNING)
        fErrorCount++;

    return toContinueProcess;
}

bool DOMWriterImpl::reportError(const DOMNode* const    errorNode
                              , DOMError::ErrorSeverity errorType
                              , XMLDOMMsg::Codes        toEmit)
{
    const unsigned int msgSize = 1023;
    XMLCh errText[msgSize + 1];

    DOMImplementationImpl::getMsgLoader4DOM()->loadMsg(toEmit, errText, msgSize);

    bool toContinueProcess = true;   // default value for no error handler

    if (fErrorHandler)
    {
        DOMLocatorImpl  locator(0, 0, (DOMNode* const) errorNode, 0, 0);
        DOMErrorImpl    domError(errorType , errText, &locator);
        toContinueProcess = fErrorHandler->handleError(domError);
    }

    if (errorType != DOMError::DOM_SEVERITY_WARNING)
        fErrorCount++;

    if (errorType == DOMError::DOM_SEVERITY_FATAL_ERROR || !toContinueProcess)
        throw toEmit;

    return toContinueProcess;
}

//
//
//
void DOMWriterImpl::procCdataSection(const XMLCh*   const nodeValue
                                   , const DOMNode* const nodeToWrite
                                   , int level)
{
    /***
     * Append a ']]>' at the end
     */
    int len = XMLString::stringLen(nodeValue);
    XMLCh* repNodeValue = (XMLCh*) fMemoryManager->allocate
    (
        (len + offset + 1) * sizeof(XMLCh)
    );//new XMLCh [len + offset + 1];
    XMLString::copyString(repNodeValue, nodeValue);
    XMLString::catString(repNodeValue, gEndCDATA);
    ArrayJanitor<XMLCh>  jName(repNodeValue, fMemoryManager);

    XMLCh* curPtr  = (XMLCh*) repNodeValue;
    XMLCh* nextPtr = 0;
    int    endTagPos = -1;

    bool   endTagFound = true;

    while (endTagFound)
    {
        endTagPos = XMLString::patternMatch(curPtr, gEndCDATA);
        if (endTagPos != -1)
        {
            nextPtr = curPtr + endTagPos + offset;  // skip the ']]>'
            *(curPtr + endTagPos) = chNull;         //nullify the first ']'
            if (endTagPos != len)
                reportError(nodeToWrite, DOMError::DOM_SEVERITY_WARNING, XMLDOMMsg::Writer_NestedCDATA);
            len = len - endTagPos - offset;
        }
        else
        {
            endTagFound = false;
        }

        /***
            to check ]]>]]>
        ***/
        if (endTagPos == 0)
        {
            printNewLine();
            printIndent(level);
            TRY_CATCH_THROW
            (
                *fFormatter << XMLFormatter::NoEscapes << gStartCDATA << gEndCDATA;
                , true
            )
        }
        else
        {
            procUnrepCharInCdataSection(curPtr, nodeToWrite, level);
        }

        if (endTagFound)
        {
            *(nextPtr - offset) = chCloseSquare;   //restore the first ']'
            curPtr = nextPtr;
        }
    }

    return;
}

//
//
//
void DOMWriterImpl::procUnrepCharInCdataSection(const XMLCh*   const nodeValue
                                              , const DOMNode* const nodeToWrite
                                              , int level)
{
    //
    //  We have to check each character and see if it could be represented.
    //  As long as it can, we just keep up with where we started and how
    //  many chars we've checked. When we hit an unrepresentable one, we
    //  stop, transcode everything we've collected, then start handling
    //  the unrepresentables via char refs. We repeat this until we get all
    //  the chars done.
    //
    const XMLCh*    srcPtr = nodeValue;
    const XMLCh*    endPtr = nodeValue +  XMLString::stringLen(nodeValue);

    // Set up the common part of the buffer that we build char refs into
    XMLCh tmpBuf[32];
    tmpBuf[0] = chAmpersand;
    tmpBuf[1] = chPound;
    tmpBuf[2] = chLatin_x;

    while (srcPtr < endPtr)
    {
        const XMLCh* tmpPtr = srcPtr;
        while (tmpPtr < endPtr)
        {
            if (fFormatter->getTranscoder()->canTranscodeTo(*tmpPtr))
                tmpPtr++;
            else
                break;
        }

        if (tmpPtr > srcPtr)
        {
            printNewLine();
            printIndent(level);
            TRY_CATCH_THROW
           (
                *fFormatter << XMLFormatter::NoEscapes << gStartCDATA;
                , true
            )

            // We got at least some chars that can be done normally
            fFormatter->formatBuf
            (
                srcPtr
                , tmpPtr - srcPtr
                , XMLFormatter::NoEscapes
                , XMLFormatter::UnRep_Fail
            );

            TRY_CATCH_THROW
            (
                *fFormatter << XMLFormatter::NoEscapes << gEndCDATA;
                , true
            )

            // Update the source pointer to our new spot
            srcPtr = tmpPtr;
        }
        else
        {
            //
            //  We hit something unrepresentable. So continue forward doing
            //  char refs until we hit something representable again or the
            //  end of input.
            //

            // one warning for consective unrep chars
            reportError(nodeToWrite, DOMError::DOM_SEVERITY_WARNING, XMLDOMMsg::Writer_NotRepresentChar);

            while (srcPtr < endPtr)
            {
                // Build a char ref for the current char
                XMLString::binToText(*srcPtr, &tmpBuf[3], 8, 16, fMemoryManager);
                const unsigned int bufLen = XMLString::stringLen(tmpBuf);
                tmpBuf[bufLen] = chSemiColon;
                tmpBuf[bufLen+1] = chNull;

                // And now call recursively back to our caller to format this
                fFormatter->formatBuf
                (
                    tmpBuf
                    , bufLen + 1
                    , XMLFormatter::NoEscapes
                    , XMLFormatter::UnRep_Fail
                );

                // Move up the source pointer and break out if needed
                srcPtr++;
                if (fFormatter->getTranscoder()->canTranscodeTo(*srcPtr))
                    break;
            }
        }
    }
}

void DOMWriterImpl::processNode(const DOMNode* const nodeToWrite)
{
    processNode(nodeToWrite, 0);
}

bool DOMWriterImpl::canSetFeature(const int featureId
                                       , bool      val) const
{
    return featuresSupported[2*featureId + (val? 0: 1)];
}

void DOMWriterImpl::printNewLine()
{
    if (getFeature(FORMAT_PRETTY_PRINT_ID))
    {
        fCurrentLine++;
        *fFormatter << fNewLineUsed;
    }
}

void DOMWriterImpl::printIndent(int level) const
{
    if (getFeature(FORMAT_PRETTY_PRINT_ID))
    {
        if (lastWhiteSpaceInTextNode)
        {
            level -= lastWhiteSpaceInTextNode/2; // two chSpaces equals one indent level
            lastWhiteSpaceInTextNode = 0;
            // if lastWhiteSpaceInTextNode/2 is greater than level, then
            // it means too many spaces have been written to the
            // output stream and we can no longer indent properly
        }

        for(int i = 0; i < level; i++)
            *fFormatter << chSpace << chSpace;
    }
}

void DOMWriterImpl::release()
{
    DOMWriterImpl* writer = (DOMWriterImpl*) this;
    delete writer;
}

void DOMWriterImpl::processBOM()
{
    // if the feature is not set, don't output bom
    if (!getFeature(BYTE_ORDER_MARK_ID))
        return;

    if ((XMLString::compareIStringASCII(fEncoding, XMLUni::fgUTF16LEncodingString)  == 0) ||
        (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUTF16LEncodingString2) == 0)  )
    {
        fFormatter->writeBOM(BOM_utf16le, 2);
    }
    else if ((XMLString::compareIStringASCII(fEncoding, XMLUni::fgUTF16BEncodingString)  == 0) ||
             (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUTF16BEncodingString2) == 0)  )
    {
        fFormatter->writeBOM(BOM_utf16be, 2);
    }
    else if ((XMLString::compareIStringASCII(fEncoding, XMLUni::fgUTF16EncodingString)  == 0) ||
             (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUTF16EncodingString2) == 0) ||
             (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUTF16EncodingString3) == 0) ||
             (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUTF16EncodingString4) == 0) ||
             (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUTF16EncodingString5) == 0) ||
             (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUTF16EncodingString6) == 0) ||
             (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUTF16EncodingString7) == 0)  ) 
    {
    	if (XMLPlatformUtils::fgXMLChBigEndian)
            fFormatter->writeBOM(BOM_utf16be, 2);
        else
            fFormatter->writeBOM(BOM_utf16le, 2);
    }
    else if ((XMLString::compareIStringASCII(fEncoding, XMLUni::fgUCS4LEncodingString)  == 0) ||
             (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUCS4LEncodingString2) == 0)  )
    {
        fFormatter->writeBOM(BOM_ucs4le, 4);
    }
    else if ((XMLString::compareIStringASCII(fEncoding, XMLUni::fgUCS4BEncodingString)  == 0) ||
             (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUCS4BEncodingString2) == 0)  )
    {
        fFormatter->writeBOM(BOM_ucs4be, 4);
    }
    else if ((XMLString::compareIStringASCII(fEncoding, XMLUni::fgUCS4EncodingString)  == 0) ||
             (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUCS4EncodingString2) == 0) ||
             (XMLString::compareIStringASCII(fEncoding, XMLUni::fgUCS4EncodingString3) == 0)  )
    {
		if (XMLPlatformUtils::fgXMLChBigEndian)
	        fFormatter->writeBOM(BOM_ucs4be, 4);
	    else
			fFormatter->writeBOM(BOM_ucs4le, 4);
    }

    return;

}

XERCES_CPP_NAMESPACE_END

