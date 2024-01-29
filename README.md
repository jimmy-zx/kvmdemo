# kvmdemo

## Tasks

1. There are some vulunerbility in the hypervisor `main.c`. Identify this and
modify the guest application `guest.S` to force the hypervisor to call the function `exploit()`.

2. Implement a handler for handling a guest request to read a character from
the hypervisor's STDIN. The guest should be able to use `inb $0x30,[register]`
to read a character from the hypervisor's STDIN.

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

## References

- [kvm-hello-world](https://github.com/dpw/kvm-hello-world/)
- [kvmsample](https://github.com/soulxu/kvmsample)
- [Linux KVM API](https://docs.kernel.org/virt/kvm/api.html)
- [How can I temporarily disable ASLR?](https://askubuntu.com/a/507954)
