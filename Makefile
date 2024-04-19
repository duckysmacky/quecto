TARGET = quecto
CC = gcc
CFLAGS = -std=c99 -Wall -I.
LINKER = gcc
LFLAGS = -Wall -I. -lm

ifeq ($(OS),Windows_NT)
	SRCS := $(shell where.exe /r src *.c*)
else
	SRCS := $(shell find src -name '*.c')
endif

OBJS := $(SRCS:src/%.c=obj/%.o)
# OBJS := obj/input.o obj/output.o

RM = rm -f

all: $(TARGET)

$(TARGET): $(OBJS)
ifeq ($(OS),Windows_NT)
	$(LINKER) $(LFLAGS) $(OBJS) -o $@ -lncurses -DNCURSES_STATIC
else
	@$(LINKER) $(LFLAGS) $(OBJS) -o $@ -lcurses
endif
	@echo "Linking complete!"

obj/%.o: src/%.c
	@mkdir $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compilation complete!"

clean:
	@$(RM) $(OBJS) $(TARGET)
	@echo "Cleanup complete!"
