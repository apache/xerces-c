#!/usr/bin/perl 
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

my $filename = $ARGV[0];
open(IN,"< $filename") || die("Couldn't open input file: $filename");

while (<IN>) {
  if ($_=~/^\s*class\s+[A-Z0-9_]+_EXPORT\s+([^\s]+)/) {
    $classname = $1;
    last;
  }
}

open(OUT,">out.html") || die("Could not write to out.html");

print OUT <<"END1";
<h3>
<font color="#000000">$filename:</font></h3>

<div id="$classname"><font face="Courier New,Courier"><font size=-1>class
$classname</font></font>
<table>
END1

while (<IN>) {
  if ($_=~/^\s*public\s*:/) {
    print OUT <<"END5";

<tr ALIGN=LEFT VALIGN=TOP>
<td><font face="Courier New,Courier"><font size=-1>{</font></font></td>

<td></td>

<td></td>

<td></td>

</tr>

<tr ALIGN=LEFT VALIGN=TOP>
<td><font face="Courier New,Courier"><font size=-1>public:</font></font></td>

<td></td>

<td></td>

<td></td>

</tr>
END5
    last;
  }
}

while (<IN>) {
  if ($_=~/^\s*(virtual)\s+([^\s\(][^\(]*[^\s\(])\s+([^\s\(]+\(.*)/) {
    $a=$1;
    $b=$2;
    $c=$3;
    while ($c!~/\)/) {
      $c.=<IN>;
    }
    print OUT <<"END";

<tr ALIGN=LEFT VALIGN=TOP>
<td></td>

<td><font face="Courier New,Courier"><font size=-1>$a</font></font></td>

<td><font face="Courier New,Courier"><font size=-1>$b</font></font></td>

<td><font face="Courier New,Courier"><font size=-1>$c</font></font></td>
</tr>
END
  } elsif ($_=~/^\s*(enum)\s+([^\s]+)\s*{/) {
    print OUT <<"END2";

<tr ALIGN=LEFT VALIGN=TOP>
<td></td>

<td><font face="Courier New,Courier"><font size=-1>$1</font></font></td>

<td><font face="Courier New,Courier"><font size=-1>$2 {</font></font></td>

<td></td>
</tr>
END2

    while (<IN>) {
      if ($_=~/([^\s]+)\s*=\s*([^\s,]+),?(\s|$)/) {
        print OUT <<"END3";
<tr ALIGN=LEFT VALIGN=TOP>
<td></td>

<td></td>

<td><font face="Courier New,Courier"><font size=-1>$1</font></font></td>

<td><font face="Courier New,Courier"><font size=-1>= $2,</font></font></td>
</tr>
END3
      }
      if ($_=~/}\s*;/) {
        print OUT <<"END4";
<tr ALIGN=LEFT VALIGN=TOP>
<td><font face="Courier New,Courier"><font size=-1>};</font></font></td>

<td></td>

<td></td>

<td></td>
</tr>
END4
        last;
      }
    }
  }

#  enum ExceptionCode {
#    INVALID_EXPRESSION_ERR = 51,
#    TYPE_ERR = 52,
#  };
}

print OUT <<"END6";
<tr ALIGN=LEFT VALIGN=TOP>
<td><font face="Courier New,Courier"><font size=-1>};</font></font></td>

<td></td>

<td></td>

<td></td>
</tr>
</table>
END6

close(OUT);
close(IN);
