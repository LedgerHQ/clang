// RUN: %clang_cc1 -fropi-lowering -verify -emit-llvm -o /dev/null %s

extern int extern_int;
extern const int extern_const_int;
extern void extern_func(void);
typedef void (*func_ptr)(void);

static       int *       a = &extern_int;       // no-warning
static       int * const b = &extern_int;       // no-warning
static const int *       c = &extern_const_int; // no-warning
static const int * const d = &extern_const_int; // no-warning

extern       int *       e;
extern       int * const f;
extern const int *       g;
extern const int * const h;
             int *       e = &extern_int;       // no-warning
             int * const f = &extern_int;       // no-warning
       const int *       g = &extern_const_int; // no-warning
       const int * const h = &extern_const_int; // expected-warning {{embedded position-independent lowering of h made it non-const, other translation units will access it incorrectly}}

static       func_ptr i = &extern_func; // no-warning
static const func_ptr j = &extern_func; // no-warning
extern       func_ptr k;
extern const func_ptr l;
             func_ptr k = &extern_func; // no-warning
       const func_ptr l = &extern_func; // expected-warning {{embedded position-independent lowering of l made it non-const, other translation units will access it incorrectly}}

void foo() {
  static       int *       m = &extern_int; // no-warning
  static       int * const n = &extern_int; // no-warning
  static const int *       o = &extern_const_int; // no-warning
  static const int * const p = &extern_const_int; // no-warning
  static       func_ptr q = &extern_func; // no-warning
  static const func_ptr r = &extern_func; // no-warning
}

struct A;
struct B { int field; };
extern       struct A s; // no-warning
extern const struct A t; // expected-warning {{externally-visible const variable has incomplete type, other translation units may access it incorrectly when using embedded position-independence}}
extern       struct B u; // no-warning
extern const struct B v; // no-warning
