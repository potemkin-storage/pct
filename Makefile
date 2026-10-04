# kiwinatra, 2026 (c)

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
INCLUDES  = -I. -Isrc/utils/include -Isrc/commands/include

SRC = main.cpp parser.cpp \
      src/utils/fs.cpp src/utils/utils.cpp \
      src/commands/pull.cpp src/commands/update.cpp src/commands/remove.cpp

OBJ = $(SRC:.cpp=.o)

BIN = pct

.PHONY: all clean install

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

cleano:
	rm -f $(OBJ)

install: $(BIN)
	install -m 755 $(BIN) $(DESTDIR)/usr/local/bin/$(BIN)