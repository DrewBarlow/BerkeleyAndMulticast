
compile: machineRuleNormal machineRuleCausal

machineRuleNormal: machine/machine.o util/berkeley/berkeley.o util/normal/multicast.o util/socks/socks.o
	gcc -g -pthread -o bin/machineMainNormal machine/machineMain.c machine/machine.o util/berkeley/berkeley.o util/normal/multicast.o util/socks/socks.o

machineRuleCausal: machine/machine.o util/berkeley/berkeley.o util/causal/multicast.o util/socks/socks.o
	gcc -g -D CAUSAL=1 -pthread -o bin/machineMainCausal machine/machineMain.c machine/machine.o util/berkeley/berkeley.o util/causal/multicast.o util/socks/socks.o

machine/machine.o:
	gcc -g -pthread -c machine/machine.c
	mv machine.o machine/.

util/berkeley/berkeley.o:
	gcc -g -pthread -c util/berkeley/berkeley.c
	mv berkeley.o util/berkeley/.

util/normal/multicast.o:
	gcc -g -pthread -c util/normal/multicast.c
	mv multicast.o util/normal/.

util/causal/multicast.o:
	gcc -g -pthread -c util/causal/multicast.c
	mv multicast.o util/causal/.

util/socks/socks.o:
	gcc -pthread -g -c util/socks/socks.c
	mv socks.o util/socks/.

runn: machineRuleNormal 
	python3 spinMachines.py $(n)

runc: machineRuleCausal 
	python3 spinMachines.py $(n) -c

clean:
	rm bin/* machine/machine.o util/socks/socks.o util/causal/multicast.o util/normal/multicast.o util/berkeley/berkeley.o
