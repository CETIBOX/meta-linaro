DESCRIPTION = "Magic wand library"
HOMEPAGE = "http://docs.wand-py.org/en/0.4.3/"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=170eafd687d4a2b950819cd5e067e6d5"

SRCNAME = "wand"

SRC_URI = "git://github.com/dahlia/wand.git"
SRCREV = "81b2ce5ac117cd69e4894780925fab12ff0383b9"

S = "${WORKDIR}/git"

inherit setuptools

DEPENDS += " imagemagick-native"

BBCLASSEXTEND = "native"
