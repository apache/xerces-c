/*
 * (C) Copyright IBM Corp. 1999  All rights reserved.
 *
 * US Government Users Restricted Rights Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 * The program is provided "as is" without any warranty express or
 * implied, including the warranty of non-infringement and the implied
 * warranties of merchantibility and fitness for a particular purpose.
 * IBM will not be liable for any damages suffered by you as a result
 * of using the Program. In no event will IBM be liable for any
 * special, indirect or consequential damages or lost profits even if
 * IBM has been advised of the possibility of their occurrence. IBM
 * will not be liable for any third party claims against you.
 */

/**
 * @version Revision:  02 1.2 src/com/ibm/xml/util/MsgLoaders/MsgCatalog/MsgCatalogLoader.hpp, xml4c3Utils, xml4c3 
 */

#if !defined(ICONVMSGLOADER_HPP)
#define ICONVMSGLOADER_HPP

#include <util/XML4CDefs.hpp>
#include <util/XMLMsgLoader.hpp>

#include <nl_types.h> 

//
//  This is a simple in Iconv RC message loader implementation.
//
class XMLUTIL_EXPORT MsgCatalogLoader : public XMLMsgLoader
{
public :
    // -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
    MsgCatalogLoader(const XMLCh* const msgDomain);
    ~MsgCatalogLoader();


    // -----------------------------------------------------------------------
    //  Implementation of the virtual message loader API
    // -----------------------------------------------------------------------
    virtual bool loadMsg
    (
        const   XMLMsgLoader::XMLMsgId  msgToLoad
        ,       XMLCh* const            toFill
        , const unsigned long           maxChars
    );

    virtual bool loadMsg
    (
        const   XMLMsgLoader::XMLMsgId  msgToLoad
        ,       XMLCh* const            toFill
        , const unsigned long           maxChars
        , const XMLCh* const            repText1
        , const XMLCh* const            repText2 = 0
        , const XMLCh* const            repText3 = 0
        , const XMLCh* const            repText4 = 0
    );

    virtual bool loadMsg
    (
        const   XMLMsgLoader::XMLMsgId  msgToLoad
        ,       XMLCh* const            toFill
        , const unsigned long           maxChars
        , const char* const             repText1
        , const char* const             repText2 = 0
        , const char* const             repText3 = 0
        , const char* const             repText4 = 0
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    MsgCatalogLoader();
    MsgCatalogLoader(const MsgCatalogLoader&);
    void operator=(const MsgCatalogLoader&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fModHandle
    //      This is our DLL module handle that we need in order to load
    //      resource messages. This is set during construction.
    //
    //  fMsgDomain
    //      This is the name of the error domain that this loader is for.
    // -----------------------------------------------------------------------
    nl_catd 	fCatalogHandle;
    XMLCh*      fMsgDomain;
	
};

#endif
