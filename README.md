# DPLL-SAT-Solver
## DPLL SAT Solver for Propositional SAT Solving and applied in Sudoku
## input example 
p cnf 7 8
-2 -3 -4 5 0
-1 -5 6 0
-5 7 0
-1 -6 -7 0
-1 -2 5 0
-1 -3 5 0
-1 -4 5 0
-1 2 3 4 5 -6 0

### some note
\begin{array}{l}
1. \bigwedge_{x = 1}^{9} \bigwedge_{y = 1}^{9} \bigvee_{z = 1}^{9} s_{x y z} \\
2.\bigwedge_{y = 1}^{9} \bigwedge_{z = 1}^{9} \bigwedge_{x = 1}^{8} \bigwedge_{i = x+1}^{9}\left(\neg s_{x y z} \vee \neg s_{i y z}\right)\\
3.\bigwedge_{x=1}^{9} \bigwedge_{z=1}^{9} \bigwedge_{y=1}^{8} \bigwedge_{i=y+1}^{9}\left(\neg s_{x y z} \vee \neg s_{x i z}\right)\\
4.\bigwedge_{z=1}^{9}\bigwedge_{i=0}^{2}\bigwedge_{j=0}^{2}\bigwedge_{x=1}^{3}\bigwedge_{y=1}^{3 }\bigwedge_{k=1}^{3,k\ne x}\bigwedge_{l=1}^{3,l\ne y}\left(\neg s_{(3i + x)(3j + y)z} \vee \neg s_{(3i + k)(3j + l)z}\right)
\end{array}

