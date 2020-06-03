/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include    <xercesc/util/PlatformUtils.hpp>
#include    <xercesc/util/XMLString.hpp>
#include    <xercesc/util/XMLURL.hpp>
#include    <xercesc/util/XMLNetAccessor.hpp>
#include    <xercesc/util/BinInputStream.hpp>

#include	<iostream>

using namespace XERCES_CPP_NAMESPACE;


inline std::ostream& operator<<(std::ostream& os, const XMLCh* xmlStr)
{
	char* transcoded = XMLString::transcode(xmlStr);
    os << transcoded;
    XMLString::release(&transcoded);
    return os;
}


void
exercise(BinInputStream& stream)
{
	static float percents[] = { 1.0, 0.5, 0.25, 0.1, 0.15, 0.113, 0.333, 0.0015, 0.0013 };
	int numPercents = sizeof(percents) / sizeof(float);
	
	const unsigned int bufferMax = 4096;
	XMLByte buffer[bufferMax];

	int iteration = 0;
	unsigned int bytesRead = 0;
	do {
		// Calculate a percentage of our maximum buffer size, going through
		// them round-robin
		float percent = percents[iteration % numPercents];
		unsigned int bufCnt = (unsigned int)(bufferMax * percent);
		
		// Check to make sure we didn't go out of bounds
		if (bufCnt <= 0)
			bufCnt = 1;
		if (bufCnt > bufferMax)
			bufCnt = bufferMax;
		
		// Read bytes into our buffer
		bytesRead = stream.readBytes(buffer, bufCnt);
		//std::cerr << "Read " << bytesRead << " bytes into a " << bufCnt << " byte buffer\n";

		if (bytesRead > 0)
		{
			// Write the data to standard out
			std::cout.write((char*)buffer, bytesRead);
		}
		
		++iteration;
	} while (bytesRead > 0);
}


// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int
main(int argc, char** argv)
{
    // Init the XML platform
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch(const XMLException& toCatch)
    {
        std::cout << "Error during platform init! Message:\n"
             << toCatch.getMessage() << std::endl;
        return 1;
    }
    
    // Look for our one and only parameter
    if (argc != 2)
    {
    	std::cerr << "Usage: NetAccessorTest url\n"
    			"\n"
    			"This test reads data from the given url and writes the result\n"
    			"to standard output.\n"
    			"\n"
    			"A variety of buffer sizes is are used during the test.\n"
    			"\n"
    			;
    	exit(1);
    }
    
    // Get the URL
    char* url = argv[1];
    
    // Do the test
    try
    {
    	XMLURL xmlURL(url);
    	
		// Get the netaccessor
		XMLNetAccessor* na = XMLPlatformUtils::fgNetAccessor;
		if (na == 0)
		{
			std::cerr <<  "No netaccessor is available. Aborting.\n";
			exit(2);
		}
		
		// Build a binary input stream
		BinInputStream* is = na->makeNew(xmlURL);
		if (is == 0)
		{
			std::cerr <<  "No binary input stream created. Aborting.\n";
			exit(3);
		}
		
		// Exercise the inputstream
		exercise(*is);
		
		// Delete the is
		delete is;
	
    }
    catch(const XMLException& toCatch)
    {
        std::cout << "Exception during test:\n    "
             << toCatch.getMessage()
             << std::endl;
    }

    // And call the termination method
    XMLPlatformUtils::Terminate();

    return 0;
}

