# Intrinsics

List of all logia intrinsics functions.

# IR

```
node .\intrinsics\intrinsics.js > .\intrinsics\intrinsics.cpp
& 'C:\Program Files\LLVM\bin\clang++.exe' -S -emit-llvm .\intrinsics\intrinsics.cpp -o .\intrinsics\intrinsics.ll
```

## libc 

* https://gist.github.com/PewZ/8b473c2a6888c5c528635550d07c6186
* https://www.ibm.com/docs/en/i/7.5.0?topic=extensions-standard-c-library-functions-table-by-name

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