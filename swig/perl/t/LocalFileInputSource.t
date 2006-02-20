# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# LocalFileInputSource.t'

######################### We start with some black magic to print on failure.


END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 6;
use Cwd;

use lib 't';
use TestUtils qw($DOM $PERSONAL_NO_DOCTYPE_FILE_NAME $SAMPLE_DIR $PERSONAL_NO_DOCTYPE);
use vars qw($loaded $error);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

my $is = eval{XML::Xerces::LocalFileInputSource->new($PERSONAL_NO_DOCTYPE_FILE_NAME)};
XML::Xerces::error($@) if $@;
ok(UNIVERSAL::isa($is,'XML::Xerces::InputSource')
   && $is->isa('XML::Xerces::LocalFileInputSource')
  );

# test that a bogus relative path causes an exception
# 2003-06-10 JES: it seems that this has changed for 2.3 and 
# now a fatal error is thrown at parse time instead
#
# $is = eval {XML::Xerces::LocalFileInputSource->new('../foo/bar.xml')};
# my $error = $@;
# ok($error &&
#    UNIVERSAL::isa($error,'XML::Xerces::XMLException'));

# test that relative paths work
$is = eval{XML::Xerces::LocalFileInputSource->new($PERSONAL_NO_DOCTYPE_FILE_NAME)};
XML::Xerces::error($@) if $@;
ok(UNIVERSAL::isa($is,'XML::Xerces::LocalFileInputSource'));

eval{$DOM->parse($is)};
XML::Xerces::error($@) if $@;
my $serialize = $DOM->getDocument->serialize;
ok($serialize eq $PERSONAL_NO_DOCTYPE);

# test the overloaded constructor
my $cwd = cwd();
$is = XML::Xerces::LocalFileInputSource->new($cwd, "$SAMPLE_DIR/personal-no-doctype.xml");
ok(UNIVERSAL::isa($is,'XML::Xerces::LocalFileInputSource'));

eval{$DOM->parse($is)};
XML::Xerces::error($@) if $@;
$serialize = $DOM->getDocument->serialize;
ok($serialize eq $PERSONAL_NO_DOCTYPE);
