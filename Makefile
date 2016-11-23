.PHONY: run clean gdb

Kaleido := Kaleido

CC := g++

OPENCV_LIBS := -lopencv_core -lopencv_highgui -lopencv_imgproc

CFLAGS := -Wall -Werror
CFLAGS += -Wfatal-errors
CFLAGS += -std=c++11
CFLAGS += -I .
CFLAGS += -g3
CFLAGS += -c

SRC_DIR := src
OBJ_DIR := obj

CPPFILES := $(shell find $(SRC_DIR) -name "*.cpp")
OBJFILES := $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(CPPFILES))

$(Kaleido): $(OBJFILES)
	@echo LD $@
	@$(CC) $(OPENCV_LIBS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo CC $<
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $< -o $@

run: $(Kaleido)
	./$< image.bmp

gdb: $(Kaleido)
	gdb $<

clean:
	-rm -r $(OBJ_DIR)
	-rm $(Kaleido)
