#
# XML::Xerces::Writer.pm
#   a module for exporting XML. This code is *untested* and is provided as
#   a starting point for someone to create there own XML writer module.
#
package XML::Xerces::Writer;

use strict;

sub new {
  my $class = shift;
  my $obj;
  if (ref($class)) {
    # copy an existing object
    $obj = $class;
    $class = ref($class);
  }
  my $self = bless {}, $class;
  if (defined $obj) {
    $self->set_slots([$obj->get_slot_names],
                     [$obj->get_slots($obj->get_slot_names)],
                    );
  } else {
    $self->set_slots(@_) if @_;
  }
  my $rc = $self->initialize;
  return undef if $rc == -1;
  return $self;
}


=item $obj->set_slots(%parameters)

=item $obj->set_slots(\@name_list, \@value_list)

The C<set_slots()> method is used to set a number of slots at the same
time. It has two different invocation methods. The first takes a named
parameter list, and the second takes two array references.

B<Return value>: none

B<Side effects>: will call C<die()> if a slot_name is used that the class
does not define.

=cut

sub set_slots {
  my $self = shift;
  my %slots;
  if (ref($_[0])) {
    my @slot_names = @{shift()};
    my @slot_values = @{shift()};
    @slots{@slot_names} = @slot_values;
  } else {
    %slots = @_;
  }
  while (my ($slot_name,$slot_val) = each %slots) {
    $self->set_slot($slot_name,$slot_val);
  }
}


=item $obj->get_slots(@name_list)

The C<get_slots()> method is used to get the values of a number of
slots at the same time.

B<Return value>: a list of instance objects

B<Side effects>: none

=cut

sub get_slots {
  my ($self, @slot_names) = @_;
  my @return;
  foreach my $slot (@slot_names) {
    push(@return,$self->get_slot($slot));
  }
  return @return;
}


=item $val = $obj->set_slot($name,$val)

The C<set_slot()> method sets the slot C<$name> to the value C<$val>

B<Return value>: the new value of the slot, i.e. C<$val>

B<Side effects>: none

=cut

sub set_slot {
  my ($self, $slot_name, $slot_val) = @_;
  my $method = 'set' . ucfirst($slot_name);
  unless ($self->can($method)) {
    unless ($self->can($slot_name)) {
      die(__PACKAGE__ . "::set_slot: slot $slot_name doesn't exist");
    }
    # this is a class slot, not an attribute or association. They still
    # use the confusing polymorphic setter/getter methods.
    $method = $slot_name;
  }
  {
    no strict 'refs';
    # invoke the setter directly to gain type checking
    return $self->$method($slot_val);
  }
}


=item $val = $obj->get_slot($name)

The C<get_slot()> method is used to get the values of a number of
slots at the same time.

B<Return value>: a single slot value, or undef if the slot has not been
initialized.

B<Side effects>: none

=cut

sub get_slot {
  my ($self, $slot_name) = @_;
  my $method = 'get' . ucfirst($slot_name);
  unless ($self->can($method)) {
    unless ($self->can($slot_name)) {
      die(__PACKAGE__ . "::get_slot: slot $slot_name doesn't exist");
    }
    # this is a class slot, not an attribute or association. They still
    # use the confusing polymorphic setter/getter methods.
    $method = $slot_name;
  }
  {
    no strict 'refs';
    # invoke the getter directly
    return $self->$method();
  }
}


=item @names = $obj->get_slot_names()

The C<get_slot_names()> method is used to retrieve the name of all
slots defined for a given object.

B<Return value>: a single slot value, or undef if the slot has not been
initialized.

B<Side effects>: none

=cut

sub get_slot_names {
  my ($self) = @_;
  return $self->{__SLOT_NAMES};
}


sub initialize {
  my ($self) = shift;
  $self->tag_buffer([]);
  $self->attrs_on_one_line(0)
    unless defined $self->attrs_on_one_line();
  $self->attr_indent(1)
    unless defined $self->attr_indent();
  $self->indent_increment(2)
    unless defined $self->indent_increment();
  $self->indent_level(0)
    unless defined $self->indent_level();
  $self->encoding('ISO-8859-1')
    unless defined $self->encoding();

  return $self;
}

sub encoding {
  my $self = shift;
  if (scalar @_) {
    $self->{_ENCODING} = shift;
  }
  return $self->{_ENCODING};
}

sub fh {
  my $self = shift;
  if (scalar @_) {
    $self->{_FH} = shift;
  }
  return $self->{_FH};
}

