#!perl
#
# The Apache Software License, Version 1.1
#
# Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
# reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#
# 3. The end-user documentation included with the redistribution,
#    if any, must include the following acknowledgment:
#       "This product includes software developed by the
#        Apache Software Foundation (http://www.apache.org/)."
#    Alternately, this acknowledgment may appear in the software itself,
#    if and wherever such third-party acknowledgments normally appear.
#
# 4. The names "Xerces" and "Apache Software Foundation" must
#    not be used to endorse or promote products derived from this
#    software without prior written permission. For written
#    permission, please contact apache\@apache.org.
#
# 5. Products derived from this software may not be called "Apache",
#    nor may "Apache" appear in their name, without prior written
#    permission of the Apache Software Foundation.
#
# THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
# ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
# ====================================================================
#
# This software consists of voluntary contributions made by many
# individuals on behalf of the Apache Software Foundation, and was
# originally based on software copyright (c) 1999, International
# Business Machines, Inc., http://www.ibm.com .  For more information
# on the Apache Software Foundation, please see
# <http://www.apache.org/>.
#

#
# $Id$
#


#  Author(s):   Mike Strosaker
#
#  Verify that a Xerces-C build is successful.
#  Command-Line Parameter:
#      The OS type ("win" or "unix")
#
#  Prerequisites:
#      Set the path to include the appropriate executables
#      and the library path to include the libraries

$os     = $ARGV[0];

if (lc($os) eq "win") {
    $pathsep = "\\";
}
else {
    $pathsep = "/";
}

chdir "samples".$pathsep."data";

#  Run SAXCount
system ("SAXCount");
system ("SAXCount -v=never personal.xml");
system ("SAXCount personal.xml");
system ("SAXCount -n -s personal-schema.xml");

#  Run SAXPrint
system ("SAXPrint");
system ("SAXPrint -v=never personal.xml");
system ("SAXPrint personal.xml");
system ("SAXPrint -n -s personal-schema.xml");

#  Run SAX2Count
system ("SAX2Count");
system ("SAX2Count -v=never personal.xml");
system ("SAX2Count personal.xml");
system ("SAX2Count personal-schema.xml");

#  Run SAX2Print
system ("SAX2Print");
system ("SAX2Print -v=never personal.xml");
system ("SAX2Print personal.xml");
system ("SAX2Print personal-schema.xml");

#  Run MemParse
system ("MemParse");
system ("MemParse -v=never");

#  Run Redirect
system ("Redirect");
system ("Redirect personal.xml");

#  Run DOMCount
system ("DOMCount");
system ("DOMCount -v=never personal.xml");
system ("DOMCount personal.xml");
system ("DOMCount -n -s personal-schema.xml");

#  Run DOMPrint
system ("DOMPrint");
system ("DOMPrint -v=never personal.xml");
system ("DOMPrint personal.xml");
system ("DOMPrint -n -s personal-schema.xml");

#  Run StdInParse
system ("StdInParse < personal.xml");
system ("StdInParse -v=never < personal.xml");
system ("StdInParse -n -s < personal-schema.xml");

#  Run PParse
system ("PParse");
system ("PParse personal.xml");
system ("PParse -n -s personal-schema.xml");

#  Run EnumVal
system ("EnumVal");
system ("EnumVal personal.xml");

chdir "..".$pathsep."..";
