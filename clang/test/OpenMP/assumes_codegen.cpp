// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=60 -x c++ -emit-llvm %s -fexceptions -fcxx-exceptions -triple x86_64-unknown-unknown -o - | FileCheck %s
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=60 -ast-print %s | FileCheck %s --check-prefix=AST
// RUN: %clang_cc1 -fopenmp -fopenmp-version=60 -x c++ -std=c++11 -triple x86_64-unknown-unknown -fexceptions -fcxx-exceptions -emit-pch -verify -o %t %s
// RUN: %clang_cc1 -fopenmp -fopenmp-version=60 -x c++ -triple x86_64-unknown-unknown -fexceptions -fcxx-exceptions -std=c++11 -include-pch %t -verify=pch %s -emit-llvm -o - | FileCheck %s
// RUN: %clang_cc1 -verify -triple x86_64-apple-darwin10 -fopenmp -fopenmp-version=60 -fexceptions -fcxx-exceptions -debug-info-kind=line-tables-only -x c++ -emit-llvm %s -o - | FileCheck %s
// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=60 -fopenmp-enable-irbuilder -x c++ -emit-llvm %s -fexceptions -fcxx-exceptions -triple x86_64-unknown-unknown -o - | FileCheck %s
// RUN: %clang_cc1 -fopenmp -fopenmp-version=60 -fopenmp-enable-irbuilder -x c++ -std=c++11 -triple x86_64-unknown-unknown -fexceptions -fcxx-exceptions -emit-pch -verify -o %t %s
// RUN: %clang_cc1 -fopenmp -fopenmp-version=60 -fopenmp-enable-irbuilder -x c++ -triple x86_64-unknown-unknown -fexceptions -fcxx-exceptions -std=c++11 -include-pch %t -verify=pch %s -emit-llvm -o - | FileCheck %s

// pch-no-diagnostics

#ifndef HEADER
#define HEADER

void foo() {
}

#pragma omp assumes no_openmp_routines warning ext_another_warning(1) ext_after_invalid_clauses // expected-warning {{valid assumes clauses start with 'ext_', 'absent', 'contains', 'holds', 'no_openmp', 'no_openmp_routines', 'no_openmp_constructs', 'no_parallelism'; token will be ignored}} expected-warning {{'ext_another_warning' clause should not be followed by arguments; tokens will be ignored}} expected-note {{the ignored tokens spans until here}}

#pragma omp assumes no_openmp

#pragma omp begin assumes ext_range_bar_only

#pragma omp begin assumes ext_range_bar_only_2

class BAR {
public:
  BAR() {}

  void bar1() {
  }

  static void bar2() {
  }
};

void bar() { BAR b; }

#pragma omp end assumes
#pragma omp end assumes

#pragma omp begin assumes ext_not_seen
#pragma omp end assumes

#pragma omp begin assumes ext_1234
void baz();

template<typename T>
class BAZ {
public:
  BAZ() {}

  void baz1() {
  }

  static void baz2() {
  }
};

void baz() { BAZ<float> b; }
#pragma omp end assumes

#pragma omp begin assumes ext_lambda_assumption
int lambda_outer() {
  auto lambda_inner = []() { return 42; };
  return lambda_inner();
}
#pragma omp end assumes

// AST{LITERAL}:      void foo() [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] {
// AST-NEXT{LITERAL}: }
// AST-NEXT{LITERAL}: class BAR {
// AST-NEXT{LITERAL}: public:
// AST-NEXT{LITERAL}:     [[omp::assume("ompx_range_bar_only")]] [[omp::assume("ompx_range_bar_only_2")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] BAR() {
// AST-NEXT{LITERAL}:     }
// AST-NEXT{LITERAL}:     [[omp::assume("ompx_range_bar_only")]] [[omp::assume("ompx_range_bar_only_2")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] void bar1() {
// AST-NEXT{LITERAL}:     }
// AST-NEXT{LITERAL}:     [[omp::assume("ompx_range_bar_only")]] [[omp::assume("ompx_range_bar_only_2")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] static void bar2() {
// AST-NEXT{LITERAL}:     }
// AST-NEXT{LITERAL}: };
// AST-NEXT{LITERAL}: [[omp::assume("ompx_range_bar_only")]] [[omp::assume("ompx_range_bar_only_2")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] void bar() {
// AST-NEXT{LITERAL}:     BAR b;
// AST-NEXT{LITERAL}: }
// AST-NEXT{LITERAL}: [[omp::assume("ompx_1234")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] void baz();
// AST-NEXT{LITERAL}: template <typename T> class BAZ {
// AST-NEXT{LITERAL}: public:
// AST-NEXT{LITERAL}:     [[omp::assume("ompx_1234")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] BAZ<T>() {
// AST-NEXT{LITERAL}:     }
// AST-NEXT{LITERAL}:     [[omp::assume("ompx_1234")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] void baz1() {
// AST-NEXT{LITERAL}:     }
// AST-NEXT{LITERAL}:     [[omp::assume("ompx_1234")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] static void baz2() {
// AST-NEXT{LITERAL}:     }
// AST-NEXT{LITERAL}: };
// AST-NEXT{LITERAL}: template<> class BAZ<float> {
// AST-NEXT{LITERAL}: public:
// AST-NEXT{LITERAL}:     [[omp::assume("ompx_1234")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] BAZ() {
// AST-NEXT{LITERAL}:     }
// AST-NEXT{LITERAL}:     [[omp::assume("ompx_1234")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] void baz1();
// AST-NEXT{LITERAL}:     [[omp::assume("ompx_1234")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] static void baz2();
// AST-NEXT{LITERAL}: };
// AST-NEXT{LITERAL}: [[omp::assume("ompx_1234")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] void baz() {
// AST-NEXT{LITERAL}:     BAZ<float> b;
// AST-NEXT{LITERAL}: }
// AST-NEXT{LITERAL}: [[omp::assume("ompx_lambda_assumption")]] [[omp::assume("omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses")]] [[omp::assume("omp_no_openmp")]] int lambda_outer() {
// AST-NEXT{LITERAL}:     auto lambda_inner = []() {
// AST-NEXT{LITERAL}:         return 42;
// AST-NEXT{LITERAL}:     };
// AST-NEXT{LITERAL}:     return lambda_inner();
// AST-NEXT{LITERAL}: }

