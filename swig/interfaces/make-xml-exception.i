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

%{

static XMLExceptionHandler* XML_EXCEPTION_HANDLER  = NULL;

static void
makeXMLException(const XMLException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) XML_EXCEPTION_HANDLER->copyXMLException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__XMLException, SWIG_SHADOW|0);
}

/*
 * The generic exception handler
 *    'goto fail' must be called - either explicitly, or via SWIG_croak()
 *    to ensure that any variable cleanup is done - to avoid memory leaks.
 *    By making these macros, it reduces the code *file* size dramatically
 *    (but doesn't reduce the compiled file size at all...)
 */

#define CATCH_XML_EXCEPTION         \
    catch (const XMLException& e)   \
    {                               \
        makeXMLException(e);        \
	goto fail;                  \
    }                               \
    catch (...)                     \
    {                               \
        SWIG_croak("Handling Unknown exception"); \
        goto fail;                  \
    }

%}

