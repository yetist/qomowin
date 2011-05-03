export SHELL = sh
PACKAGE = qomowin
ICON = data/qomo-logo.ico
REVISION = $(shell date +%Y%m%d)
#VERSION = $(shell head -n 1 debian/changelog | sed -e "s/^$(PACKAGE) (\(.*\)).*/\1/g")
GRUB_MKIMAGE := grub-mkimage
VERSION = 1.0.1
WINVERSION1 = $(shell echo $(VERSION).1)
WINVERSION2 = $(shell echo $(WINVERSION1) |tr . ,)
COPYRIGHTYEAR = 2010
AUTHOR = Wu Xiaotian
EMAIL = yetist@gmail.com

all: setup

setup: qomowin-pre-setup
	iconv -f utf-8 -t gb18030 nsis/qomowin-setup.nsi -o dist/qomowin-setup.nsi
	iconv -f utf-8 -t gb18030 nsis/mylang.nsh -o dist/mylang.nsh
	sed -i 's/@PACKAGE@/$(PACKAGE)/; s/@VERSION@/$(VERSION)/; s/@WINVERSION@/$(WINVERSION1)/'       dist/qomowin-setup.nsi
	cp data/qomowin.ini dist
	cp data/grub.cfg.in dist
	tools/makensis dist/qomowin-setup.nsi
	#tools/osslsigncode -spc dist/yetist.spc -key dist/yetist.der -n "ssss" -in dist/$(PACKAGE)-$(VERSION).exe -out $(PACKAGE)-$(VERSION).exe
	cp -f dist/$(PACKAGE)-$(VERSION).exe $(PACKAGE)-$(VERSION).exe

wubizip: qomowin-pre-setup
	cd nsis; zip -r wubi.zip wubi

qomowin-pre-setup: check_wine winboot2 qomowin grublocale
	rm -rf dist
	mkdir -p dist/{locale,bin}
	#cd dist; ../tools/gencert.sh
	#tools/osslsigncode -spc dist/yetist.spc -key dist/yetist.der -n "ssss" -in build/src/qomowin.exe -out dist/qomowin.exe
	#tools/osslsigncode -spc dist/yetist.spc -key dist/yetist.der -n "ssss" -in build/src/locale/zh_CN.dll -out dist/locale/zh_CN.dll
	cp -f  build/src/qomowin.exe dist/qomowin.exe
	cp -f build/src/locale/zh_CN.dll dist/locale/zh_CN.dll
	cp -f wine/drive_c/Program\ Files/7-Zip/7z.{exe,dll} dist/bin
	cp -f tools/syslinux.exe dist/bin
	cp -rf build/data/qomo-logo.ico build/winboot nsis/* dist
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
		 cp /usr/lib/grub*/i386-pc/$$i.mod nsis/boot/grub/; \
		 done

qomowin:
	cp -rf data src build
	sed -i 's/@PACKAGE@/$(PACKAGE)/; s/@VERSION@/$(WINVERSION1)/'       build/src/qomowin.exe.manifest
	sed -i 's/@VERSION@/$(WINVERSION1)/; s/@WINVERSION@/$(WINVERSION2)/' build/src/*.rc
	sed -i 's/@VERSION@/$(WINVERSION1)/; s/@WINVERSION@/$(WINVERSION2)/' build/src/locale/*.rc
	make -C build/data
	make -C build/src

winboot2:
	mkdir -p build/winboot
	cp -f data/qomoldr.cfg build/winboot/
	tools/grubinst --grub2 --boot-file=qomoldr -o build/winboot/qomoldr.mbr
	cd build/winboot && tar cf qomoldr.tar qomoldr.cfg
	$(GRUB_MKIMAGE) -O i386-pc -c data/qomoldr-bootstrap.cfg -m build/winboot/qomoldr.tar -o build/winboot/core.img \
		loadenv biosdisk part_msdos part_gpt fat ntfs ext2 ntfscomp iso9660 loopback search linux boot minicmd cat cpuid chain halt help ls reboot \
		echo test configfile normal sleep memdisk tar sh
	cat /usr/lib/grub*/i386-pc/lnxboot.img build/winboot/core.img > build/winboot/qomoldr

check_wine: tools/check_wine
	tools/check_wine

clean:
	rm -rf dist/* build/*

.PHONY: all build test wubi wubizip wubi-pre-build pot runpy runbin ckeck_wine unittest
	7z translations version.py pylauncher winboot grubutil grub4dos
