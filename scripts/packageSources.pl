#!/usr/bin/perl
#
# The Apache Software License, Version 1.1
#
# Copyright (c) 1999-2003 The Apache Software Foundation.  All rights
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
# =============================================================================
# Author: Mike Pawlowski
# -----------------------------------------------------------------------------
# Packages XML4C source
# Requires Doxygen to be installed
# Requires Graphviz to be installed
# Requires X-windows to be running (on UNIX)
# If running on windows, make sure cygwin utilities bin overrides windows
# utilities bin in PATH
# This script will not work with the perl provided by cygwin
# -> use the ActiveState version of perl
# =============================================================================
# $Log$
# Revision 1.49  2003/10/06 19:45:51  peiyongz
# Rewrite from Mike Pawlowski.
#
# $Id$

# Includes ////////////////////////////////////////////////////////////////////

use strict;
use DirHandle;
use Getopt::Std;
use Cwd;

# Global Variables ////////////////////////////////////////////////////////////

my $xercescroot = undef;
my $above_dir   = undef;
my $packages    = undef;
my $old_root    = undef;

my $fs          = undef;
my $doc_cmd     = undef;
my $sub1        = undef;
my $sub2        = undef;
my $find_delim  = undef;
my $lbl1        = undef;
my $lbl2        = undef;

$| = 1;

# Functions ///////////////////////////////////////////////////////////////////


# =============================================================================
# Function [main]:
# =============================================================================
# =============================================================================

sub main {

   # --------------------------------------------------------------------------
   # --------------------------------------------------------------------------

   get_parameters();
   determine_opts();
   sanity_check();
   package_sources();


   exit( 0 );

}


# ==================================================================
# Function [get_parameters]:
# ==================================================================
# ==================================================================

sub get_parameters {

   #----------------------------------------------------------------
   #----------------------------------------------------------------

   my %opts     = ();
   my $help     = undef;

   getopt('xhp', \%opts);

   $old_root    = $opts{'x'};
   $packages    = $opts{'p'};
   $help        = $opts{'h'};

   if ( defined( $help ) ||
        !defined( $packages ) ||
        !defined( $old_root ) ) {
      print "\nperl packageSources.pl -x <xercescroot> -p <package names> [-h <help>]\n\n" .
             "-x :  the root of Xerces-C\n" .
             "-p :  the name of the source packages\n" .
             "-h :  this help screen\n\n";
      exit( 1 );
   }

   return 1;

}


# ==================================================================
# Function [determine_opts]:
# ==================================================================
# ==================================================================

sub determine_opts {

   #----------------------------------------------------------------
   #----------------------------------------------------------------

   if ( $^O =~ /win/i ) {
      $fs = "\\";
      $doc_cmd = 'createDocs.bat';
      $sub1 = "\r\n";
      $lbl1 = '\r\n';
      $sub2 = "\n";
      $lbl2 = '\n';
      $find_delim = ';';
   } else {
      $ENV{DISPLAY} = ':0';
      $fs = '/';
      $doc_cmd = 'sh createDocs.sh';
      $sub1 = "\n";
      $lbl1 = '\n';
      $sub2 = "\r\n";
      $lbl2 = '\r\n';
      $find_delim = '\;';
   }


   return 1;

}



# ==================================================================
# Function [sanity_check]:
# ==================================================================
# ==================================================================

sub sanity_check {

   #----------------------------------------------------------------
   #----------------------------------------------------------------

   my $zip_file = $old_root . $fs . '..' . $fs . $packages . '.zip';
   my $tar_file = $old_root . $fs . '..' . $fs . $packages . '.tar';
   my $gz_file  = $old_root . $fs . '..' . $fs . $packages . '.tar.gz';

   if ( ! -e $old_root ) {
      die "ERROR:  $old_root does not exist";
   } elsif ( -e $zip_file ) {
      die "ERROR:  $zip_file already exists";
   } elsif ( -e $tar_file ) {
      die "ERROR:  $tar_file already exists";
   } elsif ( -e $gz_file ) {
      die "ERROR:  $gz_file already exists";
   }

   return 1;

}


