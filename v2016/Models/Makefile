.PHONY: all clean

MODELS = Tonnetz

all clean:
	for d in $(MODELS); do echo "**** " $$d " ****"; (cd $$d && $(MAKE) $@); done

# eof
