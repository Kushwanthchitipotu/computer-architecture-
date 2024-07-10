# Cache Simulator Readme

## Overview

This C program simulates the behavior of a cache memory system based on the specifications provided in the "cache.config" and "cache.access" files. The simulator supports various cache parameters, including size, block size, associativity, replacement policy, and write policy. The simulation results, including cache hits or misses, are output to the console.

## Files

1. **cache_simulator.c**: The main C source code file that implements the cache simulator.
2. **cache.config**: Input file specifying cache configuration parameters, including size, block size, associativity, replacement policy, and write policy.
3. **cache.access**: Input file containing the access sequence, indicating read or write accesses along with memory addresses.

## Compilation

To compile the code, use the following command:

```bash
gcc cache_simulator.c -o cache_simulator
```

## Execution

Run the compiled executable with the following command:

```bash
./cache_simulator
```

Ensure that the "cache.config" and "cache.access" files are present in the same directory as the executable.

## Input Files Format

### cache.config

The "cache.config" file should have the following format:

```
SIZE_OF_CACHE BLOCK_SIZE ASSOCIATIVITY REPLACEMENT_POLICY WRITEBACK_POLICY
```

- `SIZE_OF_CACHE`: Total size of the cache in Bytes.
- `BLOCK_SIZE`: Size of each cache block in Bytes.
- `ASSOCIATIVITY`: Associativity value (1 for direct-mapped, 0 for fully associative, any other number for set associative).
- `REPLACEMENT_POLICY`: Replacement policy (FIFO, LRU, RANDOM).
- `WRITEBACK_POLICY`: Write policy (WB for WriteBack, WT for WriteThrough).

### cache.access

The "cache.access" file should have lines in the following format:

```
Mode: Address
```

- `Mode`: Access mode, either R (Read) or W (Write).
- `Address`: Memory address accessed.

## Output

The simulator outputs the results to the console, including the access mode, memory address, set index in hexadecimal, and whether it's a hit or miss.

## Testing

The code has been tested with various scenarios, including different cache configurations, access patterns, and replacement policies. You can modify the "cache.config" and "cache.access" files to test additional cases.

Feel free to explore and extend the code for more complex cache configurations or additional features.

---

Feel free to customize this readme according to your specific needs or additional features you may have implemented.