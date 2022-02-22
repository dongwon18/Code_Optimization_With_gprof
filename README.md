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
  5.  
## Contents of Document

## Result

## How to run

## File structure

## 배운 점
## 한계점
