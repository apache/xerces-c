%define tarversion 2_7_0

# threads
# values: pthreads, none
%define threads pthreads

Summary:	Xerces-C++ validating XML parser
Name:		xerces-c
Version:	2.7.0
Release:	3
URL:		http://xml.apache.org/xerces-c/
Source0:    %{name}-src_%{tarversion}.tar.gz
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
%setup -q -n %{name}-src_%{tarversion}

%build
export XERCESCROOT=$RPM_BUILD_DIR/%{name}-src_%{tarversion}
cd $XERCESCROOT/src/xercesc
./runConfigure -plinux -cgcc -xg++ -minmem -nsocket -tnative -r%{threads} -P%{prefix}
make
cd $XERCESCROOT/samples
./runConfigure -plinux -cgcc -xg++
make

%install
export XERCESCROOT=$RPM_BUILD_DIR/%{name}-src_%{tarversion}
cd $XERCESCROOT/src/xercesc
make PREFIX=$RPM_BUILD_ROOT%{prefix} install
ln -sf %{prefix}/lib/libxerces-c.so.27 $RPM_BUILD_ROOT%{prefix}/lib/libxerces-c.so
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
%{prefix}/lib/libxerces-depdom.so* 

%files devel
%defattr(-,root,root)
%{prefix}/include/xercesc
%{prefix}/share/%{name}/samples

%files doc
%defattr(-,root,root)
%doc LICENSE NOTICE STATUS credits.txt Readme.html doc/

%changelog
* Fri Jun  6 2003 Tuan Hoang <tqhoang@bigfoot.com>
- updated for new Xerces-C filename and directory format
- fixed date format in changelog section

* Fri Mar 14 2003 Tinny Ng <tng@ca.ibm.com>
- changed to 2.3

* Wed Dec 18 2002 Albert Strasheim <albert@stonethree.com>
- added symlink to libxerces-c.so in lib directory

* Fri Dec 13 2002 Albert Strasheim <albert@stonethree.com>
- added seperate doc package
- major cleanups

* Tue Sep 03 2002  <thomas@linux.de>
- fixed missing DESTDIR in Makefile.util.submodule

* Mon Sep 02 2002  <thomas@linux.de>
- Initial build.
