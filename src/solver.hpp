/*
 * solver.hpp
 *
 *  Created on: 03-May-2021
 *      Author: abrar
 */

#ifndef SOLVER_HPP_
#define SOLVER_HPP_

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

#define lli                 long long int
#define lld                 long double
#define F                   first
#define S                   second
#define FOR(i,x)            for(int i=0;i<(x); i++)
#define For(i,s,x)            for(int i=(s);i<=(x); i++)
#define FOR1(j,x)            for(int j=1;j<=(x); j++)
#define MOD(a)            ((a)>(0) ? (a):(-a))
#define MIN(a,b)            ((a)>(b) ? (b):(a))
#define MAX(a,b)            ((a)>(b) ? (a):(b))

using namespace std;

class Solver{
	int V ;
	vector< vector<int> > adj;
	bool* visited;
	vector< vector<int> > mat;
	vector<int> bfs_ar,dfs_ar;

	void addEdge(int ,int);
	void fill();
	bool check(int,int);
	void show();
	void DFS(int);
	void BFS(int);
	void refresh();

	public:
		bool solved;
		Solver();
		void setMat(vector< vector<int>> &arr);
		vector< vector<int>> getMat();
		void solve(int);
};

#endif /* SOLVER_HPP_ */
