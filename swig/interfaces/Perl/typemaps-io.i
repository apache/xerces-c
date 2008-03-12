// XMLByte arrays are just unisgned char*'s
// force loading of FromCharPtr fragment - needed for DOM
%typemap(out, noblock=1, fragment="SWIG_FromCharPtr") const XMLByte* getRawBuffer() {
  %set_output(SWIG_FromCharPtr((char*)$1));
}

%typemap(in) (const XMLByte* const srcDocBytes, XMLSize_t byteCount) {
  if (SvPOK($input)||SvIOK($input)||SvNOK($input)) {
    STRLEN len;
    XMLByte *xmlbytes = (XMLByte *)SvPV($input, len);
    $2 = len;
    $1 = new XMLByte[len];
    memcpy($1, xmlbytes, len);
  } else {
    SWIG_croak("Type error in argument 1 of $symname, Expected perl-string.");
  }
}

