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

// varout typemap (for global variables)
// useful for XMLUni constants
%typemap(varout) XMLCh[] %{
  sv_setsv((SV*)$result, UTF8_TRANSCODER->XMLString2Local($1));
%}

// fgBooleanValueSpace is an array of XMLCh*
%typemap(varout) XMLCh[][8] %{
    AV *myav = newAV();
    for (int i=0;i<4;i++) {
        av_push(myav, UTF8_TRANSCODER->XMLString2Local($1[i]));
    }

    SV* rv = newRV((SV*)myav);
    sv_setsv((SV*)$result, rv);
%}
