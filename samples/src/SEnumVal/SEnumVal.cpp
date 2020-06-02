/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id$
 */
// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/NameIdPool.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/framework/psvi/XSAnnotation.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/validators/schema/SchemaValidator.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <iostream>
#include <cstdlib>
#include <cstring>


using namespace XERCES_CPP_NAMESPACE;

// ---------------------------------------------------------------------------
//  Forward references
// ---------------------------------------------------------------------------
static void usage();

void process(char* const);
void processAttributes( XMLAttDefList& attList, bool margin = false );
void processDatatypeValidator( const DatatypeValidator*, bool margin = false
);
void processContentSpecNode( const ContentSpecNode* specNode, bool margin =
false );

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class StrX
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    StrX(const XMLCh* const toTranscode)
    {
        // Call the private transcoding method
        fLocalForm = XMLString::transcode(toTranscode);
    }

    ~StrX()
    {
        XMLString::release(&fLocalForm);
    }


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const char* localForm() const
    {
        return fLocalForm;
    }

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fLocalForm
    //      This is the local code page form of the string.
    // -----------------------------------------------------------------------
    char*   fLocalForm;
};

inline std::ostream& operator<<(std::ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}

// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
static void usage()
{
    std::cout << "\nUsage:\n"
            "    SEnumVal <XML file>\n\n"
            "This program parses a file, then shows how to enumerate the\n"
            "contents of the Schema Grammar. Essentially, shows how one can\n"
            "access the Schema information stored in internal data structures.\n"
         << std::endl;
}

// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
    // cannot return out of catch-blocks lest exception-destruction
    // result in calls to destroyed memory handler!
    int errorCode = 0;
    // Initialize the XML4C system
    try
    {
         XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
         std::cerr   << "Error during initialization! Message:\n"
                << StrX(toCatch.getMessage()) << std::endl;
         errorCode = 1;
    }
    if(errorCode) {
        XMLPlatformUtils::Terminate();
        return errorCode;
    } 

    // Check command line and extract arguments.
    // We only have one required parameter, which is the file to process
    if ((argC != 2) ||
        (*(argV[1]) == '-'))
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 2;
    }

    try
    {
		process(argV[1]);
    }
    catch (const OutOfMemoryException&)
    {
        std::cerr << "OutOfMemoryException" << std::endl;
        errorCode = 5;
    }
    catch (const XMLException& e)
    {
        std::cerr << "\nError during parsing: '" << argV[1] << "'\n"
             << "Exception message is:  \n"
             << StrX(e.getMessage()) << "\n" << std::endl;
        errorCode = 3;
    }

    XMLPlatformUtils::Terminate();

	return errorCode;
}

