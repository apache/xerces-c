/*
 * methods not needed by the public Parser interfaces
 *
 */

// XMLEntityHandler interface
%ignore endInputSource;
%ignore expandSystemId;
%ignore resetEntities;
%ignore resolveEntity;
%ignore startInputSource;

// depricated methods - don't ask me to include these
%ignore attDef;
%ignore doctypeComment;
%ignore doctypeDecl;
%ignore doctypePI;
%ignore doctypeWhitespace;
%ignore elementDecl;
%ignore endAttList;
%ignore endIntSubset;
%ignore endExtSubset;
%ignore entityDecl;
%ignore resetDocType;
%ignore notationDecl;
%ignore startAttList;
%ignore startIntSubset;
%ignore startExtSubset;
%ignore TextDecl;

/* 
 * Ignore these char* methods for all Parsers
 */
%ignore loadGrammar(const char* const systemId,
                                 const short grammarType,
                                 const bool toCache = false);


%ignore parse(const char* const);
%ignore parseFirst(const char *const,XMLPScanToken&,const bool);

%ignore setExternalSchemaLocation(const char* const);
%ignore setExternalNoNamespaceSchemaLocation(const char* const);

// These are odd cases. We want access to skip the method with the
// memory manager argument, but we want access to the optional argument
// *after* the memory manager. So we just ignore the method that doesn't 
// include the memory manager - since our typemap defaults that argument
// they are redundant
%ignore createLSParser(short const,const XMLCh* const);
%ignore SAXParser(XMLValidator*   const);
%ignore XercesDOMParser(XMLValidator*   const);

// FIXME SWIG-1.3.30 broke this
// %ignore createXMLReader();

// exceptions needed by SAXParser and XercesDOMParser
%include "xercesc/sax/SAXException.hpp"
%include "xercesc/sax/SAXParseException.hpp"

// scan token helper class for progressive parsing
%include "xercesc/framework/XMLPScanToken.hpp"

// advanced document handler interface
%include "framework/XMLDocumentHandler.i"

// old entity resolver interface
%include "xercesc/sax/EntityResolver.hpp"

// new entity resolver interface
%include "util/XMLResourceIdentifier.i"
%include "util/XMLEntityResolver.i"

// PSVI
%include "xercesc/framework/psvi/PSVIItem.hpp"
%include "xercesc/framework/psvi/PSVIElement.hpp"
%include "xercesc/framework/psvi/PSVIAttribute.hpp"
%include "xercesc/framework/psvi/PSVIAttributeList.hpp"
%include "xercesc/framework/psvi/PSVIHandler.hpp"

