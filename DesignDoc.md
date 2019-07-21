# BIeR Language Design Doc

## Primary instances
- Type - One of the fundamental integer types (i1, i8, i16, i32, i64) or pointer T* where T is fundumental type or type layout
- Type Layout - memory layout
  ```ir
      !some_layout = {i32*, i8*[16], i32, i1[10], i64}
  ```
  In order to access some element the special command 'gep' should be used.
  The syntaxis: gep <element_type> <layout_name>* %ptr_to_layout, <integer index of layout n>, <number of desired element> 
  ```ir
      %third_element_ptr = gep i32 some_layout* %ptr_to_layout, 0, 17 
  ```
  
- Value - any instance that stores some data
  - Global data - the only type for global constant data. Immutable by definiition.
    ✓ SSA
    ✓ Use
    ✗ Multiple Assign
    ```ir
        @some_value = {i32 (42), i64[2] (56, 32)} 
    ```
  - Local Value - local value inside the function. Cn be assigned only once inside the function block:
    ✓ SSA
    ✓ Use
    ✗ Multiple Assign
    ```ir
        %tmp = i32 12
    ```
  - Variable - local variable inside memory - can be assigned multiple times. Effectively every access to variable translates to read from memory into local value, every write translates to store :
    ✗ SSA
    ✓ Use
    ✓ Multiple Assign
    ```ir
        $var = i32 15
        %x = add i32 $var, %other 
    ```
    the same as
    ```ir
        %ptr_var = stallock i32* 1
        store i32 %ptr_var, 15
        %tmp_var = load i32 %ptr_var
        %x = add i32 %tmp_var, %other
    ```
    
  - Inline Constant - inplace constant variable:
    ✓ SSA
    ✓ Use
    ✗ Multiple Assign
    ```ir
        %tmp = i32 15
    ```
