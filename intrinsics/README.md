# Intrinsics (UNSTABLE)

Intrinsics in logia are stored in a LLVM IR file. This file is loaded at
backend, it could be considered as the logia runtime but is thinner.

Logia compiler rely heavely in intrinsics and compile time multiple dispatch to
select the right functionality.

Intrinsics are in `cpp` because `clang` LLVM IR is cleaner than `c` counterpart,
even with the same code.

`logia` is not freestanding and it's not an reallistic objetive but we should
focus on having very little libc dependency.

## TODO

- force inline
- data layout and other considerations to make it portable -> maybe multiple
  modules!

# Generate cpp files

Some `cpp` intrinsics file are generated.

```
& .\intrinsics.ps1
```

This will generate [`./intrinsics.ll`](./intrinsics.ll) that it's the module.

## libc

- https://gist.github.com/PewZ/8b473c2a6888c5c528635550d07c6186
- https://www.ibm.com/docs/en/i/7.5.0?topic=extensions-standard-c-library-functions-table-by-name

Here is the list of functions that are used and why.

```
// stdio.h
size_t fwrite (const void *data, size_t size, size_t count, FILE *stream);
```

fwrite is used to write to stdout/stderr

```
// stdio.h
int fputs(const char *string, FILE *stream);
```

fwrite is used to write to stdout/stderr

# Literature

Here some notes to continue learning about what other languages do.

## optimizations

- https://github.com/nadavrot/memset_benchmark
