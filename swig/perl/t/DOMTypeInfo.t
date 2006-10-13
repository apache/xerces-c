# Before `make install' is performed this script should be runnable
# with `make test'. After `make install' it should work as `perl
# Grammar.t'

######################### Begin module loading

# use blib;
use Test::More tests => 1000;
BEGIN { use_ok(XML::Xerces::DOM) };

use lib 't';
use TestUtils qw($TYPEINFO_NAME
		$TYPEINFO_NO_DTD_NAME
		$TYPEINFO_COMBINED_NAME
		$TYPEINFO_JUST_DTD_NAME);

use vars qw($file $test_elem $test_attr $parser $doc);
use strict;

######################### Begin Test



$file = $TYPEINFO_NAME;
eval {
  $parser = XML::Xerces::XercesDOMParser->new();
  $parser->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Auto);
  $parser->setDoNamespaces(1);
  $parser->setCreateSchemaInfo(1);
  $parser->setDoSchema(1);

  # my $error_handler = XML::Xerces::PerlErrorHandler->new();
  # $parser->setErrorHandler($error_handler);

  $parser->parse($file);
};
XML::Xerces::error($@) if $@;

$doc = $parser->getDocument();
if (ok(defined $doc, "$file - doc")) {
  testInBuiltTypesOnAttributes($doc, my $has_dtd = 1);
  testInBuiltTypesOnElements($doc);
  testSimpleDerived($doc);
  testComplexTypes($doc);
  testUnions($doc);
  testAnonymous($doc);
  testXsiTypes($doc);
  testAnys($doc);
  testInvaild($doc);
}

$file = $TYPEINFO_NO_DTD_NAME;
eval {
  $parser = XML::Xerces::XercesDOMParser->new();
  $parser->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Auto);
  $parser->setDoNamespaces(1);
  $parser->setCreateSchemaInfo(1);
  $parser->setDoSchema(1);
  $parser->useScanner("SGXMLScanner");
  $parser->parse($file);
};
XML::Xerces::error($@) if $@;

$doc = $parser->getDocument();
if (ok(defined $doc, "$file - doc")) {
  testInBuiltTypesOnAttributes($doc, my $has_dtd = 0);
  testInBuiltTypesOnElements($doc);
  testSimpleDerived($doc);
  testComplexTypes($doc);
  testUnions($doc);
  testAnonymous($doc);
  testXsiTypes($doc);
  testAnys($doc);
  testInvaild($doc);
}

$file = $TYPEINFO_JUST_DTD_NAME;
eval {
  $parser = XML::Xerces::XercesDOMParser->new();
  $parser->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Auto);
  $parser->setCreateSchemaInfo(1);
  $parser->parse($file);
};
XML::Xerces::error($@) if $@;

$doc = $parser->getDocument();
if (ok(defined $doc, "$file - doc")) {
  testDTD($doc);
}

eval {
  $parser = XML::Xerces::XercesDOMParser->new();
  $parser->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Auto);
  $parser->useScanner("DGXMLScanner");
  $parser->setCreateSchemaInfo(1);
  $parser->parse($file);
};
XML::Xerces::error($@) if $@;

$doc = $parser->getDocument();
if (ok(defined $doc, "$file - doc")) {
  testDTD($doc);
}

$file = $TYPEINFO_COMBINED_NAME;
eval {
  $parser = XML::Xerces::XercesDOMParser->new();
  $parser->setValidationScheme($XML::Xerces::AbstractDOMParser::Val_Auto);
  $parser->setDoNamespaces(1);
  $parser->setCreateSchemaInfo(1);
  $parser->setDoSchema(1);
  $parser->parse($file);
};
XML::Xerces::error($@) if $@;

$doc = $parser->getDocument();
if (ok(defined $doc, "$file - doc")) {
  combinedTest($doc);
}

print STDERR "Finished\n";

