*
* The Apache Software License, Version 1.1
*
* Copyright (c) 2004 The Apache Software Foundation.  All rights
* reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
*
* 3. The end-user documentation included with the redistribution,
*    if any, must include the following acknowledgment:
*       "This product includes software developed by the
*        Apache Software Foundation (http://www.apache.org/)."
*    Alternately, this acknowledgment may appear in the software
*    itself, if and wherever such third-party acknowledgments 
*    normally appear.
*
* 4. The names "Xerces" and "Apache Software Foundation" must
*    not be used to endorse or promote products derived from this
*    software without prior written permission. For written
*    permission, please contact apache\@apache.org.
*
* 5. Products derived from this software may not be called "Apache",
*    nor may "Apache" appear in their name, without prior written
*    permission of the Apache Software Foundation.
*
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
* ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
* USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
* ====================================================================
*
* This software consists of voluntary contributions made by many
* individuals on behalf of the Apache Software Foundation, and was
* originally based on software copyright (c) 1999, International
* Business Machines, Inc., http://www.ibm.com .  For more information
* on the Apache Software Foundation, please see
* <http://www.apache.org/>.
*

*
* $Log$
* Revision 1.1  2004/02/06 15:02:11  cargilld
* Intrinsic transcoding support for 390.
*
*

TROT     CSECT
TROT     RMODE ANY
         USING   *,15
         STM     14,12,12(13)  Save register content
         USING   PARMS,1       R1 = parameters
         L       6,INPUT       R6 = input data
         L       4,LENGTH@     
         L       9,0(4)        R9 = length of input data
         L       4,STOP
         L       0,0(4)        R0 = Stop Character
         LA      4,0
         LA      5,1
         L       8,OUTPUT      R8 = output data
         L       1,TBL         R1 = Translate table @
RETR     TROT    8,6   Translate input pointed to output buffer pointed
         BZ      END           Branch to END if done
         BM      END           Branch to END if Stop Char encountered
         B       RETR          Continue translate
END      L       14,12(,13)    Restore the registers
         LM      2,12,28(13)
         BALR    1,14          Return
PARMS    DSECT
INPUT    DS A
OUTPUT   DS A
LENGTH@  DS A
TBL      DS A
STOP     DS A
   END
