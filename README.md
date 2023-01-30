## Summary
  * An implementation of the berkeley clock synchronization algorithm, as well as causal and unordered multicast.

## How to compile and run:
  - `cd src/`
  - `make compile`
  - FOR NORMAL MULTICAST: `make runn n=<num-machines-to-spin>`
  - FOR CAUSAL MULTICAST: `make runc n=<num-machines-to-spin>`
  - NOTES:
    - All of my code works under the assumption that leader election has already taken place.
      - This variant of leader election works based on port number, where the LOWEST port wins.
    - The time daemon is the elected leader (always port 10,000).
    - Each "machine" port is `10,000 + i`, where `i` is `0..(n - 1)`.
    - On startup, a machine is spawned once every second for `n` seconds (for random seeding).
    - Because of how I have chosen to spin the machines, a lot of messages are buffered.

