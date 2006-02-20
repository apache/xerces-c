/*
 * XML Schema Grammar support (DTD and W3C XML Schema)
 */

// for now these have no methods we need, and are only imported
%import "xercesc/framework/XMLContentModel.hpp"

// these are abstract classes for both DTD and Schema
// they have getter methods and must be included
%include "xercesc/framework/XMLElementDecl.hpp"
%include "xercesc/framework/XMLEntityDecl.hpp"
%include "xercesc/framework/XMLNotationDecl.hpp"
%include "xercesc/framework/XMLAttDefList.hpp"
%include "xercesc/framework/XMLAttDef.hpp"
%include "xercesc/framework/XMLValidator.hpp"
%include "xercesc/validators/common/Grammar.hpp"
%include "xercesc/framework/XMLGrammarDescription.hpp"
%include "xercesc/framework/XMLDTDDescription.hpp"
%include "xercesc/framework/XMLSchemaDescription.hpp"

// these are needed for both the DTD and Schema templates
%import "xercesc/util/XMLEnumerator.hpp"

/*
 * DTD Support
 */ 
// these are needed for the DTD templates
%import "xercesc/util/NameIdPool.hpp"

%include "xercesc/validators/DTD/DTDElementDecl.hpp"
namespace XERCES_CPP_NAMESPACE {
  %template()  XMLEnumerator<DTDElementDecl>;
  %template()  NameIdPool<DTDElementDecl>;
  %template(DTDElementDeclEnumerator)  NameIdPoolEnumerator<DTDElementDecl>;
}

%include "xercesc/validators/DTD/DTDEntityDecl.hpp"
namespace XERCES_CPP_NAMESPACE {
  %template()  XMLEnumerator<DTDEntityDecl>;
  %template()  NameIdPool<DTDEntityDecl>;
  %template(DTDEntityDeclEnumerator)  NameIdPoolEnumerator<DTDEntityDecl>;
}

%include "xercesc/validators/DTD/DTDAttDefList.hpp"
%include "xercesc/validators/DTD/DTDAttDef.hpp"
%include "xercesc/validators/DTD/DTDGrammar.hpp"
%include "xercesc/validators/DTD/DTDValidator.hpp"

/*
 * W3C XML Schema support
 */ 
// these are needed for the templates for schema
%import "xercesc/util/RefHash3KeysIdPool.hpp"

%include "xercesc/validators/schema/SchemaElementDecl.hpp"
namespace XERCES_CPP_NAMESPACE {
  %template()  XMLEnumerator<SchemaElementDecl>;
  %template()  RefHash3KeysTableBucketElem<SchemaElementDecl>;
  %template()  RefHash3KeysIdPool<SchemaElementDecl>;
  %template(SchemaElementDeclEnumerator)  RefHash3KeysIdPoolEnumerator<SchemaElementDecl>;
}

%include "xercesc/validators/schema/SchemaGrammar.hpp"
%include "xercesc/validators/schema/SchemaValidator.hpp"
%include "xercesc/validators/schema/SchemaAttDefList.hpp"
%include "xercesc/validators/schema/SchemaAttDef.hpp"

