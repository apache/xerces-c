/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2004 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 * $Log$
 * Revision 1.1  2004/02/24 22:57:28  peiyongz
 * XercesDeprecatedDOMLib
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLRegisterCleanup.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// This is a mutex for exclusive use by this class
extern XMLMutex* gXMLCleanupListMutex;

// This is the head of a list of XMLRegisterCleanup objects that
// is used during XMLPlatformUtils::Terminate() to find objects to
// clean up
extern XMLRegisterCleanup* gXMLCleanupList;

void XMLRegisterCleanup::doCleanup()
{
    // When performing cleanup, we only do this once, but we can
    // cope if somehow we have been called twice.
    if (m_cleanupFn)
        m_cleanupFn();

    // We need to remove "this" from the list
    // irregardless of the cleanup Function
    unregisterCleanup();
}

// This function is called during initialisation of static data to
// register a function to be called on XMLPlatformUtils::Terminate.
// It gives an object that uses static data an opportunity to reset
// such data.
void XMLRegisterCleanup::registerCleanup(XMLCleanupFn cleanupFn)
{
    // Store the cleanup function
    m_cleanupFn = cleanupFn;
		
    // Add this object to the list head, if it is not already
    // present - which it shouldn't be.
    // This is done under a mutex to ensure thread safety.
    gXMLCleanupListMutex->lock();

    if (!m_nextCleanup && !m_prevCleanup) 
    {
        m_nextCleanup = gXMLCleanupList;
        gXMLCleanupList = this;

        if (m_nextCleanup)
            m_nextCleanup->m_prevCleanup = this;
    }

    gXMLCleanupListMutex->unlock();

}

// This function can be called either from XMLPlatformUtils::Terminate
// to state that the cleanup has been performed and should not be
// performed again, or from code that you have written that determines
// that cleanup is no longer necessary.
void XMLRegisterCleanup::unregisterCleanup()
{
    gXMLCleanupListMutex->lock();

    //
    // To protect against some compiler's (eg hp11) optimization
    // to change "this" as they update gXMLCleanupList
    //
    // refer to
    // void XMLPlatformUtils::Terminate()
    //       ...
    //       while (gXMLCleanupList)
    //            gXMLCleanupList->doCleanup();
    //

    XMLRegisterCleanup *tmpThis = (XMLRegisterCleanup*) this;

    // Unlink this object from the cleanup list
    if (m_nextCleanup) 
        m_nextCleanup->m_prevCleanup = m_prevCleanup;
		
    if (!m_prevCleanup) 
        gXMLCleanupList = m_nextCleanup;
    else 
        m_prevCleanup->m_nextCleanup = m_nextCleanup;

    gXMLCleanupListMutex->unlock();
		
    // Reset the object to the default state
    tmpThis->resetCleanup();

}

// The default constructor sets a state that ensures that this object
// will do nothing
XMLRegisterCleanup::XMLRegisterCleanup()
{
	resetCleanup();
}

// This function reinitialises the object to the default state
void XMLRegisterCleanup::resetCleanup() 
{
	m_nextCleanup = 0;
	m_prevCleanup = 0;
	m_cleanupFn = 0;
}

XERCES_CPP_NAMESPACE_END
