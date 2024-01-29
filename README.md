# CSC427 - Virtual machines

## Introduction

### Why VM

In CSC347:
- Run another O/S with vulunerbility
- Emulate network topology

Generally:
- Kernel development (do not want to crash the working system during debugging)
- Multiple O/S ABI, compatability (Gaming in Windows, Running MS Office on Linux, WSL)
- Multiplexing (Cloud computing like AWS)
- Isolation (Testing a virus in a VM)
- Multiple versions of a shared library

### VM Vendors

- VMWare
- VirtualBox (Oracle)
- QEMU/KVM
- Xen

### Similar technologies

Containers:
- Docker
- LinuX Containers
- K8s
- Chroot
- Wine

### What is a VM

- Virtualizes system resources
- Transparency

But an O/S already does that!

[Image for vm topology](img1.png)

### How to build a VM

Goal: run an O/S kernel

Run in an existing O/S, e.g., the Linux Kernel.

O/S provides CPU multiplexing using processes, but in user mode (Ring 3).

What should we do when the kernel wants an priviledged instruction?
(Setting up interrupt handlers, page tables, etc.)

### Goals

Performance! We want the guest to be run with small overheads.

Interpreting the instructions on the fly is very slow! (Like Python)
We want the binary to be executed on the CPU directly.

### Native execution

#### Virtualize priviledged instruction

When the guest wants to execute some priviledge instruction, it must be
**intercepted** and trapped into the hypervisor so that the request can be
**interpreted** so that the hypervisor does the request for the guest and
pass back the control to the guest.

Ideally the guest should not be aware of this process, transparency.

#### How to virtualize - Traps

When a priviledged instruction is executed in user mode, the CPU will
generate an interrupt (usually Generally Protection) and traps to the kernel.

Problem: some instructions does not get trapped in the original `x86` ISA,
some gets failed silently while others does not need priviledge.

A guest O/S might see nonsense when executing a `SIDT` instruction, which does
not require priviledge.

Ideally, sensitive instruction should be a subset of priviledged instructions.

#### How to virtualize - Modification

Intercept the execution of sensitive instructions, by replacing
them with a call to the hypervisor. (hypercall)

- Binary rewriting: before loading a binary into the memory, examine them
    so that every sensitive instruction is replaced by a hypercall.
    Maintains a cache for performance. (VMWare)
- Customized kernel: rewrite the kernel such that priviledged instructions
    are replaced by a hypercall. (Xen)

### Paravirtualization

Now the CPU is virtualized. What about devices?


