# ---------------------------------------------------------------------------
!if !$d(BCB)
BCB = $(MAKEDIR)\..
!endif

# ---------------------------------------------------------------------------
# IDE SECTION
# ---------------------------------------------------------------------------
# The following section of the project makefile is managed by the BCB IDE.
# It is recommended to use the IDE to change any of the values in this
# section.
# ---------------------------------------------------------------------------

VERSION = BCB.05.03
# ---------------------------------------------------------------------------
PROJECT = ..\..\..\..\..\Build\Win32\BCB5\XercesLib.dll
OBJFILES = ..\..\..\..\..\Build\Win32\BCB5\obj\XercesLib.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\AttrImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\AttrMapImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\AttrNSImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\CDATASectionImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\CharacterDataImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ChildNode.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\CommentImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DeepNodeListImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DocumentFragmentImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DocumentImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DocumentTypeImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_Attr.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_CDATASection.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_CharacterData.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_Comment.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_Document.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_DocumentFragment.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_DocumentType.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_DOMException.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_DOMImplementation.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_Element.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_Entity.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_EntityReference.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_NamedNodeMap.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_Node.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_NodeFilter.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_NodeIterator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_NodeList.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_Notation.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_ProcessingInstruction.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_Range.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_RangeException.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_Text.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_TreeWalker.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOM_XMLDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DomMemDebug.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOMString.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DStringPool.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ElementDefinitionImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ElementImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ElementNSImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\EntityImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\EntityReferenceImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\NamedNodeMapImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\NodeIDMap.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\NodeImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\NodeIteratorImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\NodeListImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\NodeVector.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\NotationImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ParentNode.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ProcessingInstructionImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\RangeImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\RefCountedImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\TextImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\TreeWalkerImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLDeclImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\LocalFileInputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\MemBufInputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\StdInInputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\URLInputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLAttDef.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLAttr.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLBuffer.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLBufferMgr.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLContentModel.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLElementDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLEntityDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLFormatter.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLNotationDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLRecognizer.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ElemStack.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ReaderMgr.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\VecAttributesImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\VecAttrListImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLReader.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLScanner.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLScanner2.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DOMParser.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SAX2XMLReaderImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SAXParser.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\Dummy.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\InputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SAXException.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SAXParseException.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\sax2Dummy.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\Base64.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\BinFileInputStream.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\BinInputStream.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\BinMemInputStream.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\BitSet.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\HashPtr.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\HashXMLCh.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\HeaderDummy.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\HexBin.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\KVStringPair.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\Win32MsgLoader.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\Mutexes.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\BinHTTPURLInputStream.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\WinSockNetAccessor.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\Win32PlatformUtils.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\PlatformUtils.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\QName.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ASCIIRangeFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\BlockRangeFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\BMPattern.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\CharToken.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ClosureToken.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ConcatToken.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ConditionToken.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\Match.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ModifierToken.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\Op.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\OpFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ParenToken.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ParserForXMLSchema.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\RangeFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\RangeToken.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\RangeTokenMap.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\RegularExpression.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\RegxParser.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\RegxUtil.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\StringToken.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\Token.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\TokenFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\UnicodeRangeFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\UnionToken.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLRangeFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLUniCharacter.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\StringPool.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\Win32TransService.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\TransService.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XML256TableTranscoder.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XML88591Transcoder.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLASCIITranscoder.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLNumber.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLBigDecimal.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLBigInteger.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLChTranscoder.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLAbstractDoubleFloat.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLDouble.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLDateTime.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLEBCDICTranscoder.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLException.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLFloat.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLIBM1140Transcoder.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLString.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLStringTokenizer.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLUCSTranscoder.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLUni.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLUri.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLURL.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLUTF16Transcoder.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLUTF8Transcoder.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XMLWin1252Transcoder.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\AllContentModel.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\CMAny.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\CMBinaryOp.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\CMUnaryOp.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ContentLeafNameTypeVector.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ContentSpecNode.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DFAContentModel.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\GrammarResolver.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\MixedContentModel.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SimpleContentModel.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\AbstractNumericFacetValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\AbstractNumericValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\AbstractStringValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\AnySimpleTypeDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\AnyURIDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\Base64BinaryDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\BooleanDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DatatypeValidatorFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DecimalDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DoubleDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ENTITYDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\FloatDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\HexBinaryDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\IDDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\IDREFDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ListDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\NOTATIONDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\QNameDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\NameDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\NCNameDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\StringDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\UnionDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DateTimeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DateTimeDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DateDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\TimeDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DayDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\MonthDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\MonthDayDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\YearDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\YearMonthDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DurationDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DTDAttDef.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DTDAttDefList.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DTDElementDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DTDGrammar.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DTDScanner.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\DTDValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ComplexTypeInfo.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\GeneralAttributeCheck.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\NamespaceScope.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SchemaAttDef.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SchemaAttDefList.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SchemaElementDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SchemaGrammar.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SchemaInfo.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SchemaSymbols.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SchemaValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\SubstitutionGroupComparator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\TraverseSchema.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XercesAttGroupInfo.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XercesElementWildcard.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XercesGroupInfo.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XUtil.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\FieldActivator.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\FieldValueMap.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\IC_Field.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\IC_Key.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\IC_KeyRef.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\IC_Selector.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\IC_Unique.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\IdentityConstraint.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ValueStore.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\ValueStoreCache.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XercesXPath.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XPathMatcher.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XPathMatcherStack.obj \
    ..\..\..\..\..\Build\Win32\BCB5\obj\XPathSymbols.obj
