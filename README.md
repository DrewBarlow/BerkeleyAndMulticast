# CMSC621 Project 2
## Drew Barlow (JU08810)

#### How to compile and run:
  - `cd src/`
  - `make compile`
  - `make run n=<num-machines-to-spin>`
  - NOTES:
    - All of my code works under the assumption that leader election has already taken place.
      - This variant of leader election works based on port number, where the LOWEST port wins.
    - The time daemon is the elected leader (always port 10,000).
    - Each "machine" port is `10,000 + i`, where `i` is `0..(n - 1)`.
    - On startup, a machine is spawned once every second for `n` seconds (for random seeding).

#### GOALS:
- Start the system
- Each node synchronizes logical clocks to same initial value
  - Use the algorithm discussed in class
- Implement distributed locking scheme to prevent the nodes
    from editing a shared file at the same time.
  - Use centralized, decentralized, or distributed algorithm to do this

3. Use two threads per process: one to send and one to receive.
  - Use vector clocks to enforce ordering
  - After delivering a message, print out the message on the screen.
  - Assume number of processes is fixed (>= 3), and won't fail/leave or join.
  - Implement two version: one without causaully ordered multicasting and one with.
  - Compare the results.

4. Actually add the feature of distributed locking to the system.
  - File only contains a counter that can be read and updated by a process.
  - Implement require() and release() on a lock variable.
  - Initially, each process tries to open and write to the variable in the file
    and verify it in the update (?).
  - Critical section:
    - point the file offset to the counter
    - update the counter
    - read and print counter value
  - Can use any mutual exclusion algorithm for this.
  - Expected result: read counter value always matches value of updated process if locking enabled.
