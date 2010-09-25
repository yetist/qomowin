export SHELL = sh
PACKAGE = qomowin
ICON = data/qomo-logo.ico
REVISION = $(shell date +%Y%m%d)
#VERSION = $(shell head -n 1 debian/changelog | sed -e "s/^$(PACKAGE) (\(.*\)).*/\1/g")
VERSION = 1.0.0
WINVERSION1 = $(shell echo $(VERSION).1)
WINVERSION2 = $(shell echo $(WINVERSION1) |tr . ,)
COPYRIGHTYEAR = 2010
AUTHOR = Wu Xiaotian
EMAIL = yetist@gmail.com

all: build

build: setup

setup: qomowin-pre-setup
	sed -i 's/@PACKAGE@/$(PACKAGE)/; s/@VERSION@/$(VERSION)/'       dist/qomowin-setup.nsi
	tools/makensis dist/qomowin-setup.nsi
	mv dist/$(PACKAGE)-$(VERSION).exe .

wubizip: qomowin-pre-setup
	cd nsis; zip -r wubi.zip wubi

qomowin-pre-setup: check_wine qomowin winboot2
	rm -rf dist; mkdir -p dist/{locale,bin}
	cp -f wine/drive_c/Program\ Files/7-Zip/7z.{exe,dll} dist/bin
	cp build/src/locale/*dll dist/locale
	cp -rf build/src/*.exe build/winboot nsis/* dist
	rm -rf build/{winboot,src,data}

qomowin:
	cp -rf data src build
	sed -i 's/@PACKAGE@/$(PACKAGE)/; s/@VERSION@/$(WINVERSION1)/'       build/src/qomowin.exe.manifest
	sed -i 's/@VERSION@/$(WINVERSION1)/; s/@WINVERSION@/$(WINVERSION2)/' build/src/*.rc
	sed -i 's/@VERSION@/$(WINVERSION1)/; s/@WINVERSION@/$(WINVERSION2)/' build/src/locale/*.rc
	make -C build/src

winboot2: grubutil
	mkdir -p build/winboot
	cp -f data/qomoldr.cfg data/qomoldr-bootstrap.cfg build/winboot/
	./build/grubutil/grubinst/grubinst --grub2 --boot-file=qomoldr -o build/winboot/qomoldr.mbr
	cd build/winboot && tar cf qomoldr.tar qomoldr.cfg
	grub-mkimage -c build/winboot/qomoldr-bootstrap.cfg -m build/winboot/qomoldr.tar -o build/grubutil/core.img \
		loadenv biosdisk part_msdos part_gpt fat ntfs ext2 ntfscomp iso9660 loopback search linux boot minicmd cat cpuid chain halt help ls reboot \
		echo test configfile normal sleep memdisk tar
	cat /usr/lib/grub/i386-pc/lnxboot.img build/grubutil/core.img > build/winboot/qomoldr

grubutil: 
	[ -d build/grubutil ] || svn export svn://svn.gna.org/svn/grubutil/trunk build/grubutil
	cd build/grubutil/grubinst; make

check_wine: tools/check_wine
	tools/check_wine

clean:
	rm -rf dist/* build/*

.PHONY: all build test wubi wubizip wubi-pre-build pot runpy runbin ckeck_wine unittest
	7z translations version.py pylauncher winboot grubutil grub4dos