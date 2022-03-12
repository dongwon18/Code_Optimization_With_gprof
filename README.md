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
Total 6 optimization steps are done. 
Detailed result is included in the document.

## File structure
```
|-- data
  |-- all_month_data.csv
  |-- all_stock_data.csv
|-- src
  |-- optimized_source_code.c
  |-- original_code.c
|-- code_optimization_document.pdf
```

## 배운 점
- 코드의 실행시간에 있어 가장 영향을 많이 미치는 부분이 loop 부분임을 인지하고 아래의 방법을 통해 loop efficiency를 증가시킬 수 있다.
  - loop 내에서 중복적으로 call 되는 고정 값을 외부에서 전달해주거나 loop 바깥에서 정의함으로써 불필요한 function call을 줄일 수 있다.
  - 값이 변경되지 않는 값은 loop 바깥에서 정의하여 loop inefficiency를 줄일 수 있었다.
  - 불필요한 loop를 찾아 제거함으로써 실행 시간을 줄일 수 있었다.
- 2x2 unrolling은 하드웨어 적인 차이기 때문에 SW의 측면에서 효율성에 차이를 미치지는 않는다.
## 한계점
- 1년 전에 작성했던 코드를 분석하고 최적화하는 과정에서 이전에 작성한 코드에 주석이 많음에도 불구하고 **소스 파일을 나눠 관리**하는 것이 가독성이좋다는 것을 깨닫게 되었다.
- 최적화한 코드 자체가 비교적 단순하다보니 하드웨어적인 최적화를 진행하였을 때 차이가 극명하게 보이지 않아 차후 기회가 있다면 복잡한 코드를 최적화하는 것일 좋을 듯 하다.
- 불필요한 변수, 함수 등을 정리하고 가독성을 높이는 refactoring 기법을 도입하면 더욱 좋을 것이다.  
  
  
  
  
