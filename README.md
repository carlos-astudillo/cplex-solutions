CPLEX Solutions
===================

CPLEX programs for different Problems

Command to compile code
-----------------------
<pre>
g++ -L/opt/ibm/ILOG/CPLEX_Studio125/cplex/lib/x86-64_sles10_4.1/static_pic -L/opt/ibm/ILOG/CPLEX_Studio125/concert/lib/x86-64_sles10_4.1/static_pic -I/opt/ibm/ILOG/CPLEX_Studio125/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio125/concert/include <filename>.cpp -lilocplex -lconcert -lcplex -lm -lpthread  -DIL_STD -o <filename>
</pre>