RESFILES = ..\..\..\..\..\Build\Win32\BCB5\obj\Version.res
MAINSOURCE = XercesLib.cpp
RESDEPEN = $(RESFILES)
LIBFILES = 
IDLFILES = 
IDLGENFILES = 
LIBRARIES = vcl50.lib
PACKAGES = vcl50.bpi vclx50.bpi vcljpg50.bpi
SPARELIBS = vcl50.lib
DEFFILE = 
# ---------------------------------------------------------------------------
PATHCPP = .;..\..\..\..\..\src\dom;..\..\..\..\..\src\framework;..\..\..\..\..\src\internal;..\..\..\..\..\src\parsers;..\..\..\..\..\src\sax;..\..\..\..\..\src\sax2;..\..\..\..\..\src\util;..\..\..\..\..\src\util\MsgLoaders\Win32;..\..\..\..\..\src\util\NetAccessors\WinSock;..\..\..\..\..\src\util\Platforms\Win32;..\..\..\..\..\src\util\regx;..\..\..\..\..\src\util\Transcoders\Win32;..\..\..\..\..\src\validators\common;..\..\..\..\..\src\validators\datatype;..\..\..\..\..\src\validators\DTD;..\..\..\..\..\src\validators\schema;..\..\..\..\..\src\validators\schema\identity
PATHASM = .;
PATHPAS = .;
PATHRC = .;..\..\..\..\..\src\util\Platforms\Win32
DEBUGLIBPATH = $(BCB)\lib\debug
RELEASELIBPATH = $(BCB)\lib\release
USERDEFINES = _DEBUG;PLATFORM_WIN32;_CRTDBG_MAP_ALLOC;PROJ_XMLPARSER;PROJ_XMLUTIL;PROJ_PARSERS;PROJ_SAX4C;PROJ_SAX2;PROJ_DOM;PROJ_VALIDATORS;XML_SINGLEDLL;XML_USE_WIN32_TRANSCODER;XML_USE_WIN32_MSGLOADER;XML_USE_NETACCESSOR_WINSOCK
SYSDEFINES = _RTLDLL;NO_STRICT
INCLUDEPATH = ..\..\..\..\..\src;..\..\..\..\..\src\dom;..\..\..\..\..\src\framework;..\..\..\..\..\src\internal;..\..\..\..\..\src\parsers;..\..\..\..\..\src\sax;..\..\..\..\..\src\sax2;..\..\..\..\..\src\util;..\..\..\..\..\src\util\MsgLoaders\Win32;..\..\..\..\..\src\util\NetAccessors\WinSock;..\..\..\..\..\src\util\Platforms\Win32;..\..\..\..\..\src\util\regx;..\..\..\..\..\src\util\Transcoders\Win32;..\..\..\..\..\src\validators\common;..\..\..\..\..\src\validators\datatype;..\..\..\..\..\src\validators\DTD;..\..\..\..\..\src\validators\schema;..\..\..\..\..\src\validators\schema\identity;$(BCB)\include;$(BCB)\include\vcl
LIBPATH = ..\..\..\..\..\src\dom;..\..\..\..\..\src\framework;..\..\..\..\..\src\internal;..\..\..\..\..\src\parsers;..\..\..\..\..\src\sax;..\..\..\..\..\src\sax2;..\..\..\..\..\src\util;..\..\..\..\..\src\util\MsgLoaders\Win32;..\..\..\..\..\src\util\NetAccessors\WinSock;..\..\..\..\..\src\util\Platforms\Win32;..\..\..\..\..\src\util\regx;..\..\..\..\..\src\util\Transcoders\Win32;..\..\..\..\..\src\validators\common;..\..\..\..\..\src\validators\datatype;..\..\..\..\..\src\validators\DTD;..\..\..\..\..\src\validators\schema;..\..\..\..\..\src\validators\schema\identity;$(BCB)\lib\obj;$(BCB)\lib
WARNINGS= -w-par
# ---------------------------------------------------------------------------
CFLAG1 = -WD -Od -Q -Vx -Ve -X- -r- -a8 -5 -b- -k -y -v -vi- -c -tWM
IDLCFLAGS = 
PFLAGS = -N2..\..\..\..\..\Build\Win32\BCB5\obj \
    -N0..\..\..\..\..\Build\Win32\BCB5\obj -$YD -$W -$O- -v -JPHNE -M
