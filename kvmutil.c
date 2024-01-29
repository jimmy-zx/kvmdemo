#define _GNU_SOURCE

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "kvmutil.h"

int vm_create(struct vm *vm) {
  int api_ver;

  if ((vm->vm_kvmfd = open("/dev/kvm", O_RDWR)) < 0) {
    perror("open /dev/kvm");
    return -1;
  }
  if ((api_ver = ioctl(vm->vm_kvmfd, KVM_GET_API_VERSION, 0)) < 0) {
    perror("KVM_GET_API_VERSION");
    return -1;
  }
  if (api_ver != KVM_API_VERSION) {
    fprintf(stderr, "KVM_API_VERSION check failed: got %d, expected %d\n", api_ver, KVM_API_VERSION);
    return -1;
  }
  if ((vm->vm_fd = ioctl(vm->vm_kvmfd, KVM_CREATE_VM, 0)) < 0) {
    perror("KVM_CREATE_VM");
    return -1;
  }
  if ((vm->vm_mem = mmap(NULL, vm->vm_mem_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0)) == MAP_FAILED) {
    perror("mmap");
    return -1;
  }
  vm->vm_mem_region.slot = 0;
  vm->vm_mem_region.flags = 0;
  vm->vm_mem_region.guest_phys_addr = 0;
  vm->vm_mem_region.memory_size = vm->vm_mem_size;
  vm->vm_mem_region.userspace_addr = (__u64) vm->vm_mem;
  if (ioctl(vm->vm_fd, KVM_SET_USER_MEMORY_REGION, &(vm->vm_mem_region)) < 0) {
    perror("KVM_SET_USER_MEMORY_REGION");
    return -1;
  }
  return 0;
}

void vm_destroy(struct vm *vm) {
  munmap(vm->vm_mem, vm->vm_mem_size);
  close(vm->vm_kvmfd);
  close(vm->vm_fd);
}

int vcpu_create(struct vcpu *vcpu) {
  if ((vcpu->vcpu_fd = ioctl(vcpu->vm->vm_fd, KVM_CREATE_VCPU, vcpu->vcpu_id)) < 0) {
    perror("KVM_CREATE_VCPU");
    return -1;
  }
  if ((vcpu->vcpu_mmap_size = ioctl(vcpu->vm->vm_kvmfd, KVM_GET_VCPU_MMAP_SIZE, 0)) < 0) {
    perror("KVM_GET_VCPU_MMAP_SIZE");
    return -1;
  }
  if ((vcpu->vcpu_kvm_run = mmap(NULL, vcpu->vcpu_mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, vcpu->vcpu_fd, 0)) == MAP_FAILED) {
    perror("mmap");
    return -1;
  }
  return 0;
}

void vcpu_destroy(struct vcpu *vcpu) {
  munmap(vcpu->vcpu_kvm_run, vcpu->vcpu_mmap_size);
  close(vcpu->vcpu_fd);
}

int vcpu_reset(struct vcpu *vcpu) {
  if (ioctl(vcpu->vcpu_fd, KVM_GET_SREGS, &vcpu->vcpu_sregs) < 0) {
    perror("KVM_GET_SREGS");
    return -1;
  }
  vcpu->vcpu_sregs.cs.selector = 0;
  vcpu->vcpu_sregs.cs.base = 0;
  if (ioctl(vcpu->vcpu_fd, KVM_SET_SREGS, &vcpu->vcpu_sregs) < 0) {
    perror("KVM_SET_SREGS");
    return -1;
  }

  if (ioctl(vcpu->vcpu_fd, KVM_GET_REGS, &vcpu->vcpu_regs) < 0) {
    perror("KVM_GET_REGS");
    return -1;
  }
  vcpu->vcpu_regs.rflags = 2;
  vcpu->vcpu_regs.rip = 0;
  if (ioctl(vcpu->vcpu_fd, KVM_SET_REGS, &vcpu->vcpu_regs) < 0) {
    perror("KVM_SET_REGS");
    return -1;
  }

  return 0;
}

