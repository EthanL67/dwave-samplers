//  Copyright 2018 D-Wave Systems Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#ifndef _BQPUTIL_H_

#define _BQPUTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <vector>

/**
 * Structure to represent a BQP
 * Q: the Q matrix
 * nVars: number of variables
 * solution: array of size nVars where every entry is 0 or 1. current solution.
 * solutionQuality: objective function value at solution
 * nIterations: number of iterations required to arrive at this solution
 */
typedef struct {
	std::vector<std::vector<double> > Q;
	int nVars;
	std::vector<int> solution;
	double solutionQuality;
	unsigned long long nIterations;
	/*added to record more statistics.*/
	unsigned long long restartNum;
	unsigned long long iterNum;
	unsigned long long evalNum;
	double upperBound;
} BQP;


/**
 * Gets the maximum abs(Q[i][j]) in a bqp
 * @param bqp
 * @return maximum Q[i][j]
 */
double bqpUtil_getMaxBQPCoeff(const BQP *bqp);

/**
 * Converts a generic bqp Q matrix into upper triangular using:
 * Q[i][j] = Q[i][j] + Q[j][i] for all i < j
 * @param bqp
 * @return void
 */
void bqpUtil_convertBQPToUpperTriangular(BQP *bqp);

/**
 * Print a BQP
 * @param bqp: BQP to be printed
 * @return void
 */
void bqpUtil_print(const BQP *bqp);

/**
 * Compute the value by which the objective function is changed if
 * exactly one bit in the solution is flipped
 * @param bqp: the BQP
 * @param oldSolution: current solution
 * @param flippedBit: the bit that is flipped
 * @return change in objective
 */
double bqpUtil_getChangeInObjective(const BQP *bqp, const std::vector<int> &oldSolution, int flippedBit);

/**
 * Computes the value of objective function of a BQP for a given solution
 * @param bqp: the BQP
 * @param solution: given solution
 * @return value of objective function
 */
double bqpUtil_getObjective(const BQP *bqp, const std::vector<int> &solution);

/**
 * Computes the value of objective function of a BQP for a given solution,
 * given some old solution.
 * If an old solution and the objective function value for that old solution
 * is known, then we can calculate the objective for the new solution quicker
 * by building up on the old solution.
 * @param bqp: the BQP
 * @param solution: new solution
 * @param oldSolution: old solution
 * @param oldCost: value of objective function at old solution
 * @return value ot objective function at new solution
 */
double bqpUtil_getObjectiveIncremental(const BQP *bqp, const std::vector<int> &solution, const std::vector<int> &oldSolution, double oldCost);

/**
 * Initialize the current solution in a BQP to all zeros
 * @param bqp: the BQP
 * @return void
 */
void bqpUtil_initBQPSolution(BQP *bqp, const std::vector<int> &initSolution);

/**
 * Initialize the current solution in a BQP randomly
 * @param bqp: the BQP
 * @return void
 */
void bqpUtil_randomizeBQPSolution(BQP *bqp);

/**
 * Print the current solution of BQP
 * @param bqp: the BQP
 * @return void
 */
void bqpUtil_printSolution(const BQP *bqp);

// High-precision per-thread monotonic clock value expressed in milliseconds
long long realtime_clock();

#endif
