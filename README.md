# Propositional-Logic-Representation-and-Deduction

Input. Propositional Formula (ϕ) as strings with propositions, negations, connectives and brackets, ‘(’ and ‘)’
Postfix Formula Representation. Propositional Formula (ϕ) as strings with propositions, negations, connectives in Postfix format (this will made available to you in code as a string for ready-made processing!)
Output. You will be asked to write separate functions for the following parts (in the already supplied code):
1. Represent the postfix propositional formula (ϕ) as a binary tree (τ) data structure, known as expression tree, which contains propositions as leaf nodes and operators {∧, ∨, ¬, →, ↔} as internal nodes
(refer to the left expression tree in Figure 1) (Marks : 5)
2. Print the expression tree (using in-order traversal of τ) and generate the formula (ϕ) 
3. Given ⊤/⊥ values for all the propositions, find the outcome of the overall Formula (ϕ) from its
expression tree (τ) 
4. Transformation of the formula step-wise (ϕ ϕI ϕN ϕC /ϕD) using the expression tree data
structure (τ τI τN τC /τD) as follows:
(a) Implication-Free Form (IFF): Formula (ϕI ) after elimination of → and ↔
Procedure: Transform τ to τI and then print ϕI from τI 
(b) Negation Normal Form (NNF): Formula (ϕN ) where ¬ appears only before propositions
Procedure: Transform τI to τN and then print ϕN from τN 
(c) Conjunctive Normal Form (CNF): Formula (ϕC ) with conjuction of disjunctive-clauses where
each disjunctive-clause is a disjunction of literals
Procedure: Transform τN to τC and then print ϕC from τC 
(d) Disjunctive Normal Form (DNF): Formula (ϕD) with disjuction of conjuctive-clauses where each
conjunctive-clause is a conjunction of literals
Procedure: Transform τN to τD and then print ϕD from τD 
5. Given the expression tree (τ), using exhaustive search, check for the following – 
(a) the validity (⊤) or the invalidity of the formula (whether it is a tautology or not), or
(b) the satisfiability or the unsatisfiability (⊥) of the formula (whether it is a contradiction or not)