# ==================================================================
# Function [rename_root]:
# ==================================================================
# ==================================================================

sub rename_root {

   #----------------------------------------------------------------
   #----------------------------------------------------------------

   # Retrive directory above Xerces-C root

   pchdir( $old_root . $fs . '..' );
   $above_dir = cwd();

   # For some reason, cygwin on windows returns / instead of \ in the cwd
   # -> get rid of them

   if ( $^O =~ /win/i ) {
      $above_dir =~ s/\//\\/g;
   }

   # Rename Xerces-C root

   $xercescroot = $above_dir . $fs . $packages;
   rename( $old_root, $xercescroot );

   return 1;

}


# ==================================================================
# Function [pchdir]:
# ==================================================================
# ==================================================================

sub pchdir {

   #----------------------------------------------------------------
   my $dir = shift;
   #----------------------------------------------------------------

   my $status = 0;

	print "\nChange Directory:  $dir\n";
   $status = chdir( $dir );
	print "\n -> Status: [$status]\n";

   return 1;
}


# ==================================================================
# Function [psystem]:
# ==================================================================
# ==================================================================

sub psystem {

   #----------------------------------------------------------------
   my $command  = shift;
   #----------------------------------------------------------------

   my $status = 0;

	print "\nSystem Command:  $command\n";
   $status = system( $command );
	print "\n -> Status: [$status]\n";

   return 1;
}


# ==================================================================
# Function [pmkdir]:
# ==================================================================
# ==================================================================

sub pmkdir {

   #----------------------------------------------------------------
   my $dir = shift;
   #----------------------------------------------------------------

   my $status = 0;

   print "\nMaking Directory:  $dir\n";
   $status = mkdir( $dir, 0755 );
	print "\n -> Status: [$status]\n";

   return 1;
}


# ==================================================================
# Function [convert]:
# ==================================================================
# ==================================================================

sub convert {

   #----------------------------------------------------------------
   #----------------------------------------------------------------

   my $file  = undef;
   my $cmd   = 'find . -name "*" -exec file {} ' . $find_delim . ' | grep "text"';
   my $line  = undef;
   my $rest  = undef;
   my $pid   = undef;

   $pid = open( README, "$cmd |");
   while ( $line = <README> ) {
      ( $file, $rest ) = split( /:/, $line );
      do_sub( $file );
   }
   close( README );

}


# ==================================================================
# Function [do_sub]:
# ==================================================================
# ==================================================================

sub do_sub {

   #----------------------------------------------------------------
   my $file = shift;
   #----------------------------------------------------------------

   my $bak  = undef;
   my $line = undef;

   print "Substituting $lbl1 with $lbl2: $file\n";

   $bak = $file . '.bak';
   rename ( $file, $bak );

   open ( IN, "<$bak" );
   open ( OUT, ">$file" );

   while ( $line = <IN> ) {
      $line =~ s/$sub1/$sub2/g;
      print OUT $line;
   }
   close ( OUT );
   close ( IN );
   unlink ( $bak );

   return 1;

}


# ==================================================================
# Function [remove_exports]:
# ==================================================================
# ==================================================================

sub remove_exports {

   #----------------------------------------------------------------
   my $dir = shift;
   #----------------------------------------------------------------

   my $dh   = undef;
   my $file = undef;
   my $bak  = undef;
   my $line = undef;

   $dh = DirHandle->new( $dir );

   while ( defined ( $file = $dh->read() ) ) {

      if ( $file =~ /\.html$/ ) {

         print "Removing export clauses: $file\n";

         $file = $dir . $fs . $file;
         $bak = $file . '.bak';
         rename ( $file, $bak );

         open ( IN, "<$bak" );
         open ( OUT, ">$file" );

         while ( $line = <IN> ) {
            $line =~ s/SAX_EXPORT//g;
            $line =~ s/SAX2_EXPORT//g;
            $line =~ s/CDOM_EXPORT//g;
            $line =~ s/XMLPARSER_EXPORT//g;
            $line =~ s/PARSERS_EXPORT//g;
            $line =~ s/XMLUTIL_EXPORT//g;
            print OUT $line;
        }
        close ( OUT );
        close ( IN );
        unlink ( $bak );

      }
   }

   return 1;
}