#endif

// CHECK: define{{.*}} void @_Z3foov()
// CHECK-SAME: [[attr0:#[0-9]]]
// CHECK: define{{.*}} void @_Z3barv()
// CHECK-SAME: [[attr1:#[0-9]]]
// CHECK:   call{{.*}} @_ZN3BARC1Ev(ptr{{.*}} %b)
// CHECK-SAME: [[attr8:#[0-9]]]
// CHECK: define{{.*}} void @_ZN3BARC1Ev(ptr{{.*}} %this)
// CHECK-SAME: [[attr1:#[0-9]]]
// CHECK:   call{{.*}} @_ZN3BARC2Ev(ptr{{.*}} %this1)
// CHECK-SAME: [[attr8]]
// CHECK: define{{.*}} void @_ZN3BARC2Ev(ptr{{.*}} %this)
// CHECK-SAME: [[attr2:#[0-9]]]
// CHECK: define{{.*}} void @_Z3bazv()
// CHECK-SAME: [[attr3:#[0-9]]]
// CHECK:   call{{.*}} @_ZN3BAZIfEC1Ev(ptr{{.*}} %b)
// CHECK-SAME: [[attr9:#[0-9]]]
// CHECK: define{{.*}} void @_ZN3BAZIfEC1Ev(ptr{{.*}} %this)
// CHECK-SAME: [[attr4:#[0-9]]]
// CHECK:   call{{.*}} @_ZN3BAZIfEC2Ev(ptr{{.*}} %this1)
// CHECK-SAME: [[attr9]]
// CHECK: define{{.*}} void @_ZN3BAZIfEC2Ev(ptr{{.*}} %this)
// CHECK-SAME: [[attr5:#[0-9]]]
// CHECK: define{{.*}} i32 @_Z12lambda_outerv()
// CHECK-SAME: [[attr6:#[0-9]]]
// CHECK: call{{.*}} @"_ZZ12lambda_outervENK3$_0clEv"
// CHECK-SAME: [[attr10:#[0-9]]]
// CHECK: define{{.*}} i32 @"_ZZ12lambda_outervENK3$_0clEv"(ptr{{.*}} %this)
// CHECK-SAME: [[attr7:#[0-9]]]

// CHECK:     attributes [[attr0]]
// CHECK-SAME:  "llvm.assume"="omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp"
// CHECK:     attributes [[attr1]]
// CHECK-SAME:  "llvm.assume"="ompx_range_bar_only,ompx_range_bar_only_2,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp"
// CHECK:     attributes [[attr2]]
// CHECK-SAME:  "llvm.assume"="ompx_range_bar_only,ompx_range_bar_only_2,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp"
// CHECK:     attributes [[attr3]]
// CHECK-SAME:  "llvm.assume"="ompx_1234,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp,ompx_1234,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp"
// CHECK:     attributes [[attr4]]
// CHECK-SAME:  "llvm.assume"="ompx_1234,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp"
// CHECK:     attributes [[attr5]]
// CHECK-SAME:  "llvm.assume"="ompx_1234,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp"
// CHECK:     attributes [[attr6]]
// CHECK-SAME:  "llvm.assume"="ompx_lambda_assumption,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp"
// CHECK:     attributes [[attr7]]
// CHECK-SAME:  "llvm.assume"="ompx_lambda_assumption,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp"
// CHECK:     attributes [[attr8]]
// CHECK-SAME:  "llvm.assume"="ompx_range_bar_only,ompx_range_bar_only_2,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp"
// CHECK:     attributes [[attr9]]
// CHECK-SAME:  "llvm.assume"="ompx_1234,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp"
// CHECK:     attributes [[attr10]]
// CHECK-SAME:  "llvm.assume"="ompx_lambda_assumption,omp_no_openmp_routines,ompx_another_warning,ompx_after_invalid_clauses,omp_no_openmp"
