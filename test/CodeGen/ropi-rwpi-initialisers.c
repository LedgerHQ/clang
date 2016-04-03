// RUN: %clang_cc1 -triple armv7a-none-eabi -emit-llvm -o - %s                                 | FileCheck %s --check-prefix=CHECK --check-prefix=NO-ROPI --check-prefix=NO-RWPI --check-prefix=NO-ROPI-OR-RWPI
// RUN: %clang_cc1 -triple armv7a-none-eabi -emit-llvm -o - %s -fropi-lowering                 | FileCheck %s --check-prefix=CHECK --check-prefix=ROPI    --check-prefix=NO-RWPI --check-prefix=ROPI-OR-RWPI
// RUN: %clang_cc1 -triple armv7a-none-eabi -emit-llvm -o - %s -frwpi-lowering                 | FileCheck %s --check-prefix=CHECK --check-prefix=NO-ROPI --check-prefix=RWPI    --check-prefix=ROPI-OR-RWPI
// RUN: %clang_cc1 -triple armv7a-none-eabi -emit-llvm -o - %s -fropi-lowering -frwpi-lowering | FileCheck %s --check-prefix=CHECK --check-prefix=ROPI    --check-prefix=RWPI    --check-prefix=ROPI-OR-RWPI

// CHECK: @a = external global i32
// RWPI: @b = externally_initialized global i32* undef
// NO-RWPI: @b = global i32* @a
// CHECK: @c = external constant i32
// ROPI: @d = externally_initialized global i32* undef
// NO-ROPI: @d = global i32* @c
// CHECK: @e = external global [0 x i32]
// RWPI: @f = externally_initialized global i32* undef
// NO-RWPI: @f = global i32* getelementptr inbounds ([0 x i32], [0 x i32]* @e, i32 0, i32 0)
// CHECK: @g = external constant [0 x i32]
// ROPI: @h = externally_initialized global i32* undef
// NO-ROPI: @h = global i32* getelementptr inbounds ([0 x i32], [0 x i32]* @g, i32 0, i32 0)
// ROPI: @j = externally_initialized global void ()* undef
// NO-ROPI: @j = global void ()* @i
// RWPI: @k.l = internal externally_initialized global i32* undef
// NO-RWPI: @k.l = internal global i32* @a
// ROPI: @k.m = internal externally_initialized global i32* undef
// NO-ROPI: @k.m = internal global i32* @c
// RWPI: @n = externally_initialized global i32* undef
// NO-RWPI: @n = global i32* getelementptr inbounds ([0 x i32], [0 x i32]* @e, i32 0, i32 0)
// ROPI: @o = externally_initialized global i32* undef
// NO-ROPI: @o = global i32* bitcast (i8* getelementptr (i8, i8* bitcast ([0 x i32]* @g to i8*), i64 4) to i32*)
// CHECK: @p = external global %struct.S0, align 4
// RWPI: @q = externally_initialized global i32* undef
// NO-RWPI: @q = global i32* getelementptr inbounds (%struct.S0, %struct.S0* @p, i32 0, i32 0)
// CHECK: @r = external constant %struct.S0, align 4
// ROPI: @s = externally_initialized global i32* undef
// NO-ROPI: @s = global i32* getelementptr inbounds (%struct.S0, %struct.S0* @r, i32 0, i32 0)
// RWPI: @t = externally_initialized global [3 x i32*] undef
// NO-RWPI: @t = global [3 x i32*] [i32* @a, i32* getelementptr inbounds ([0 x i32], [0 x i32]* @e, i32 0, i32 0), i32* getelementptr inbounds (%struct.S0, %struct.S0* @p, i32 0, i32 0)]
// ROPI: @u = externally_initialized global [3 x i32*] undef
// NO-ROPI: @u = global [3 x i32*] [i32* @c, i32* getelementptr inbounds ([0 x i32], [0 x i32]* @g, i32 0, i32 0), i32* getelementptr inbounds (%struct.S0, %struct.S0* @r, i32 0, i32 0)]
// ROPI-OR-RWPI: @v = externally_initialized global %struct.S1 undef
// NO-ROPI-OR-RWPI: @v = global %struct.S1 { [3 x i32*] [i32* @a, i32* getelementptr inbounds ([0 x i32], [0 x i32]* @e, i32 0, i32 0), i32* getelementptr inbounds (%struct.S0, %struct.S0* @p, i32 0, i32 0)], [3 x i32*] [i32* @c, i32* getelementptr inbounds ([0 x i32], [0 x i32]* @g, i32 0, i32 0), i32* getelementptr inbounds (%struct.S0, %struct.S0* @r, i32 0, i32 0)] }
// RWPI: @w = externally_initialized global i32* undef
// NO-RWPI: @w = constant i32* @a,
// ROPI-OR-RWPI: @x = externally_initialized global i32** undef
// NO-ROPI-OR-RWPI: @x = constant i32** @w

// RWPI: define internal void @__cxx_global_var_init{{.*}}
// RWPI: store i32* @a, i32** @b
// ROPI: define internal void @__cxx_global_var_init
// ROPI: store i32* @c, i32** @d
// RWPI: define internal void @__cxx_global_var_init
// RWPI: store i32* getelementptr inbounds ([0 x i32], [0 x i32]* @e, i32 0, i32 0), i32** @f
// ROPI: define internal void @__cxx_global_var_init
// ROPI: store i32* getelementptr inbounds ([0 x i32], [0 x i32]* @g, i32 0, i32 0), i32** @h
// ROPI: define internal void @__cxx_global_var_init
// ROPI: store void ()* @i, void ()** @j

// CHECK: define void @k()
// CHECK-NOT: call i32 @__cxa_guard_acquire(
// CHECK-NOT: call i32 @__cxa_guard_release(

