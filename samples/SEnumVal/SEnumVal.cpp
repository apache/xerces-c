// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/NameIdPool.hpp>
#include <util/PlatformUtils.hpp>
#include <framework/XMLValidator.hpp>
#include <parsers/SAXParser.hpp>
#include <validators/schema/SchemaValidator.hpp>
#include <validators/common/ContentSpecNode.hpp>
#include <validators/schema/SchemaSymbols.hpp>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>


// ---------------------------------------------------------------------------
//  Forward references
// ---------------------------------------------------------------------------
static void usage();

void processAttributes( XMLAttDefList& attList, bool margin = false );
void processDatatypeValidator( const DatatypeValidator*, bool margin = false );
void processContentSpecNode( const ContentSpecNode* specNode, bool margin = false );

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
        delete [] fLocalForm;
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

inline ostream& operator<<(ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}


// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
static void usage()
{
    cout << "\nUsage:\n"
            "    EnumVal <XML file>\n\n"
            "This program parses a file, then shows how to enumerate the\n"
            "contents of the validator pools. Essentially, shows how one can\n"
            "access the Schema information stored in internal data structures.\n\n"
            "  * = Default if not provided explicitly\n"
         << endl;
}


// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
    // Initialize the XML4C system
    try
    {
         XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
         cerr   << "Error during initialization! Message:\n"
                << StrX(toCatch.getMessage()) << endl;
         XMLPlatformUtils::Terminate();
         return 1;
    }

    // Check command line and extract arguments.
    if (argC < 2)
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }

    // We only have one required parameter, which is the file to process
    if ((argC == 2) && (*(argV[1]) == '-'))
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 2;
    }

    const char*              xmlFile   = argV[1];
    SAXParser::ValSchemes    valScheme = SAXParser::Val_Never;

    //
    //  Create a Schema validator to be used for our validation work. Then create
    //  a SAX parser object and pass it our validator. Then, according to what
    //  we were told on the command line, set it to validate or not. He owns
    //  the validator, so we have to allocate it.
    //
    //SchemaValidator* valToUse = new SchemaValidator;
    SAXParser parser;//(valToUse);
    parser.setValidationScheme(valScheme);
    parser.setDoNamespaces(true);
    parser.setDoSchema(true);


    //
    //  Get the starting time and kick off the parse of the indicated
    //  file. Catch any exceptions that might propogate out of it.
    //
    try
    {
        parser.parse(xmlFile);
    }

    catch (const XMLException& e)
    {
        cerr << "\nError during parsing: '" << xmlFile << "'\n"
             << "Exception message is:  \n"
             << StrX(e.getMessage()) << "\n" << endl;
        XMLPlatformUtils::Terminate();
        return 3;
    }

    //
    //  Now we will get an enumerator for the element pool from the validator
    //  and enumerate the elements, printing them as we go. For each element
    //  we get an enumerator for its attributes and print them also.
    //
	//SchemaGrammar* grammar = (SchemaGrammar*) valToUse->getGrammar();
    SchemaGrammar* grammar = (SchemaGrammar*) parser.getValidator().getGrammar();
    RefHash3KeysIdPoolEnumerator<SchemaElementDecl> elemEnum = grammar->getElemEnumerator();
    if (elemEnum.hasMoreElements())
    {
        while(elemEnum.hasMoreElements())
        {
            // Element's properties
            const SchemaElementDecl& curElem = elemEnum.nextElement();
            
            // Name
            cout << "Name:\t\t\t" << StrX(curElem.getFullName()) << "\n";

            // Model Type
            cout << "Model Type:\t\t";
            switch( curElem.getModelType() )
            {
            case SchemaElementDecl::Empty:      cout << "Empty";    break;
            case SchemaElementDecl::Any:        cout << "Any";      break;
            case SchemaElementDecl::Mixed:      cout << "Mixed";    break;
            case SchemaElementDecl::Children:   cout << "Children"; break;
            case SchemaElementDecl::Simple:     cout << "Simple";   break;
            default:                            cout << "Unknown";  break;
            }
            cout << "\n";

            // Create Reason
            cout << "Create Reason:\t";
            switch( curElem.getCreateReason() )
            {
                case XMLElementDecl::NoReason:          cout << "Empty";            break;
                case XMLElementDecl::Declared:          cout << "Declared";         break;
                case XMLElementDecl::AttList:           cout << "AttList";          break;
                case XMLElementDecl::InContentModel:    cout << "InContentModel";   break;
                case XMLElementDecl::AsRootElem:        cout << "AsRootElem";       break;
                case XMLElementDecl::JustFaultIn:       cout << "JustFaultIn";      break;

            }
            cout << "\n";
            

            // Content Spec Node
            processContentSpecNode( curElem.getContentSpec() );

            // Misc Flags
            int mflags = curElem.getMiscFlags();
            if( mflags !=0 )
            {
                cout << "Misc. Flags:\t";
            }

            if(       mflags == SchemaSymbols::INFINITY  )
            {
                cout << "Infinity ";
            }
            else 
            {
                if ( mflags & SchemaSymbols::NILLABLE  != 0 )
                    cout << "Nillable ";

                if ( mflags & SchemaSymbols::ABSTRACT  != 0 )
                    cout << "Abstract ";

                if ( mflags & SchemaSymbols::FIXED     != 0 )
                    cout << "Fixed ";
            }

            if( mflags !=0 )
            {
                cout << "\n";
            }

            // Substitution Name
            XMLCh* subsGroup = curElem.getSubstitutionGroupName();
            if( subsGroup )
            {
                cout << "Substitution Name:\t" << StrX(subsGroup) << "\n";
            }


            // Block Set
            /*
            int blockSet = curElem.getBlockSet();
            cout << "Block. Set:\t\t";

            if( blockSet == SchemaSymbols::EMPTY_SET  )
                cout << "Empty Set ";
            else
            {
                if( blockSet & SchemaSymbols::SUBSTITUTION  )
                    cout << "Substitution ";
                if ( blockSet & SchemaSymbols::EXTENSION  != 0 )
                    cout << "Extension ";
                if ( blockSet & SchemaSymbols::RESTRICTION     != 0 )
                    cout << "Restriction ";
                if ( blockSet & SchemaSymbols::LIST  != 0 )
                    cout << "List ";
                if ( blockSet & SchemaSymbols::UNION     != 0 )
                    cout << "Union ";
                if ( blockSet & SchemaSymbols::ENUMERATION     != 0 )
                    cout << "Enumeration ";
            }
            cout << "\n";
            */


            // Content Model
            const XMLCh* fmtCntModel = curElem.getFormattedContentModel();
            if( fmtCntModel != NULL )
            {
                cout << "Content Model:\t" << StrX(fmtCntModel) << "\n";
            }

            const ComplexTypeInfo* ctype = curElem.getComplexTypeInfo();
            if( ctype != NULL)
            {
                cout << "ComplexType:\n";
                cout << "\tTypeName:\t" << StrX(ctype->getTypeName()) << "\n";
                
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

            cout << "--------------------------------------------";
            cout << endl;

        }
    }
     else
    {
        cout << "The validator has no elements to display\n" << endl;
    }
    // And call the termination method
    XMLPlatformUtils::Terminate();

    return 0;
}


