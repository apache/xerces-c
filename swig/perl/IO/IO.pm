 #
 # Licensed to the Apache Software Foundation (ASF) under one or more
 # contributor license agreements.  See the NOTICE file distributed with
 # this work for additional information regarding copyright ownership.
 # The ASF licenses this file to You under the Apache License, Version 2.0
 # (the "License"); you may not use this file except in compliance with
 # the License.  You may obtain a copy of the License at
 #
 #      http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

# This file was created automatically by SWIG 1.3.30.
# Don't modify this file, modify the SWIG interface instead.
package XML::Xerces::IO;
use base qw(DynaLoader);
require XML::Xerces;
bootstrap XML::Xerces::IO;
package XML::Xerces::IO;
package XML::Xerces::IO;

# ------- FUNCTION WRAPPERS --------

package XML::Xerces;


############# Class : XML::Xerces::InputSource ##############

package XML::Xerces::InputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_InputSource($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::InputSource_makeStream;
*getEncoding = *XML::Xercesc::InputSource_getEncoding;
*getPublicId = *XML::Xercesc::InputSource_getPublicId;
*getSystemId = *XML::Xercesc::InputSource_getSystemId;
*getIssueFatalErrorIfNotFound = *XML::Xercesc::InputSource_getIssueFatalErrorIfNotFound;
*getMemoryManager = *XML::Xercesc::InputSource_getMemoryManager;
*setEncoding = *XML::Xercesc::InputSource_setEncoding;
*setPublicId = *XML::Xercesc::InputSource_setPublicId;
*setSystemId = *XML::Xercesc::InputSource_setSystemId;
*setIssueFatalErrorIfNotFound = *XML::Xercesc::InputSource_setIssueFatalErrorIfNotFound;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSInput ##############

package XML::Xerces::DOMLSInput;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSInput($self);
        delete $OWNER{$self};
    }
}

*getStringData = *XML::Xercesc::DOMLSInput_getStringData;
*getByteStream = *XML::Xercesc::DOMLSInput_getByteStream;
*getEncoding = *XML::Xercesc::DOMLSInput_getEncoding;
*getPublicId = *XML::Xercesc::DOMLSInput_getPublicId;
*getSystemId = *XML::Xercesc::DOMLSInput_getSystemId;
*getBaseURI = *XML::Xercesc::DOMLSInput_getBaseURI;
*setStringData = *XML::Xercesc::DOMLSInput_setStringData;
*setByteStream = *XML::Xercesc::DOMLSInput_setByteStream;
*setEncoding = *XML::Xercesc::DOMLSInput_setEncoding;
*setPublicId = *XML::Xercesc::DOMLSInput_setPublicId;
*setSystemId = *XML::Xercesc::DOMLSInput_setSystemId;
*setBaseURI = *XML::Xercesc::DOMLSInput_setBaseURI;
*setIssueFatalErrorIfNotFound = *XML::Xercesc::DOMLSInput_setIssueFatalErrorIfNotFound;
*getIssueFatalErrorIfNotFound = *XML::Xercesc::DOMLSInput_getIssueFatalErrorIfNotFound;
*release = *XML::Xercesc::DOMLSInput_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::Wrapper4DOMLSInput ##############