# ==================================================================
# Function [package_sources]:
# ==================================================================
# ==================================================================

sub package_sources {

   #----------------------------------------------------------------
   #----------------------------------------------------------------

   print "\nPACKAGING SOURCES\n";

   print "\nRENAMING ROOT\n\n";

   rename_root();

   print "\nEXECUTING DOXYGEN\n\n";

   pchdir( $xercescroot . $fs . 'doc' );
   psystem( 'doxygen' );

   print "\nCREATING DOCS\n\n";

   pchdir( $xercescroot );
   psystem( $doc_cmd );

   print "\nDELETING FILES\n\n";

   pchdir( $xercescroot );
   psystem( 'find . -depth -type "d" -name "CVS" -exec rm -fr {} ' . $find_delim );
   psystem( 'rm -rf .' . $fs . 'samples' . $fs . 'Projects' . $fs . 'OS2' );
   psystem( 'rm -rf .' . $fs . 'Projects' . $fs . 'OS2' );
   psystem( 'rm -rf .' . $fs . 'Projects' . $fs . 'Win32' . $fs . 'BCB4' );
   psystem( 'rm -rf .' . $fs . 'Projects' . $fs . 'Win32' . $fs . 'BCB5' );
   psystem( 'rm -rf .' . $fs . 'Projects' . $fs . 'Win32' . $fs . 'Unsupported' );
   psystem( 'rm -rf .' . $fs . 'Projects' . $fs . 'Win32' . $fs . 'VACPP40' );

   print "\nCHANGING FILE PERMISSIONS\n\n";

   pchdir( $xercescroot );
   psystem( 'find . -type f -exec chmod 644 {} ' . $find_delim );
   psystem( 'find . -type d -exec chmod 755 {} ' . $find_delim );
   psystem( 'find . -name "con*" -exec chmod 755 {} ' . $find_delim );
   psystem( 'find . -name "run*" -exec chmod 755 {} ' . $find_delim );
   psystem( 'find . -name "install*" -exec chmod 755 {} ' . $find_delim );

   print "\nREMOVING EXPORT CLAUSES\n\n";

   remove_exports( $xercescroot . $fs . 'doc' . $fs . 'html' . $fs . 'apiDocs' );

   print "\nREMOVING MYSELF\n\n";

   pchdir( $xercescroot );
   psystem( 'rm .' . $fs . 'scripts' . $fs . 'packageSources.pl' );

   if ( $^O =~ /win/i ) {

      print "\nCREATING WINDOWS PACKAGE\n\n";

      pchdir( $xercescroot . $fs . '..' );
      psystem ( "zip -qr ${packages}\.zip $packages" );

   } else {

      print "\nCREATING UNIX PACKAGE\n\n";

      pchdir( $xercescroot . $fs . '..' );
      psystem ( "tar -cf ${packages}\.tar $packages" );
      psystem ( "gzip ${packages}\.tar" );

   }

   print "\nCONVERTING ASCII FILES\n\n";

   pchdir( $xercescroot );
   convert();

   if ( $^O =~ /win/i ) {

      print "\nCREATING UNIX PACKAGE\n\n";

      pchdir( $xercescroot . $fs . '..' );
      psystem ( "tar -cf ${packages}\.tar $packages" );
      psystem ( "gzip ${packages}\.tar" );

   } else {

      print "\nCREATING WINDOWS PACKAGE\n\n";

      pchdir( $xercescroot . $fs . '..' );
      psystem ( "zip -qr ${packages}\.zip $packages" );

   }


   print "\nSOURCE PACKAGING COMPLETE\n\n";

   return 1;
}


# Main Program /////////////////////////////////////////////////////////////

main();

