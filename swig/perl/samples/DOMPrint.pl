######################################################################
#
# The Apache Software License, Version 1.1
# 
# Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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
######################################################################
#
# DOMPrint
#
# This sample is loosely modeled after its XML4C counterpart.
# It parses a DOM document tree and reconstructs the "printed"
# form of the document.  One important difference from the XXML4C
# example is that this one makes use of the DOMPARSE utilities only
# available in XML4P.
#
######################################################################

use strict;
# use blib;
use XML::Xerces;
use Getopt::Long;
use vars qw();

#
# Read and validate command line args
#

my $USAGE = <<EOU;
USAGE: $0 [-v=xxx][-n] file
Options:
    -v=xxx      Validation scheme [always | never | auto*]
    -n          Enable namespace processing. Defaults to off.
    -s          Enable schema processing. Defaults to off.

  * = Default if not provided explicitly

EOU
my $VERSION = q[$Id: DOMPrint.pl,v 1.11 2002/10/19 23:01:12 jasons Exp $];
my %OPTIONS;
my $rc = GetOptions(\%OPTIONS,
		    'v=s',
		    'n',
		    's');

die $USAGE unless $rc;

die $USAGE unless scalar @ARGV;

my $file = $ARGV[0];
-f $file or die "File '$file' does not exist!\n";

my $namespace = $OPTIONS{n} || 0;
my $schema = $OPTIONS{s} || 0;
my $validate = $OPTIONS{v} || 'auto';

if (uc($validate) eq 'ALWAYS') {
  $validate = $XML::Xerces::AbstractDOMParser::Val_Always;
} elsif (uc($validate) eq 'NEVER') {
  $validate = $XML::Xerces::AbstractDOMParser::Val_Never;
} elsif (uc($validate) eq 'AUTO') {
  $validate = $XML::Xerces::AbstractDOMParser::Val_Auto;
} else {
  die("Unknown value for -v: $validate\n$USAGE");
}

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();


#
# Parse and print
#

my $parser = XML::Xerces::XercesDOMParser->new();
$parser->setValidationScheme ($validate);
$parser->setDoNamespaces ($namespace);
$parser->setCreateEntityReferenceNodes(1);
$parser->setDoSchema ($schema);

my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
$parser->setErrorHandler($ERROR_HANDLER);
eval {$parser->parse ($file)};
XML::Xerces::error($@) if ($@);

my $doc = $parser->getDocument();
my $impl = XML::Xerces::DOMImplementationRegistry::getDOMImplementation('LS');
my $writer = $impl->createDOMWriter();
if ($writer->canSetFeature($XML::Xerces::XMLUni::fgDOMWRTFormatPrettyPrint,1)) {
  $writer->setFeature($XML::Xerces::XMLUni::fgDOMWRTFormatPrettyPrint,1);
}
my $target = XML::Xerces::StdOutFormatTarget->new();
$writer->writeNode($target,$doc);

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}

exit(0);

