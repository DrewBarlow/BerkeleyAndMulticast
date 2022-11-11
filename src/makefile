
compile: machineRule

machineRule: machine/machine.o util/berkeley/berkeley.o util/socks/socks.o
	gcc  -g -pthread -o bin/machineMain machine/machineMain.c machine/machine.o util/berkeley/berkeley.o util/socks/socks.o

machine/machine.o:
	gcc -g -pthread -c machine/machine.c
	mv machine.o machine/.

util/berkeley/berkeley.o:
	gcc -g -pthread -c util/berkeley/berkeley.c
	mv berkeley.o util/berkeley/.

util/socks/socks.o:
	gcc -pthread -g -c util/socks/socks.c
	mv socks.o util/socks/.

rund: compile 
	./bin/machineMain 10000 10000 3

runc: compile 
	./bin/machineMain $(n) 10000 3

clean:
	rm bin/* machine/machine.o util/socks/socks.o util/berkeley/berkeley.o
