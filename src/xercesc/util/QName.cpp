/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * $Log$
 * Revision 1.15  2005/01/07 15:12:10  amassari
 * Removed warnings
 *
 * Revision 1.14  2004/10/28 20:14:41  peiyongz
 * Data member reshuffle
 *
 * Revision 1.13  2004/10/19 15:08:53  knoaman
 * Performance improvement
 *
 * Revision 1.12  2004/09/08 13:56:22  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.11  2003/10/01 16:32:39  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.10  2003/09/25 22:24:28  peiyongz
 * Using writeString/readString
 *
 * Revision 1.9  2003/09/25 15:22:34  peiyongz
 * Implementation of Serialization
 *
 * Revision 1.8  2003/05/16 06:01:52  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.7  2003/05/15 19:04:35  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.6  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.5  2002/10/30 21:52:00  tng
 * [Bug 13641] compiler-generated copy-constructor for QName doesn't do the right thing.
 *
 * Revision 1.4  2002/09/24 19:51:24  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.3  2002/09/05 16:06:41  tng
 * [Bug 12232] Make operator to be constant.
 *
 * Revision 1.2  2002/04/02 15:31:48  knoaman
 * Modiy QName comparison (operator=).
 *
 * Revision 1.1.1.1  2002/02/01 22:22:11  peiyongz
 * sane_include
 *
 * Revision 1.8  2001/12/06 17:48:36  tng
 * Performance Enhancement.  Added setNPrefix and setNLocalPart methods that allow code to take advantage of the fact that it knows the length of the prefix and local name, when possible.  That can avoid a copy of the prefix into a null-terminated temporary variable before copying into the fPrefix.
 * Also changed the getRawName method so that it would simply return the local part when there is no prefix, instead of allocating another buffer to copy the local part into the fRawName.
 * When there is a prefix, changed the getRawName to copy the prefix and local part into the fRawName using XMLString::moveChars instead of using XMLString::copyString and XMLString::catString.  The catString method has to loop past the prefix portion of the fRawName, which seems like a waste.
 * By Henry Zongaro.
 *
 * Revision 1.7  2001/07/24 18:31:47  knoaman
 * Added support for <group> + extra constraint checking for complexType
 *
 * Revision 1.6  2001/05/11 13:26:28  tng
 * Copyright update.
 *
 * Revision 1.5  2001/04/19 18:17:08  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.4  2001/03/21 21:56:11  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.3  2001/02/27 14:48:35  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 * Revision 1.2  2001/02/26 21:56:15  tng
 * Schema: QName can also be constructed with rawName.
 *
 * Revision 1.1  2001/02/26 19:44:25  tng
 * Schema: add utility class QName, by Pei Yong Zhang.
 *
 */

#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/QName.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  QName: Constructors and Destructor
// ---------------------------------------------------------------------------
QName::QName(MemoryManager* const manager)
:fPrefixBufSz(0)
,fLocalPartBufSz(0)
,fRawNameBufSz(0)
,fURIId(0)
,fPrefix(0)
,fLocalPart(0)
,fRawName(0)
,fMemoryManager(manager)
{
}

typedef JanitorMemFunCall<QName>    CleanupType;

QName::QName( const XMLCh* const   prefix
            , const XMLCh* const   localPart
            , const unsigned int   uriId
            , MemoryManager* const manager)
:fPrefixBufSz(0)
,fLocalPartBufSz(0)
,fRawNameBufSz(0)
,fURIId(0)
,fPrefix(0)
,fLocalPart(0)
,fRawName(0)
,fMemoryManager(manager)
{
    CleanupType cleanup(this, &QName::cleanUp);

    try
    {
        //
        //  Just call the local setters to set up everything. Too much
        //  work is required to replicate that functionality here.
        //
        setName(prefix, localPart, uriId);
    }
    catch(const OutOfMemoryException&)
    {
        cleanup.release();

        throw;
    }

    cleanup.release();
}

