%define tarversion 2_2_0

# threads
# values: pthreads, none
%define threads pthreads

Summary:	Xerces-C++ validating XML parser
Name:		xerces-c
Version:	2.2.0
Release:	2
URL:		http://xml.apache.org/xerces-c/
Source0:	%{name}-src%{tarversion}.tar.gz
Copyright:	Apache
Group:		Libraries
BuildRoot:	%{_tmppath}/%{name}-root
Prefix:		/usr

%description
Xerces-C++ is a validating XML parser written in a portable subset of C++.
Xerces-C++ makes it easy to give your application the ability to read and
write XML data. A shared library is provided for parsing, generating,
manipulating, and validating XML documents.

The parser provides high performance, modularity, and scalability. Source
code, samples and API documentation are provided with the parser. For
portability, care has been taken to make minimal use of templates, no RTTI,
and minimal use of #ifdefs.

%package devel
Requires:	xerces-c = %{version}
Group:		Development/Libraries
Summary:	Header files for Xerces-C++ validating XML parser

%description devel
Header files you can use to develop XML applications with Xerces-C++.

Xerces-C++ is a validating XML parser written in a portable subset of C++.
Xerces-C++ makes it easy to give your application the ability to read and
write XML data. A shared library is provided for parsing, generating,
manipulating, and validating XML documents.

%package doc
Group:		Documentation
Summary:	Documentation for Xerces-C++ validating XML parser

%description doc
Documentation for Xerces-C++.

Xerces-C++ is a validating XML parser written in a portable subset of C++.
Xerces-C++ makes it easy to give your application the ability to read and
write XML data. A shared library is provided for parsing, generating,
manipulating, and validating XML documents.

%prep
%setup -q -n %{name}-src%{tarversion}

%build
export XERCESCROOT=$RPM_BUILD_DIR/%{name}-src%{tarversion}
cd $XERCESCROOT/src/xercesc
./runConfigure -plinux -cgcc -xg++ -minmem -nsocket -tnative -r%{threads} -P%{prefix}
make
cd $XERCESCROOT/samples
./runConfigure -plinux -cgcc -xg++
make

%install
export XERCESCROOT=$RPM_BUILD_DIR/%{name}-src%{tarversion}
cd $XERCESCROOT/src/xercesc
make PREFIX=$RPM_BUILD_ROOT%{prefix} install
ln -sf %{prefix}/lib/libxerces-c.so.22 $RPM_BUILD_ROOT%{prefix}/lib/libxerces-c.so
mkdir -p $RPM_BUILD_ROOT%{prefix}/bin
#we don't want obj directory
install `find $XERCESCROOT/bin -type f -maxdepth 1` $RPM_BUILD_ROOT%{prefix}/bin
mkdir -p $RPM_BUILD_ROOT%{prefix}/share/%{name}
cp -a $XERCESCROOT/samples $RPM_BUILD_ROOT%{prefix}/share/%{name}

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(755,root,root)
%{prefix}/bin/*
%{prefix}/lib/libxerces-c.so*

%files devel
%defattr(-,root,root)
%{prefix}/include/xercesc
%{prefix}/share/%{name}/samples

%files doc
%defattr(-,root,root)
%doc LICENSE.txt STATUS credits.txt Readme.html doc/

%changelog
* Wed Dec 18 2002 Albert Strasheim <albert@stonethree.com>
- added symlink to libxerces-c.so in lib directory

* Fri Dec 13 2002 Albert Strasheim <albert@stonethree.com>
- added seperate doc package
- major cleanups

* Tue Sep 03 2002  <thomas@linux.de>
- fixed missing DESTDIR in Makefile.util.submodule

* Mon Sep 02 2002  <thomas@linux.de>
- Initial build.
