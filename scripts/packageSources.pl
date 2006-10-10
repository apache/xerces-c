#
# Copyright 1999-2004 The Apache Software Foundation.
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
# $Id$
#

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
   
   print "\nCOMPRESS DOCS\n\n";

   pchdir( $xercescroot );

   if ( $^O =~ /win/i ) {
      psystem ( "zip -qr ${packages}-doc\.zip doc" );
   } else {
      psystem ( "tar -cf ${packages}-doc\.tar doc" );
      psystem ( "gzip ${packages}-doc\.tar" );
   }
   
   pchdir( $xercescroot . $fs . 'doc');
   psystem ('rm -rf *');
   pchdir( $xercescroot );
   psystem ('mv ${packages}-doc* doc');

   print "\nDELETING FILES\n\n";

   pchdir( $xercescroot );
   psystem( 'find . -depth -type "d" -name "CVS" -exec rm -fr {} ' . $find_delim );
   psystem( 'find . -depth -type "d" -name ".svn" -exec rm -fr {} ' . $find_delim );
   psystem( 'rm -rf .' . $fs . 'Projects' . $fs . 'Win32' . $fs . 'BCB5' );

   print "\nCHANGING FILE PERMISSIONS\n\n";

   pchdir( $xercescroot );
   psystem( 'find . -type f -exec chmod 644 {} ' . $find_delim );
   psystem( 'find . -type d -exec chmod 755 {} ' . $find_delim );
   psystem( 'find . -name "con*" -exec chmod 755 {} ' . $find_delim );
   psystem( 'find . -name "install*" -exec chmod 755 {} ' . $find_delim );
   psystem( 'find . -name "pretty-make" -exec chmod 755 {} ' . $find_delim );

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

