/*
 * solver.cpp
 *
 *  Created on: 03-May-2021
 *      Author: abrar
 */

#include "solver.hpp"

Solver::Solver(){
	this->V = 9;
	this->adj = vector<vector<int>> (100,vector<int>());
	this->mat = vector< vector<int> > (9,vector<int>(9,0));
	this->visited = new bool[100];
	refresh();
}

void Solver::refresh(){
	this->solved=false;
	FOR(i,V){
		FOR(j,V)
			visited[i*10+j]=false;
	}
}

void Solver::setMat(vector< vector<int> > &arr){
	this->mat = arr;
	fill();
}

vector< vector<int> > Solver::getMat()
{
	return this->mat;
}

void Solver::addEdge(int u,int v){
	//printf("u=%d v=%d\n",u,v);
	adj[u].push_back(v);
	adj[v].push_back(u);
}

void Solver::fill(){
	FOR(i,V){
		FOR(j,V){
			int u=i*10+j;
			if(mat[i][j]==0){
				FOR(k,V) {
					int v=k*10+j;
					if(mat[v/10][v%10]==0 && u!=v){
						addEdge(u,v);
					}
				}
				FOR(k,V) {
					int v=i*10+k;
					if(mat[v/10][v%10]==0 && u!=v){
						addEdge(u,v);
					}
				}
				FOR(k,V) {
					int srt=3*((i/3)*10+(j/3));
					int v= srt+(k/3)*10+(k%3);
					if(mat[v/10][v%10]==0 && u!=v){
						addEdge(u,v);
					}
				}
			}
		}
	}
	FOR(i,V){
		FOR(j,V){
			int u=i*10+j;
    		sort(adj[u].begin(), adj[u].end());
		    auto ip = unique(adj[u].begin(), adj[u].end());
		    adj[u].resize(distance(adj[u].begin(), ip));
		}
	}
	bool flg=false;
	int s;
	FOR(i,V*V){
		if(mat[i/V][i%V] == 0){
			s = i;
			break;
		}
	}

	BFS(s);
	//FOR(i,bfs_ar.size()) cout<<bfs_ar[i]<<" "; cout<<endl;
}

bool Solver::check(int u, int num){
	int i=u/10,j=u%10;
	FOR(k,V) {
		int v=k*10+j;
		if(mat[k][j]==num && u!=v){
			return false;
		}
	}
	FOR(k,V) {
		int v=i*10+k;
		if(mat[i][k]==num && u!=v){
			return false;
		}
	}
	FOR(k,V) {
		int srt=3*((i/3)*10+(j/3));
		int v= srt+(k/3)*10+(k%3);
		if(mat[v/10][v%10]==num && u!=v){
			return false;
		}
	}
	return true;
}

void Solver::BFS(int s) {
    refresh();
    queue<int> q;
    visited[s] = true;
    q.push(s);
    while(!q.empty()) {
        s = q.front();
        this->bfs_ar.push_back(s);
        q.pop();
        FOR(i,adj[s].size()){
            if (!visited[adj[s][i]]) {
                visited[adj[s][i]] = true;
                q.push(adj[s][i]);
            }
        }
    }
}

void Solver::DFS(int s){
	visited[s]=true;
	this->dfs_ar.push_back(s);
	FOR(i,adj[s].size()){
		int dst=adj[s][i];
		if(!visited[dst]){
			DFS(dst);
		}
	}
}

void Solver::solve(int s){
	if(s >= bfs_ar.size()) {
		solved=true;
		cout<<"solved...!!!\n";
		return;
	}
	int pos = bfs_ar[s];
	FOR1(i,9){
		if(check(pos,i)){
			this->mat[pos/10][pos%10]=i;
			solve(s+1);
		}
	}
	if(!solved) mat[pos/10][pos%10]=0;
}
