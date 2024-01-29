#include <stddef.h>
#include <linux/kvm.h>

struct vm {
  int vm_kvmfd;  // fd for `/dev/kvm`
  int vm_fd;
  size_t vm_mem_size;
  char *vm_mem;
  struct kvm_userspace_memory_region vm_mem_region;
};

struct vcpu {
  struct vm *vm;
  int vcpu_fd;
  int vcpu_id;
  int vcpu_mmap_size;
  struct kvm_run *vcpu_kvm_run;

  struct kvm_regs vcpu_regs;
  struct kvm_sregs vcpu_sregs;
};

int vm_create(struct vm *vm);
void vm_destroy(struct vm *vm);

int vcpu_create(struct vcpu *vcpu);
void vcpu_destroy(struct vcpu *vcpu);
int vcpu_reset(struct vcpu *vcpu);
