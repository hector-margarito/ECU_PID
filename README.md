# An implementation of an ECU configuring a PID controller in SYSTEMC
Project class for Virtual prototyping HW & SW Design
Author: [Hector Margarito](https://hector-margarito.github.io/)

This an example of an Electronic Control Unit (ECU) that configures
a PID controller of a control loop. The control loop is connected 
to the master module(ECU) through a TLM interface.


## Attention
You need to have installed SystemC in your OS, version used in this
example is 2.3.1

## to compile and execute

1. Set an environment variable SYSTEMC to SystemC installation directory:
    > setenv SYSTEMC "/opt/systemc/systemc-2.3.1"
2. make a directory build in the top level directory of counter example
	> mkdir build
3. Go to the directory build 
	> cd build
4. Build it
	> cmake .. 
6. To compile the example do:
    > make  
7. To run example, go to the folder build/src and do: 
    > ./run


