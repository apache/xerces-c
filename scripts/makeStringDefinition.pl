#!/usr/bin/perl -w
#
# The Apache Software License, Version 1.1
#
# Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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

use vars qw($opt_h $opt_x);
use Getopt::Std;

my %charMapping = ( ':', 'chColon',
                    '\\', 'chBackSlash',
                    '[', 'chOpenSquare',
                    ']', 'chCloseSquare',
                    '{', 'chOpenCurly',
                    '}', 'chCloseCurly',
                    '?', 'chQuestion',
                    '(', 'chOpenParen',
                    ')', 'chCloseParen',
                    '/', 'chForwardSlash',
                    '*', 'chAsterisk',
                    '+', 'chPlus',
                    '.', 'chPeriod',
                    '-', 'chDash',
                    '|', 'chPipe',
                    '_', 'chUnderscore',
                    ',', 'chComma',
                    '&', 'chAmpersand',
                    '0', 'chDigit_0', '1', 'chDigit_1', '2', 'chDigit_2',
                    '3', 'chDigit_3', '4', 'chDigit_4', '5', 'chDigit_5',
                    '6', 'chDigit_6', '7', 'chDigit_7', '8', 'chDigit_8',
                    '9', 'chDigit_9');

#
# usage: display usage message
#
sub usage() {
    print<<EOF;
usage: $0 [ options ] word

Takes a word and produces a static XMLCh * definition for it.

Options:
    -h Displays this help message
    -x add the XERCES_CPP_NAMESPACE_QUALIFIER before each item
EOF
    exit(1);
}

#
# main:
#

getopts("hx");

if ($opt_h or @ARGV == 0) {
    usage();
}

my $word = $ARGV[0];

print "{ ";

while ($word=~s/^(.)//) {
  if (defined($charMapping{$1})) {
    $ch = $charMapping{$1};
  } else {
    $ch = $1;
    if ($ch=~/[A-Za-z]/) {
      $ch = "chLatin_$ch";
    } else {
      $ch = "UNKNOWN_CHAR_$ch";
    }
  }
	if($opt_x) {
			print "XERCES_CPP_NAMESPACE_QUALIFIER ";
	}
	print "$ch, ";
}

	if($opt_x) {
			print "XERCES_CPP_NAMESPACE_QUALIFIER ";
	}
print "chNull };\n";
