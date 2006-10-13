############# Class : XML::Xerces::PerlDefaultHandler ##############
package XML::Xerces::PerlDefaultHandler;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::ContentHandler
	  XML::Xerces::DocumentHandler);
sub new {
  my $class = shift;

  # support copy constructor syntax
  $class = ref($class) if ref($class);

  return bless {}, $class;
}

sub startElement {
  my $self = shift;
  $self->start_element(@_);
}
sub endElement {
  my $self = shift;
  $self->end_element(@_);
}
sub startPrefixMapping {
  my $self = shift;
  $self->start_prefix_mapping(@_);
}
sub endPrefixMapping {
  my $self = shift;
  $self->end_prefix_mapping(@_);
}
sub skippedEntity {
  my $self = shift;
  $self->skipped_entity(@_);
}
sub startDocument {
  my $self = shift;
  $self->start_document();
}
sub endDocument {
  my $self = shift;
  $self->end_document();
}
sub resetDocument {
  my $self = shift;
  $self->reset_document();
}
sub characters {}
sub processingInstruction {
  my $self = shift;
  $self->processing_instruction(@_);
}
sub setDocumentLocator {
  my $self = shift;
  $self->set_document_locator(@_);
}
sub ignorableWhitespace {
  my $self = shift;
  $self->ignorable_whitespace(@_);
}

#
# support alternate Perl-friendly syntax
#
sub start_element {}
sub end_element {}
sub start_prefix_mapping {}
sub end_prefix_mapping {}
sub skipped_entity {}
sub start_document {}
sub end_document {}
sub reset_document {}
sub processing_instruction {}
sub set_document_locator {}
sub ignorable_whitespace {}


############# Class : XML::Xerces::PerlContentHandler ##############
package XML::Xerces::PerlContentHandler;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlDefaultHandler);

############# Class : XML::Xerces::PerlDocumentHandler ##############
package XML::Xerces::PerlDocumentHandler;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::PerlDefaultHandler);

package XML::Xerces::Attributes;
sub to_hash {
  my $self = shift;
  my %hash;
  for (my $i=0; $i < $self->getLength(); $i++) {
    my $qname = $self->getQName($i);
    $hash{$qname}->{localName} = $self->getLocalName($i);
    $hash{$qname}->{URI} = $self->getURI($i);
    $hash{$qname}->{value} = $self->getValue($i);
    $hash{$qname}->{type} = $self->getType($i);
  }
  return %hash;
}

package XML::Xerces::AttributeList;
sub to_hash {
  my $self = shift;
  my %hash;
  for (my $i=0;$i<$self->getLength();$i++) {
    $hash{$self->getName($i)} = $self->getValue($i)
  }
  return %hash;
}

