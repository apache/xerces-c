# use blib;
use XML::Xerces;
use Cwd;
use Test::More tests => 42;
use vars qw($text);
use utf8;
use lib 't';
use TestUtils qw($PERSONAL_NO_DOCTYPE);

pass("loaded");

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

my $utf8_text = "\x{66}\x{128}\x{144}";
my $ascii_text = "just a test";
my $utf8_document = <<XML;
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<text>$utf8_text</text>
XML

my $ascii_document = <<XML;
<?xml version="1.0" encoding="ASCII" standalone="yes"?>
<text>$ascii_text</text>
XML

package MyContentHandler;
use strict;
use base qw(XML::Xerces::PerlContentHandler);

sub start_element {
  my ($self, $uri, $localname, $qname, $attrs) = @_;
# print STDERR "Start element: uri: $uri, localname: $localname, qname: $qname, attrs: $attrs\n";
  push(@{$self->{start_elem_list}}, {localname=>$localname,
			       uri=>$uri,
			       qname=>$qname,
			      });
  push(@{$self->{attr_list}}, $attrs);
}

sub end_element {
  my ($self, $uri, $localname, $qname) = @_;
# print STDERR "End element: uri: $uri, localname: $localname, qname: $qname\n";
  push(@{$self->{end_elem_list}}, {localname=>$localname,
				   uri=>$uri,
				   qname=>$qname,
				  });
}

sub characters {
  my ($self,$str,$len) = @_;
# print STDERR "Characters: len: $len, str: $str\n";
  $::text .= $str;
}
sub ignorable_whitespace {
  my ($self,$str,$len) = @_;
}

sub processing_instruction {
  my ($self,$target,$data) = @_;
  push(@{$self->{pi_list}}, {target=>$target,
			     data=>$data,
			    });
}

sub reset_document {
  my ($self) = @_;
  $self->{start_elem_list} = [];
  $self->{end_elem_list} = [];
  $self->{attr_list} = [];
  $self->{pi_list} = [];
  $self->{map_start} = undef;
  $self->{map_end} = undef;
  $::text = '';
}

sub start_prefix_mapping {
  my ($self,$prefix,$uri) = @_;
# print STDERR "Prefix mapping start: prefix: $prefix, uri: $uri\n";
  $self->{map_start} = {prefix=>$prefix,
			uri=>$uri,
		       };
}

sub end_prefix_mapping {
  my ($self,$prefix) = @_;
# print STDERR "Prefix mapping end: prefix: $prefix\n";
  $self->{map_end} = {prefix=>$prefix,
		       };
}

package MyDocumentHandler;
use strict;
use base qw(XML::Xerces::PerlDocumentHandler);

sub start_element {
  my ($self, $name, $attrs) = @_;
# print STDERR "Start element: name: $name, attrs: $attrs\n";
  push(@{$self->{start_elem_list}}, {name=>$name,
			      });
  push(@{$self->{attr_list}}, $attrs);
}

sub end_element {
  my ($self, $name) = @_;
# print STDERR "End element: uri: $uri, name: $name\n";
  push(@{$self->{end_elem_list}}, {name=>$name,
				  });
}

sub characters {
  my ($self,$str,$len) = @_;
# print STDERR "Characters: len: $len, str: $str\n";
  $::text .= $str;
}
sub ignorable_whitespace {
  my ($self,$str,$len) = @_;
}

sub processing_instruction {
  my ($self,$target,$data) = @_;
  push(@{$self->{pi_list}}, {target=>$target,
			     data=>$data,
			    });
}

sub reset_document {
  my ($self) = @_;
  $self->{start_elem_list} = [];
  $self->{end_elem_list} = [];
  $self->{attr_list} = [];
  $self->{pi_list} = [];
  $::text = '';
}

package main;
my $SAX2 = XML::Xerces::XMLReaderFactory::createXMLReader();
isa_ok($SAX2, "XML::Xerces::SAX2XMLReader");

