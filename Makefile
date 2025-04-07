LIB_NAME = multy~
EXTENSION = pd_linux
BIN_NAME = $(LIB_NAME).$(EXTENSION)
PD_DIR = $(HOME)/Documents/Pd/externals
INSTALL_DIR = $(PD_DIR)/$(LIB_NAME)
PD_CFLAGS = $(shell pkg-config --cflags pd)
SDL_CFLAGS = $(shell pkg-config --cflags SDL2_gfx)
SDL_LIBS = $(shell pkg-config --libs SDL2_gfx)

all: $(BIN_NAME)
.PHONY: all

install: $(BIN_NAME)
	mkdir -p $(INSTALL_DIR)
	cp -v $(BIN_NAME) $(INSTALL_DIR)
.PHONY: install

debug:
	gf2 --args `which puredata` -nrt test.pd &
.PHONY: debug

clean:
	rm -f $(BIN_NAME) multy.o
.PHONY: clean

multy.o: multy.c
	cc -Wall -fPIC -ggdb -o $@ -c $< $(PD_CFLAGS) $(SDL_CFLAGS)

$(BIN_NAME): multy.o
	cc -rdynamic -shared -o $@ $< -lc -lm $(SDL_LIBS)