package XML::Xerces::Wrapper4DOMLSInput;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::InputSource XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_Wrapper4DOMLSInput(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_Wrapper4DOMLSInput($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::Wrapper4DOMLSInput_makeStream;
*getEncoding = *XML::Xercesc::Wrapper4DOMLSInput_getEncoding;
*getPublicId = *XML::Xercesc::Wrapper4DOMLSInput_getPublicId;
*getSystemId = *XML::Xercesc::Wrapper4DOMLSInput_getSystemId;
*getIssueFatalErrorIfNotFound = *XML::Xercesc::Wrapper4DOMLSInput_getIssueFatalErrorIfNotFound;
*setEncoding = *XML::Xercesc::Wrapper4DOMLSInput_setEncoding;
*setPublicId = *XML::Xercesc::Wrapper4DOMLSInput_setPublicId;
*setSystemId = *XML::Xercesc::Wrapper4DOMLSInput_setSystemId;
*setIssueFatalErrorIfNotFound = *XML::Xercesc::Wrapper4DOMLSInput_setIssueFatalErrorIfNotFound;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::LocalFileInputSource ##############

package XML::Xerces::LocalFileInputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::InputSource XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_LocalFileInputSource(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_LocalFileInputSource($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::LocalFileInputSource_makeStream;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::StdInInputSource ##############

package XML::Xerces::StdInInputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::InputSource XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_StdInInputSource(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_StdInInputSource($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::StdInInputSource_makeStream;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::URLInputSource ##############

package XML::Xerces::URLInputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::InputSource XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_URLInputSource(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_URLInputSource($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::URLInputSource_makeStream;
*urlSrc = *XML::Xercesc::URLInputSource_urlSrc;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::MemBufInputSource ##############

package XML::Xerces::MemBufInputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::InputSource XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
  my $pkg = shift;
  # SYSTEM ID is *optional*
  if (scalar @_ == 1) {
    push(@_,'FAKE_SYSTEM_ID');
  }

  my $self = XML::Xercesc::new_MemBufInputSource(@_);
    
  bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_MemBufInputSource($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::MemBufInputSource_makeStream;
*setCopyBufToStream = *XML::Xercesc::MemBufInputSource_setCopyBufToStream;
*resetMemBufInputSource = *XML::Xercesc::MemBufInputSource_resetMemBufInputSource;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLFormatter ##############

package XML::Xerces::XMLFormatter;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*NoEscapes = *XML::Xercesc::XMLFormatter_NoEscapes;
*StdEscapes = *XML::Xercesc::XMLFormatter_StdEscapes;
*AttrEscapes = *XML::Xercesc::XMLFormatter_AttrEscapes;
*CharEscapes = *XML::Xercesc::XMLFormatter_CharEscapes;
*EscapeFlags_Count = *XML::Xercesc::XMLFormatter_EscapeFlags_Count;
*DefaultEscape = *XML::Xercesc::XMLFormatter_DefaultEscape;
*UnRep_Fail = *XML::Xercesc::XMLFormatter_UnRep_Fail;
*UnRep_CharRef = *XML::Xercesc::XMLFormatter_UnRep_CharRef;
*UnRep_Replace = *XML::Xercesc::XMLFormatter_UnRep_Replace;
*DefaultUnRep = *XML::Xercesc::XMLFormatter_DefaultUnRep;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_XMLFormatter(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLFormatter($self);
        delete $OWNER{$self};
    }
}

*formatBuf = *XML::Xercesc::XMLFormatter_formatBuf;
*writeBOM = *XML::Xercesc::XMLFormatter_writeBOM;
*getEncodingName = *XML::Xercesc::XMLFormatter_getEncodingName;
*getTranscoder = *XML::Xercesc::XMLFormatter_getTranscoder;
*setEscapeFlags = *XML::Xercesc::XMLFormatter_setEscapeFlags;
*setUnRepFlags = *XML::Xercesc::XMLFormatter_setUnRepFlags;
*__lshift__ = *XML::Xercesc::XMLFormatter___lshift__;
*getEscapeFlags = *XML::Xercesc::XMLFormatter_getEscapeFlags;
*getUnRepFlags = *XML::Xercesc::XMLFormatter_getUnRepFlags;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLFormatTarget ##############

package XML::Xerces::XMLFormatTarget;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLFormatTarget($self);
        delete $OWNER{$self};
    }
}

*writeChars = *XML::Xercesc::XMLFormatTarget_writeChars;
*flush = *XML::Xercesc::XMLFormatTarget_flush;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::StdOutFormatTarget ##############

package XML::Xerces::StdOutFormatTarget;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLFormatTarget XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_StdOutFormatTarget(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_StdOutFormatTarget($self);
        delete $OWNER{$self};
    }
}

*writeChars = *XML::Xercesc::StdOutFormatTarget_writeChars;
*flush = *XML::Xercesc::StdOutFormatTarget_flush;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::LocalFileFormatTarget ##############

package XML::Xerces::LocalFileFormatTarget;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLFormatTarget XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_LocalFileFormatTarget(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_LocalFileFormatTarget($self);
        delete $OWNER{$self};
    }
}

*writeChars = *XML::Xercesc::LocalFileFormatTarget_writeChars;
*flush = *XML::Xercesc::LocalFileFormatTarget_flush;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::MemBufFormatTarget ##############

package XML::Xerces::MemBufFormatTarget;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLFormatTarget XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_MemBufFormatTarget(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_MemBufFormatTarget($self);
        delete $OWNER{$self};
    }
}

*writeChars = *XML::Xercesc::MemBufFormatTarget_writeChars;
*getRawBuffer = *XML::Xercesc::MemBufFormatTarget_getRawBuffer;
*getLen = *XML::Xercesc::MemBufFormatTarget_getLen;
*reset = *XML::Xercesc::MemBufFormatTarget_reset;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


# ------- VARIABLE STUBS --------

package XML::Xerces;

1;
