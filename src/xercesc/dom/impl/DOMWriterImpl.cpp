/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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

#include <xercesc/dom/DOM.hpp>
#include "DOMErrorImpl.hpp"
#include "DOMLocatorImpl.hpp"

#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/TranscodingException.hpp>
#include <xercesc/util/Janitor.hpp>

// ---------------------------------------------------------------------------
//  Local const data
//
// ---------------------------------------------------------------------------
static const XMLCh  gTrue[] =
{
	chLatin_T, chNull
};

static const XMLCh  gFalse[] =
{
	chLatin_F, chNull
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

//<?xml version="1.0
static const XMLCh  gXMLDecl1[] =
{
	chOpenAngle, chQuestion, chLatin_x,     chLatin_m,  chLatin_l,  chSpace,
	chLatin_v,   chLatin_e,  chLatin_r,     chLatin_s,  chLatin_i,  chLatin_o,
	chLatin_n,   chEqual,    chDoubleQuote, chDigit_1,  chPeriod,   chDigit_0,
	chNull
};

//" encoding="
static const XMLCh  gXMLDecl2[] =
{
	chDoubleQuote, chSpace,   chLatin_e, chLatin_n, chLatin_c,
    chLatin_o,     chLatin_d, chLatin_i, chLatin_n, chLatin_g, chEqual,
    chDoubleQuote, chNull
};

//"?>
static const XMLCh  gXMLDecl3[] =
{
	chDoubleQuote, chQuestion, chCloseAngle,  chNull
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
    chCloseSquare, chCloseSquare, chCloseAngle, chNull
};

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

// Unrecognized node type
static const XMLCh  gUnrecognizedNodeType[] =
{
	chLatin_U, chLatin_n, chLatin_r, chLatin_e, chLatin_c, chLatin_o,
    chLatin_g, chLatin_n, chLatin_i, chLatin_z, chLatin_e, chLatin_d,
	chSpace,   chLatin_N, chLatin_o, chLatin_d, chLatin_e, chSpace,
	chLatin_T, chLatin_y, chLatin_p, chLatin_e, chNull
};

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
// thing here, since the serializer is a recusively called function, we
// can't use return, obviously. However we may have multiple try/catch
// along it is way go back to writeNode(). So far we can't think of a
// "short-cut" to go "directly" back.
//
#define  TRY_CATCH_THROW(action, forceToRethrow)                 \
fFormatter->setUnRepFlags(XMLFormatter::UnRep_Fail);             \
try                                                              \
{                                                                \
     action;                                                     \
}                                                              \
catch(TranscodingException const &e)                             \
{                                                                \
    DOMLocatorImpl  locator(0                                    \
                          , 0                                    \
                          , (DOMNode* const)nodeToWrite          \
                          , 0                                    \
                          , 0);                                  \
    DOMErrorImpl    domError(DOMError::DOM_SEVERITY_FATAL_ERROR      \
                           , e.getMessage()                      \
                           , &locator);                          \
    bool  retVal = fErrorHandler->handleError(domError);         \
                                                                 \
    if (forceToRethrow || !retVal)                               \
        throw;                                                   \
    else                                                         \
        fErrorCount++;                                           \
}

DOMWriterImpl::~DOMWriterImpl()
{
	delete fFeatures;
	delete [] fEncoding;
	delete [] fNewLine;

	// we don't own/adopt error handler and filter
}

//    feature                      true                       false
// ================================================================================
//canonical-form                 [optional] Not Supported     [required] (default)
//discard-default-content        [required] (default)         [required]
//entity                         [required] (default)         [optional]
//format-pretty-print            [optional] Not Supported     [required] (default)
//normalize-characters           [optional] Not Supported     [required] (default)
//split-cdata-sections           [required] (default)         [required]
//validation                     [optional] Not Supported     [required] (default)
//whitespace-in-element-content  [requierd] (default)         [optional] Not Supported
//
DOMWriterImpl::DOMWriterImpl()
:fFeatures(0)
,fEncoding(0)
,fNewLine(0)
,fErrorHandler(0)
,fFilter(0)
{
	fFeatures = new RefHashTableOf<KVStringPair>(9, true);

	fFeatures->put((void*)XMLUni::fgDOMWRTCanonicalForm,
		            new KVStringPair(XMLUni::fgDOMWRTCanonicalForm, gFalse));
	fFeatures->put((void*)XMLUni::fgDOMWRTDiscardDefaultContent,
		            new KVStringPair(XMLUni::fgDOMWRTDiscardDefaultContent, gTrue));
	fFeatures->put((void*)XMLUni::fgDOMWRTEntities,
		            new KVStringPair(XMLUni::fgDOMWRTEntities, gTrue));
	fFeatures->put((void*)XMLUni::fgDOMWRTFormatPrettyPrint,
		            new KVStringPair(XMLUni::fgDOMWRTFormatPrettyPrint, gFalse));
	fFeatures->put((void*)XMLUni::fgDOMWRTNormalizeCharacters,
                    new KVStringPair(XMLUni::fgDOMWRTNormalizeCharacters, gFalse));
	fFeatures->put((void*)XMLUni::fgDOMWRTSplitCdataSections,
		            new KVStringPair(XMLUni::fgDOMWRTSplitCdataSections, gTrue));
	fFeatures->put((void*)XMLUni::fgDOMWRTValidation,
		            new KVStringPair(XMLUni::fgDOMWRTValidation, gFalse));
	fFeatures->put((void*)XMLUni::fgDOMWRTWhitespaceInElementContent,
		            new KVStringPair(XMLUni::fgDOMWRTWhitespaceInElementContent, gTrue));

}
//
// refer to the feature table above
//
bool DOMWriterImpl::canSetFeature(const XMLCh* const featName
          						, bool               state) const
{
	// featName not recognized
	if ((!featName) || (!fFeatures->get(featName)))
		return false;

	if ((XMLString::compareString(featName, XMLUni::fgDOMWRTCanonicalForm)==0) && state)
		return false;
	else if ((XMLString::compareString(featName, XMLUni::fgDOMWRTFormatPrettyPrint)==0) && state)
		return false;
	else if ((XMLString::compareString(featName, XMLUni::fgDOMWRTNormalizeCharacters)==0) && state)
		return false;
	else if ((XMLString::compareString(featName, XMLUni::fgDOMWRTValidation)==0) && state)
		return false;
	else if ((XMLString::compareString(featName, XMLUni::fgDOMWRTWhitespaceInElementContent)==0) && !state)
		return false;
	else
		return true;
}

void DOMWriterImpl::setFeature(const XMLCh* const featName
                             , bool               state)
{
	if ((!featName) || (XMLString::stringLen(featName)==0))
		throw DOMException(DOMException::NOT_FOUND_ERR, 0);			

	if (!fFeatures->get(featName))
		throw DOMException(DOMException::NOT_FOUND_ERR, featName);			
        //"Feature "+featName+" not found");

	if (!canSetFeature(featName, state))
		throw DOMException(DOMException::NOT_SUPPORTED_ERR, featName);
        //DOMException.NOT_SUPPORTED_ERR,"Feature "+featName+" cannot be set as "+state);

	// REVISIT
	// canonical-form and format-pretty-print can not be both set to true
	// meaning set canonical-form true will automatically set
	// format-pretty-print to false and vise versa.
	// right now only false is supported for both of them, we need
	// not worry about that, but later if we decide to support true for
	// them, we need to add code to ensure they are not true at the same time.
	fFeatures->put((void*)featName, new KVStringPair(featName, (state? gTrue : gFalse)));

	return;
}

bool DOMWriterImpl::getFeature(const XMLCh* const featName) const
{
	if ((!featName) || (XMLString::stringLen(featName)==0))
		throw DOMException(DOMException::NOT_FOUND_ERR, 0);			

	KVStringPair *kvData = fFeatures->get(featName);
	if (!kvData)
	{
		throw DOMException(DOMException::NOT_FOUND_ERR, featName);			
        //DOMException.NOT_FOUND_ERR,"Feature "+featName+" not found");
	}

	return ( (XMLString::compareString(kvData->getValue(), gTrue)==0)? true : false);

}

// we don't check the validity of the encoding set
void DOMWriterImpl::setEncoding(const XMLCh* const encoding)
{
	delete [] fEncoding;
	fEncoding = XMLString::replicate(encoding);
}

const XMLCh* DOMWriterImpl::getEncoding() const
{
	return fEncoding;
}

void DOMWriterImpl::setNewLine(const XMLCh* const newLine)
{
	delete [] fNewLine;
	fNewLine = XMLString::replicate(newLine);
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
		fFormatter = new XMLFormatter(fEncodingUsed
                        			, destination
	                                , XMLFormatter::NoEscapes
                                    , XMLFormatter::UnRep_CharRef);
		Janitor<XMLFormatter> janName(fFormatter);

		processNode(&nodeToWrite);
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
		return false;
	}

	catch (const DOMException&)
	{
		return false;
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
	MemBufFormatTarget  destination;
	bool retVal;

	try
	{
		retVal = writeNode(&destination, nodeToWrite);
	}
	catch (...)
	{
		//
		// there is a possibility that memeory allocation
		// exception thrown in XMLBuffer class
		//
		return 0;
	}
	
	return (retVal ? destination.getString() : 0);
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

	if (fEncoding && XMLString::stringLen(fEncoding))
	{
		fEncodingUsed = fEncoding;
	}
	else
	{
        // REVISIT: DOM3 core may provide getEncoding()
		// fEncodingUsed = nodeToWrite.getEncoding();
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
	fNewLineUsed = (fNewLine && XMLString::stringLen(fNewLine))? fNewLine : gEOLSeq;

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
void DOMWriterImpl::processNode(const DOMNode* const nodeToWrite)
{
    // Get the name and value out for convenience
    const XMLCh*    nodeName = nodeToWrite->getNodeName();
    const XMLCh*    nodeValue = nodeToWrite->getNodeValue();
    unsigned long   lent = XMLString::stringLen(nodeValue);

	/***
{FILTER_ACCEPT = 1,
                           FILTER_REJECT = 2,
                           FILTER_SKIP   = 3};
***/


    switch (nodeToWrite->getNodeType())
    {
	case DOMNode::TEXT_NODE:
        {
			if (checkFilter(nodeToWrite) != DOMNodeFilter::FILTER_ACCEPT)
				break;

			setURCharRef();      // character data
            fFormatter->formatBuf(nodeValue, lent, XMLFormatter::CharEscapes);
            break;
        }

	case DOMNode::PROCESSING_INSTRUCTION_NODE:
        {
			if (checkFilter(nodeToWrite) != DOMNodeFilter::FILTER_ACCEPT)
				break;

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
			setURCharRef();
			*fFormatter << gXMLDecl1 << gXMLDecl2;
			*fFormatter << fEncodingUsed << gXMLDecl3;
			*fFormatter << fNewLineUsed;

            DOMNode *child = nodeToWrite->getFirstChild();
            while( child != 0)
            {
                processNode(child);
                *fFormatter << fNewLineUsed;      // add linefeed in requested output encoding

                child = child->getNextSibling();
            }
            break;
        }

	case DOMNode::ELEMENT_NODE:
        {
			DOMNodeFilter::FilterAction filterAction = checkFilter(nodeToWrite);
			
			if ( filterAction == DOMNodeFilter::FILTER_REJECT)
				break;

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

				bool discard = getFeature(XMLUni::fgDOMWRTDiscardDefaultContent);
				for (int i = 0; i < attrCount; i++)
				{
					DOMNode  *attribute = attributes->item(i);

					// Not to be shown to Filter

					//
					//"discard-default-content"
					//	true
					//	[required] (default)
					//	Use whatever information available to the implementation
					//  (i.e. XML schema, DTD, the specified flag on Attr nodes,
					//  and so on) to decide what attributes and content should be
					//  discarded or not.
					//  Note that the specified flag on Attr nodes in itself is
					//  not always reliable, it is only reliable when it is set
					//  to false since the only case where it can be set to false
					//  is if the attribute was created by the implementation.
					//  The default content won't be removed if an implementation
					//  does not have any information available.
					//	false
					//	[required]
					//	Keep all attributes and all content.
					//
					if (discard && !((DOMAttr*)attribute )->getSpecified())
						continue;
					//
					//  Again the name has to be completely representable. But the
					//  attribute can have refs and requires the attribute style
					//  escaping.
					//

					*fFormatter  << XMLFormatter::NoEscapes
                                 << chSpace << attribute->getNodeName()
                                 << chEqual << chDoubleQuote
                                 << XMLFormatter::AttrEscapes
                                 << attribute->getNodeValue()
                                 << XMLFormatter::NoEscapes
                                 << chDoubleQuote;
				} // end of for
			} // end of FILTER_ACCEPT

			// FILTER_SKIP may start from here

            //
            //  Test for the presence of children, which includes both
            //  text content and nested elements.
            //
            DOMNode *child = nodeToWrite->getFirstChild();
            if (child != 0)
            {
                // There are children. Close start-tag, and output children.
                // No escapes are legal here
				if (filterAction == DOMNodeFilter::FILTER_ACCEPT)
					*fFormatter << XMLFormatter::NoEscapes << chCloseAngle;

                while( child != 0)
                {
                    processNode(child);
                    child = child->getNextSibling();
                }

				if (filterAction == DOMNodeFilter::FILTER_ACCEPT)
				{
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

			if (getFeature(XMLUni::fgDOMWRTEntities))
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
				DOMNode *child;
				for (child = nodeToWrite->getFirstChild();
				child != 0;
				child = child->getNextSibling())
				{
                    processNode(child);
				}
			}
			break;
		}

	case DOMNode::CDATA_SECTION_NODE:
		{
			if (checkFilter(nodeToWrite) != DOMNodeFilter::FILTER_ACCEPT)
				break;

			TRY_CATCH_THROW
			(
    			*fFormatter << XMLFormatter::NoEscapes << gStartCDATA;
			    , true
			)

			if (getFeature(XMLUni::fgDOMWRTSplitCdataSections))
			{
				setURCharRef();
				*fFormatter << nodeValue;
			}
			else
			{
				TRY_CATCH_THROW
				(
    				*fFormatter << nodeValue;
				    , true
	            )
			}

			TRY_CATCH_THROW
            (
                 *fFormatter << gEndCDATA;
			    , true
			)

		break;

		}

	case DOMNode::COMMENT_NODE:
        {
			if (checkFilter(nodeToWrite) != DOMNodeFilter::FILTER_ACCEPT)
				break;

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
            DOMDocumentType *doctype = (DOMDocumentType *)nodeToWrite;;

			fFormatter->setEscapeFlags(XMLFormatter::NoEscapes);
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
						DOMLocatorImpl  locator(0, 0, (DOMNode* const)nodeToWrite, 0, 0);
						DOMErrorImpl    domError(DOMError::DOM_SEVERITY_FATAL_ERROR
                                 			   , gUnrecognizedNodeType
		                                       , &locator);
						fErrorHandler->handleError(domError);
						throw DOMException(DOMException::NOT_FOUND_ERR, 0);
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

			fFormatter->setEscapeFlags(XMLFormatter::NoEscapes);
			*fFormatter << gStartEntity	<< nodeName;

			const XMLCh * id = ((DOMEntity*)nodeToWrite)->getPublicId();
			if (id)
				*fFormatter << gPublic << id << chDoubleQuote;

			id = ((DOMEntity*)nodeToWrite)->getSystemId();
			if (id)
				*fFormatter << gSystem << id << chDoubleQuote;

			id = ((DOMEntity*)nodeToWrite)->getNotationName();
			if (id)
				*fFormatter << gNotation << id << chDoubleQuote;

			*fFormatter << chCloseAngle << fNewLineUsed;

			break;
        }

	default:
    	/***
		    This is an implementation specific behaviour, we abort serialization
			once unrecognized node type encountered.
	     ***/
        {
			DOMLocatorImpl  locator(0, 0, (DOMNode* const)nodeToWrite, 0, 0);
			DOMErrorImpl    domError(DOMError::DOM_SEVERITY_FATAL_ERROR
                      			   , gUnrecognizedNodeType
			                       , &locator);
			fErrorHandler->handleError(domError);
			throw DOMException(DOMException::NOT_FOUND_ERR, 0);
			// UnreognizedNodeType;
		}

		break;
    }

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

inline void DOMWriterImpl::setURCharRef()
{
	fFormatter->setUnRepFlags(XMLFormatter::UnRep_CharRef);
}