//---------------------------------------------------------------------
//  Prints the Attribute's properties
//---------------------------------------------------------------------
void processAttributes( XMLAttDefList& attList, bool margin )
{
    if( attList.isEmpty() )
    {
        return;
    }

    if( margin )
    {
        cout << "\t";
    }

    cout << "Attributes:\n";
    while( attList.hasMoreElements() )
    {
        // Name
        SchemaAttDef& curAttDef = (SchemaAttDef&)attList.nextElement();
        cout << "\tName:\t\t\t" << StrX(curAttDef.getFullName()) << "\n";
        
        // Type
        cout << "\tType:\t\t\t";
        const XMLAttDef::AttTypes type = curAttDef.getType();
        switch(type)
        {
        case XMLAttDef::CData:      cout << "CDATA";        break;
        case XMLAttDef::ID:         cout << "ID";           break;
        
        case XMLAttDef::IDRef:
        case XMLAttDef::IDRefs:     cout << "IDREF(S)";     break;
        
        case XMLAttDef::Entity:
        case XMLAttDef::Entities:   cout << "ENTITY(IES)";  break;

        case XMLAttDef::NmToken:
        case XMLAttDef::NmTokens:   cout << "NMTOKEN(S)";   break;

        case XMLAttDef::Notation:   cout << "NOTATION";     break;
        case XMLAttDef::Enumeration:cout << "ENUMERATION";  break;

        case XMLAttDef::Simple:     cout << "Simple";       break;

        default:                    cout << "(" << type << ")";
        }
        cout << "\n";

        // Default Type
        cout << "\tDefault Type:\t";
        XMLAttDef::DefAttTypes defType = curAttDef.getDefaultType();
        switch( defType )
        {
        case XMLAttDef::Default:                cout << "Default";      break;
        case XMLAttDef::Required:               cout << "Required";     break;
        case XMLAttDef::Implied:                cout << "Implied";      break;
        case XMLAttDef::Fixed:                  cout << "Fixed";        break;
        case XMLAttDef::Prohibited:             cout << "Prohibited";   break;
        case XMLAttDef::Required_And_Fixed:     cout << "Required_And_Fixed";       break;
        case XMLAttDef::ProcessContents_Strict: cout << "ProcessContents_Strict";   break;
        case XMLAttDef::ProcessContents_Lax:    cout << "ProcessContents_Lax";      break;
        case XMLAttDef::ProcessContents_Skip:   cout << "ProcessContents_Skip";     break;
        default:
            cout << "(" << defType << ")";
        }
        cout << "\n";


        // Value
        if( curAttDef.getValue() )
        {
            cout << "\tValue:\t\t\t";
            cout << StrX(curAttDef.getValue());
            cout << "\n";
        }

        // Enum. values
        if( curAttDef.getEnumeration() )
        {
            cout << "\tEnumeration:\t";
            cout << StrX(curAttDef.getEnumeration());
            cout << "\n";
        }

         const DatatypeValidator* dv = curAttDef.getDatatypeValidator();
         processDatatypeValidator( dv, true );

        cout << "\n";
    }   
}

