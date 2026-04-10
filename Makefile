CC     = clang
CFLAGS = -Wall -g
LIBS   = -lm

TARGET = sugar
OBJS   = sugar.o show.o util.o update.o initialize.o agent.o option.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

sugar.o: sugar.c sugar.h setting.h util.h show.h agent.h update.h initialize.h option.h
util.o: util.c util.h
show.o: show.c show.h sugar.h
update.o: update.c update.h sugar.h util.h initialize.h agent.h
initialize.o: initialize.c initialize.h agent.h sugar.h util.h
agent.o: agent.c agent.h
option.o: option.c option.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o

cleanall: clean
	rm -f $(TARGET)
