CFLAGS = -Wall -Og -g
LFLAGS = -lreadline
all: tredis

tredis: tredis.c htable.c
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

.PHONY: run
run:
	./tredis


.PHONY: clean
clean:
	rm tredis
