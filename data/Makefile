ICON := qomo-logo.ico

all: cert icon

icon: qomo16x16.png qomo32x32.png qomo48x48.png
	png2ico $(ICON) --colors 256 $^

cert:
	../tools/gencert.sh

clean:
	rm -f $(ICON) *.pem *.spc *.pvk *.pfx

.PHONY: all clean icon cert
