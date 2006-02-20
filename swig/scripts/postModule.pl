#!/usr/bin/perl -w

use lib '.';
use strict;
use SWIG qw(remove_method skip_to_closing_brace fix_method);
use File::Temp qw/tempfile/;
use File::Copy;

my $INFILE = "Xerces-tmp.pm";
my $OUTFILE = "Xerces.pm";

open(FILE, $INFILE)
  or die "Couldn't open $INFILE for reading";

my ($temp_fh, $temp_filename) = tempfile();

#
# Put the apache license in the output file
#

my $LICENSE = <<LICENSE;
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

LICENSE

print $temp_fh $LICENSE;

my $CURR_CLASS = '';
while ($_ = <FILE>) {

  if (/^package/) {
    if (m/package\s+XML::Xerces::(\w+);/) {
      ($CURR_CLASS) = $1;
    }
    print $temp_fh $_;
    next;
  }

#   # for some reason (I don't want to figure out) SWIG puts a bunch of
#   # methods directly in the XML::Xerces namespace that don't belong there
#   # and are duplicated within their proper classes, so we delete them
#   if (/FUNCTION WRAPPERS/) {
#     while ($_ = <FILE>) {
#       next unless /\#\#\#\#\#\#\#\#\#\#\#\#\#/;
#       last;
#     }
#   }
# 
#   # we remove all the enums inherited through DOMNode
#   next if /^\*[_A-Z]+_NODE =/ && !/DOMNode/;
# 
#   # now we set these aliases correctly
#   s/\*XML::Xerces::/*XML::Xercesc::/;

  #######################################################################
  #
  # MUNGE MODULE for XMLCh support
  #
  #    CHANGE "$args[0] = tied(%{$args[0]})"
  #    TO     "$args[0] = tied(%{$args[0]}) || $args[0]"
  #    then we wrap it in a defined $args[0] to remove the irritating warning
#   if (m{\$args\[0\]\s+=\s+tied\(\%\{\$args\[0\]\}\)}) {
#     print $temp_fh <<'EOT';
#        if (defined $args[0]) {
# 	 $args[0] = tied(%{$args[0]}) || $args[0];
#        }
# EOT
#     next;
#   }


  #   CHANGE "return undef if (!defined($result))"
  #   TO     "return $result unless ref($result) =~ m[XML::Xerces]"
  # split on multiple lines to be readable, using s{}{}x
#   s{
#     return\s*undef\s*if\s*\(\!defined\(\$result\)\)
#   }{return \$result unless UNIVERSAL::isa(\$result,'XML::Xerces')}x;

  print $temp_fh $_;
}

close(FILE);
close($temp_fh);

copy($temp_filename, $OUTFILE);

END {unlink($temp_filename)}
