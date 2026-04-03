CC     = clang
CFLAGS = -Wall -g
LIBS   =

TARGET = sugar
OBJS   = sugar.o show.o util.o update.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

sugar.o: sugar.c sugar.h setting.h util.h show.h agent.h
util.o: util.c util.h
show.o: show.c show.h sugar.h
update.o: update.c update.h sugar.h util.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o

celanall: clean
	rm -f $(TARGET)
