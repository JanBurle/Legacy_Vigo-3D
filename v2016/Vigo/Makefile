.PHONY: all vigo check dox clean help

all vigo check:
	$(MAKE) -f vigo.mak $@

dox:
	doxygen

clean:
	$(MAKE) -f vigo.mak $@ OMIT_DEPS=1

help:
	echo "usage: make [all|vigo|check|dox|clean]"

# eof
