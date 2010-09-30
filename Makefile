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

all: setup

setup: makecert
	iconv -f utf-8 -t gb18030 nsis/qomowin-setup.nsi -o dist/qomowin-setup.nsi
	iconv -f utf-8 -t gb18030 nsis/mylang.nsh -o dist/mylang.nsh
	sed -i 's/@PACKAGE@/$(PACKAGE)/; s/@VERSION@/$(VERSION)/; s/@WINVERSION@/$(WINVERSION1)/'       dist/qomowin-setup.nsi
	tools/makensis dist/qomowin-setup.nsi
	mv dist/$(PACKAGE)-$(VERSION).exe .

wubizip: makecert
	cd nsis; zip -r wubi.zip wubi

makecert: qomowin-pre-setup
	cd dist; echo "abc123\nabc123\nabc123\nabc123\n" | ../tools/gencert.sh
	tools/signtool signwizard

qomowin-pre-setup: check_wine qomowin winboot2 grublocale
	rm -rf dist; mkdir -p dist/{locale,bin}
	cp -f wine/drive_c/Program\ Files/7-Zip/7z.{exe,dll} dist/bin
	cp build/src/locale/*dll dist/locale
	cp -rf build/data/qomo-logo.ico build/src/*.exe build/winboot nsis/* dist
	rm -rf build/{winboot,src,data}

grublocale:
	[ -f nsis/boot/grub/unicode.pf2 ] || cp -f /usr/share/grub/unicode.pf2 nsis/boot/grub
	[ -d nsis/boot/grub/locale ] || mkdir -p nsis/boot/grub/locale
	for i in /usr/share/locale/*/LC_MESSAGES/grub.mo; \
		do \
		lang=`echo $$i|cut -d/ -f5`; \
		cp -f $$i nsis/boot/grub/locale/$$lang.mo; \
		done
	 for i in png video vga video_fb bitmap_scale font extcmd bitmap bufio gfxterm vbe gzio gettext; \
		 do \
		 cp /usr/lib/grub/i386-pc/$$i.mod nsis/boot/grub/; \
		 done

qomowin:
	cp -rf data src build
	sed -i 's/@PACKAGE@/$(PACKAGE)/; s/@VERSION@/$(WINVERSION1)/'       build/src/qomowin.exe.manifest
	sed -i 's/@VERSION@/$(WINVERSION1)/; s/@WINVERSION@/$(WINVERSION2)/' build/src/*.rc
	sed -i 's/@VERSION@/$(WINVERSION1)/; s/@WINVERSION@/$(WINVERSION2)/' build/src/locale/*.rc
	make -C build/data
	make -C build/src

winboot2: grubutil
	mkdir -p build/winboot
	cp -f data/qomoldr.cfg data/qomoldr-bootstrap.cfg build/winboot/
	./build/grubutil/grubinst/grubinst --grub2 --boot-file=qomoldr -o build/winboot/qomoldr.mbr
	cd build/winboot && tar cf qomoldr.tar qomoldr.cfg
	grub-mkimage -c build/winboot/qomoldr-bootstrap.cfg -m build/winboot/qomoldr.tar -o build/grubutil/core.img \
		loadenv biosdisk part_msdos part_gpt fat ntfs ext2 ntfscomp iso9660 loopback search linux boot minicmd cat cpuid chain halt help ls reboot \
		echo test configfile normal sleep memdisk tar sh
	cat /usr/lib/grub/i386-pc/lnxboot.img build/grubutil/core.img > build/winboot/qomoldr

grubutil:
	[ -d build/grubutil ] || svn export svn://svn.gna.org/svn/grubutil/trunk build/grubutil
	cd build/grubutil/grubinst; make

osslsigncode:
	[ -d build/osslsigncode-1.3.1 ] || wget -O - http://downloads.sourceforge.net/project/osslsigncode/osslsigncode/1.3.1/osslsigncode-1.3.1.tar.gz|tar -C build/ -xf -
	cd build/osslsigncode-1.3.1; ./configure && make
	#cp -f build/osslsigncode-1.3.1/osslsigncode tools

check_wine: tools/check_wine
	tools/check_wine

clean:
	rm -rf dist/* build/*

.PHONY: all build test wubi wubizip wubi-pre-build pot runpy runbin ckeck_wine unittest
	7z translations version.py pylauncher winboot grubutil grub4dos
