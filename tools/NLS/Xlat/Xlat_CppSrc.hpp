/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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
 * Revision 1.6  2004/09/08 13:57:07  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/04/14 08:41:00  gareth
 * Xlat now works under linux - Big thanks to Neil Graham (I no longer have to find a windows box). Still slight problems working with glibc before 2.2.4 (If you mess up the parameters it seg faults due to handling of wprintf)
 *
 * Revision 1.4  2000/03/02 19:55:53  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/06 07:48:41  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/05 20:24:58  roddey
 * Some changes to simplify life for the Messge Catalog message loader. The formatter
 * for the message loader now spits out a simple header of ids that allows the loader to
 * be independent of hard coded set numbers.
 *
 * Revision 1.1.1.1  1999/11/09 01:01:18  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:42:05  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


class CppSrcFormatter : public XlatFormatter
{
public :
    // -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
    CppSrcFormatter() :

        fCurDomainName(0)
        , fFirst(true)
        , fOutFl(0)
    {
    }

    virtual ~CppSrcFormatter()
    {
    	XMLString::release(&fCurDomainName);
    }


    // -----------------------------------------------------------------------
    //  Implementation of the formatter interface
    // -----------------------------------------------------------------------
    virtual void endDomain
    (
        const   XMLCh* const    domainName
        , const unsigned int    msgCount
    );

    virtual void endMsgType
    (
        const   MsgTypes        type
    );

    virtual void endOutput();

    virtual void nextMessage
    (
        const   XMLCh* const    msgText
        , const XMLCh* const    msgId
        , const unsigned int    messageId
        , const unsigned int    curId
    );

    virtual void startDomain
    (
        const   XMLCh* const    domainName
        , const XMLCh* const    nameSpace
    );

    virtual void startMsgType
    (
        const   MsgTypes        type
    );

    virtual void startOutput
    (
        const   XMLCh* const locale
        , const XMLCh* const outPath
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    CppSrcFormatter(const CppSrcFormatter&);
    void operator=(const CppSrcFormatter&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fCurDomainName
    //      This is the short name for the current domain being processed.
    //      This is used to make the names of the array and array size
    //      constant.
    //
    //  fFirst
    //      A trigger to catch the first message for a domain and do some
    //      slightly different formatting.
    //
    //  fOutFl
    //      This is the current output file. Its created when a new domain
    //      is begun via startDomain() and closed when endDomain is called.
    // -----------------------------------------------------------------------
    XMLCh*  fCurDomainName;
    bool    fFirst;
    FILE*   fOutFl;
};
