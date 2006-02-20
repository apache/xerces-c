package TestUtils;
use XML::Xerces;
use strict;
use vars qw($VERSION
	    @ISA
	    @EXPORT
	    @EXPORT_OK
	    $CATALOG
	    $DOM
	    $PERSONAL
	    $PUBLIC_RESOLVER_FILE_NAME
	    $SYSTEM_RESOLVER_FILE_NAME
            $PERSONAL_SCHEMA_INVALID_FILE_NAME
	    $PERSONAL_SCHEMA_FILE_NAME
	    $SCHEMA_FILE_NAME
	    $SAMPLE_DIR
	    $PERSONAL_FILE_NAME
	    $PERSONAL_DTD_NAME
	    $PERSONAL_NO_DOCTYPE
	    $PERSONAL_NO_DOCTYPE_FILE_NAME
		$PERSONAL_NO_XMLDECL_FILE_NAME
		$PERSONAL_NO_XMLDECL
	   );
use Carp;
use Cwd;
require Exporter;

@ISA = qw(Exporter XML::Xerces::PerlEntityResolver);


@EXPORT_OK = qw(error
		$DOM
		$CATALOG
		$PERSONAL_FILE_NAME
		$PUBLIC_RESOLVER_FILE_NAME
		$SYSTEM_RESOLVER_FILE_NAME
		$SCHEMA_FILE_NAME
		$PERSONAL_SCHEMA_FILE_NAME
		$PERSONAL_SCHEMA_INVALID_FILE_NAME
		$PERSONAL_DTD_NAME
		$PERSONAL_NO_DOCTYPE_FILE_NAME
		$PERSONAL_NO_DOCTYPE
		$PERSONAL_NO_XMLDECL_FILE_NAME
		$PERSONAL_NO_XMLDECL
		$SAMPLE_DIR
		$PERSONAL);

BEGIN {
  # turn off annoying warnings
  $SIG{__WARN__} = 'IGNORE';
}

  # NOTICE: We must now explicitly call XMLPlatformUtils::Initialize()
  #   when the module is loaded. Xerces.pm no longer does this.
  #
  #
XML::Xerces::XMLPlatformUtils::Initialize();

  $DOM = XML::Xerces::XercesDOMParser->new();

  my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
  $DOM->setErrorHandler($ERROR_HANDLER);

  my $cwd = cwd();
  $cwd =~ s|/t/?$||;
  $SAMPLE_DIR = "$cwd/samples";
  $PERSONAL_FILE_NAME = "$SAMPLE_DIR/personal.xml";
  $PERSONAL_NO_DOCTYPE_FILE_NAME = "$SAMPLE_DIR/personal-no-doctype.xml";
  $PERSONAL_NO_XMLDECL_FILE_NAME = "$SAMPLE_DIR/personal-no-xmldecl.xml";

  $PERSONAL_DTD_NAME = $PERSONAL_FILE_NAME;
  $PERSONAL_DTD_NAME =~ s/\.xml/\.dtd/;
  $PERSONAL_SCHEMA_FILE_NAME = $PERSONAL_FILE_NAME;
  $PERSONAL_SCHEMA_FILE_NAME =~ s/\.xml/-schema.xml/;
  $PERSONAL_SCHEMA_INVALID_FILE_NAME = $PERSONAL_SCHEMA_FILE_NAME;
  $PERSONAL_SCHEMA_INVALID_FILE_NAME =~ s/-schema/-schema-invalid/;
  $SCHEMA_FILE_NAME = $PERSONAL_FILE_NAME;
  $SCHEMA_FILE_NAME =~ s/\.xml/.xsd/;
  $CATALOG = $PERSONAL_FILE_NAME;
  $CATALOG =~ s/personal/catalog/;
  $PUBLIC_RESOLVER_FILE_NAME = $PERSONAL_FILE_NAME;
  $PUBLIC_RESOLVER_FILE_NAME =~ s/personal/public/;
  $SYSTEM_RESOLVER_FILE_NAME = $PUBLIC_RESOLVER_FILE_NAME;
  $PUBLIC_RESOLVER_FILE_NAME =~ s/public/system/;
  open(PERSONAL, $PERSONAL_FILE_NAME)
    or die "Couldn't open $PERSONAL_FILE_NAME for reading";
  $/ = undef;
  $PERSONAL = <PERSONAL>;
  close PERSONAL;
  open(PERSONAL, $PERSONAL_NO_DOCTYPE_FILE_NAME)
    or die "Couldn't open $PERSONAL_NO_DOCTYPE_FILE_NAME for reading";
  $/ = undef;
  $PERSONAL_NO_DOCTYPE = <PERSONAL>;
  close PERSONAL;
  open(PERSONAL, $PERSONAL_NO_XMLDECL_FILE_NAME)
    or die "Couldn't open $PERSONAL_NO_XMLDECL_FILE_NAME for reading";
  $/ = undef;
  $PERSONAL_NO_XMLDECL = <PERSONAL>;
  close PERSONAL;

END {
  # NOTICE: We must now explicitly call XMLPlatformUtils::Terminate()
  #   when the module is unloaded. Xerces.pm no longer does this for us
  #
  #
  XML::Xerces::XMLPlatformUtils::Terminate();
}

sub error {
  my $error = shift;
  print STDERR "Error in eval: ";
  if (ref $error) {
    print STDERR $error->getMessage();
  } else {
    print STDERR $error;
  }
}

sub new {
  return bless {}, shift;
}

sub resolve_entity {
  my ($self,$pub,$sys) = @_;
#   print STDERR "Got PUBLIC: $pub\n";
#   print STDERR "Got SYSTEM: $sys\n";
  $main::test = 1;

  # we parse the example XML Catalog
  my $DOM = XML::Xerces::XercesDOMParser->new();
  my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
  $DOM->setErrorHandler($ERROR_HANDLER);
  eval{$DOM->parse($CATALOG)};
  if ($@) {
    my $msg = "Resolver: Couldn't parse catalog:";
    $@ = $@->getMessage()
      if ref $@;
    print STDERR "$msg $@\n";
  }

  # now retrieve the mappings
  my $doc = $DOM->getDocument();
  my @Maps = $doc->getElementsByTagName('Map');
  my %Maps = map {($_->getAttribute('PublicId'),
		   $_->getAttribute('HRef'))} @Maps;
  my @Remaps = $doc->getElementsByTagName('Remap');
  my %Remaps = map {($_->getAttribute('SystemId'),
		     $_->getAttribute('HRef'))} @Remaps;

  # now check which one we were asked for
  my $href;
  if ($pub) {
    $href = $Maps{$pub};
  } elsif ($sys) {
    $href = $Remaps{$sys};
  } else {
    croak("Neither PublicId or SystemId were defined");
  }
  my $is = eval {XML::Xerces::LocalFileInputSource->new($href)};
  if ($@) {
    my $msg = "Resolver: Couldn't create input source for entity:";
    $@ = $@->getMessage()
      if ref $@;
    print STDERR "$msg $@\n";
  }

  $is->DISOWN();
  return $is;
}
