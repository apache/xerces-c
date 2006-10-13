/*
 * Adopting a validator
 *
 *   we have to disown the validator, because Xerces will adopt it
 *   after all conversion has succeeded
 *   the SWIG_POINTER_DISOWN flag makes sure it is disowned.
 */
%typemap(in) XERCES_CPP_NAMESPACE::XMLValidator* valToAdopt
{
    if ($input != &PL_sv_undef) {
        int rc = SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, SWIG_POINTER_DISOWN |  0);
	if (!SWIG_IsOK(rc)) {
            %argument_fail(SWIG_TypeError, "$type", $symname, $argnum);
        }
    }
}

// 
// FOR Perl*Handler MEMBER FUNCTIONS, SO PERL SCALAR DOESN'T GET WRAPPED 
// 
%typemap(in) SV * {
  $1 = $input;
}

// For setProperty() - store the SV* directly
%typemap(in) void * {
  $1 = (void*) $input;
}

// The typecheck functions are for use by SWIG's auto-overloading support
// we must set a *really* low precedence on this because it will return
// true for *everything* 
%typemap(typecheck, precedence=60)
SV*
{
  $1 = 1;
}

