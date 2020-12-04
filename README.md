# CAD_contest-2016

  Static timing analysis (STA) is a method of validating the timing performance of a design by checking all possible paths for timing violations.  STA breaks a design down into timing paths, calculates the signal propagation delay along each path, and checks for violations of timing constraints inside the design and at the input/output interface. We (Me and 3 friends) attended the CAD contest in 2016 and tried to address this problem.

#### An example of true path in a circuit.

![](https://i.imgur.com/FpGKmYN.png)


-----

### Goal : 
  The goal is to develop an efficient STA program to find the true paths under a given multi-core computing environment.
  

### The Results:
The fourth rank among competitors
![](https://i.imgur.com/T8Wpgjm.png)

### Terminolgy:
There are some terms used in this project, which can be referenced here.
https://cad-contest-2016.el.cycu.edu.tw/Problem_D/default.html
* True path
* floating-mode
* ...etc


### The conditions in given circuit benchmark
We were given information of different combinational circuits, each containing three kinds of logic gates, NAND, NOR and NOT.
all combinational circuits
* every gate delay is 1ns
* every gate has its controlling value
* the wire delay is zero
* The input files contain a Verilog gate-level netlist and its associated Verilog model.

### Our Algorithm and DataStructure.

#### Algo.



#### Data Structure.

