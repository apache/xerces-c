#!/usr/bin/perl
use lib '.';
use SWIG qw(remove_method skip_to_closing_brace fix_method);
use File::Temp qw/tempfile/;
use File::Copy;

use strict;

###
### SWIG has now improved to the point that this file only installs the license
###

my $INFILE = "Xerces-tmp.cpp";
my $OUTFILE = "Xerces.cpp";

open(FILE, $INFILE)
  or die "Couldn't open $INFILE for reading";

my ($temp_fh, $temp_filename) = tempfile();

#
# Put the apache license in the output file
#

my $LICENSE = <<LICENSE;
/*
 * Copyright 2002,2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

LICENSE

print $temp_fh $LICENSE;

FILE: while(<FILE>) {
  print $temp_fh $_;
}

close FILE;
close $temp_fh;

copy($temp_filename, $OUTFILE)
  or die "Couldn't move $temp_filename to $OUTFILE: $!";

END {unlink($temp_filename)}

exit(0);
