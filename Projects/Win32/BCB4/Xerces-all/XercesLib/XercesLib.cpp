//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//  Wichtiger Hinweis zur DLL-Speicherverwaltung, wenn Ihre DLL die
//  statische Version der Laufzeitbibliothek verwendet:
//
//  Wenn Ihre DLL Funktionen exportiert, die String-Objekte (oder struct/class,
//  die verschachtelte Strings enthalten) als Parameter oder Funktionsergebnisse übergeben,
//  müssen Sie die Bibliothek MEMMGR.LIB zum DLL-Projekt und
//  auch zu anderen Projekten, die diese DLL benutzen, hinzufügen. Sie müssen auch
//  MEMMGR.LIB verwenden, wenn andere Projekte, die die DLL benutzen, new- oder delete-Operation auf
//  Klassen durchführen, die nicht von TObject abstammen und von der DLL exportiert werden.
//  Durch das Hinzufügen von MEMMGR.LIB werden die DLL und die aufrufenden EXEs
//  dazu veranlasst, BORLNDMM.DLL als Speicher-Manager zu verwenden. In diesen Fällen
//  muß die Datei BORLNDMM.DLL mit der DLL weitergegeben werden.
//
//  Um die Verwendung von BORLNDMM.DLL zu vermeiden, sollten String-Informationen
//  als "char *" oder ShortString übergeben werden.
//
//  Wenn Ihre DLL die dynamische Version der RTL verwendet, müssen Sie
//  MEMMGR.LIB nicht explizit hinzufügen, da dies implizit geschieht.
//---------------------------------------------------------------------------
USEUNIT("..\..\..\..\..\src\dom\TreeWalkerImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\CDATASectionImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\CharacterDataImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\CommentImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\DeepNodeListImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\DocumentFragmentImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\DocumentImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\DocumentTypeImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_Attr.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_CDATASection.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_CharacterData.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_Comment.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_Document.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_DocumentFragment.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_DocumentType.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_DOMException.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_DOMImplementation.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_Element.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_Entity.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_EntityReference.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_NamedNodeMap.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_Node.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_NodeFilter.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_NodeIterator.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_NodeList.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_Notation.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_ProcessingInstruction.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_Text.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOM_TreeWalker.cpp");
USEUNIT("..\..\..\..\..\src\dom\DomMemDebug.cpp");
USEUNIT("..\..\..\..\..\src\dom\DOMString.cpp");
USEUNIT("..\..\..\..\..\src\dom\DStringPool.cpp");
USEUNIT("..\..\..\..\..\src\dom\ElementDefinitionImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\ElementImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\EntityImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\EntityReferenceImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\NamedNodeMapImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\NodeFilterImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\NodeImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\NodeIteratorImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\NodeListImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\NodeVector.cpp");
USEUNIT("..\..\..\..\..\src\dom\NotationImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\ProcessingInstructionImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\RefCountedImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\TextImpl.cpp");
USEUNIT("..\..\..\..\..\src\dom\AttrImpl.cpp");
USEUNIT("..\..\..\..\..\src\util\Platforms\Win32\Win32PlatformUtils.cpp");
USEUNIT("..\..\..\..\..\src\util\XMLUni.cpp");
USEUNIT("..\..\..\..\..\src\util\BinInputStream.cpp");
USEUNIT("..\..\..\..\..\src\util\BinMemInputStream.cpp");
USEUNIT("..\..\..\..\..\src\util\BitSet.cpp");
USEUNIT("..\..\..\..\..\src\util\HeaderDummy.cpp");
USEUNIT("..\..\..\..\..\src\util\KVStringPair.cpp");
USEUNIT("..\..\..\..\..\src\util\Mutexes.cpp");
USEUNIT("..\..\..\..\..\src\util\PlatformUtils.cpp");
USEUNIT("..\..\..\..\..\src\util\StdOut.cpp");
USEUNIT("..\..\..\..\..\src\util\StringPool.cpp");
USEUNIT("..\..\..\..\..\src\util\TextOutputStream.cpp");
USEUNIT("..\..\..\..\..\src\util\URL.cpp");
USEUNIT("..\..\..\..\..\src\util\XMLException.cpp");
USEUNIT("..\..\..\..\..\src\util\XMLString.cpp");
USEUNIT("..\..\..\..\..\src\util\BinFileInputStream.cpp");
USEFILE("..\..\..\..\..\src\util\Compilers\BorlandCDefs.hpp");
USEUNIT("..\..\..\..\..\src\util\Transcoders\Win32\Win32TransService.cpp");
USEUNIT("..\..\..\..\..\src\util\MsgLoaders\Win32\Win32MsgLoader.cpp");
USEUNIT("..\..\..\..\..\src\framework\XMLValidator.cpp");
USEUNIT("..\..\..\..\..\src\framework\XMLAttr.cpp");
USEUNIT("..\..\..\..\..\src\framework\XMLBuffer.cpp");
USEUNIT("..\..\..\..\..\src\framework\XMLBufferMgr.cpp");
USEUNIT("..\..\..\..\..\src\framework\XMLContentModel.cpp");
USEUNIT("..\..\..\..\..\src\framework\XMLElementDecl.cpp");
USEUNIT("..\..\..\..\..\src\framework\XMLEntityDecl.cpp");
USEUNIT("..\..\..\..\..\src\framework\XMLNotationDecl.cpp");
USEUNIT("..\..\..\..\..\src\framework\XMLRecognizer.cpp");
USEUNIT("..\..\..\..\..\src\framework\XMLAttDef.cpp");
USEUNIT("..\..\..\..\..\src\internal\XMLScanner2.cpp");
USEUNIT("..\..\..\..\..\src\internal\MemBufInputSource.cpp");
USEUNIT("..\..\..\..\..\src\internal\ReaderMgr.cpp");
USEUNIT("..\..\..\..\..\src\internal\StdInInputSource.cpp");
USEUNIT("..\..\..\..\..\src\internal\URLInputSource.cpp");
USEUNIT("..\..\..\..\..\src\internal\VecAttrListImpl.cpp");
USEUNIT("..\..\..\..\..\src\internal\XMLReader.cpp");
USEUNIT("..\..\..\..\..\src\internal\XMLScanner.cpp");
USEUNIT("..\..\..\..\..\src\internal\ElemStack.cpp");
USEUNIT("..\..\..\..\..\src\parsers\SAXParser.cpp");
USEUNIT("..\..\..\..\..\src\parsers\DOMParser.cpp");
USEUNIT("..\..\..\..\..\src\sax\SAXParseException.cpp");
USEUNIT("..\..\..\..\..\src\sax\InputSource.cpp");
USEUNIT("..\..\..\..\..\src\sax\Dummy.cpp");
USEUNIT("..\..\..\..\..\src\validators\DTD\SimpleContentModel.cpp");
USEUNIT("..\..\..\..\..\src\validators\DTD\CMUnaryOp.cpp");
USEUNIT("..\..\..\..\..\src\validators\DTD\ContentSpecNode.cpp");
USEUNIT("..\..\..\..\..\src\validators\DTD\DFAContentModel.cpp");
USEUNIT("..\..\..\..\..\src\validators\DTD\DTDAttDef.cpp");
USEUNIT("..\..\..\..\..\src\validators\DTD\DTDAttDefList.cpp");
USEUNIT("..\..\..\..\..\src\validators\DTD\DTDElementDecl.cpp");
USEUNIT("..\..\..\..\..\src\validators\DTD\DTDValidator.cpp");
USEUNIT("..\..\..\..\..\src\validators\DTD\DTDValidator2.cpp");
USEUNIT("..\..\..\..\..\src\validators\DTD\MixedContentModel.cpp");
USEUNIT("..\..\..\..\..\src\validators\DTD\CMBinaryOp.cpp");
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
  return 1;
}
//---------------------------------------------------------------------------