$SAX2->setFeature("$XML::Xerces::XMLUni::fgSAX2CoreValidation", 0);
$SAX2->setFeature("$XML::Xerces::XMLUni::fgXercesDynamic", 0);

my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
isa_ok($ERROR_HANDLER, "XML::Xerces::PerlErrorHandler");

$SAX2->setErrorHandler($ERROR_HANDLER);

my $CONTENT_HANDLER = MyContentHandler->new();
isa_ok($CONTENT_HANDLER, "MyContentHandler");
isa_ok($CONTENT_HANDLER, "XML::Xerces::PerlContentHandler");

$SAX2->setContentHandler($CONTENT_HANDLER);

my $SAX = XML::Xerces::SAXParser->new();
isa_ok($SAX, "XML::Xerces::SAXParser");

$SAX->setValidationScheme ($XML::Xerces::SAXParser::Val_Never);
$SAX->setErrorHandler($ERROR_HANDLER);

my $DOCUMENT_HANDLER = MyDocumentHandler->new();
isa_ok($DOCUMENT_HANDLER, "MyDocumentHandler");
isa_ok($DOCUMENT_HANDLER, "XML::Xerces::PerlDocumentHandler");

$SAX->setDocumentHandler($DOCUMENT_HANDLER);


#
# parse a document with only ASCII characters
#
$CONTENT_HANDLER->reset_document();

my $is = eval {XML::Xerces::MemBufInputSource->new($ascii_document)};
XML::Xerces::error($@) if $@;
isa_ok($is, "XML::Xerces::MemBufInputSource");

eval {$SAX2->parse($is)};
XML::Xerces::error($@) if $@;

is($text, $ascii_text, "ASCII text content - SAX2");

$DOCUMENT_HANDLER->reset_document();

eval {$SAX->parse($is)};
XML::Xerces::error($@) if $@;

is($text, $ascii_text, "ASCII text content - SAX");

#
# parse a document with UTF8 characters
#
$CONTENT_HANDLER->reset_document();

$is = eval {XML::Xerces::MemBufInputSource->new($utf8_document)};
XML::Xerces::error($@) if $@;
isa_ok($is, "XML::Xerces::MemBufInputSource");

eval {$SAX2->parse($is)};
XML::Xerces::error($@) if $@;

is($text, $utf8_text, "UTF8 text content - SAX2");

$DOCUMENT_HANDLER->reset_document();

eval {$SAX->parse($is)};
XML::Xerces::error($@) if $@;

is($text, $utf8_text, "UTF8 text content - SAX");

$CONTENT_HANDLER->reset_document();

#
# parse a document with UTF8 element names
#
my $uri = "http://www.boyscouts.$utf8_text/";
my $ns = "ns_$utf8_text";

my $elem_name = "elem_$utf8_text";
my $qname = "$ns:$elem_name";
$utf8_document = <<XML;
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<test xmlns:$ns="$uri"><$ns:$elem_name>$utf8_text</$ns:$elem_name></test>
XML

$is = eval {XML::Xerces::MemBufInputSource->new($utf8_document)};
XML::Xerces::error($@) if $@;
isa_ok($is, "XML::Xerces::MemBufInputSource");

eval {$SAX2->parse($is)};
XML::Xerces::error($@) if $@;

is($CONTENT_HANDLER->{start_elem_list}->[-1]->{localname}, $elem_name, "UTF8 element name - start handler");
is($CONTENT_HANDLER->{start_elem_list}->[-1]->{uri}, $uri, "UTF8 uri - start handler");
is($CONTENT_HANDLER->{start_elem_list}->[-1]->{qname}, $qname, "UTF8 qname - start handler");

is($CONTENT_HANDLER->{end_elem_list}->[0]->{localname}, $elem_name, "UTF8 element name - end handler");
is($CONTENT_HANDLER->{end_elem_list}->[0]->{uri}, $uri, "UTF8 uri - end handler");
is($CONTENT_HANDLER->{end_elem_list}->[0]->{qname}, $qname, "UTF8 qname - end handler");

