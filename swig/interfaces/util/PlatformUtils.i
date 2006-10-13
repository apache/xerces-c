// both of these static variables cause trouble
// the transcoding service is only useful to C++ anyway.
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgTransService;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgNetAccessor;

// these are other static variables that are useless to Perl
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgUserPanicHandler;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgDefaultPanicHandler;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgMemoryManager;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgDefaulPanicHandler;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgArrayMemoryManager;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgAtomicMutex;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgFileMgr;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgMutexMgr;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgAtomicOpMgr;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fgXMLChBigEndian;

// these are methods that are useless in Perl
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::curFilePos;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::closeFile;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::fileSize;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::openFile;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::openFileToWrite;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::openStdInHandle;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::readFileBuffer;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::writeBufferToFile;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::resetFile;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::getFullPath;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::getCurrentDirectory;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::isAnySlash;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::removeDotSlash;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::removeDotDotSlash;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::isRelative;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::weavePaths;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::getCurrentMillis;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::closeMutex;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::lockMutex;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::makeMutex;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::unlockMutex;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::loadMsgSet;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::compareAndSwap;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::atomicIncrement;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::atomicDecrement;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::recognizeNEL;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::isNELRecognized;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::strictIANAEncoding;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::isStrictIANAEncoding;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::alignPointerForNewBlockAllocation;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::makeFileMgr;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::makeMutexMgr;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::makeAtomicOpMgr;
%ignore XERCES_CPP_NAMESPACE::XMLPlatformUtils::panic;

// changing the locale and the NLS home is probably useful
// but changing the panic manager and memory manager is not
%ignore Initialize(const char*          const
                         , const char*          const
                         ,       PanicHandler*  const
                         ,       MemoryManager* const);
%ignore Initialize(const char*          const
                         , const char*          const
                         ,       PanicHandler*  const);

%include "xercesc/util/PlatformUtils.hpp"
