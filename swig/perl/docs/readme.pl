#!/usr/bin/perl -w
# author: Jason E. Stewart (jason@openinformatics.com)
# Copyright 2002 Jason E. Stewart
#

use strict;
use Getopt::Long;
use File::Temp qw(tempfile);
use XML::Xalan;
use Text::Wrap;
my %OPTIONS;
# $OPTIONS{program} = 'xalan';
$OPTIONS{output}  = '/home/jasons/work/xml-xerces/perl/README';
$OPTIONS{xslt}    = '/home/jasons/work/xml-xerces/perl/docs/readme.xsl';
$OPTIONS{input}   = '/home/jasons/build/apach-www-tools/xml-site/sources/xerces-p/readme.xml';
my $rc = GetOptions(\%OPTIONS,
		    '--output=s',
		    '--xslt=s',
		    '--input=s',
		    '--program=s',
		    'help');
my $USAGE = <<EOU;
usage: $0 [options]
 options:
    --input=file        - name of input xml file
                          (defaults to $OPTIONS{input})
    --output=file  - name of output genex2 xml file
                          (defaults to $OPTIONS{output})
    --program=file - name of xslt program
                          (defaults to $OPTIONS{program})
    --xslt         - name of xslt stylesheet to use
                          (defaults to $OPTIONS{xslt})
    --help         - this message
EOU

die $USAGE if exists $OPTIONS{help};
die $rc, $USAGE unless $rc;

my ($temp_fh,$temp_file) = tempfile('tempXXXX',UNLINK=>1);
my $DOCTYPE = "<!DOCTYPE s1 [\n";

open(IN,$OPTIONS{input})
  or die "Couldn't open $OPTIONS{input} for reading";


# first we print out the XML_Decl and the new DOCTYPE
my $input;
$input = <IN>;
$input .= $DOCTYPE;

# then throw away the original DOCTYPE
$_ = <IN>;

while (<IN>) {
  $input .= $_;
}

close(IN);
my $tr = XML::Xalan::Transformer->new();
my $namespace = "http://xml.apache.org/xalan-p/external_function";
my $func = sub {
  my ($first_indent,$subsequent_indent,$text) = @_;
  return wrap(' ' x $first_indent,
	      ' ' x $subsequent_indent,
	      $text);
};

$tr->install_function($namespace, 'wrap', $func);


my $parsed = $tr->parse_string($input);
my $res = $tr->transform_to_file($parsed, # input
				 $OPTIONS{xslt}, # stylesheet
				 $OPTIONS{output}); # output
die "Error transforming: ", $tr->errstr
  unless $res;

exit(0);