QName::QName( const XMLCh* const rawName
            , const unsigned int uriId
            , MemoryManager* const manager)
:fPrefixBufSz(0)
,fLocalPartBufSz(0)
,fRawNameBufSz(0)
,fURIId(0)
,fPrefix(0)
,fLocalPart(0)
,fRawName(0)
,fMemoryManager(manager)
{
    CleanupType cleanup(this, &QName::cleanUp);

    try
    {
        //
        //  Just call the local setters to set up everything. Too much
        //  work is required to replicate that functionality here.
        //
        setName(rawName, uriId);
    }
    catch(const OutOfMemoryException&)
    {
        cleanup.release();

        throw;
    }

    cleanup.release();
}

QName::~QName()
{
	cleanUp();
}

// ---------------------------------------------------------------------------
//  QName: Copy Constructors
// ---------------------------------------------------------------------------
QName::QName(const QName& qname)
:XSerializable(qname)
,XMemory(qname)
,fPrefixBufSz(0)
,fLocalPartBufSz(0)
,fRawNameBufSz(0)
,fURIId(0)
,fPrefix(0)
,fLocalPart(0)
,fRawName(0)
,fMemoryManager(qname.fMemoryManager)
{
    unsigned int newLen;

    newLen = XMLString::stringLen(qname.getLocalPart());
    fLocalPartBufSz = newLen + 8;
    fLocalPart = (XMLCh*) fMemoryManager->allocate
    (
        (fLocalPartBufSz + 1) * sizeof(XMLCh)
    ); //new XMLCh[fLocalPartBufSz + 1];
    XMLString::moveChars(fLocalPart, qname.getLocalPart(), newLen + 1);

    newLen = XMLString::stringLen(qname.getPrefix());
    fPrefixBufSz = newLen + 8;
    fPrefix = (XMLCh*) fMemoryManager->allocate
    (
        (fPrefixBufSz + 1) * sizeof(XMLCh)
    ); //new XMLCh[fPrefixBufSz + 1];
    XMLString::moveChars(fPrefix, qname.getPrefix(), newLen + 1);

    fURIId = qname.getURI();
}

// ---------------------------------------------------------------------------
//  QName: Getter methods
// ---------------------------------------------------------------------------
const XMLCh* QName::getRawName() const
{
    //
    //  If there is no buffer, or if there is but we've not faulted in the
    //  value yet, then we have to do that now.
    //
    if (!fRawName || !*fRawName)
    {
        //
        //  If we have a prefix, then do the prefix:name version. Else, its
        //  just the name.
        //
        if (*fPrefix)
        {
            //
            //  Calculate the worst case size buffer we will need. We use the
            //  current high water marks of the prefix and name buffers, so it
            //  might be a little wasteful of memory but we don't have to do
            //  string len operations on the two strings.
            //
            const unsigned int neededLen = fPrefixBufSz + fLocalPartBufSz + 1;

            //
            //  If no buffer, or the current one is too small, then allocate one
            //  and get rid of any old one.
            //
            if (!fRawName || (neededLen > fRawNameBufSz))
            {
                fMemoryManager->deallocate(fRawName); //delete [] fRawName;

                // We have to cast off the const'ness to do this
                ((QName*)this)->fRawNameBufSz = neededLen;
                ((QName*)this)->fRawName = (XMLCh*) fMemoryManager->allocate
                (
                    (neededLen + 1) * sizeof(XMLCh)
                ); //new XMLCh[neededLen + 1];

                // Make sure its initially empty
                *fRawName = 0;
            }

            const unsigned int prefixLen = XMLString::stringLen(fPrefix);

            XMLString::moveChars(fRawName, fPrefix, prefixLen);
            fRawName[prefixLen] = chColon;
            XMLString::copyString(&fRawName[prefixLen+1], fLocalPart);
        }
         else
        {
            return fLocalPart;
        }
    }
    return fRawName;
}