void processDatatypeValidator( const DatatypeValidator* dtValidator, bool margin )
{
    if( dtValidator == NULL ) 
    {
        return;
    }

    if( margin )
    {
        cout << "\t";
    }

    cout << "Base Datatype:\t\t";
    switch( dtValidator->getType() )
    {
    case DatatypeValidator::String:         cout << "string";      break;
    case DatatypeValidator::AnyURI:         cout << "AnyURI";      break;
    case DatatypeValidator::QName:          cout << "QName";       break;
    case DatatypeValidator::Boolean:        cout << "Boolean";     break;
    case DatatypeValidator::Float:          cout << "Float";       break;
    case DatatypeValidator::Double:         cout << "Double";      break;
    case DatatypeValidator::Decimal:        cout << "Decimal";     break;
    case DatatypeValidator::HexBinary:      cout << "HexBinary";   break;
    case DatatypeValidator::Base64Binary:   cout << "Base64Binary";break;
    case DatatypeValidator::Duration:       cout << "Duration";    break;
    case DatatypeValidator::DateTime:       cout << "DateTime";    break;
    case DatatypeValidator::Date:           cout << "Date";        break;
    case DatatypeValidator::Time:           cout << "Time";        break;
    case DatatypeValidator::MonthDay:       cout << "MonthDay";    break;
    case DatatypeValidator::YearMonth:      cout << "YearMonth";   break;
    case DatatypeValidator::Year:           cout << "Year";        break;
    case DatatypeValidator::Month:          cout << "Month";       break;
    case DatatypeValidator::Day:            cout << "Day";         break;
    case DatatypeValidator::ID:             cout << "ID";          break;
    case DatatypeValidator::IDREF:          cout << "IDREF";       break;
    case DatatypeValidator::ENTITY:         cout << "ENTITY";      break;
    case DatatypeValidator::NOTATION:       cout << "NOTATION";    break;
    case DatatypeValidator::List:           cout << "List";        break;
    case DatatypeValidator::Union:          cout << "Union";       break;
    }
    
    cout << "\n";

    // Facets
	RefHashTableOf<KVStringPair>* facets = dtValidator->getFacets();
    if( facets != NULL )
    {
        RefHashTableOfEnumerator<KVStringPair> enumFacets(facets);
        if( enumFacets.hasMoreElements() )
        {
            cout << "Facets:\t\t\n";
        }


        while(enumFacets.hasMoreElements())
        {
            // Element's properties
            const KVStringPair& curPair = enumFacets.nextElement();
            cout << "\t" << StrX( curPair.getKey() )    << "=" 
                         << StrX( curPair.getValue() )  << "\n";
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
        cout << "\t";
    }

    cout << "ContentType:\t";
    switch( cSpecNode->getType() )
    {
        case ContentSpecNode::Leaf:             cout << "Leaf"; break;
        case ContentSpecNode::ZeroOrOne:        cout << "ZeroOrOne"; break;
        case ContentSpecNode::ZeroOrMore:       cout << "ZeroOrMore"; break;
        case ContentSpecNode::OneOrMore:        cout << "OneOrMore"; break;
        case ContentSpecNode::Choice:           cout << "Choice"; break;
        case ContentSpecNode::Sequence:         cout << "Sequence"; break;
        case ContentSpecNode::Any:              cout << "Any"; break;
        case ContentSpecNode::Any_Other:        cout << "Any_Other"; break;
        case ContentSpecNode::Any_NS:           cout << "Any_NS"; break;
        case ContentSpecNode::Any_Lax:          cout << "Any_Lax"; break;
        case ContentSpecNode::Any_Other_Lax:    cout << "Any_Other_Lax"; break;
        case ContentSpecNode::Any_NS_Lax:       cout << "Any_NS_Lax"; break;
        case ContentSpecNode::Any_Skip:         cout << "Any_Skip"; break;
        case ContentSpecNode::Any_Other_Skip:   cout << "Any_Other_Skip"; break;
        case ContentSpecNode::Any_NS_Skip:   cout << "Any_NS_Skip"; break;
        case ContentSpecNode::UnknownType:      cout << "UnknownType"; break;
    }
    cout << "\n";
}

