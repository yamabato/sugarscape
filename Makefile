CC     = clang
CFLAGS = -Wall -g
LIBS   = -lm

TARGET = sugar
OBJS   = sugar.o show.o util.o update.o initialize.o agent.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

sugar.o: sugar.c sugar.h setting.h util.h show.h agent.h
util.o: util.c util.h
show.o: show.c show.h sugar.h
update.o: update.c update.h sugar.h util.h
initialize.o: initialize.c initialize.h agent.h sugar.h
agent.o: agent.c agent.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o

celanall: clean
	rm -f $(TARGET)
