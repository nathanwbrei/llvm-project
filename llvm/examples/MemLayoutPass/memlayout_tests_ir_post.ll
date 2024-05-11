; ModuleID = 'memlayout_tests_ir.ll'
source_filename = "memlayout_tests_ir.ll"

@g = global i32 22, align 4

define i32 @foo() {
  %a = load i32, ptr @g, align 4
  %b = add i32 %a, 3
  ret i32 %b
}
