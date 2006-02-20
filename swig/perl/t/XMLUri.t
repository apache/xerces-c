# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# XMLUri.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 15;

use lib 't';
use TestUtils qw($DOM $PERSONAL_FILE_NAME);
use vars qw($loaded);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):
my $scheme = 'http';
my $host = 'www.openinformatics.com';
my $path = '/samples/personal.pl';
my $port = '2727';
my $query = 'additem=yes';

# test the overloaded constructor
my $uri;
eval {
  $uri = XML::Xerces::XMLUri->new("$scheme://$host:$port");
};
if ($@) {
  die $@->getMessage()
    if ref($@);
  die $@;
}
ok($uri &&
   UNIVERSAL::isa($uri,'XML::Xerces::XMLUri'));

# test the constructor
my $uri2;
eval {
  $uri2 = XML::Xerces::XMLUri->new($uri,"$path?$query");
};
if ($@) {
  die $@->getMessage()
    if ref($@);
  die $@;
}
ok($uri2 && UNIVERSAL::isa($uri2,'XML::Xerces::XMLUri'));

ok($uri2->getScheme() eq $scheme);
$scheme = 'ftp';
$uri2->setScheme($scheme);
ok($uri2->getScheme() eq $scheme);

ok($uri2->getHost() eq $host);
$host = 'www.openscience.org';
$uri2->setHost($host);
ok($uri2->getHost() eq $host);

ok($uri2->getPath() eq $path);
$path = '/test.pl';
$uri2->setPath($path);
ok($uri2->getPath() eq $path);

ok($uri2->getPort() eq $port);
$port = '4747';
$uri2->setPort($port);
ok($uri2->getPort() eq $port);

ok($uri2->getQueryString() eq $query);
$query = 'test=foo';
$uri2->setQueryString($query);
ok($uri2->getQueryString() eq $query);

$scheme = 'mailto';
my $user = 'jasons';
eval {
  $uri2 = XML::Xerces::XMLUri->new("$scheme://$user\@$host");
};
if ($@) {
  die $@->getMessage()
    if ref($@);
  die $@;
}
ok($uri2->getUserInfo() eq $user);

$user = 'bob';
$uri2->setUserInfo($user);
ok($uri2->getUserInfo() eq $user);
