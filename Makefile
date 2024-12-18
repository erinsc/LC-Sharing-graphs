RM     :=  rm -f
CC     :=  gcc
CFLAGS :=  -std=c11 -W -Wall -Wextra -Wpedantic -g
LFLAGS :=

TARGET := LC
SRCS   := $(wildcard src/*.c)
OBJS   := $(patsubst src/%.c,obj/%.o,$(SRCS))
DEPS   := $(patsubst src/%.c,obj/%.d,$(SRCS))

-include $(DEPS)

.PHONY: all clean run compile

run: $(TARGET)
	./$(TARGET)

compile: $(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LFLAGS)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) -c $< -o $@ $(CFLAGS)

