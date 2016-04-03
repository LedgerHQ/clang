// RUN: %clang_cc1 -triple armv7a-none-eabi -emit-llvm -o - %s | FileCheck %s --check-prefix=CHECK

// CHECK: @__dso_handle = external constant i8
struct A {
  ~A();
} a;
