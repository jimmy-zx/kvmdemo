# kvmdemo

Estimated time to complete: `30mins`.

## Overview

We've built a mini hypervisor based on `kvm`, which runs the binary `guest.bin`
by default. You may optional review `kvmutil.{h,c}`, but this is not required.

Skim through `main.c`.
The hypervisor loads the guest binary `guest.bin`. When the virtual CPU encounters
some instruction that it cannot proceed,
the virtual machine will [trap into the hypervisor](https://github.com/jimmy-zx/kvmdemo/blob/652cb499cc58d5bbc2c96ca676d47200d4626fb0/main.c#L64), providing an `exit_reason`
to hint how the hypervisor should handle the trap. We only care about the case for
`KVM_EXIT_IO`, which is handled in [`io_handler()`](https://github.com/jimmy-zx/kvmdemo/blob/652cb499cc58d5bbc2c96ca676d47200d4626fb0/main.c#L30).

The IO handler first prints out all the IO-related fields, and dispatches
into different functionality based on `io.direction` (IN/OUT) and `io.port`.
The starter code provides two IO handlers: IN (0x10) and IN (0x20).
See the table below for details.

You may play with `guest.S` to see how the IO handler works. In particular,

- Print a string of your choice.
- Perform some IO that is not defined in the hypervisor.
- Try to crash the hypervisor by calling the print handler IN (0x10) with a non-null
terminated string.

## Tasks

1. There are some buffer overflow vulunerbility in the hypervisor `main.c`.
Identify this and modify the guest application `guest.S` to force
the hypervisor to call the function `exploit()`.

2. Implement a handler for handling a guest request to read a character from
the hypervisor's STDIN. The guest should be able to use `inb $0x30,[register]`
to read a character from the hypervisor's STDIN.

Play with your implementation to confirm if the handler really works. You may
optionally implement a `cat` program that reads from the STDIN and prints
out every character it received.

You should NOT modify any part of the hypervisor.

Task 1 and task 2 are independent of each other.

### What to submit:

- The modified `guest.S` in task 1.
- The modified `main.c` in task 2.

## IO Ports

| I/O | Port | Value | Description |
|-|-|-|-|
| OUT | 0x10 | Pointer to null-terminated string | Print a string to STDOUT of the hypervisor. |
| OUT | 0x20 | ??? | ??? |
| IN | 0x30 | A character | Read a char from the hypervisor. |

## FAQ

- You might want to disable ASLR. See the makefile entry for `make run`.
- This only runs on a Linux machine with permission to access `/dev/kvm`.
    Search for `how to enable kvm` if such device does not exists.
    (Use a lab machine! They have `kvm` enabled and accessable.)
- This project only works for `x86-64`. If you are using a virtual machine
    to run this project, make sure nested virtualization is enabled.

## Hints

[Useful link](https://sourceware.org/binutils/docs/as/Pseudo-Ops.html)
to construct your payload using GNU `as`.

[struct kvm_run](https://docs.kernel.org/virt/kvm/api.html#the-kvm-run-structure)

## References

- [kvm-hello-world](https://github.com/dpw/kvm-hello-world/)
- [kvmsample](https://github.com/soulxu/kvmsample)
- [Linux KVM API](https://docs.kernel.org/virt/kvm/api.html)
- [How can I temporarily disable ASLR?](https://askubuntu.com/a/507954)