sub testInBuiltTypesOnAttributes {
  my $doc = shift;
  my $has_dtd = shift;

  $test_elem = find_element($doc,'attrTest');
  isa_ok($test_elem, "XML::Xerces::DOMElement",
	 'found <attrTest>')
    or die();

  no warnings qw(uninitialized);

  DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "attrTestType", "", "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "anySimpleType");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_ANYSIMPLETYPE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "string");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_STRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "boolean");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_BOOLEAN, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "decimal");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DECIMAL, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "float");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_FLOAT, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "double");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DOUBLE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "duration");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DURATION, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "dateTime");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DATETIME, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "time");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_TIME, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "date");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DATE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "gYearMonth");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_YEARMONTH, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "gYear");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_YEAR, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "gMonthDay");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_MONTHDAY, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "gDay");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DAY, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "gMonth");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_MONTH, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "hexBinary");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_HEXBINARY, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "base64Binary");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_BASE64BINARY, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "anyURI");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_ANYURI, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "QName");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_QNAME, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "normalizedString");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NORMALIZEDSTRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "token");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_TOKEN, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "language");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_LANGUAGE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "NMTOKEN");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::XMLUni::fgNmTokenString, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "NMTOKENS");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::XMLUni::fgNmTokensString, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "Name");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NAME, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "NCName");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NCNAME, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "ID");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::XMLUni::fgIDString, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "IDREF");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::XMLUni::fgIDRefString, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "IDREFS");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::XMLUni::fgIDRefsString, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);


  if ($has_dtd) {
    $test_attr = $test_elem->getAttributeNodeNS(undef, "ENTITY");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::XMLUni::fgEntityString, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS(undef, "ENTITIES");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::XMLUni::fgEntitiesString, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);
  }

  $test_attr = $test_elem->getAttributeNodeNS(undef, "integer");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_INTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "nonPositiveInteger");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NONPOSITIVEINTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "negativeInteger");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NEGATIVEINTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "long");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_LONG, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "int");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_INT, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "short");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_SHORT, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "byte");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_BYTE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "nonNegativeInteger");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NONNEGATIVEINTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "unsignedLong");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_ULONG, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "unsignedInt");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_UINT, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "unsignedShort");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_USHORT, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "unsignedByte");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_UBYTE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "positiveInteger");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_POSITIVEINTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  # couple of defaulted ones
    $test_attr = $test_elem->getAttributeNodeNS(undef, "defaultString");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_STRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS(undef, "defaultInt");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_INTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  # ns attr
    $test_attr = $test_elem->getAttributeNodeNS("http://www.w3.org/2000/xmlns/", "prefix");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_ANYURI, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS("http://www.w3.org/2001/XMLSchema-instance", "noNamespaceSchemaLocation");
  DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_ANYURI, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

}

sub testDTD {
  my $doc = shift;

  no warnings qw(uninitialized);


    $test_elem = $doc->getDocumentElement();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);

  $test_attr = $test_elem->getAttributeNodeNS("", "cdata");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "CDATA", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "enum");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "ENUMERATION", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "id");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "ID", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "idRef");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "IDREF", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "idRefs");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "IDREFS", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "nmToken");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "NMTOKEN", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "nmTokenDefault");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "NMTOKEN", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "nmTokenDefault2");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "NMTOKEN", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "nmTokens");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "NMTOKENS", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "entity");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "ENTITY", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "entities");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "ENTITIES", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "notation");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "NOTATION", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "noDecl");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "CDATA", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNode("xmlns:foo");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "CDATA", "http://www.w3.org/TR/REC-xml", "$file, line: " . __LINE__);

}


sub testInBuiltTypesOnElements {
  my $doc = shift;

  no warnings qw(uninitialized);

  my $elem_name = "eleTest";
  my $test_elem = find_element($doc, $elem_name);
  isa_ok($test_elem, "XML::Xerces::DOMElement",
	 "found <$elem_name>")
    or die();

    $test_elem = $test_elem->getFirstChild()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_STRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);
    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_BOOLEAN, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DECIMAL, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_FLOAT, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DOUBLE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DURATION, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DATETIME, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_TIME, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DATE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_YEARMONTH, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_YEAR, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_MONTHDAY, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_DAY, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_MONTH, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_HEXBINARY, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_BASE64BINARY, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_ANYURI, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_QNAME, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NORMALIZEDSTRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_TOKEN, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_LANGUAGE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::XMLUni::fgNmTokenString, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::XMLUni::fgNmTokensString, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NAME, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NCNAME, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_INTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NONPOSITIVEINTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NEGATIVEINTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_LONG, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_INT, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_SHORT, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_BYTE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_NONNEGATIVEINTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_ULONG, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_UINT, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_USHORT, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_UBYTE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_POSITIVEINTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgATTVAL_ANYTYPE, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

}

