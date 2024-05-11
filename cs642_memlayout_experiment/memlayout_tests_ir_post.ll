; ModuleID = 'memlayout_tests_ir.ll'
source_filename = "memlayout_tests_ir.ll"

define i32 @foo() {
  %a = add i32 2, 3
  ret i32 %a
}

define void @bar() {
  ret void
}
