# Different C++ compilers on the same architecture are (most likely)
# not binary compatible.  This poses difficulties in writing hints for
# MakeMaker.  Suggestions welcome.

use Config;

# if we are using gcc
if ($Config{gccversion}) {

  # gcc on solaris needs HAS_BOOL
  $self->{CCFLAGS} .= " -DHAS_BOOL";
} else {
  # Workshop 6 CC needs C++ runtime library linked
  $self->{LIBS}->[0] .= " -lCrun";
}

