# Quick Solution

## Overview

This challenge is written in Dart as suggested by the title "DART.Y" and several strings in the supplied snapshot:

```
dart:isolate-patch/isolate_patch.dart
dart.isolate
dart:developer-patch/profiler.dart
dart:core-patch/core_patch.dart
dart:io-patch/file_patch.dart
```

The file `pico.aot` is a Dart AOT snapshot compiled for Linux 64 bit:

```
$ file pico.aot 
pico.aot: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, BuildID[md5/uuid]=f167016dea44f5a09e5534cb4aad7e5f, with debug_info, not stripped

$ python3 ~/git/misc-code/flutter/flutter-header.py -i pico.aot
========== Flutter Snapshot Header Parser ============
Snapshot
	offset  = 512 (0x200)
	size    = 3519
	kind    = SnapshotKindEnum.kMessage
	version = 2.19.1
	features= product no-code_comments no-dwarf_stack_traces_mode no-lazy_dispatchers dedup_instructions no-asserts x64 linux no-compressed-pointers null-safety
-----------
Snapshot
...

```

To run a Dart AOT snapshot, use `dartaotruntime` from Flutter (`FLUTTER_SDK/bin/cache/dart-sdk/bin/dartaotruntime`):

```
====== DART.Y - Your Secure & Smart Fridge ======
Password: flag
Content		 | Index
---------------- | -------------------
deli		 | 966
ph0wn{		 | 967
{		 | 563
...
chartreuse		 | 1014
chamrousse		 | 1015
The door is locked
=====================
```

We are expected to enter a password to unlock the door of the fridge.
Additonally, a list of strings with indexes are printed.

## Reversing

We use Radare2 on `pico.aot` and analyze the sample `aaa`.
We head to the main:

```
[0x00058000]> afl~main
0x000ae370    6    191 main
0x000b0ff4    3     33 sym.main_1
[0x00058000]> s 0x000ae370
```

It calls an intriguing function names `createFlag`:

```
call sym.stdin
push rax
call sym.Stdin.readLineSync
pop rcx
mov qword [var_8h], rax
call sym.createFlag
```

We disassemble `sym.createFlag`:

```
[0x00058000]> s sym.createFlag
[0x000ae430]> pdf
```

### Overview of createFlag

The function:

- Prints 2 lines to console (`sym.printToConsole`)
- Creates an array where it pushes several strings from the Object Pool

```
0x000ae467         41ba70000000  mov r10d, 0x70
0x000ae46d           e89270ffff  call sym.stub__iso_stub_AllocateArrayStub
0x000ae472             488945f8  mov qword [rbp - 8], rax
0x000ae476       4d8b9f371e0000  mov r11, qword [r15 + 0x1e37]
0x000ae47d             4c895817  mov qword [rax + 0x17], r11
```

- Creates another array where it pushes integers

```
0x000ae763         41ba70000000  mov r10d, 0x70
0x000ae769           e8966dffff  call sym.stub__iso_stub_AllocateArrayStub
0x000ae76e             488945e8  mov qword [rbp - 0x18], rax
0x000ae772         41bb8c070000  mov r11d, 0x78c
0x000ae778             4c895817  mov qword [rax + 0x17], r11
```

- Loop where you print stuff

```
0x000aeac4           e8af010000  call sym._IOSinkImpl.write
0x000aeac9                   59  pop rcx
0x000aeaca                   59  pop rcx
0x000aeacb               ff75e0  push qword [rbp - 0x20]
0x000aeace       4d8b9fcf1f0000  mov r11, qword [r15 + 0x1fcf]
0x000aead5                 4153  push r11
0x000aead7           e89c010000  call sym._IOSinkImpl.write
```

- Several calls to `fcn.000783e8`, with 2 arguments being strings of the Object Pool. So, we can assume the function is a concatenation function.

```
0x000aeb0d       4d8b9f5f1e0000  mov r11, qword [r15 + 0x1e5f]
0x000aeb14                 4153  push r11
0x000aeb16       4d8b9f8f1e0000  mov r11, qword [r15 + 0x1e8f]
0x000aeb1d                 4153  push r11
0x000aeb1f           e8c498fcff  call fcn.000783e8
0x000aeb24                   59  pop rcx
```

- Finally returns

```
0x000aec56                   59  pop rcx
0x000aec57                   59  pop rcx
0x000aec58               4889ec  mov rsp, rbp
0x000aec5b                   5d  pop rbp
0x000aec5c                   c3  ret
```

### R15, Object Pool

In Dart, all constant strings are grouped in an Object Pool and later accessed indirectly by the index. Dart uses a dedicated register to access the Object Pool: R15. 

References: 

- https://www.pnfsoftware.com/blog/dart-aot-snapshot-helper-plugin-to-better-analyze-flutter-based-apps/
- https://www.guardsquare.com/blog/current-state-and-future-of-reversing-flutter-apps
- https://blog.tst.sh/reverse-engineering-flutter-apps-part-1/
- https://medium.com/@cryptax/darts-custom-calling-convention-8aa96647dcc6

Therefore, the following lines access an element of the Object Pool at a given index.

```
0x000aeb0d       4d8b9f5f1e0000  mov r11, qword [r15 + 0x1e5f]
0x000aeb14                 4153  push r11
```

The index is computed `0x1e5f // 8` (the fact we need to divide by 8 can be found in the PNF Software blog post for example, on in my own).

So, we are actually retrieving index 971. Fortunately, the program tells us what is at what index:

```
croco		 | 970
GH23{		 | 971
caviar		 | 972
```

Index 971 is `GH23{`, which is very conveniently the beginning of a GreHack flag :)

### Uncovering the rest of the flag


`mov r11, qword [r15 + 0x1e8f]` corresponds to `;`
`mov r11, qword [r15 + 0x1f1f]` corresponds to `s`
`mov r11, qword [r15 + 0x1f37]` corresponds to `lurp`

If you search for each string one by one and concatenate them all you get the flag:

`GH23{_slurp_it_s_delicious_with_some_lobster!}`
