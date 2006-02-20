 #
 # Copyright 2002,2004 The Apache Software Foundation.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #      http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

use ExtUtils::MakeMaker;
use Env qw(XERCES_CONFIG
	   XERCES_DEVEL
	   SWIG);
use strict;
use vars qw($INCLUDES $CXX $CFLAGS @LDFLAGS @OPTIMIZE);
use File::Basename;
use Cwd qw(abs_path);

# We need to know which version of libxerces we are to use.
# As of Xerces-C 1.5.1, minor version numbers are used
my $XERCES_MAJOR_VERSION = '3.0';
my $XERCES_MINOR_VERSION = '0';
my $XERCES_LIB_VERSION = "$XERCES_MAJOR_VERSION.$XERCES_MINOR_VERSION";
my $XERCES_PERL_VERSION = $XERCES_LIB_VERSION . '-0';
my $XERCES_VERSION = "$XERCES_LIB_VERSION";

eval {
  require 5.6.0;
};
if ($@) {
  die <<ERROR;


    !!!WHOA!!!

You are using an *ancient* version of Perl. Because of the need to
support Unicode, XML::Xerces requires you upgrade to at least
v5.6.0. If you plan on using Unicode in your XML, you should really
upgrade to 5.7.2.

ERROR
}

my $XERCESCROOT = abs_path('../..');
my $XERCES_INCLUDE = "$XERCESCROOT/src";
my $XERCES_LIB = "$XERCESCROOT/obj/.libs";
my $LIBS = '-lpthread';

$INCLUDES = '-I. -IHandler';

print STDERR "Using XERCES_LIB = $XERCES_LIB\n";
$LIBS .= " -L$XERCES_LIB ";

print STDERR "Using XERCES_INCLUDE = $XERCES_INCLUDE\n";
$INCLUDES .= " -I$XERCES_INCLUDE -I$XERCESCROOT";


my $config_file = 'config.status';
my $CXXFLAGS;
my $LDFLAGS;
my $XERCES_CONFIG = "$XERCESCROOT/$config_file";

unless (-f $XERCES_CONFIG) {
  print STDERR <<EOW;

   WARNING

I am looking for the Xerces-C compilation config file. This should
have been located at:

  $XERCES_CONFIG

but this does not seem to point to the $config_file file that was used
to build your Xerces-C library. Without that file, I may not be able
to properly build the C++ glue files that come with Xerces.pm.

Proceeding anyway ...
EOW

}

if (-f $XERCES_CONFIG) {
  print STDERR "using XERCES_CONFIG: $XERCES_CONFIG\n";
  open(CONF,$XERCES_CONFIG)
    or die "Couldn't open $XERCES_CONFIG for reading";
  my $sep;
  while (<CONF>) {
    if (/\@CXXFLAGS\@/) {
      my ($sep, $flags) = /^s(.)\@CXXFLAGS\@\1([^\1]*)\1;/;
      my @flags = split /\s+/, $flags;
      my @new_flags;
      foreach my $flag (@flags) {
	if ($flag =~ /^(-(g|O).?)$/) {
	  push(@OPTIMIZE, $1);
	  next;
	} elsif ($flag =~ /^-DPROJ_/) {
	  next;
	}
	push(@new_flags,$flag);
      }
      if (scalar @OPTIMIZE) {
	@OPTIMIZE = (OPTIMIZE => join(" ", @OPTIMIZE));
      }
      ($CXXFLAGS) = join(' ',@new_flags);
    } elsif (/\@LDFLAGS\@/) {
      ($sep, $LDFLAGS) = /^s(.)\@LDFLAGS\@\1([^\1]*)\1;/;
      @LDFLAGS = ('LDFLAGS'     => $LDFLAGS)
	if $LDFLAGS !~ /^\s*$/;
    } elsif (/\@CXX\@/) {
      ($sep, $CXX) = /^s(.)\@CXX\@\1([^\1]*)\1;/
    }
  }
  print STDERR <<EOM;

Found CXX      = $CXX
Found CXXFLAGS = $CXXFLAGS
Found LDFLAGS  = $LDFLAGS
Found OPTIMIZE = @OPTIMIZE
EOM
}

$CFLAGS .= " $CXXFLAGS ";

# find the version of Xerces-C we are using
my $file_name = 'XercesVersion.hpp';
my $file = "$XERCES_INCLUDE/xercesc/util/$file_name";
open(VERSION,$file)
  or die "Couldn't find $file_name in your include directory $XERCES_INCLUDE";
print STDERR "Using Xerces-C version info from $file\n";
my ($major,$minor,$rev);
while (<VERSION>) {
  next unless /^\#define\s+XERCES_VERSION_/;
  if (/MAJOR\s+(\d)/) {
    $major = $1;
  } elsif (/MINOR\s+(\d)/) {
    $minor = $1;
  } elsif (/REVISION\s(\d)/) {
    $rev = $1;
  }
}
die "Couldn't find Xerces-C MAJOR version"
  unless defined $major;
die "Couldn't find Xerces-C MINOR version"
  unless defined $minor;
die "Couldn't find Xerces-C REVISION version"
  unless defined $rev;

my $XERCES_C_VERSION = "$major.$minor.$rev";

die <<EOE if $XERCES_VERSION ne $XERCES_C_VERSION;
      *** Version Mismatch ***

You are attempt to build XML::Xerces-$XERCES_PERL_VERSION using Xerces-C-$XERCES_C_VERSION,
this will most likely fail, so I am aborting. 

You must use Xerces-C-$XERCES_LIB_VERSION

EOE

# give some nice feedback for the user
print STDERR "Located Xerces-C version: $XERCES_VERSION\n";

