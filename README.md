# kvmdemo

## 

1. We've built a mini hypervisor based on `kvm`, which runs the binary `guest.bin`
by default. You may optional review `kvmutil.{h,c}`, but this is not required.

The hypervisor loads the guest binary `guest.bin`. When the virtual CPU encounters
some instruction that it cannot proceed (in this lab we only care about I/O),
the virtual machine will [trap into the hypervisor](https://github.com/jimmy-zx/kvmdemo/blob/652cb499cc58d5bbc2c96ca676d47200d4626fb0/main.c#L64), providing an `exit_reason`
to hint how the hypervisor should handle the trap. We care about the case for
`KVM_EXIT_IO`, which is handled in [`io_handler`](https://github.com/jimmy-zx/kvmdemo/blob/652cb499cc58d5bbc2c96ca676d47200d4626fb0/main.c#L30).

1. There are some buffer overflow vulunerbility in the hypervisor `main.c`.
Identify this and modify the guest application `guest.S` to force
the hypervisor to call the function `exploit()`.

You should NOT modify any part of the hypervisor.

2. Implement a handler for handling a guest request to read a character from
the hypervisor's STDIN. The guest should be able to use `inb $0x30,[register]`
to read a character from the hypervisor's STDIN. Make sure to verify
your implementation using your own guest, but do not submit your modified guest.

The two tasks above are independent of each other.

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
- This project only works for `x86-64`. If you are using a virtual machine
    to run this project, make sure nested virtualization is enabled.

## Hints

[Useful link](https://sourceware.org/binutils/docs/as/Pseudo-Ops.html)
to construct your payload using GNU as.

[struct kvm_run](https://docs.kernel.org/virt/kvm/api.html#the-kvm-run-structure)

## References

- [kvm-hello-world](https://github.com/dpw/kvm-hello-world/)
- [kvmsample](https://github.com/soulxu/kvmsample)
- [Linux KVM API](https://docs.kernel.org/virt/kvm/api.html)
- [How can I temporarily disable ASLR?](https://askubuntu.com/a/507954)
