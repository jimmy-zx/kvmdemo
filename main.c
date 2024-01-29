#define _GNU_SOURCE

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/kvm.h>

#include "kvmutil.h"

#define PORT_PRINT 0x10
#define PORT_BUGGY_PRINT 0x20

static void exploit() {
  printf("This hypervisor has been exploited.\n");
}

void buggy_print(struct vcpu *vcpu) {
  char buf[16];
  void *data = (char *) vcpu->vcpu_kvm_run + vcpu->vcpu_kvm_run->io.data_offset;

  // For some strange reason this handler decides to buffer the data.
  strcpy(buf, vcpu->vm->vm_mem + *(__u16 *) data);

  printf("%s\n", buf);
}

int io_handler(struct vcpu *vcpu) {
  void *data = (char *) vcpu->vcpu_kvm_run + vcpu->vcpu_kvm_run->io.data_offset;
  printf("io_handler: dir=%d sz=%d port=%d count=%d offset=%lld, data=%x\n",
      vcpu->vcpu_kvm_run->io.direction,
      vcpu->vcpu_kvm_run->io.size,
      vcpu->vcpu_kvm_run->io.port,
      vcpu->vcpu_kvm_run->io.count,
      vcpu->vcpu_kvm_run->io.data_offset,
      *(int *) data
      );
  if (vcpu->vcpu_kvm_run->io.direction == KVM_EXIT_IO_IN) {
    return -1;
  } else if (vcpu->vcpu_kvm_run->io.direction == KVM_EXIT_IO_OUT) {
    switch (vcpu->vcpu_kvm_run->io.port) {
      case PORT_PRINT:
        printf("%s\n", vcpu->vm->vm_mem + *(__u16 *) data);
        break;
      case PORT_BUGGY_PRINT:
        buggy_print(vcpu);
        break;
    }
  }
  return 0;
}

void *vcpu_thread(void *data) {
  struct vcpu *vcpu = data;
  vcpu_reset(vcpu);
  for (;;) {
    printf("vcpu_thread: running %d\n", vcpu->vcpu_id);
    if ((ioctl(vcpu->vcpu_fd, KVM_RUN, 0)) < 0) {
      perror("KVM_RUN");
      return (void *) -1;
    }
    switch (vcpu->vcpu_kvm_run->exit_reason) {
      case KVM_EXIT_IO:
        if (io_handler(vcpu) < 0) {
          fprintf(stderr, "io_handler failed, panicing\n");
          return (void *) -1;
        }
        break;
      case KVM_EXIT_INTR:
      case KVM_EXIT_SHUTDOWN:
      case KVM_EXIT_HLT:
        printf("vcpu_thread: %d shutting down gracefully\n", vcpu->vcpu_id);
        return (void *) 0;
      default:
        fprintf(stderr, "kvm exited for unknown (%d), panicing\n", vcpu->vcpu_kvm_run->exit_reason);
        return (void *) -1;
    }
  }
  return NULL;
}

int load_binary(struct vm *vm, const char *path) {
  int fd = open(path, O_RDONLY);
  char *dest = vm->vm_mem;
  ssize_t r;
  if (fd < 0) {
    perror("open");
    return -1;
  }
  while ((r = read(fd, dest, 4096)) > 0) {
    dest += r;
  }
  close(fd);
  return 0;
}

int main() {
  // A helper to help you to find the address of the exploited function.
  // This would always leads to a warning.
  // COMMENT IT OUT BEFORE YOUR SUBMISSION!

  printf("exploit() located at %p\n", (void *) exploit);

  struct vm vm = {.vm_mem_size = 0x200000 };
  struct vcpu vcpu = {.vm = &vm, .vcpu_id = 0};
  if (vm_create(&vm) != 0) {
    fprintf(stderr, "Failed to create vm\n");
    return -1;
  }
  if (vcpu_create(&vcpu) != 0) {
    fprintf(stderr, "Failed to create vcpu\n");
    return -1;
  }
  if (load_binary(&vm, "./guest.bin") != 0) {
    fprintf(stderr, "Failed to load binary\n");
    return -1;
  }

  vcpu_thread((void *) &vcpu);

  vcpu_destroy(&vcpu);
  vm_destroy(&vm);
}
