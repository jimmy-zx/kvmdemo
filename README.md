# kvmdemo

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
