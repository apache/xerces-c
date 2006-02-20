%{
static void
SWIG_Perl_SetErrorf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  sv_vsetpvfn(perl_get_sv("@", TRUE), fmt, strlen(fmt), &args, Null(SV**), 0, Null(bool*));
  va_end(args);
}

SWIGRUNTIMEINLINE void
SWIG_Perl_NullRef(const char *type, int argnum, const char *symname)
{
  if (type) {
    SWIG_Perl_SetErrorf("null reference of type: '%s' was received as argumnent: %d of %s",type,argnum,symname);
  } else {
    SWIG_Perl_SetErrorf("null reference was received");
  }
}

SWIGRUNTIMEINLINE void
SWIG_Perl_NullRef(const char *type, const char *argname, const char *symname)
{
  if (type) {
    SWIG_Perl_SetErrorf("null reference of type: '%s' was received as argumnent: %s of %s",type,argname,symname);
  } else {
    SWIG_Perl_SetErrorf("null reference was received");
  }
}

SWIGRUNTIME void
SWIG_Perl_TypeError(const char *type, int argnum, const char *symname)
{
  if (type) {
    SWIG_Perl_SetErrorf("Type error in argument %d of %s. Expected %s", argnum, symname, type);
  } else {
    SWIG_Perl_SetErrorf("Type error in argument $argnum of $symname", argnum, symname);
  }
}
%}