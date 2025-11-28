CXXFLAGS = -Wall -Wextra -ggdb

CXX = g++

SRC_FOLDER = src
BUILD_FOLDER = build
BIN_BOLDER = bin
TARGET = $(BIN_BOLDER)/slr

SRC_EXT := cpp
SOURCES := $(wildcard $(SRC_FOLDER)/*.$(SRC_EXT))
OBJECTS := $(patsubst $(SRC_FOLDER)/%,$(BUILD_FOLDER)/%,$(SOURCES:.$(SRC_EXT)=.o))

$(TARGET) : $(OBJECTS)
	@mkdir -p $(BIN_BOLDER)
	@echo " Linking..."; $(CXX) $(CXXFLAGS) $^ -o $(TARGET)
	@echo " Done!"

$(BUILD_FOLDER)/%.o : $(SRC_FOLDER)/%.$(SRC_EXT)
	@mkdir -p $(BUILD_FOLDER)
	@echo " Generating: $@"; $(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; rm -r $(BUILDDIR) $(TARGET)
	@echo " Done!"

info:
	@echo " Info..."
	@echo " SOURCES = $(SOURCES)"
	@echo " OBJECTS = $(OBJECTS)"