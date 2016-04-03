// RUN: %clang_cc1 -triple armv7a-none-eabi -emit-llvm -o - %s | FileCheck %s --check-prefix=CHECK

// CHECK: @a = external constant %struct.A
extern const struct A {
  int b;
} a;

const A * foo() { return &a; }