sub testSimpleDerived {
  my $doc = shift;

  no warnings qw(uninitialized);

my $root = $doc->getDocumentElement();
    # element simpleDerTest
    $test_elem = $root->getFirstChild()->getNextSibling()->getNextSibling()
        ->getNextSibling()->getNextSibling()->getNextSibling();

  $test_attr = $test_elem->getAttributeNodeNS(undef, "decimalDerived");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "decimalDerivedType", "",  "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS(undef, "stringDerived");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "stringDerivedType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getFirstChild()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "decimalDerivedType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "stringDerivedType", "", "$file, line: " . __LINE__);

}

sub testComplexTypes {
  my $doc = shift;

  no warnings qw(uninitialized);


  my $elem_name = "complexTest";
  my $test_elem = find_element($doc, $elem_name);
  isa_ok($test_elem, "XML::Xerces::DOMElement",
	 "found <$elem_name>")
    or die();

    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "complexTestType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "complexDerTestType", "", "$file, line: " . __LINE__);
}


sub testUnions {
  my $doc = shift;

  no warnings qw(uninitialized);


  my $elem_name = "unionTest";
  my $test_elem = find_element($doc, $elem_name);
  isa_ok($test_elem, "XML::Xerces::DOMElement",
	 "found <$elem_name>")
    or die();

    $test_elem = $test_elem->getFirstChild()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "decimalDerivedType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "stringDerivedType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "decimal", $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "string", $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    $test_attr = $test_elem->getAttributeNodeNS(undef, "testAttr");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "decimalDerivedType", "", "$file, line: " . __LINE__);
    $test_attr = $test_elem->getAttributeNodeNS(undef, "testAttr2");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "stringDerivedType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    $test_attr = $test_elem->getAttributeNodeNS(undef, "testAttr");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "stringDerivedType", "", "$file, line: " . __LINE__);
    $test_attr = $test_elem->getAttributeNodeNS(undef, "testAttr2");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "decimalDerivedType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    $test_attr = $test_elem->getAttributeNodeNS(undef, "testAttr");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "decimal", $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);
    $test_attr = $test_elem->getAttributeNodeNS(undef, "testAttr2");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "string", $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    $test_attr = $test_elem->getAttributeNodeNS(undef, "testAttr");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "string", $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);
    $test_attr = $test_elem->getAttributeNodeNS(undef, "testAttr2");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "decimal", $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

}

sub testAnonymous {
  my $doc = shift;

  no warnings qw(uninitialized);


  my $elem_name = "anonymousTest";
  my $test_elem = find_element($doc, $elem_name);
  isa_ok($test_elem, "XML::Xerces::DOMElement",
	 "found <$elem_name>")
    or die();


    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "__AnonC1", "", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "partNum");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "__AnonS7", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getFirstChild()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "__AnonS2", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "__AnonS4", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "__AnonS6", "", "$file, line: " . __LINE__);

}


sub testXsiTypes {
  my $doc = shift;

  no warnings qw(uninitialized);


  my $elem_name = "xsiTypeTest";
  my $test_elem = find_element($doc, $elem_name);
  isa_ok($test_elem, "XML::Xerces::DOMElement",
	 "found <$elem_name>")
    or die();

    $test_elem = $test_elem->getFirstChild()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "base", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "level1", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "level2", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "baseComplex", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "level1Complex", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "level2Complex", "", "$file, line: " . __LINE__);

}