void process(char* const xmlFile)
{
    //
    //  Create a Schema validator to be used for our validation work. Then create
    //  a SAX parser object and pass it our validator. Then, according to what
    //  we were told on the command line, set it to validate or not. He owns
    //  the validator, so we have to allocate it.
    //
    SAXParser parser;
    parser.setValidationScheme(SAXParser::Val_Always);
    parser.setDoNamespaces(true);
    parser.setDoSchema(true);

	parser.parse(xmlFile);

    if (parser.getErrorCount())
	{
        std::cout << "\nErrors occurred, no output available\n" << std::endl;
		return;
	}

	if (!parser.getValidator().handlesSchema())
	{
		std::cout << "\n Non schema document, no output available\n" << std::endl;
		return;
	}

	Grammar* rootGrammar = parser.getRootGrammar();
	if (!rootGrammar || rootGrammar->getGrammarType() != Grammar::SchemaGrammarType)
	{
		std::cout << "\n Non schema grammar, no output available\n" << std::endl;
		return;
	}

	//
	//  Now we will get an enumerator for the element pool from the validator
	//  and enumerate the elements, printing them as we go. For each element
	//  we get an enumerator for its attributes and print them also.
	//

	SchemaGrammar* grammar = (SchemaGrammar*) rootGrammar;
	RefHash3KeysIdPoolEnumerator<SchemaElementDecl> elemEnum = grammar->getElemEnumerator();

	if (!elemEnum.hasMoreElements())
	{
		std::cout << "\nThe validator has no elements to display\n" << std::endl;
		return;
	}

	while(elemEnum.hasMoreElements())
	{
		const SchemaElementDecl& curElem = elemEnum.nextElement();

		// Name
		std::cout << "Name:\t\t\t" << StrX(curElem.getFullName()) << "\n";

		// Model Type
		std::cout << "Model Type:\t\t";
		switch( curElem.getModelType() )
		{
		case SchemaElementDecl::Empty:          std::cout << "Empty";         break;
		case SchemaElementDecl::Any:            std::cout << "Any";           break;
		case SchemaElementDecl::Mixed_Simple:   std::cout << "Mixed_Simple";  break;
		case SchemaElementDecl::Mixed_Complex:  std::cout << "Mixed_Complex"; break;
		case SchemaElementDecl::Children:       std::cout << "Children";      break;
		case SchemaElementDecl::Simple:         std::cout << "Simple";        break;
        case SchemaElementDecl::ElementOnlyEmpty:    std::cout << "ElementOnlyEmpty";    break;

		default:                                std::cout << "Unknown";       break;
		}

		std::cout << "\n";

		// Create Reason
		std::cout << "Create Reason:\t";
		switch( curElem.getCreateReason() )
		{
		case XMLElementDecl::NoReason:          std::cout << "Empty";            break;
		case XMLElementDecl::Declared:          std::cout << "Declared";         break;
		case XMLElementDecl::AttList:           std::cout << "AttList";          break;
		case XMLElementDecl::InContentModel:    std::cout << "InContentModel";   break;
		case XMLElementDecl::AsRootElem:        std::cout << "AsRootElem";       break;
		case XMLElementDecl::JustFaultIn:       std::cout << "JustFaultIn";      break;

		default:                            std::cout << "Unknown";  break;
		}

		std::cout << "\n";

		// Content Spec Node
		processContentSpecNode( curElem.getContentSpec() );

		// Misc Flags
		int mflags = curElem.getMiscFlags();
		if( mflags !=0 )
		{
			std::cout << "Misc. Flags:\t";
		}

        if ( mflags & SchemaSymbols::XSD_NILLABLE )
			std::cout << "Nillable ";

		if ( mflags & SchemaSymbols::XSD_ABSTRACT )
			std::cout << "Abstract ";

		if ( mflags & SchemaSymbols::XSD_FIXED )
			std::cout << "Fixed ";

		if( mflags !=0 )
		{
			std::cout << "\n";
		}

		// Substitution Name
		SchemaElementDecl* subsGroup = curElem.getSubstitutionGroupElem();
		if( subsGroup )
		{
			const XMLCh* uriText = parser.getURIText(subsGroup->getURI());
			std::cout << "Substitution Name:\t" << StrX(uriText)
			     << "," << StrX(subsGroup->getBaseName()) << "\n";
		}

		// Content Model
		const XMLCh* fmtCntModel = curElem.getFormattedContentModel();
		if( fmtCntModel != NULL )
		{
			std::cout << "Content Model:\t" << StrX(fmtCntModel) << "\n";
		}

		const ComplexTypeInfo* ctype = curElem.getComplexTypeInfo();
		if( ctype != NULL)
		{
			std::cout << "ComplexType:\n";
			std::cout << "\tTypeName:\t" << StrX(ctype->getTypeName()) << "\n";

			ContentSpecNode* cSpecNode = ctype->getContentSpec();
			processContentSpecNode(cSpecNode, true );
		}

		// Datatype
		DatatypeValidator* dtValidator = curElem.getDatatypeValidator();
		processDatatypeValidator( dtValidator );

		// Get an enumerator for this guy's attributes if any
		if ( curElem.hasAttDefs() )
		{
			processAttributes( curElem.getAttDefList() );
		}

		std::cout << "--------------------------------------------";
		std::cout << std::endl;

    }

    return;
}


