%define tarversion 2_2_0

# threads
# values: pthreads, none
%define threads pthreads

Summary:	Validating XML parser
Name:		xerces-c
Version:	2.2.0
Release:	2
URL:		http://xml.apache.org/xerces-c/
Source0:	%{name}-src%{tarversion}.tar.gz
Copyright:	Apache
Group:		Libraries
BuildRoot:	%{_tmppath}/%{name}-root
BuildRequires:	gcc-c++
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
Summary:	Header files, documentation and samples for xerces-c.

%description devel
Xerces-C++ is a validating XML parser written in a portable subset of C++.
Xerces-C++ makes it easy to give your application the ability to read and
write XML data. A shared library is provided for parsing, generating,
manipulating, and validating XML documents.

%prep
%setup -q -n %{name}-src%{tarversion}

%build
export XERCESCROOT=$RPM_BUILD_DIR/%{name}-src%{tarversion}
cd src/xercesc
./runConfigure -plinux -cgcc -xg++ -minmem -nsocket -tnative -r%{threads} -P%{prefix}
make
cd $RPM_BUILD_DIR/%{name}-src%{tarversion}/samples
./runConfigure -plinux -cgcc -xg++
make
rm -rf $RPM_BUILD_DIR/%{name}-src%{tarversion}/bin/obj

%install
export XERCESCROOT=$RPM_BUILD_DIR/%{name}-src%{tarversion}
cd src/xercesc
make install DESTDIR=$RPM_BUILD_ROOT
cd $RPM_BUILD_DIR/%{name}-src%{tarversion}
mkdir -p $RPM_BUILD_ROOT%{prefix}/bin
cp -a bin/* $RPM_BUILD_ROOT%{prefix}/bin/
mkdir -p $RPM_BUILD_ROOT%{prefix}/share/%{name}
cp -a doc $RPM_BUILD_ROOT%{prefix}/share/%{name}/
cp -a samples $RPM_BUILD_ROOT%{prefix}/share/%{name}/

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(755,root,root)
%{prefix}/bin
%{prefix}/lib/libxerces-c.so.*

%files devel
%defattr(-,root,root)
%{prefix}/include/xercesc
%{prefix}/lib/libxerces-c.so
%{prefix}/share/%{name}/doc
%{prefix}/share/%{name}/samples

%changelog
* Tue Sep 03 2002  <thomas@linux.de>
- fixed missing DESTDIR in Makefile.util.submodule

* Mon Sep 02 2002  <thomas@linux.de>
- Initial build.
