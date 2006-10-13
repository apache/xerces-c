/*
 * XML Schema Grammar support (DTD and W3C XML Schema)
 */

/*
 * methods not needed by the XMLValidator interfaces
 */
%ignore XERCES_CPP_NAMESPACE::XMLValidator::emitError;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::reinitMsgMutex;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::reinitMsgLoader;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::setErrorReporter;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::setScannerInfo;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::validateElement;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::validateAttrValue;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::reset;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::postParseValidation;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::preContentValidation;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::faultInAttr;
%ignore XERCES_CPP_NAMESPACE::XMLValidator::checkContent;

// ignore all the constructors for the Grammar components
%ignore XERCES_CPP_NAMESPACE::DTDAttDef::DTDAttDef;
%ignore XERCES_CPP_NAMESPACE::DTDElementDecl::DTDElementDecl;
%ignore XERCES_CPP_NAMESPACE::DTDEntityDecl::DTDEntityDecl;
%ignore XERCES_CPP_NAMESPACE::SchemaElementDecl::SchemaElementDecl;
%ignore XERCES_CPP_NAMESPACE::SchemaAttDef::SchemaAttDef;
%ignore	XERCES_CPP_NAMESPACE::DTDAttDefList::DTDAttDefList;
%ignore	XERCES_CPP_NAMESPACE::SchemaAttDefList::SchemaAttDefList;

// these methods are useless until we implement the XSerialize interface
%ignore XERCES_CPP_NAMESPACE::Grammar::loadGrammar;
%ignore XERCES_CPP_NAMESPACE::Grammar::storeGrammar;

// these methods are useless
%ignore XERCES_CPP_NAMESPACE::Grammar::findOrAddElemDecl;

// these methods I'm not sure about
%ignore XERCES_CPP_NAMESPACE::Grammar::getElemId;
%ignore XERCES_CPP_NAMESPACE::Grammar::getElemDecl;
%ignore XERCES_CPP_NAMESPACE::Grammar::getNotationDecl;
%ignore XERCES_CPP_NAMESPACE::Grammar::putElemDecl;
%ignore XERCES_CPP_NAMESPACE::Grammar::putNotationDecl;
%ignore XERCES_CPP_NAMESPACE::Grammar::reset;
// %ignore XERCES_CPP_NAMESPACE::Grammar::setGrammarDescription;
// %ignore XERCES_CPP_NAMESPACE::Grammar::getGrammarDescription;
%ignore XERCES_CPP_NAMESPACE::Grammar::setValidated;

%ignore XERCES_CPP_NAMESPACE::XMLGrammarDescription::getMemoryManager;

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

%import "xercesc/util/BaseRefVectorOf.hpp";
%import "xercesc/util/RefArrayVectorOf.hpp";
namespace XERCES_CPP_NAMESPACE {
  %template(BaseXMLChVector)  BaseRefVectorOf<XMLCh>;
  %template(XMLChVector)  RefArrayVectorOf<XMLCh>;
}
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