sub testAnys {
  my $doc = shift;

  no warnings qw(uninitialized);


  my $elem_name = "anyTestPartial";
  my $test_elem = find_element($doc, $elem_name);
  isa_ok($test_elem, "XML::Xerces::DOMElement",
	 "found <$elem_name>")
    or die();

  my $back = $test_elem;

    $test_attr = $test_elem->getAttributeNodeNS("http://www.w3.org/1999/xhtml", "attr2");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);


    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "anyTestPartialType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getFirstChild()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_STRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);


    # element anyTest
    $test_elem = $back->getNextSibling()->getNextSibling();
    $back = $test_elem;

    $test_attr = $test_elem->getAttributeNodeNS("http://www.secondSchema", "attr1");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_STRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("http://www.secondSchema", "attr2");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_INTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("http://www.secondSchema", "attr3");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_STRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("http://www.secondSchema", "attr4");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_INTEGER, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "anyTestType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getFirstChild()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_STRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_STRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);

  # anyTestAttr1
    $test_elem = $back->getNextSibling()->getNextSibling();
    $back = $test_elem;

    $test_attr = $test_elem->getAttributeNodeNS("http://www.secondSchema", "attr5");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);

  # anyTestAttr2
    $test_elem = $back->getNextSibling()->getNextSibling();
    $back = $test_elem;

    $test_attr = $test_elem->getAttributeNodeNS("http://www.secondSchema", "attr5");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);

  # anyTestMissing
    $test_elem = $back->getNextSibling()->getNextSibling();
    $back = $test_elem;
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "anyTestType", "", "$file, line: " . __LINE__);
}



sub testInvaild {
  my $doc = shift;

  no warnings qw(uninitialized);

  my $root = $doc->getDocumentElement();
  DOMTypeInfoTest($root->getSchemaTypeInfo(), "rootType", "", "$file, line: " . __LINE__);

  my $elem_name = "invalidTest";
  my $test_elem = find_element($doc, $elem_name);
  isa_ok($test_elem, "XML::Xerces::DOMElement",
	 "found <$elem_name>")
    or die();

    $test_attr = $test_elem->getAttributeNodeNS("", "simple");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), "stringDerivedType", "", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "invalid");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getFirstChild()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "stringDerivedType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "baseComplex", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    # this is a "number" of type "base" but it has a xsi:type="stringDerivedType"
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "stringDerivedType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "baseComplex", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "uType", "", "$file, line: " . __LINE__);
    # an undeclared element does not have anon value. Test this here
    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);

  my $test_elem2 = $test_elem->getFirstChild()->getNextSibling();
    DOMTypeInfoTest($test_elem2->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);
    
    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "attrOnlyType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "attrOnlyType", "", "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "prohibitedAttrType", "", "$file, line: " . __LINE__);

}

sub combinedTest {
  my $doc = shift;

  no warnings qw(uninitialized);


    $test_elem = $doc->getDocumentElement();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), "rootType", "", "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "attBoth");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_STRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "attSchema");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_STRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_attr = $test_elem->getAttributeNodeNS("", "attDTD");
    DOMTypeInfoTest($test_attr->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getFirstChild()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), $XML::Xerces::SchemaSymbols::fgDT_STRING, $XML::Xerces::SchemaSymbols::fgURI_SCHEMAFORSCHEMA, "$file, line: " . __LINE__);

    $test_elem = $test_elem->getNextSibling()->getNextSibling();
    DOMTypeInfoTest($test_elem->getSchemaTypeInfo(), undef, undef, "$file, line: " . __LINE__);

}

sub DOMTypeInfoTest {
  my ($type_info, $type, $uri, $test_name) = @_;
  isa_ok($type_info, "XML::Xerces::DOMTypeInfo", $test_name);
  is($type_info->getTypeName(), $type,
     "$test_name - TypeName");
  is($type_info->getTypeNamespace(), $uri,
     "$test_name - TypeNamespace");
}

sub find_element {
  my $doc = shift;
  my $name = shift;

  my $to_test = $doc->getDocumentElement()->getFirstChild();
  while($to_test->getNodeName() ne $name) {
    $to_test = $to_test->getNextSibling();
  }
  return $to_test;
}


