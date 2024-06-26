// Tests trace pc guard coverage collection.
//
// REQUIRES: x86_64-linux
// XFAIL: tsan
//
// RUN: DIR=%t_workdir
// RUN: rm -rf $DIR
// RUN: mkdir -p $DIR
// RUN: cd $DIR
/// In glibc 2.39+, fprintf has a nonnull attribute. Disable nonnull-attribute,
/// which would increase counters for ubsan.
// RUN: %clangxx -O0 -fsanitize-coverage=trace-pc-guard -fno-sanitize=nonnull-attribute %s -o %t
// RUN: %env_tool_opts=coverage=1 %t 2>&1 | FileCheck %s
// RUN: rm -rf $DIR

#include <stdio.h>

int foo() {
  fprintf(stderr, "foo\n");
  return 1;
}

int main() {
  fprintf(stderr, "main\n");
  foo();
  foo();
}

// CHECK: main
// CHECK: SanitizerCoverage: ./sanitizer_coverage_symbolize.{{.*}}.sancov: 2 PCs written
