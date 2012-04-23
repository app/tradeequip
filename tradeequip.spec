# Portable trade equipment project
# Spec file for building rpm and rpms under ALT
# $Id: tradeequip.spec,v 1.12 2006/07/12 09:16:57 app Exp $
# $Name:  $
#

#%define name1 tradeequip-qt
%define ShortName tradeequip
%define _qtdir %(echo $QTDIR)

Name: tradeequip
Version: 1.1.0
Release: alt0
Buildroot: %{_tmppath}/%{name}-%{version}-root

Summary: Portable trade equipment Applications common library

License: GPL
Group: Office
Url: http://www.leaderit.ru/
Vendor:	Project Portable Trade Equipment
Source:		%name-%version.tar.bz2

#Requires:	qt3 >= 3.2
Requires:       %(if [ -e /etc/altlinux-release ] ; then echo "libqt3 >= 3.2"; else echo "qt >= 3.2.0"; fi)




#BuildRequires: XFree86-devel XFree86-libs fontconfig freetype2 
BuildRequires: gcc-c++ libstdc++-devel
BuildRequires: doxygen tetex-latex tetex-dvips
BuildRequires:  %(if [ -e /etc/altlinux-release ] ; then echo "libqt3-devel"; else echo "qt-devel"; fi)

%description
Portable trade equipment Applications

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

#==============================================================
%package -n tradeequip-devel
Summary: Portable trade equipment Applications

Group: Office
#Requires:	qt3 >= 3.2

%description -n tradeequip-devel
Portable trade equipment Applications

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

#==============================================================
%package -n tradeequip-doc
Summary: Portable trade equipment Applications common library documentation

Group: Office
#Requires:	qt3 >= 3.2

%description -n tradeequip-doc
Portable trade equipment Applications common library documentation

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

%prep
%setup

%build
#export QTDIR=/usr/lib/qt3
export PATH=${PATH}:$QTDIR/bin
make
#make tr

%install
#export QTDIR=/usr/lib/qt3
export PATH=${PATH}:$QTDIR/bin
make doc && make install INSTALL_ROOT="$RPM_BUILD_ROOT" LIBDIR=%_libdir BINDIR=%_bindir DOCDIR=%_docdir/%name-%version INCLUDEDIR=%_includedir/%name

if [ -e /etc/altlinux-release ] ; then

%__mkdir_p %buildroot/%_bindir
%__mkdir_p %buildroot/%_libdir
%__mkdir_p %buildroot/%_sysconfdir/%name
%__mkdir_p %buildroot/%_datadir/%name/translations
%__mkdir_p %buildroot/%_includedir/%name
%__mkdir_p %buildroot/%_docdir/%name-%version/html
%__mkdir_p %buildroot/%_docdir/%name-%version/html-ru

else

mkdir -p %buildroot%_bindir
mkdir -p %buildroot%_libdir
mkdir -p %buildroot%_sysconfdir/%name
mkdir -p %buildroot%_datadir/%name/translations
mkdir -p %buildroot%_includedir/%name
mkdir -p %buildroot%_docdir/%name-%version/html
mkdir -p %buildroot%_docdir/%name-%version/html-ru

fi


cp -df %_builddir/%name-%version/bin/* %buildroot%_bindir
cp -df %_builddir/%name-%version/lib/lib* %buildroot%_libdir
cp -f %_builddir/%name-%version/src/lib/*.h %buildroot%_includedir/%name
cp -f %_builddir/%name-%version/src/libshell/*.h %buildroot%_includedir/%name
cp -f %_builddir/%name-%version/doc/html/* %buildroot%_docdir/%name-%version/html
cp -f %_builddir/%name-%version/doc/html-ru/* %buildroot%_docdir/%name-%version/html-ru
cp -f %_builddir/%name-%version/doc/tradeequip* %buildroot%_docdir/%name-%version

%post

%postun
rm -f %_datadir/%name/translations/*.qm
rm -f %_datadir/%name/templates/*.*

rmdir --ignore-fail-on-non-empty %_datadir/%name/translations
rmdir --ignore-fail-on-non-empty %_datadir/%name/templates
rmdir --ignore-fail-on-non-empty %_sysconfdir/%name
rmdir --ignore-fail-on-non-empty %_datadir/%name

%clean

%files
%_libdir/*
%_bindir/*

%files -n tradeequip-doc
%_docdir/%name-%version/*

%files -n tradeequip-devel
%_includedir/%name/*.h

%changelog
* Wed Jul 12 2006 Andrey Paskal <apaskal at gmail tochka com>
  + Adopted spec for Fedora Core Linux
  
* Sat Mar 25 2006 Valery Grazdankin <leader@altlinux.ru> 1.1.0-alt0
  + Add TEIniFile object
  + Clear code 

* Tue Feb 21 2006 Valery Grazdankin <leader@altlinux.ru> 1.0.0-alt4
  + Change cash registers base class. 

* Wed Feb 15 2006 Valery Grazdankin <leader@altlinux.ru> 1.0.0-alt3
  + Add command line shell 
  + Add plugin system

* Sun Feb 05 2006 Valery Grazdankin <leader@altlinux.ru> 1.0.0-alt2
  + Serial port object updated for posix variant.

* Sat Mar 19 2005 Valery Grazdankin <leader@altlinux.ru> 1.0.0-alt1
  + init

