LIB_NAME = multy~
EXTENSION = pd_linux
BIN_NAME = $(LIB_NAME).$(EXTENSION)
PD_DIR = $(HOME)/Documents/Pd/externals
INSTALL_DIR = $(PD_DIR)/$(LIB_NAME)
PD_CFLAGS = $(shell pkg-config --cflags)
SDL_CFLAGS = $(shell pkg-config --cflags)
SDL_LIBS = $(shell pkg-config --libs sdl2)

%.o: multy.c
	cc -Wall -fPIC -o $@ -c $< $(PD_CFLAGS) $(SDL_CFLAGS)

%.$(EXTENSION): %.o
	cc -rdynamic -shared -o $@ $< -lc -lm $(SDL_LIBS)

all: $(BIN_NAME)
.PHONY: all

install: $(BIN_NAME)
	mkdir -p $(INSTALL_DIR)
	cp -v $(BIN_NAME) $(INSTALL_DIR)
.PHONY: install

clean:
	rm -f $(BIN_NAME)
.PHONY: clean
