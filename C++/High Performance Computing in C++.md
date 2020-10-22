# High Performance Computing in C++

[toc]

- Some problems require more computing power than a typical PC/laptop can provide
- Different means of optimization
- Single Core: SIMD
- Many cores (CPUs)
  - Multithreading: declarative (OpenMP), imperative (std::thread, pthreads), library-based (Intel TBB/Microsoft PPL)

- Machine clusters: MPI
- Custom Hardware
  - GPGPU (general purpose GPU): OpenCL, CUDA, C++ AMP
  - H/W accelerators: Intel Xeon Phi, ASIC/FPGA architectures

Uses Intel Parallel Studio; warning: course is Intel-biased

## Single Instruction Multiple Data (SIMD)

### Motivation

Processing instructions have a cost (Number of clock cycles); e.g. add < multiply < divide < square root (cost vary by CPU and data type - float vs. double)

a lot of efforts spent on optimization ($ax^2+bx->x(ax+b)$)

Divide & conquer

SIMD lets us accelerate these calculations in the context of a single CPU core (parallelism in the instruction level)

### Registers

### Instructions

### Inline Assembly

### Intrinsics

### Automatic Vectorization

### Vectorized Libraries

## Open Multi-Processing (OpenMP)

## Message Passing Interface (MPI)

## C++ Accelerated Massive Parallelism (C++ AMP)

## Generative Art Demo