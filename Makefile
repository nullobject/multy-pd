LIB_NAME = multy~
EXTENSION = pd_linux
BIN_NAME = $(LIB_NAME).$(EXTENSION)
PD_FLAGS = $(shell pkg-config --cflags --libs pd)
PD_DIR = $(HOME)/Documents/Pd/externals
INSTALL_DIR = $(PD_DIR)/$(LIB_NAME)

%.o: multy.c
	cc -Wall -fPIC -o $@ -c $< $(PD_FLAGS)

%.$(EXTENSION): %.o
	cc -rdynamic -shared -o $@ $< -lc -lm

all: $(BIN_NAME)
.PHONY: all

install: $(BIN_NAME)
	mkdir -p $(INSTALL_DIR)
	cp -v $(BIN_NAME) $(INSTALL_DIR)
.PHONY: install

clean:
	rm -f $(BIN_NAME)
.PHONY: clean
