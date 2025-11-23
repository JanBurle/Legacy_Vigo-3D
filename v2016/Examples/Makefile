.PHONY: all clean

# note that 5-odevigo is not included in EXAMPLES (needs ode)
# to build 5-odevigo, cd to 5-odevigo and make ...

EXAMPLES = 1-application 2-gravity 3-neighbours 4-dock 6-boids 7-QtGui

all clean:
	for d in $(EXAMPLES); do echo "**** " $$d " ****"; (cd $$d && $(MAKE) $@); done

# eof
