TARGET = quecto
CC = gcc
CFLAGS = -std=c99 -Wall -I.
LINKER = gcc
LFLAGS = -Wall -I. -lm

SRCS := $(shell find src -name '*.c')
OBJS := $(SRCS:src/%.c=obj/%.o)

RM = rm -f

all: $(TARGET)

$(TARGET): $(OBJS)
	@$(LINKER) $(LFLAGS) $(OBJS) -o $@ -lcurses
	@echo "Linking complete!"

obj/%.o: src/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compilation complete!"

clean:
	@$(RM) $(OBJS) $(TARGET)
	@echo "Cleanup complete!"