XMLCh* QName::getRawName()
{
    //
    //  If there is no buffer, or if there is but we've not faulted in the
    //  value yet, then we have to do that now.
    //
    if (!fRawName || !*fRawName)
    {
        //
        //  If we have a prefix, then do the prefix:name version. Else, its
        //  just the name.
        //
        if (*fPrefix)
        {
            //
            //  Calculate the worst case size buffer we will need. We use the
            //  current high water marks of the prefix and name buffers, so it
            //  might be a little wasteful of memory but we don't have to do
            //  string len operations on the two strings.
            //
            const unsigned int neededLen = fPrefixBufSz + fLocalPartBufSz + 1;

            //
            //  If no buffer, or the current one is too small, then allocate one
            //  and get rid of any old one.
            //
            if (!fRawName || (neededLen > fRawNameBufSz))
            {
                fMemoryManager->deallocate(fRawName); //delete [] fRawName;

                // We have to cast off the const'ness to do this
                ((QName*)this)->fRawNameBufSz = neededLen;
                ((QName*)this)->fRawName = (XMLCh*) fMemoryManager->allocate
                (
                    (neededLen + 1) * sizeof(XMLCh)
                ); //new XMLCh[neededLen + 1];

                // Make sure its initially empty
                *fRawName = 0;
            }


            const unsigned int prefixLen = XMLString::stringLen(fPrefix);

            XMLString::moveChars(fRawName, fPrefix, prefixLen);
            fRawName[prefixLen] = chColon;
            XMLString::copyString(&fRawName[prefixLen+1], fLocalPart);
        }
         else
        {
            return fLocalPart;
        }
    }
    return fRawName;
}

// ---------------------------------------------------------------------------
//  QName: Setter methods
// ---------------------------------------------------------------------------
void QName::setName(const XMLCh* const    prefix
                  , const XMLCh* const    localPart
                  , const unsigned int    uriId)
{
    setPrefix(prefix);
    setLocalPart(localPart);

    // And clean up any QName and leave it undone until/if asked for again
    if (fRawName)
        *fRawName = 0;

    // And finally store the URI id parameter
    fURIId = uriId;
}

void QName::setName(const XMLCh* const    rawName
                  , const unsigned int    uriId)
{
    //set the rawName
    unsigned int newLen = XMLString::stringLen(rawName);
    //find out the prefix and localPart from the rawName
    const int colonInd = XMLString::indexOf(rawName, chColon);

    if (colonInd >= 0)
    {
        if (!fRawNameBufSz || (newLen > fRawNameBufSz))
        {
            fMemoryManager->deallocate(fRawName); //delete [] fRawName;
            fRawNameBufSz = newLen + 8;
            fRawName = (XMLCh*) fMemoryManager->allocate
            (
                (fRawNameBufSz + 1) * sizeof(XMLCh)
            ); //new XMLCh[fRawNameBufSz + 1];
        }
        XMLString::moveChars(fRawName, rawName, newLen + 1);
        setNPrefix(rawName, colonInd);
    }
    else
    {
        // No colon, so we just have a name with no prefix
        setPrefix(XMLUni::fgZeroLenString);

        // And clean up any QName and leave it undone until/if asked for again
        if (fRawName)
            *fRawName = 0;
    }

    setNLocalPart(&rawName[colonInd+1], newLen-colonInd-1);

    // And finally store the URI id parameter
    fURIId = uriId;
}

void QName::setPrefix(const XMLCh* prefix)
{
    unsigned int newLen;

    newLen = XMLString::stringLen(prefix);
    if (!fPrefixBufSz || (newLen > fPrefixBufSz))
    {
        fMemoryManager->deallocate(fPrefix); //delete [] fPrefix;
        fPrefixBufSz = newLen + 8;
        fPrefix = (XMLCh*) fMemoryManager->allocate
        (
            (fPrefixBufSz + 1) * sizeof(XMLCh)
        ); //new XMLCh[fPrefixBufSz + 1];
    }
    XMLString::moveChars(fPrefix, prefix, newLen + 1);
}

