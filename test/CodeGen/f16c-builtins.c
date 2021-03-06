// RUN: %clang_cc1 %s -triple=x86_64-apple-darwin -target-feature +f16c -emit-llvm -o - -Werror | FileCheck %s

// Don't include mm_malloc.h, it's system specific.
#define __MM_MALLOC_H

#include <x86intrin.h>

float test_cvtsh_ss(unsigned short a) {
  // CHECK-LABEL: test_cvtsh_ss
  // CHECK: @llvm.x86.vcvtph2ps.128
  return _cvtsh_ss(a);
}

unsigned short test_cvtss_sh(float a) {
  // CHECK-LABEL: test_cvtss_sh
  // CHECK: @llvm.x86.vcvtps2ph.128
  return _cvtss_sh(a, 0);
}

__m128 test_mm_cvtph_ps(__m128i a) {
  // CHECK-LABEL: test_mm_cvtph_ps
  // CHECK: @llvm.x86.vcvtph2ps.128
  return _mm_cvtph_ps(a);
}

__m256 test_mm256_cvtph_ps(__m128i a) {
  // CHECK-LABEL: test_mm256_cvtph_ps
  // CHECK: @llvm.x86.vcvtph2ps.256
  return _mm256_cvtph_ps(a);
}

__m128i test_mm_cvtps_ph(__m128 a) {
  // CHECK-LABEL: test_mm_cvtps_ph
  // CHECK: @llvm.x86.vcvtps2ph.128
  return _mm_cvtps_ph(a, 0);
}

__m128i test_mm256_cvtps_ph(__m256 a) {
  // CHECK-LABEL: test_mm256_cvtps_ph
  // CHECK: @llvm.x86.vcvtps2ph.256
  return _mm256_cvtps_ph(a, 0);
}
