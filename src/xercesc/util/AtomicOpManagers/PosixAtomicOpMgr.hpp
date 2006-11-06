
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
 */

#ifndef POSIXATOMICOPMGL_HPP
#define POSIXATOMICOPMGL_HPP

#include <xercesc/util/XMLAtomicOpMgr.hpp>
#include <xercesc/util/Mutexes.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//	Posix pthread mutex implementation.
class PosixAtomicOpMgr : public XMLAtomicOpMgr
{
    public:
        PosixAtomicOpMgr();
        virtual ~PosixAtomicOpMgr();

		// Atomic operations
		virtual void* 	compareAndSwap(void**            toFill
									 , const void* const newValue
									 , const void* const toCompare);
		virtual int		increment(int &location);
		virtual int		decrement(int &location);
		
	private:
		XMLMutex		fMutex;
};

XERCES_CPP_NAMESPACE_END


#endif

