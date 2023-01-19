CC = g++
CFLAGS = -O3

all: randtrack randtrack_global_lock randtrack_list_lock randtrack_element_lock randtrack_reduction

randtrack: list.h hash.h defs.h randtrack.cc
	$(CC) $(CFLAGS) -D DEFAULT randtrack.cc -o randtrack -lpthread

randtrack_global_lock: list.h hash.h defs.h randtrack.cc
	$(CC) $(CFLAGS) -D GLOBAL_LOCK randtrack.cc -o randtrack_global_lock -lpthread

randtrack_list_lock: list.h hash.h defs.h randtrack.cc
	$(CC) $(CFLAGS) -D LLEVEL_LOCK randtrack.cc -o randtrack_list_lock -lpthread

randtrack_element_lock: list.h hash.h defs.h randtrack.cc
	$(CC) $(CFLAGS) -D ELEVEL_LOCK randtrack.cc -o randtrack_element_lock -lpthread

randtrack_reduction: list.h hash.h defs.h randtrack.cc
	$(CC) $(CFLAGS) -D RAND_REDUCTION randtrack.cc -o randtrack_reduction -lpthread

clean:
	rm -f *.o randtrack randtrack_global_lock randtrack_list_lock randtrack_element_lock randtrack_reduction
