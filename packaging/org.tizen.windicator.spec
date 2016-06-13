%define PREFIX /usr/apps/org.tizen.windicator
Name: org.tizen.windicator
Version:    0.0.1
Release:    0
Summary: Tizen W indicator application
Source: %{name}-%{version}.tar.gz
License: Flora
Group: Applications/System

BuildRequires: cmake
BuildRequires: pkgconfig(edje)
BuildRequires: pkgconfig(embryo)
BuildRequires: pkgconfig(ecore)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(appcore-efl)
BuildRequires: pkgconfig(appcore-common)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(feedback)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(deviced)
BuildRequires: pkgconfig(capi-system-device)
BuildRequires: pkgconfig(capi-system-info)
BuildRequires: pkgconfig(appsvc)
BuildRequires: pkgconfig(capi-media-sound-manager)
BuildRequires: pkgconfig(edbus)
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(capi-system-system-settings)
BuildRequires: pkgconfig(capi-ui-efl-util)
BuildRequires: pkgconfig(pkgmgr)
BuildRequires: pkgconfig(pkgmgr-info)
BuildRequires: pkgconfig(aul)
BuildRequires: efl-assist
BuildRequires: efl-assist-devel
BuildRequires: gettext-tools
BuildRequires: edje-bin, embryo-bin

%ifarch %{arm}
%define ARCH arm
%else
%define ARCH emulator
%endif

%description
Notification panel for wearable devices

%prep
%setup -q

%build

%if 0%{?sec_build_binary_debug_enable}
export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"
%endif

%if 0%{?tizen_build_binary_release_type_eng}
export CFLAGS+=" -DTIZEN_ENGINEER_MODE"
export CXXFLAGS+=" -DTIZEN_ENGINEER_MODE"
export FFLAGS+=" -DTIZEN_ENGINEER_MODE"
%endif

%if 0%{?sec_product_feature_telephony_disable}
export CFLAGS+=" -D_TIZEN_3G_ENABLE"
export CXXFLAGS+=" -D_TIZEN_3G_ENABLE"
export FFLAGS+=" -D_TIZEN_3G_ENABLE"
%endif

CFLAGS+=" -fPIC -fpie -O2 "
LDFLAGS+="-Wl,--rpath=%{PREFIX}/lib -Wl,--as-needed -Wl,--hash-style=both"; export LDFLAGS
cmake . -DCMAKE_INSTALL_PREFIX=%{PREFIX} -DARCH=%{ARCH}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}

%make_install
mkdir -p %{buildroot}/opt/usr/apps/org.tizen.windicator

mkdir -p %{buildroot}%{_libdir}/systemd/system/tizen-system.target.wants

%post
/sbin/ldconfig
/usr/bin/signing-client/hash-signer-client.sh -a -d -p platform /usr/apps/org.tizen.windicator

%files
%manifest %{name}.manifest

%defattr(-,root,root,-)
%attr(-,inhouse,inhouse)
%{PREFIX}/bin/*
%{PREFIX}/res/*
/usr/share/packages/*
/usr/share/icons/org.tizen.windicator.png
