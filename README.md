---
finished_date: 2019-05-23
tags:
  - system_programming
  - code_optimization
  - gprof
  - C
  - linux
---
# Code Optimization with gprof tool
- optimize previously written code by myself
- analyze the result using gprof tool
  - focus on profile result, call graph
  - loop, the number of function arguments, redundantly repeated calculation  
- profiling and optimization is done in 6 steps
  1. original code
    - focus on irregular function calls and runtime
    - result: count_line(), count_variety()
  2. optimization 1
    - focus on reducing the number of count_line calls
    - result: find out that count_line() was used for loop condition
  3. optimization 2
    - focus on reducing the number of count_variety calls
    - result: since count_variety is called in calculate_profile() and make_output_file(), reduce the number of calls for the both functions
  4. optimization 3
    - focus on reducing the number of count_variety calls
    - result: save the return value of count_variety to a variable 'stock_variety' and pass it to functions
  5. optimization 4
    - focus on reducing redundant variable declaration and initialization in loop, also focus on row major order in array
    - result: declare variable outside the loop, no column major order found
  6. optimization 5
    - done with meaningless large data to find out other part to optimize since original data was too small to get proper result on the behalf of running time.
    - focus on data_selection() function which contains redundant repeated work
    - result: modify code not to repeat the same work (total repeated time $$line<sup>2$$ → $$line$$)
  7. optimization 6
    - focus on assembly code and loop unrolling
    - result: optimization done
  
## Contents of Document
1. Introduction
  - Brief explanation about the program to be simulated
2. How to use original stock simulation program
3. Optimization
4. Conclusion
5. References
## Result

## How to run

## File structure

## 배운 점
## 한계점
