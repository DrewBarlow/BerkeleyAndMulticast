
compile: machineRule

machineRule: machine/machine.o util/berkeley/berkeley.o util/multicast/multicast.o util/socks/socks.o
	gcc  -g -pthread -o bin/machineMain machine/machineMain.c machine/machine.o util/berkeley/berkeley.o util/multicast/multicast.o util/socks/socks.o

machine/machine.o:
	gcc -g -pthread -c machine/machine.c
	mv machine.o machine/.

util/berkeley/berkeley.o:
	gcc -g -pthread -c util/berkeley/berkeley.c
	mv berkeley.o util/berkeley/.

util/multicast/multicast.o:
	gcc -g -pthread -c util/multicast/multicast.c
	mv multicast.o util/multicast/.

util/socks/socks.o:
	gcc -pthread -g -c util/socks/socks.c
	mv socks.o util/socks/.

run: compile 
	python3 spinMachines.py $(n)

clean:
	rm bin/* machine/machine.o util/socks/socks.o util/multicast/multicast.o util/berkeley/berkeley.o
