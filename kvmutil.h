#include <stddef.h>
#include <linux/kvm.h>

struct vm {
  /* File descriptor for `/dev/kvm` */
  int vm_kvmfd;

  /* File descriptor for the vm */
  int vm_fd;

  /* Memory size */
  size_t vm_mem_size;

  /* The actual memory */
  char *vm_mem;

  /* kvm structure to map user memory to kvm */
  struct kvm_userspace_memory_region vm_mem_region;
};

struct vcpu {
  /* Pointer to which vm this vcpu belongs to */
  struct vm *vm;

  /* File descriptor for the vcpu */
  int vcpu_fd;

  /* ID of the vcpu */
  int vcpu_id;

  /* Size of control structure `vcpu_kvm_run` */
  int vcpu_mmap_size;

  /* Control structure */
  struct kvm_run *vcpu_kvm_run;

  /* Registers of this vcpu */
  struct kvm_regs vcpu_regs;

  /* Special registers of this vcpu */
  struct kvm_sregs vcpu_sregs;
};

/* Creates a new vm. vm->vm_mem_size must have been filled.
 * Returns 0 on success. */
int vm_create(struct vm *vm);

/* Destroys a vm. */
void vm_destroy(struct vm *vm);

/* Creates a new vcpu. vcpu->vm must have been filled.
 * Returns 0 on success. */
int vcpu_create(struct vcpu *vcpu);

/* Destroys a vcpu. */
void vcpu_destroy(struct vcpu *vcpu);

/* Resets a vcpu to its default state. */
int vcpu_reset(struct vcpu *vcpu);
