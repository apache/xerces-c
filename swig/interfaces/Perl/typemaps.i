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

%typemap(in) (const XMLByte* const srcDocBytes, 
	      unsigned int byteCount) {
  if (SvPOK($input)||SvIOK($input)||SvNOK($input)) {
    STRLEN len;
    XMLByte *xmlbytes = (XMLByte *)SvPV($input, len);
    $2 = len;
    $1 = new XMLByte[len];
    memcpy($1, xmlbytes, len);
  } else {
    SWIG_croak("Type error in argument 2 of $symname, Expected perl-string.");
  }
}

// XMLByte arrays are just unisgned char*'s
%typemap(out) const XMLByte* getRawBuffer() {
  %set_output(SWIG_FromCharPtr((char*)result));
}

/*
 * Adopting a validator
 *
 *   we have to disown the validator, because Xerces will adopt it
 *   first we create a temp variable to store it's value in the
 *   'in' typemap, and the after all conversion has succeeded
 *   the 'check' typemap will disown it.
 */
%typemap(in) XERCES_CPP_NAMESPACE::XMLValidator* valToAdopt (SV *temp)
{
        temp = $input;
	if (SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor,0) < 0) {
            SWIG_Perl_TypeError("$1_mangle",$argnum,"$symname");
        }
}

%typemap(check) XERCES_CPP_NAMESPACE::XMLValidator* valToAdopt
{
   SWIG_Disown(temp$argnum);
}

// The typecheck functions are for use by SWIG's auto-overloading support
%typemap(typecheck, precedence=60)
SV*
{
  $1 = SvOK($input) ? 1 : 0;
}

// The typecheck functions are for use by SWIG's auto-overloading support
%typemap(typecheck, precedence=60)
SV*
{
  $1 = SvOK($input) ? 1 : 0;
}

// %typemap(typecheck, precedence=70)
// as long as the SV is not undef, convert it to a string
%typecheck(SWIG_TYPECHECK_UNISTRING)
XMLCh*, const XMLCh* 
{
  $1 = SvOK($input) ? 1 : 0; 
}