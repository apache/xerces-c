#!/usr/bin/perl -w
use strict;

use File::Temp qw/tempfile/;
use File::Copy;
use Getopt::Long;

my $USAGE = <<USAGE;
USAGE: $0 --infile=name --outfile=name
USAGE

my %OPTIONS;
my $rc = GetOptions(\%OPTIONS,
		    'infile=s',
		    'outfile=s',
		   );
die $USAGE unless $rc;

die $USAGE unless exists $OPTIONS{infile};
die $USAGE unless exists $OPTIONS{outfile};

open(FILE, $OPTIONS{infile})
  or die "Couldn't open $OPTIONS{infile} for reading";

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
  print $temp_fh $_;
}

close(FILE);
close($temp_fh);

copy($temp_filename, $OPTIONS{outfile});

END {unlink($temp_filename)}

exit(0);
