# Minimum cost sequences folding problem

[TOC]

## problem definition

Given a set of string $S=\{s_1,s_2,...,s_n\}$, bounds $lb,ub$ and cost $c,d$,  let $T_i=\{s_i[a,b]:\forall 0\leq a<b\leq l(s_i)-1\}$ be the set of all substring of $s_i$, and $\mathcal{T}=\cup_{i=1}^n T_i$. The minimum cost sequences folding problem is to find two set $P,Q\subset \mathcal{T}$, such that:

- $\forall s\in P$, $lb\leq l(s) <ub$
- $\forall s\in Q$, $ub\leq l(s)$
- $\forall s_i\in S$, $\exist$ a partition $(s_i[a_0,a_1],s_i[a_1,a_2],...,s_i[a_k-1,a_k])$ with $0\leq a_0 <a_1,...,a_k\leq l(s_i)-1$, such that $s_i[a_j,a_j+1]\in P\cup Q$ for all $j=0,...,k-1$
- total cost $\sum_{s\in P}c(s)+\sum_{s\in Q}d(s)$ is minimized

## alternative approximation

## find all common patterns

## find maximum weighted independent set

## post processing