RFLAGS = 
AFLAGS = /mx /w2 /zd
LFLAGS = -l"..\..\..\..\..\Build\Win32\BCB5\" -I..\..\..\..\..\Build\Win32\BCB5\obj \
    -D"" -aa -Tpd -x -Gn -Gi -v
# ---------------------------------------------------------------------------
ALLOBJ = c0d32.obj Memmgr.Lib sysinit.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) $(LIBRARIES) import32.lib cp32mti.lib
# ---------------------------------------------------------------------------
!ifdef IDEOPTIONS

[Version Info]
IncludeVerInfo=0
AutoIncBuild=0
MajorVer=1
MinorVer=0
Release=0
Build=0
Debug=0
PreRelease=0
Special=0
Private=0
DLL=0

[Version Info Keys]
CompanyName=
FileDescription=
FileVersion=1.0.0.0
InternalName=
LegalCopyright=
LegalTrademarks=
OriginalFilename=
ProductName=
ProductVersion=1.0.0.0
Comments=

[Debugging]
DebugSourceDirs=$(BCB)\source\vcl

!endif





# ---------------------------------------------------------------------------
# MAKE SECTION
# ---------------------------------------------------------------------------
# This section of the project file is not used by the BCB IDE.  It is for
# the benefit of building from the command-line using the MAKE utility.
# ---------------------------------------------------------------------------

.autodepend
# ---------------------------------------------------------------------------
!if "$(USERDEFINES)" != ""
AUSERDEFINES = -d$(USERDEFINES:;= -d)
!else
AUSERDEFINES =
!endif

!if !$d(BCC32)
BCC32 = bcc32
!endif

!if !$d(CPP32)
CPP32 = cpp32
!endif

!if !$d(DCC32)
DCC32 = dcc32
!endif

!if !$d(TASM32)
TASM32 = tasm32
!endif

!if !$d(LINKER)
LINKER = ilink32
!endif

!if !$d(BRCC32)
BRCC32 = brcc32
!endif


# ---------------------------------------------------------------------------
!if $d(PATHCPP)
.PATH.CPP = $(PATHCPP)
.PATH.C   = $(PATHCPP)
!endif

!if $d(PATHPAS)
.PATH.PAS = $(PATHPAS)
!endif

!if $d(PATHASM)
.PATH.ASM = $(PATHASM)
!endif

!if $d(PATHRC)
.PATH.RC  = $(PATHRC)
!endif
# ---------------------------------------------------------------------------
$(PROJECT): $(IDLGENFILES) $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) -L$(LIBPATH) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES)
!
# ---------------------------------------------------------------------------
.pas.hpp:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.pas.obj:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.cpp.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.cpp.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.asm.obj:
    $(BCB)\BIN\$(TASM32) $(AFLAGS) -i$(INCLUDEPATH:;= -i) $(AUSERDEFINES) -d$(SYSDEFINES:;= -d) $<, $@

.rc.res:
    $(BCB)\BIN\$(BRCC32) $(RFLAGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -fo$@ $<
# ---------------------------------------------------------------------------