// RWPI: define internal void @__cxx_global_var_init
// RWPI: store i32* @a, i32** @k.l
// ROPI: define internal void @__cxx_global_var_init
// ROPI: store i32* @c, i32** @k.m
// RWPI: define internal void @__cxx_global_var_init
// RWPI: store i32* getelementptr inbounds ([0 x i32], [0 x i32]* @e, i32 0, i32 0), i32** @n
// ROPI: define internal void @__cxx_global_var_init
// ROPI: store i32* getelementptr inbounds ([0 x i32], [0 x i32]* @g, i32 0, i32 1), i32** @o
// RWPI: define internal void @__cxx_global_var_init
// RWPI: store i32* getelementptr inbounds (%struct.S0, %struct.S0* @p, i32 0, i32 0), i32** @q
// ROPI: define internal void @__cxx_global_var_init
// ROPI: store i32* getelementptr inbounds (%struct.S0, %struct.S0* @r, i32 0, i32 0), i32** @s
// RWPI: define internal void @__cxx_global_var_init
// RWPI: store i32* @a, i32** getelementptr inbounds ([3 x i32*], [3 x i32*]* @t, i32 0, i32 0)
// RWPI: store i32* getelementptr inbounds ([0 x i32], [0 x i32]* @e, i32 0, i32 0), i32** getelementptr inbounds ([3 x i32*], [3 x i32*]* @t, i32 0, i32 1)
// RWPI: store i32* getelementptr inbounds (%struct.S0, %struct.S0* @p, i32 0, i32 0), i32** getelementptr inbounds ([3 x i32*], [3 x i32*]* @t, i32 0, i32 2)
// ROPI: define internal void @__cxx_global_var_init
// ROPI: store i32* @c, i32** getelementptr inbounds ([3 x i32*], [3 x i32*]* @u, i32 0, i32 0)
// ROPI: store i32* getelementptr inbounds ([0 x i32], [0 x i32]* @g, i32 0, i32 0), i32** getelementptr inbounds ([3 x i32*], [3 x i32*]* @u, i32 0, i32 1)
// ROPI: store i32* getelementptr inbounds (%struct.S0, %struct.S0* @r, i32 0, i32 0), i32** getelementptr inbounds ([3 x i32*], [3 x i32*]* @u, i32 0, i32 2)
// ROPI-OR-RWPI: define internal void @__cxx_global_var_init
// ROPI-OR-RWPI: store i32* @a, i32** getelementptr inbounds (%struct.S1, %struct.S1* @v, i32 0, i32 0, i32 0)
// ROPI-OR-RWPI: store i32* getelementptr inbounds ([0 x i32], [0 x i32]* @e, i32 0, i32 0), i32** getelementptr inbounds (%struct.S1, %struct.S1* @v, i32 0, i32 0, i32 1)
// ROPI-OR-RWPI: store i32* getelementptr inbounds (%struct.S0, %struct.S0* @p, i32 0, i32 0), i32** getelementptr inbounds (%struct.S1, %struct.S1* @v, i32 0, i32 0, i32 2)
// ROPI-OR-RWPI: store i32* @c, i32** getelementptr inbounds (%struct.S1, %struct.S1* @v, i32 0, i32 1, i32 0)
// ROPI-OR-RWPI: store i32* getelementptr inbounds ([0 x i32], [0 x i32]* @g, i32 0, i32 0), i32** getelementptr inbounds (%struct.S1, %struct.S1* @v, i32 0, i32 1, i32 1)
// ROPI-OR-RWPI: store i32* getelementptr inbounds (%struct.S0, %struct.S0* @r, i32 0, i32 0), i32** getelementptr inbounds (%struct.S1, %struct.S1* @v, i32 0, i32 1, i32 2)
// RWPI: define internal void @__cxx_global_var_init
// RWPI: store i32* @a, i32** @w
// ROPI-OR-RWPI: define internal void @__cxx_global_var_init
// ROPI-OR-RWPI: store i32** @w, i32*** @x

// Address of int, needs RWPI lowering
extern int a;
int *b = &a;

// Address of const int, needs ROPI lowering
extern const int c;
const int *d = &c;

// Address of int array, needs RWPI lowering
extern int e[];
int *f = e;

// Address of const int array, needs ROPI lowering
extern const int g[];
const int *h = g;

// Address of function, needs ROPI lowering
extern void i(void);
void (*j)(void) = i;

void k() {
  // Address of int, needs RWPI lowering
  static int *l = &a;
  // Address of const int, needs ROPI lowering
  static const int *m = &c;
}

// Address of array member, needs RWPI lowering
int *n = &e[0];
// Address of const array member, needs ROPI lowering
const int *o = &g[1];

struct S0 {
  int f1;
};

// Address of struct member, needs RWPI lowering
extern struct S0 p;
int *q = &p.f1;

// Address of const struct member, needs ROPI lowering
extern const struct S0 r;
const int *s = &r.f1;

// Array initialiser, needs ROPI lowering
int *t[] = {&a, &e[0], &p.f1};

// Array initialiser, needs RWPI lowering
const int *u[] = {&c, &g[0], &r.f1};

struct S1 {
  int *f1[3];
  const int * f2[3];
};

// Struct initialiser, needs ROPI and RWPI lowering
// FIXME: For ROPI-only and RWPI-only modes, some elements could be
// statically initialised.
struct S1 v = {{&a, &e[0], &p.f1}, {&c, &g[0], &r.f1}};

// Address of int, needs RWPI lowering, becomes non-const
int * const w = &a;
// Address of variable subject to RWPI lowering
// * In ROPI-only mode, needs lowering because w is const
// * In RWPI-only and ROPI+RWPI modes, needs lowering because w is made non-const
int * const * const x = &w;