void QName::setNPrefix(const XMLCh* prefix, const unsigned int newLen)
{
    if (!fPrefixBufSz || (newLen > fPrefixBufSz))
    {
        fMemoryManager->deallocate(fPrefix); //delete [] fPrefix;
        fPrefixBufSz = newLen + 8;
        fPrefix = (XMLCh*) fMemoryManager->allocate
        (
            (fPrefixBufSz + 1) * sizeof(XMLCh)
        ); //new XMLCh[fPrefixBufSz + 1];
    }
    XMLString::moveChars(fPrefix, prefix, newLen);
    fPrefix[newLen] = chNull;
}

void QName::setLocalPart(const XMLCh* localPart)
{
    unsigned int newLen;

    newLen = XMLString::stringLen(localPart);
    if (!fLocalPartBufSz || (newLen > fLocalPartBufSz))
    {
        fMemoryManager->deallocate(fLocalPart); //delete [] fLocalPart;
        fLocalPartBufSz = newLen + 8;
        fLocalPart = (XMLCh*) fMemoryManager->allocate
        (
            (fLocalPartBufSz + 1) * sizeof(XMLCh)
        ); //new XMLCh[fLocalPartBufSz + 1];
    }
    XMLString::moveChars(fLocalPart, localPart, newLen + 1);
}

void QName::setNLocalPart(const XMLCh* localPart, const unsigned int newLen)
{
    if (!fLocalPartBufSz || (newLen > fLocalPartBufSz))
    {
        fMemoryManager->deallocate(fLocalPart); //delete [] fLocalPart;
        fLocalPartBufSz = newLen + 8;
        fLocalPart = (XMLCh*) fMemoryManager->allocate
        (
            (fLocalPartBufSz + 1) * sizeof(XMLCh)
        ); //new XMLCh[fLocalPartBufSz + 1];
    }
    XMLString::moveChars(fLocalPart, localPart, newLen);
    fLocalPart[newLen] = chNull;
}

void QName::setValues(const QName& qname)
{
    setPrefix(qname.getPrefix());
    setLocalPart(qname.getLocalPart());
    setURI(qname.getURI());
}

// -----------------------------------------------------------------------
//  comparison
// -----------------------------------------------------------------------
bool QName::operator==(const QName& qname) const
{
    if (fURIId == 0) // null URI
        return (XMLString::equals(getRawName(),qname.getRawName()));

    return ((fURIId == qname.getURI()) &&
           (XMLString::equals(fLocalPart, qname.getLocalPart())));
}

// ---------------------------------------------------------------------------
//  QName: Private, helper methods
// ---------------------------------------------------------------------------
void QName::cleanUp()
{
    fMemoryManager->deallocate(fLocalPart); //delete [] fLocalPart;
    fMemoryManager->deallocate(fPrefix); //delete [] fPrefix;
    fMemoryManager->deallocate(fRawName); //delete [] fRawName;
    fLocalPart = fPrefix = fRawName = 0;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(QName)

void QName::serialize(XSerializeEngine& serEng)
{

    if (serEng.isStoring())
    {
        serEng.writeString(fPrefix, fPrefixBufSz, XSerializeEngine::toWriteBufferLen);

        serEng.writeString(fLocalPart, fLocalPartBufSz, XSerializeEngine::toWriteBufferLen);

        //do not serialize rawName

        serEng<<fURIId;
    }
    else
    {
        int dataLen = 0;

        serEng.readString(fPrefix, (int&)fPrefixBufSz, dataLen, XSerializeEngine::toReadBufferLen);

        serEng.readString(fLocalPart, (int&)fLocalPartBufSz, dataLen, XSerializeEngine::toReadBufferLen);

        //force raw name rebuilt
        fRawNameBufSz = 0;        
        fRawName = 0;

        serEng>>fURIId;
    }

}

XERCES_CPP_NAMESPACE_END
