from argparse import ArgumentParser, Namespace
from glob import glob
from multiprocessing import Process
from os import system 
from sys import stdout
from time import sleep
from typing import List

DAEMON_PORT: int = 10000
EXECUTABLE_PATH: str = "./bin/machineMain"

# YES, I KNOW SYSTEM() IS THE WORST THING EVER
# NO, I DONT CARE 

def main() -> None:
    # take num_args, spawn Pool(n) processes
    parser: ArgumentParser = ArgumentParser()
    print("\nIF RUNNING FROM MAKEFILE, USE:\nmake run n=<num-machines-to-spawn>\n")
    parser.add_argument("n", type=int, help="Number of machines spin up.")
    args: Namespace = parser.parse_args()

    # check that the client file is compiled
    if EXECUTABLE_PATH not in glob("./bin/*"):
        print("Please compile the main executable first.")
        return

    # spawn args.n processes
    processes: List[Process] = []
    for i in range(args.n):
        sleep(1)
        processes.append(Process(target=system, args=(f"{EXECUTABLE_PATH} {DAEMON_PORT + i} {DAEMON_PORT} {args.n}",)))
        processes[i].start()

    for process in processes:
        process.join()

    stdout.flush()
    return

if __name__ == "__main__":
    main()