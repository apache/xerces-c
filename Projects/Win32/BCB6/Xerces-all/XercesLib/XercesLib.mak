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

VERSION = BCB.06.00
# ---------------------------------------------------------------------------
PROJECT = ..\..\..\..\..\Build\Win32\BCB6\XercesLib.dll
OBJFILES = ..\..\..\..\..\Build\Win32\BCB6\obj\Xerceslib.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Win32PlatformUtils.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Win32MsgLoader.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Win32TransService.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\BinHTTPURLInputStream.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\WinSockNetAccessor.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ASCIIRangeFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\BlockRangeFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\BMPattern.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\CharToken.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ClosureToken.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ConcatToken.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ConditionToken.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Match.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ModifierToken.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Op.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\OpFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ParenToken.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ParserForXMLSchema.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\RangeFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\RangeToken.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\RangeTokenMap.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\RegularExpression.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\RegxParser.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\RegxUtil.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\StringToken.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Token.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\TokenFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\UnicodeRangeFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\UnionToken.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLRangeFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLUniCharacter.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Base64.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DefaultPanicHandler.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\PanicHandler.obj \    
    ..\..\..\..\..\Build\Win32\BCB6\obj\BinFileInputStream.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\BinInputStream.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\BinMemInputStream.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\BitSet.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\EncodingValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\HashPtr.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\HashXMLCh.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\HeaderDummy.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\HexBin.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\KVStringPair.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Mutexes.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\PlatformUtils.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\QName.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\StringPool.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\TransService.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMemory.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XML256TableTranscoder.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XML88591Transcoder.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLAbstractDoubleFloat.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLASCIITranscoder.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLBigDecimal.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLBigInteger.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLChar.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLChTranscoder.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLDateTime.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLDouble.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLEBCDICTranscoder.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLException.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLFloat.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLIBM1140Transcoder.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLMsgLoader.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLNumber.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLString.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLStringTokenizer.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLUCSTranscoder.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLUni.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLUri.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLURL.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLUTF16Transcoder.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLUTF8Transcoder.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLWin1252Transcoder.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\LocalFileFormatTarget.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\LocalFileInputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\MemBufFormatTarget.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\MemBufInputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\StdInInputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\StdOutFormatTarget.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\URLInputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Wrapper4DOMInputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Wrapper4InputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLAttDef.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLAttr.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLBuffer.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLBufferMgr.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLContentModel.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLElementDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLEntityDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLFormatter.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLNotationDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLRecognizer.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DGXMLScanner.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ElemStack.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\IGXMLScanner.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\IGXMLScanner2.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\MemoryManagerImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ReaderMgr.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SGXMLScanner.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\VecAttributesImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\VecAttrListImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\WFXMLScanner.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLGrammarPoolImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLReader.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLScanner.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLScannerResolver.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Dummy.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\InputSource.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SAXException.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SAXParseException.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\AbstractDOMParser.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMBuilderImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SAX2XMLReaderImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SAXParser.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XercesDOMParser.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\AllContentModel.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\CMAny.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\CMBinaryOp.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\CMUnaryOp.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ContentLeafNameTypeVector.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ContentSpecNode.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DFAContentModel.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\GrammarResolver.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\MixedContentModel.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SimpleContentModel.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\AbstractNumericFacetValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\AbstractNumericValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\AbstractStringValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\AnySimpleTypeDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\AnyURIDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\Base64BinaryDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\BooleanDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DatatypeValidatorFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DateDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DateTimeDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DateTimeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DayDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DecimalDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DoubleDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DurationDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ENTITYDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\FloatDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\HexBinaryDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\IDDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\IDREFDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ListDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\MonthDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\MonthDayDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\NameDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\NCNameDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\NOTATIONDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\QNameDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\StringDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\TimeDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\UnionDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\YearDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\YearMonthDatatypeValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DTDAttDef.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DTDAttDefList.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DTDElementDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DTDGrammar.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DTDScanner.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DTDValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLDTDDescriptionImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\FieldActivator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\FieldValueMap.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\IC_Field.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\IC_Key.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\IC_KeyRef.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\IC_Selector.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\IC_Unique.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\IdentityConstraint.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ValueStore.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ValueStoreCache.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XercesXPath.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XPathMatcher.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XPathMatcherStack.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XPathSymbols.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ComplexTypeInfo.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\GeneralAttributeCheck.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\NamespaceScope.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SchemaAttDef.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SchemaAttDefList.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SchemaElementDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SchemaGrammar.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SchemaInfo.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SchemaSymbols.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SchemaValidator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\SubstitutionGroupComparator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\TraverseSchema.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XercesAttGroupInfo.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XercesElementWildcard.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XercesGroupInfo.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSDDOMParser.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSDErrorReporter.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSDLocator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLSchemaDescriptionImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XUtil.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\sax2Dummy.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMAttrImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMAttrMapImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMAttrNSImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMCDATASectionImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMCharacterDataImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMChildNode.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMCommentImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMConfigurationImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMDeepNodeListImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMDocumentFragmentImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMDocumentImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMDocumentTypeImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMElementImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMElementNSImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMEntityImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMEntityReferenceImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMErrorImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMImplementationImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMImplementationRegistry.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMLocatorImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMNamedNodeMapImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMNodeIDMap.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMNodeImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMNodeIteratorImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMNodeListImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMNodeVector.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMNormalizer.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMNotationImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMParentNode.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMProcessingInstructionImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMRangeImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMStringPool.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMTextImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMTreeWalkerImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMTypeInfoImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMWriterImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSDElementNSImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\AttrImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\AttrMapImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\AttrNSImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\CDATASectionImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\CharacterDataImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ChildNode.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\CommentImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DeepNodeListImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DocumentFragmentImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DocumentImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DocumentTypeImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_Attr.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_CDATASection.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_CharacterData.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_Comment.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_Document.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_DocumentFragment.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_DocumentType.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_DOMException.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_DOMImplementation.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_Element.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_Entity.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_EntityReference.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_NamedNodeMap.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_Node.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_NodeFilter.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_NodeIterator.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_NodeList.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_Notation.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_ProcessingInstruction.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_Range.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_RangeException.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_Text.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_TreeWalker.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOM_XMLDecl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DomMemDebug.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMParser.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMString.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DStringPool.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ElementDefinitionImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ElementImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ElementNSImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\EntityImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\EntityReferenceImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\NamedNodeMapImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\NodeIDMap.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\NodeImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\NodeIteratorImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\NodeListImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\NodeVector.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\NotationImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ParentNode.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ProcessingInstructionImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\RangeImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\RefCountedImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\TextImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\TreeWalkerImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLDeclImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMException.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMRangeException.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\PSVIAttribute.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\PSVIAttributeList.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\PSVIElement.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\PSVIItem.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\ValidationContextImpl.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSAnnotation.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSAttributeDeclaration.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSAttributeGroupDefinition.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSAttributeUse.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSComplexTypeDefinition.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSElementDeclaration.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSFacet.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSIDCDefinition.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSModel.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSModelGroup.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSModelGroupDefinition.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSMultiValueFacet.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSNamespaceItem.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSNotationDeclaration.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSObject.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSParticle.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSSimpleTypeDefinition.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSTypeDefinition.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSWildcard.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\BinFileOutputStream.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\BinOutputStream.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\DOMXPathException.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XSObjectFactory.obj \
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLIBM1047Transcoder.obj\
    ..\..\..\..\..\Build\Win32\BCB6\obj\XMLCanRepGroup.obj
