
;; foo(x) = x + g

@g = global i32 22, align 4

define i32 @foo() {
    ; Adds a global to a constant and returns
    %a = load i32, ptr @g, align 4
    %b = add i32 %a, 3
    ret i32 %b
}