//---------------------------------------------------------------------
//  Prints the Attribute's properties
//---------------------------------------------------------------------
void processAttributes( XMLAttDefList& attList, bool margin )
{
    if ( attList.isEmpty() )
    {
        return;
    }

    if ( margin )
    {
        std::cout << "\t";
    }

    std::cout << "Attributes:\n";
    for (unsigned int i=0; i<attList.getAttDefCount(); i++)
    {
        // Name
        SchemaAttDef& curAttDef = (SchemaAttDef&)attList.getAttDef(i);
        std::cout << "\tName:\t\t\t" << StrX(curAttDef.getFullName()) << "\n";

        // Type
        std::cout << "\tType:\t\t\t";
		std::cout << StrX(XMLAttDef::getAttTypeString(curAttDef.getType()));
        std::cout << "\n";

        // Default Type
        std::cout << "\tDefault Type:\t";
		std::cout << StrX(XMLAttDef::getDefAttTypeString(curAttDef.getDefaultType()));
        std::cout << "\n";

        // Value
        if( curAttDef.getValue() )
        {
            std::cout << "\tValue:\t\t\t";
            std::cout << StrX(curAttDef.getValue());
            std::cout << "\n";
        }

        // Enum. values
        if( curAttDef.getEnumeration() )
        {
            std::cout << "\tEnumeration:\t";
            std::cout << StrX(curAttDef.getEnumeration());
            std::cout << "\n";
        }

         const DatatypeValidator* dv = curAttDef.getDatatypeValidator();
         processDatatypeValidator( dv, true );

        std::cout << "\n";
    }
}

void processDatatypeValidator( const DatatypeValidator* dtValidator, bool margin )
{
    if( !dtValidator )
    {
        return;
    }

    if( margin )
    {
        std::cout << "\t";
    }

    std::cout << "Base Datatype:\t\t";
    switch( dtValidator->getType() )
    {
    case DatatypeValidator::String:         std::cout << "string";      break;
    case DatatypeValidator::AnyURI:         std::cout << "AnyURI";      break;
    case DatatypeValidator::QName:          std::cout << "QName";       break;
    case DatatypeValidator::Name:           std::cout << "Name";        break;
    case DatatypeValidator::NCName:         std::cout << "NCName";      break;
    case DatatypeValidator::Boolean:        std::cout << "Boolean";     break;
    case DatatypeValidator::Float:          std::cout << "Float";       break;
    case DatatypeValidator::Double:         std::cout << "Double";      break;
    case DatatypeValidator::Decimal:        std::cout << "Decimal";     break;
    case DatatypeValidator::HexBinary:      std::cout << "HexBinary";   break;
    case DatatypeValidator::Base64Binary:   std::cout << "Base64Binary";break;
    case DatatypeValidator::Duration:       std::cout << "Duration";    break;
    case DatatypeValidator::DateTime:       std::cout << "DateTime";    break;
    case DatatypeValidator::Date:           std::cout << "Date";        break;
    case DatatypeValidator::Time:           std::cout << "Time";        break;
    case DatatypeValidator::MonthDay:       std::cout << "MonthDay";    break;
    case DatatypeValidator::YearMonth:      std::cout << "YearMonth";   break;
    case DatatypeValidator::Year:           std::cout << "Year";        break;
    case DatatypeValidator::Month:          std::cout << "Month";       break;
    case DatatypeValidator::Day:            std::cout << "Day";         break;
    case DatatypeValidator::ID:             std::cout << "ID";          break;
    case DatatypeValidator::IDREF:          std::cout << "IDREF";       break;
    case DatatypeValidator::ENTITY:         std::cout << "ENTITY";      break;
    case DatatypeValidator::NOTATION:       std::cout << "NOTATION";    break;
    case DatatypeValidator::List:           std::cout << "List";        break;
    case DatatypeValidator::Union:          std::cout << "Union";       break;
    case DatatypeValidator::AnySimpleType:  std::cout << "AnySimpleType"; break;
    case DatatypeValidator::UnKnown:        std::cout << "UNKNOWN";     break;
    }

    std::cout << "\n";

    // Facets
	RefHashTableOf<KVStringPair>* facets = dtValidator->getFacets();
    if( facets && facets->getCount()>0)
    {
        XMLSize_t i;
        // Element's properties
        std::cout << "Facets:\t\t\n";
        // use a list to print them sorted, or the list could be different on 64-bit machines
        RefVectorOf<XMLCh> sortedList(facets->getCount(), false);
        RefHashTableOfEnumerator<KVStringPair> enumFacets(facets);
        while( enumFacets.hasMoreElements() )
        {
            const KVStringPair& curPair = enumFacets.nextElement();
            const XMLCh* key=curPair.getKey();
            XMLSize_t len=sortedList.size();
            for(i=0;i<len;i++)
                if(XMLString::compareString(key, sortedList.elementAt(i))<0)
                {
                    sortedList.insertElementAt((XMLCh*)key,i);
                    break;
                }
            if(i==len)
                sortedList.addElement((XMLCh*)key);
        }

        XMLSize_t len=sortedList.size();
        for(i=0;i<len;i++)
        {
            const XMLCh* key = sortedList.elementAt(i);
            std::cout << "\t" << StrX( key )    << "="
                         << StrX( facets->get(key)->getValue() )  << "\n";
        }
    }

	// Enumerations
	RefVectorOf<XMLCh>* enums = (RefVectorOf<XMLCh>*) dtValidator->getEnumString();
	if (enums)
	{
		std::cout << "Enumeration:\t\t\n";

		XMLSize_t enumLength = enums->size();
		for ( XMLSize_t i = 0; i < enumLength; i++)
        {
            std::cout << "\t" << StrX( enums->elementAt(i)) << "\n";
        }

	}
}