is($text, $utf8_text, "UTF8 text content");

is($CONTENT_HANDLER->{map_start}->{prefix}, $ns, "UTF8 namespace - map start handler");
is($CONTENT_HANDLER->{map_start}->{uri}, $uri, "UTF8 uri - map start handler");

is($CONTENT_HANDLER->{map_end}->{prefix}, $ns, "UTF8 namespace - map end handler");

$DOCUMENT_HANDLER->reset_document();

eval {$SAX->parse($is)};
XML::Xerces::error($@) if $@;

is($DOCUMENT_HANDLER->{start_elem_list}->[-1]->{name}, "$ns:$elem_name", "UTF8 element name - start handler");
is($DOCUMENT_HANDLER->{end_elem_list}->[0]->{name}, "$ns:$elem_name", "UTF8 element name - end handler");

is($text, $utf8_text, "UTF8 text content");

$CONTENT_HANDLER->reset_document();

#
# parse a document with UTF8 attr names and values
#
my $attr_name = "attr_$utf8_text";
$qname = "$ns:$attr_name";

$utf8_document = <<XML;
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<test xmlns:$ns="$uri"><text $ns:$attr_name="$utf8_text">$utf8_text</text></test>
XML

$is = eval {XML::Xerces::MemBufInputSource->new($utf8_document)};
XML::Xerces::error($@) if $@;
isa_ok($is, "XML::Xerces::MemBufInputSource");

eval {$SAX2->parse($is)};
XML::Xerces::error($@) if $@;

is($CONTENT_HANDLER->{attr_list}->[-1]->getLocalName(0), $attr_name, "UTF8 attr name");
is($CONTENT_HANDLER->{attr_list}->[-1]->getValue(0), $utf8_text, "UTF8 attr value");
is($CONTENT_HANDLER->{attr_list}->[-1]->getQName(0), $qname, "UTF8 attr qname");
is($CONTENT_HANDLER->{attr_list}->[-1]->getURI(0), $uri, "UTF8 attr uri");

is($text, $utf8_text, "UTF8 text content");

$DOCUMENT_HANDLER->reset_document();

eval {$SAX->parse($is)};
XML::Xerces::error($@) if $@;

is($DOCUMENT_HANDLER->{attr_list}->[-1]->getName(0), "$ns:$attr_name", "UTF8 attr name - SAX");
is($DOCUMENT_HANDLER->{attr_list}->[-1]->getValue(0), $utf8_text, "UTF8 attr value - SAX");

is($text, $utf8_text, "UTF8 text content - SAX");

$CONTENT_HANDLER->reset_document();

#
# parse a document with UTF8 processing instruction
#
my $pi_name = "pi_$utf8_text";
$utf8_document = <<XML;
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<?$pi_name $utf8_text?>
<test/>
XML

$is = eval {XML::Xerces::MemBufInputSource->new($utf8_document)};
XML::Xerces::error($@) if $@;
isa_ok($is, "XML::Xerces::MemBufInputSource");

eval {$SAX2->parse($is)};
XML::Xerces::error($@) if $@;

is($CONTENT_HANDLER->{pi_list}->[-1]->{target}, $pi_name, "UTF8 processing instruction name");
is($CONTENT_HANDLER->{pi_list}->[-1]->{data}, $utf8_text, "UTF8 processing instruction data");

$DOCUMENT_HANDLER->reset_document();

eval {$SAX->parse($is)};
XML::Xerces::error($@) if $@;

is($DOCUMENT_HANDLER->{pi_list}->[-1]->{target}, $pi_name, "UTF8 processing instruction name - SAX");
is($DOCUMENT_HANDLER->{pi_list}->[-1]->{data}, $utf8_text, "UTF8 processing instruction data - SAX");

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}