RESFILES = ..\..\..\..\..\src\xercesc\util\Platforms\Win32\Version.res
MAINSOURCE = Xerceslib.cpp
RESDEPEN = $(RESFILES)
LIBFILES = 
IDLFILES = 
IDLGENFILES = 
LIBRARIES = 
PACKAGES = vcl.bpi rtl.bpi
SPARELIBS = rtl.lib
DEFFILE = 
OTHERFILES = 
# ---------------------------------------------------------------------------
DEBUGLIBPATH = $(BCB)\lib\debug
RELEASELIBPATH = $(BCB)\lib\release
USERDEFINES = PLATFORM_WIN32;_CRTDBG_MAP_ALLOC;PROJ_XMLPARSER;PROJ_XMLUTIL;PROJ_PARSERS;PROJ_SAX4C;PROJ_SAX2;PROJ_DOM;PROJ_VALIDATORS;XML_SINGLEDLL;XML_USE_WIN32_TRANSCODER;XML_USE_WIN32_MSGLOADER;XML_USE_NETACCESSOR_WINSOCK;_DEBUG
SYSDEFINES = NO_STRICT;_RTLDLL;USEPACKAGES
INCLUDEPATH = ..\..\..\..\..\src;$(BCB)\include
LIBPATH = ..\..\..\..\..\src;$(BCB)\lib\obj;$(BCB)\lib
WARNINGS= -w-par -w-8027 -w-8026
PATHCPP = .;..\..\..\..\..\src\xercesc\util\Platforms\Win32;..\..\..\..\..\src\xercesc\util\MsgLoaders\Win32;..\..\..\..\..\src\xercesc\util\Transcoders\Win32;..\..\..\..\..\src\xercesc\util\NetAccessors\WinSock;..\..\..\..\..\src\xercesc\util\regx;..\..\..\..\..\src\xercesc\util;..\..\..\..\..\src\xercesc\framework;..\..\..\..\..\src\xercesc\internal;..\..\..\..\..\src\xercesc\sax;..\..\..\..\..\src\xercesc\parsers;..\..\..\..\..\src\xercesc\validators\common;..\..\..\..\..\src\xercesc\validators\datatype;..\..\..\..\..\src\xercesc\validators\DTD;..\..\..\..\..\src\xercesc\validators\schema\identity;..\..\..\..\..\src\xercesc\validators\schema;..\..\..\..\..\src\xercesc\sax2;..\..\..\..\..\src\xercesc\dom\impl;..\..\..\..\..\src\xercesc\dom\deprecated;..\..\..\..\..\src\xercesc\dom;..\..\..\..\..\src\xercesc\framework\psvi
PATHASM = .;
PATHPAS = .;
PATHRC = .;..\..\..\..\..\src\xercesc\util\Platforms\Win32
PATHOBJ = .;$(LIBPATH)
# ---------------------------------------------------------------------------
CFLAG1 = -WD -Od -Q -Vx -Ve -X- -r- -a8 -b- -k -y -v -vi- -tWD -tWM -c
IDLCFLAGS = -I..\..\..\..\..\src\xercesc -I$(BCB)\include -src_suffix cpp \
    -DPLATFORM_WIN32 -D_CRTDBG_MAP_ALLOC -DPROJ_XMLPARSER -DPROJ_XMLUTIL \
    -DPROJ_PARSERS -DPROJ_SAX4C -DPROJ_SAX2 -DPROJ_DOM -DPROJ_VALIDATORS \
    -DXML_SINGLEDLL -DXML_USE_WIN32_TRANSCODER -DXML_USE_WIN32_MSGLOADER \
    -DXML_USE_NETACCESSOR_WINSOCK -D_DEBUG -boa
PFLAGS = -N2..\..\..\..\..\Build\Win32\BCB6\obj \
    -N0..\..\..\..\..\Build\Win32\BCB6\obj -$Y+ -$W -$O- -$A8 -v -JPHNE -M
RFLAGS = 
AFLAGS = /mx /w2 /zi
LFLAGS = -I..\..\..\..\..\Build\Win32\BCB6\obj -D"" -aa -Tpd -x -Gn -Gi -v
# ---------------------------------------------------------------------------
ALLOBJ = c0d32.obj $(PACKAGES) Memmgr.Lib $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) $(LIBRARIES) import32.lib cw32mti.lib
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

!if $d(PATHOBJ)
.PATH.OBJ  = $(PATHOBJ)
!endif
# ---------------------------------------------------------------------------
$(PROJECT): $(OTHERFILES) $(IDLGENFILES) $(OBJFILES) $(RESDEPEN) $(DEFFILE)
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




