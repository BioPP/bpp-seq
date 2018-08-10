%define _prefix /usr

URL: https://github.com/BioPP/bpp-seq

Name: bpp-seq
Version: 2.4.1
Release: 1%{?dist}
License: CECILL-2.0
Vendor: The Bio++ Project
Source: %{name}-%{version}.tar.gz
Summary: Bio++ Sequence library
Group: Development/Libraries/C and C++
Requires: bpp-core = %{version}

BuildRoot: %{_builddir}/%{name}-root
BuildRequires: cmake >= 2.8.11
BuildRequires: gcc-c++ >= 4.7.0
BuildRequires: libbpp-core4 = %{version}
BuildRequires: libbpp-core-devel = %{version}
AutoReq: yes
AutoProv: yes

%description
This library contains utilitary and classes for bio-sequence analysis.
It is part of the Bio++ project.

%package -n libbpp-seq12
Summary: Bio++ Sequence library
Group: Development/Libraries/C and C++

%description -n libbpp-seq12
This library contains utilitary and classes for bio-sequence analysis.
It is part of the Bio++ project.

%package -n libbpp-seq-devel
Summary: Libraries, includes to develop applications with %{name}
Group: Development/Libraries/C and C++
Requires: libbpp-seq12 = %{version}
Requires: libbpp-core4 = %{version}
Requires: libbpp-core-devel = %{version}

%description -n libbpp-seq-devel
The libbpp-seq-devel package contains the header files and static libraries for
building applications which use %{name}.

%prep
%setup -q

%build
CFLAGS="$RPM_OPT_FLAGS"
CMAKE_FLAGS="-DCMAKE_INSTALL_PREFIX=%{_prefix} -DBUILD_TESTING=OFF"
cmake $CMAKE_FLAGS .
make

%install
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%post -n libbpp-seq12 -p /sbin/ldconfig

%postun -n libbpp-seq12 -p /sbin/ldconfig

%files -n libbpp-seq12
%defattr(-,root,root)
%doc AUTHORS.txt COPYING.txt INSTALL.txt ChangeLog
%{_prefix}/%{_lib}/lib*.so.*

%files -n libbpp-seq-devel
%defattr(-,root,root)
%doc AUTHORS.txt COPYING.txt INSTALL.txt ChangeLog
%dir %{_prefix}/%{_lib}/cmake/
%dir %{_prefix}/%{_lib}/cmake/bpp-seq
%{_prefix}/%{_lib}/lib*.so
%{_prefix}/%{_lib}/lib*.a
%{_prefix}/%{_lib}/cmake/bpp-seq/bpp-seq*.cmake
%{_prefix}/include/*

%changelog
* Fri Aug 10 2018 Julien Dutheil <julien.dutheil@univ-montp2.fr> 2.4.1-1
- Compatibility update gcc8
* Fri Mar 03 2018 Julien Dutheil <julien.dutheil@univ-montp2.fr> 2.4.0-1
- Increased interface number
- Removed dynamic exceptions declarations.
* Tue Jun 06 2017 Julien Dutheil <julien.dutheil@univ-montp2.fr> 2.3.1-1
- Increased interface number
* Wed May 10 2017 Julien Dutheil <julien.dutheil@univ-montp2.fr> 2.3.0-1
- Several bugs fixed and performance improvements
- New framework for probabilistic sequences
- Upgrade to C++11
* Thu Sep 23 2014 Julien Dutheil <julien.dutheil@univ-montp2.fr> 2.2.0-1
- Alphabet classes refreshed and updated
- Bug fixes.
* Tue Mar 05 2013 Julien Dutheil <julien.dutheil@univ-montp2.fr> 2.1.0-1
- 'omics' tools now in bpp-seq-omics
- Extended BppO support
- StateProperties renamed to AlphabetIndex
* Thu Feb 09 2012 Julien Dutheil <julien.dutheil@univ-montp2.fr> 2.0.3-1
- Improved maf tools + new iterators
- Added support for GFF
- Added support for FastQ
- Several bugs fixed.
* Thu Jun 09 2011 Julien Dutheil <julien.dutheil@univ-montp2.fr> 2.0.2-1
- Improved maf tools, several bugs fixed.
* Mon Feb 28 2011 Julien Dutheil <julien.dutheil@univ-montp2.fr> 2.0.1-1
* Mon Feb 07 2011 Julien Dutheil <julien.dutheil@univ-montp2.fr> 2.0.0-1
* Thu Mar 25 2010 Julien Dutheil <julien.dutheil@univ-montp2.fr> 1.7.0-1
* Wed Jun 10 2009 Julien Dutheil <jdutheil@birc.au.dk> 1.6.0-1
* Thu Dec 11 2008 Julien Dutheil <jdutheil@birc.au.dk> 1.5.0-1
* Mon Jul 21 2008 Julien Dutheil <jdutheil@birc.au.dk> 1.4.1-1
* Fri Jan 18 2008 Julien Dutheil <Julien.Dutheil@univ-montp2.fr> 1.4.0-1
* Fri Jul 06 2007 Julien Dutheil <Julien.Dutheil@univ-montp2.fr> 1.3.0-1
* Fri Jan 19 2007 Julien Dutheil <Julien.Dutheil@univ-montp2.fr> 1.2.0-1
* Mon Aug 28 2006 Julien Dutheil <Julien.Dutheil@univ-montp2.fr> 1.1.1-1
* Tue Apr 18 2006 Julien Dutheil <Julien.Dutheil@univ-montp2.fr> 1.1.0-1
* Fri Nov 16 2005 Julien Dutheil <Julien.Dutheil@univ-montp2.fr> 1.0.0-1
- First draft of the spec file

