DESCRIPTION = "open source processor emulator"
HOMEPAGE = "http://qemu.org"
LICENSE = "GPLv2 & LGPLv2.1"
DEPENDS = "glib-2.0 zlib alsa-lib virtual/libx11 pixman dtc libsdl"
DEPENDS_class-native = "zlib-native alsa-lib-native glib-2.0-native pixman-native dtc-native"
DEPENDS_class-nativesdk = "nativesdk-zlib nativesdk-libsdl nativesdk-glib-2.0 nativesdk-pixman nativesdk-dtc"
RDEPENDS_${PN}_class-nativesdk = "nativesdk-libsdl"

require qemu-targets.inc
inherit autotools
BBCLASSEXTEND = "native nativesdk"

# QEMU_TARGETS is overridable variable
QEMU_TARGETS ?= "arm i386 mips mipsel mips64 mips64el ppc sh4 x86_64"

SDL ?= ""
SDL_class-native ?= ""
SDL_class-nativesdk ?= ""

SRC_URI = "\
    file://powerpc_rom.bin \
    file://no-strip.patch \
    file://fix-configure-checks.patch \
    file://larger_default_ram_size.patch \
    "

SRC_URI_append_class-nativesdk = "\
    file://relocatable_sdk.patch \
    "

SRC_URI_append_class-native = "\
    file://fix-libcap-header-issue-on-some-distro.patch \
    "

EXTRA_OECONF += "--target-list=${@get_qemu_target_list(d)} --disable-werror --disable-vnc-tls --audio-drv-list=oss,alsa --disable-curl --disable-vnc-jpeg --disable-bluez --with-system-pixman"

EXTRA_OECONF_class-nativesdk = "--target-list=${@get_qemu_target_list(d)} --disable-werror --disable-vnc-tls \
				--disable-curl \
				"

do_configure() {
    rm -f ${S}/include/libfdt_env.h
    ${S}/configure --prefix=${prefix} --sysconfdir=${sysconfdir} --libexecdir=${libexecdir} --disable-strip ${EXTRA_OECONF} $KVMOPTS
}

do_install () {
	export STRIP="true"
	autotools_do_install
	install -d ${D}${datadir}/qemu
	install -m 0755 ${WORKDIR}/powerpc_rom.bin ${D}${datadir}/qemu||true
}

# The following fragment will create a wrapper for qemu-mips user emulation
# binary in order to work around a segmentation fault issue. Basically, by
# default, the reserved virtual address space for 32-on-64 bit is set to 4GB.
# This will trigger a MMU access fault in the virtual CPU. With this change,
# the qemu-mips works fine.
# IMPORTANT: This piece needs to be removed once the root cause is fixed!
do_install_append() {
	if [ -e "${D}/${bindir}/qemu-mips" ]; then
		create_wrapper ${D}/${bindir}/qemu-mips \
			QEMU_RESERVED_VA=0x0
	fi
}
# END of qemu-mips workaround

PACKAGECONFIG ??= ""

# Qemu target will not build in world build for ARM or Mips
BROKEN_qemuarm = "1"
BROKEN_qemumips = "1"

INSANE_SKIP_${PN} = "arch"