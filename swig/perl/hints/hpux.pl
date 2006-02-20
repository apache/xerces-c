# Different C++ compilers on the same architecture are (most likely)
# not binary compatible.  This poses difficulties in writing hints for
# MakeMaker.  Suggestions welcome.

use Config;

# if we are using gcc
if ($Config{gccversion}) {
  # do nothing
} else {
  # aCC on hp-ux needs HAS_BOOL
  $self->{CCFLAGS} .= " -DHAS_BOOL";
  # we add -lcl to get thread local storage
  # see: http://forums.itrc.hp.com/cm/QuestionAnswer/1,,0x0d0a6d96588ad4118fef0090279cd0f9,00.html
  $self->{LIBS}->[0] .= ' -lcl';

}
