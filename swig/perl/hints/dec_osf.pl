use Config;

if ($Config{gccversion}) {
  # if we are using gcc, we need to use bool
  $self->{CCFLAGS} .= " -DHAS_BOOL";
} else {
  # if we're not using gcc, we have to set up the C++ compiler
  $self->{OPTIMIZE} =~ /-g/ and $debug=1;
  $self->{CC}= "cxx -v";
  $self->{LD}= "cxx -v";   # add cxx release dependent objects

  my $code = <<CODE;
package MY;

sub const_config {
	my \$out=shift->SUPER::const_config;
	\$out=~s/^(LDDLFLAGS)(.*)-s/\$1\$2/m if $debug;  # remove -s for debugging
	\$out;
}

sub cflags {

	my \$out=shift->SUPER::cflags(\@_);
	#
	# DEC cxx5.5 doesn't know the -std flags, which we possibly used
	# to compile perl with cc. cxx 5.6 does.
	#
	\$out=~s/-std//;             # cxx5.5-004 doesnt want this.
	\$out;
}
CODE
  eval "$code";
}