void processContentSpecNode( const ContentSpecNode* cSpecNode, bool margin )
{
    if( !cSpecNode )
    {
        return;
    }

    if( margin )
    {
        std::cout << "\t";
    }

    std::cout << "ContentType:\t";
    switch( cSpecNode->getType() )
    {
        case ContentSpecNode::Leaf:                std::cout << "Leaf";           break;
        case ContentSpecNode::ZeroOrOne:           std::cout << "ZeroOrOne";      break;
        case ContentSpecNode::ZeroOrMore:          std::cout << "ZeroOrMore";     break;
        case ContentSpecNode::OneOrMore:           std::cout << "OneOrMore";      break;
        case ContentSpecNode::ModelGroupChoice:    
        case ContentSpecNode::Choice:              std::cout << "Choice";         break;
        case ContentSpecNode::ModelGroupSequence:  
        case ContentSpecNode::Sequence:            std::cout << "Sequence";       break;
        case ContentSpecNode::All:                 std::cout << "All";            break;
        case ContentSpecNode::Loop:                std::cout << "Loop";           break;
        case ContentSpecNode::Any:                 std::cout << "Any";            break;
        case ContentSpecNode::Any_Other:           std::cout << "Any_Other";      break;
        case ContentSpecNode::Any_NS:              std::cout << "Any_NS";         break;
        case ContentSpecNode::Any_Lax:             std::cout << "Any_Lax";        break;
        case ContentSpecNode::Any_Other_Lax:       std::cout << "Any_Other_Lax";  break;
        case ContentSpecNode::Any_NS_Lax:          std::cout << "Any_NS_Lax";     break;
        case ContentSpecNode::Any_Skip:            std::cout << "Any_Skip";       break;
        case ContentSpecNode::Any_Other_Skip:      std::cout << "Any_Other_Skip"; break;
        case ContentSpecNode::Any_NS_Skip:         std::cout << "Any_NS_Skip";    break;
        case ContentSpecNode::Any_NS_Choice:       std::cout << "Any_NS_Choice";  break;
        case ContentSpecNode::UnknownType:         std::cout << "UnknownType";    break;
    }
    std::cout << "\n";
}