# this substitution will yield '1_4' from '1.4' to be used by 'LIBS'
# $XERCES_LIB_VERSION =~ s/\./_/g;
my $LIBXERCES = "-lxerces";
if ($^O eq 'cygwin') {
  $LIBXERCES .= "$major$minor.dll";
} elsif ($^O eq 'MSWin32') {
  $LIBXERCES = "-lxerces-c_2";
}
$LIBS .= " $LIBXERCES";

my $libxerces_name = "libxerces-$major.$minor.so";
my $libxerces_path = "$XERCES_LIB/$libxerces_name";
die <<EOE unless -f $libxerces_path;


    !!!WHOA!!!

I couldn\'t find the xerces library at:

  $libxerces_path

EOE

print STDERR "Successfully located $libxerces_name!!\n";

my $HANDLER_LIB = '$(INST_ARCHLIB)/auto/Handler/Handler$(LIB_EXT)';

sub MY_postamble {
qq[

$HANDLER_LIB:
	\$(MAKE) -C Handler static
];

}
*MY::postamble = \&MY_postamble;

my @MACRO;
$CFLAGS .= '-D_REENTRANT';

my $HANDLER_DIR = 'Handler';
my $INTERFACE_DIR = '../interfaces';
my $SCRIPTS_DIR = '../scripts';
# we only consider using SWIG if we are a Xerces Developer
if ($XERCES_DEVEL) {
  # replace this with the path to your Unix compatible find application
  my $FIND = 'find';

  # set up the custom headers
  my @header_files;
  my @handler_headers;
  my @interface_files;
  chomp(@handler_headers = `$FIND $HANDLER_DIR -name "*.swig.hpp"`);

  sub Xerces_postamble {
    chomp(my @import_files = `$FIND $HANDLER_DIR -name "*.i"`);
    push(@header_files,@handler_headers,@import_files);

    chomp(my @interface_files = `$FIND $INTERFACE_DIR -name "*.i"`);
    @interface_files = grep {$_ !~ /shadow\.i/} @interface_files;
    local $" = ' ';
    my $OS_DEF;
    my $CC_DEF;
    if ($^O eq 'linux') {
      $OS_DEF = '-DXML_LINUX';
    } elsif($^O eq 'darwin') {
      $OS_DEF = '-DXML_MACOSX';
    } else {
      die 'unsupported operating system';
    }
    if ($CXX =~ /g\+\+/) {
      $CC_DEF = '-DXML_GCC';
    } else {
      die 'unsupported compiler';
    }
    my $SWIG_ARGS = qq[-Wall $OS_DEF $CC_DEF -DXERCES_VERSION="$XERCES_PERL_VERSION" \$(INC) -perl5 -c++ -shadow];

    my $MAKE = '	\$(MAKE) -C Handler static';
    if ($^O eq 'MSWin32') {
      # nmake doesn't not honor '-C'
      $MAKE =  '	cd Handler && \$(MAKE) static';
    }

    ### We no longer need to munge the C++ code thanks to SWIG improvements
    # perl postSource.pl Xerces.cpp
    my $retval = <<TERMINUS;
Xerces-tmp.pm: $SCRIPTS_DIR/postModule.pl $INTERFACE_DIR/Perl/shadow.i
	\$(SWIG) $SWIG_ARGS -o Xerces-tmp.cpp $INTERFACE_DIR/Xerces.i
	mv Xerces.pm Xerces-tmp.pm

Xerces.pm: $SCRIPTS_DIR/postModule.pl Xerces-tmp.pm
	perl -I$SCRIPTS_DIR $SCRIPTS_DIR/postModule.pl

Xerces.cpp: $SCRIPTS_DIR/postSource.pl Xerces-tmp.cpp
	perl -I$SCRIPTS_DIR $SCRIPTS_DIR/postSource.pl

Xerces-tmp.cpp: $INTERFACE_DIR/Xerces.i @interface_files @header_files  $INTERFACE_DIR/Perl/Xerces-extra.pm
	\$(SWIG) $SWIG_ARGS -o Xerces-tmp.cpp $INTERFACE_DIR/Xerces.i
	mv Xerces.pm Xerces-tmp.pm
	perl  -I$SCRIPTS_DIR $SCRIPTS_DIR/postModule.pl
	cp -f Xerces.pm blib/lib/XML/Xerces.pm

$HANDLER_LIB:
$MAKE
TERMINUS
    return $retval;
  }

  print STDERR "Welcome Xerces Developer!\n";
  # if we're to use SWIG, we need to know which version is available
  my $swig = $SWIG || 'swig';
  @MACRO = ('macro'       => {
    'SWIG'         => "$swig",
  });

  # we want all warnings
  $CFLAGS .= ' -Wall';
#  print STDERR "Using SWIG version: SWIG_$ {sv_maj}_$ {sv_min}\n";
  *MY::postamble = \&Xerces_postamble;
}

# set up the list of object files to include in Xerces\$(OBJ_EXT)
my $OBJS = 'Xerces$(OBJ_EXT)';

# write the makefile
# see the hints/ directory for architecture specific stuff!
WriteMakefile(
  'NAME'        => 'XML::Xerces',
  'AUTHOR'      => 'The Xerces-P developers: p-dev@xerces.apache.org',
  'ABSTRACT'    => 'Perl Interface for Xerces XML API',
  'CC'          => $CXX,
  'CCFLAGS'     => $CFLAGS,
  'PM'          => {
    'Xerces.pm' => '$(INST_LIB)/XML/Xerces.pm',
  },
  'INC'         => $INCLUDES,
  'MYEXTLIB'    => $HANDLER_LIB,
  'LIBS'        => [$LIBS],
  'OBJECT'      => $OBJS,
  'VERSION'     => "$XERCES_PERL_VERSION",
  @OPTIMIZE,
  @MACRO,
  @LDFLAGS,
);

