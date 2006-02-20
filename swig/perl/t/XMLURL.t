# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# XMLURL.t'

######################### We start with some black magic to print on failure.

END {ok(0) unless $loaded;}

use Carp;
# use blib;
use XML::Xerces;
use Test::More tests => 27;

use lib 't';
use TestUtils qw($DOM $PERSONAL
		 $PERSONAL_FILE_NAME);
use vars qw($loaded $error);
use strict;

$loaded = 1;
ok($loaded, "module loaded");

######################### End of black magic.

# test the one argument constructor 
my $query = 'foo=bar';
my $URL = "http://www.openinformatics.com/test/samples.pl?$query";
my $xml_url = XML::Xerces::XMLURL->new($URL);
ok(
   UNIVERSAL::isa($xml_url,'XML::Xerces::XMLURL')
  );

# test the supported protocols
foreach my $proto (qw(ftp file http)) {
  $URL = "$proto://www.openinformatics.com/test/samples.pl?$query";
  $xml_url = eval {XML::Xerces::XMLURL->new($URL)};
  XML::Xerces::error($@) if $@;
  ok(UNIVERSAL::isa($xml_url,'XML::Xerces::XMLURL'));
}

# test the copy constructor
my $xml_url2;
$xml_url2 = eval {XML::Xerces::XMLURL->new($xml_url)};
XML::Xerces::error($@) if $@;

ok(UNIVERSAL::isa($xml_url2,'XML::Xerces::XMLURL')
  );

# test getQuery
ok($xml_url2->getQuery() eq $query);

# test the equality operator
ok($xml_url == $xml_url2);

# test the two argument constructor 
my $host = 'www.openinformatics.com';
my $proto = 'ftp';
my $port = '2727';
my $user = 'me';
my $password = 'metoo';
my $base = "$proto://$user:$password\@$host:$port/";
my $path = '/test/samples.pl';
$query = 'bar';
my $fragment = 'foo';
my $url_text = "$path?$query#$fragment";
my $xml_url3 = eval{XML::Xerces::XMLURL->new($base, $url_text)};
XML::Xerces::error($@) if $@;

ok(UNIVERSAL::isa($xml_url,'XML::Xerces::XMLURL')
  );

# test getFragment
ok($xml_url3->getFragment() eq $fragment);

# test getQuery
ok($xml_url3->getQuery() eq $query);

# test getPath
ok($xml_url3->getPath() eq $path);

# test getURLText
$URL = $base;
$URL =~ s|/$||;
$URL .= $url_text;
ok($xml_url3->getURLText() eq $URL);

# test getPortNum
ok($xml_url3->getPortNum() eq $port);

# test getHost
ok($xml_url3->getHost() eq $host);

# test getUser
ok($xml_url3->getUser() eq $user);

# test getPassword
ok($xml_url3->getPassword() eq $password);

# test getProtocolName
ok($xml_url3->getProtocolName() eq $proto);

# test the inequality operator
ok($xml_url3 != $xml_url2);

# test the assignment operator
$xml_url3 = $xml_url2;
ok($xml_url3 == $xml_url2);

# test setURL with a text string
$xml_url3->setURL($URL);
ok($xml_url3->getURLText() eq $URL);

# test isRelative
eval {
  $xml_url2 = XML::Xerces::XMLURL->new($path);
};
if ($@) {
  die $@->getMessage()
    if ref($@);
  die $@;
}
ok(!$xml_url3->isRelative());
ok($xml_url2->isRelative());

# test makeRelativeTo
$xml_url3 = $xml_url2;
$xml_url2->makeRelativeTo($base);
ok(!$xml_url2->isRelative());

#test overloaded makeRelativeTo
eval {
  $xml_url = XML::Xerces::XMLURL->new($base);
};
if ($@) {
  die $@->getMessage()
    if ref($@);
  die $@;
}
$xml_url3->makeRelativeTo($xml_url);
ok(!$xml_url3->isRelative());

# test overloaded setURL with XMLURL for base
$xml_url2->setURL($xml_url,$url_text);
ok($xml_url2->getURLText() eq $URL);

# test overloaded setURL with string for base
$xml_url3->setURL($base,$url_text);
ok($xml_url3->getURLText() eq $URL);
