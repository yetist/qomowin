ICON := qomo-logo.ico

all: icon

icon: qomo16x16.png qomo32x32.png qomo48x48.png
	png2ico $(ICON) --colors 256 $^

clean:
	rm -f $(ICON)

.PHONY: all clean icon
