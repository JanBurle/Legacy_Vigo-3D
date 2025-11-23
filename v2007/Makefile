.PHONY: all clean

DIRS = Vigo Examples Models ProjectTemplate

all clean:
	for d in $(DIRS); do echo "**** " $$d " ****"; (cd $$d && $(MAKE) $@); done

# eof
