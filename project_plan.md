# Project plan

## Research questions:

There are two research questions.

### 1: Is it possible to classify a CA using properties of an execution of the HashLife algorithm up to time $T$?
Execute HashLife for $T$ steps.  Measure the size of the underlying hash table as a function of $T$.

For a replicator, expect slow growth in the size of the hash table.  For chaotic evolution, expect fast growth since less memoisation occurs.

Train a classifier on CAs known to be chaotic / replicators.

The classifier may then be used to search for candidates for proof, in research question 2.

### 2: Is it possible to use HashLife to prove rigorous results about CA replication?  
Idea: identify recurrences in the HashLife hash table that may be used to prove replication by induction.

## Limitations / decisions

- All implementation to be done in C, due to needing to have control over underlying data structures, execution time, etc. (And I have TB-2 units that require C programming.)
- Restrict to $d=1$ CAs, as that is what the literature on replicators focus on. (I think it is better to go further with $d-1$ than try for generality.)
- CA prediction is undecidable in general, so any solution to (1) will be incomplete.