sub collapse_tag {
  my $self = shift;
  if (scalar @_) {
    $self->{_COLLAPSE_TAG} = shift;
  }
  return $self->{_COLLAPSE_TAG};
}

sub indent_level {
  my $self = shift;
  if (scalar @_) {
    $self->{_INDENT_LEVEL} = shift;
  }
  return $self->{_INDENT_LEVEL};
}

sub indent_increment {
  my $self = shift;
  if (scalar @_) {
    $self->{_INDENT_INCREMENT} = shift;
  }
  return $self->{_INDENT_INCREMENT};
}

sub attr_indent {
  my $self = shift;
  if (scalar @_) {
    $self->{_ATTR_INDENT} = shift;
  }
  return $self->{_ATTR_INDENT};
}

sub attrs_on_one_line {
  my $self = shift;
  if (scalar @_) {
    $self->{_ATTRS_ON_ONE_LINE} = shift;
  }
  return $self->{_ATTRS_ON_ONE_LINE};
}

sub tag_buffer {
  my $self = shift;
  if (scalar @_) {
    $self->{_TAG_BUFFER} = shift;
  }
  return $self->{_TAG_BUFFER};
}

sub incr_indent {
  my $self = shift;
  $self->indent_level($self->indent_level + $self->indent_increment);
}

sub decr_indent {
  my $self = shift;
  $self->indent_level($self->indent_level - $self->indent_increment);
}

sub write {
  my ($self) = @_;
  die __PACKAGE__."::write: must specify a file handle for output"
    unless defined $self->fh();

  # handle the basics
  $self->write_xml_decl();
  $self->write_doctype();

  ##
  ## Your code here
  ##
  $self->_write();
}

sub write_xml_decl {
  my $self = shift;
  my $fh = $self->fh();
  my $encoding = $self->encoding();
  print $fh <<"MAGEML";
<?xml version="1.0" encoding="$encoding" standalone="no"?>
MAGEML
}

sub write_doctype {
  my $self = shift;
  my $doctype = $self->doctype();
  my $DOCTYPE;
  if (defined $doctype) {
    $DOCTYPE = qq[DOCTYPE "$doctype"];
  } else {
    $DOCTYPE = '';
  }
  my $public_id = $self->public_id();
  my $PUBLIC;
  if (defined $public_id) {
    $PUBLIC = qq[PUBLIC "$public_id"];
  } else {
    $PUBLIC = '';
  }
  my $system_id = $self->system_id();
  my $SYSTEM;
  if (defined $public_id) {
    $SYSTEM = qq["$system_id"];
  } else {
    $SYSTEM = qq[SYSTEM "$system_id"];
  }
  my $fh = $self->fh();
  print $fh <<"XML";
<!DOCTYPE $DOCTYPE $PUBLIC $SYSTEM>
XML
}

sub write_start_tag {
  my ($self,$tag,$empty,%attrs) = @_;
  my $indent = ' ' x $self->indent_level();
  my $buffer;
  my (@attrs);
  foreach my $attribute_name (keys %attrs) {
    push(@attrs,qq[$attribute_name="$attrs{$attribute_name}"]);
  }
  my ($attrs,$attr_indent);
  if ($self->attrs_on_one_line()) {
    $attrs = join(' ',@attrs);
  } else {
    # we add one to compensate for the '<' in the start tag
    $attr_indent = $self->attr_indent() + 1;
    $attr_indent += length($tag);
    $attr_indent = ' ' x $attr_indent . $indent;
    $attrs = join("\n$attr_indent",@attrs);
  }
  if ($attrs) {
    $buffer .= "$indent<$tag $attrs";
  } else {
    # don't print the space after the tag because Eric said so
    $buffer .= "$indent<$tag";
  }
  if ($empty) {
    $buffer .= '/>';
  } else {
    $buffer .= '>';
  }
  $buffer .= "\n" unless $self->collapse_tag();
  $self->incr_indent()
    unless $empty;

  # print out the result
  my $fh = $self->fh();
  print $fh $buffer;
}

sub write_end_tag {
  my ($self,$tag) = @_;
  $self->decr_indent();

  my $indent = '';
  if (not $self->collapse_tag()) {
    $indent = ' ' x $self->indent_level();
  }
  my $fh = $self->fh();
  print $fh "$indent</$tag>";
  print $fh "\n"
    unless $self->collapse_tag();
}

# we purposefully avoid copying the text, since it may be BIG
sub write_text {
  my $self = shift;
  my $fh = $self->fh();
  print $fh $_[0];
}

1;
