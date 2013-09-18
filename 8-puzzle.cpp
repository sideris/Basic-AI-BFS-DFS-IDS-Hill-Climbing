#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <deque>
#include <stack>
using namespace std;

//apothikeusi katastasis
class State{ 
public:
	int a[9];
	
	State(){
		for(int i=0; i<9; i++){
				a[i] = rand() % 9;
				while (found(i)) { a[i] = rand() % 9; } 
		}
	}
	
	//ektypwsi katastasis
	void print(){
		cout << "-----------------" << endl;
		for(int i=0; i<9; i++){
			if (i>0 && i%3==0) cout << endl;
			cout << a[i] << "\t";
		}
		cout << endl << "-----------------" << endl;
	}
	
	//gia tin apofygi epanalamvanomenwn stoixeiwn
	int found(int i){
		for(int j=0; j<i; j++){
			if (a[i]==a[j]) return 1;
		}
		return 0;
	}
	
	//epistrefei tin thesi tou kenou (0)
	int findzero(){
		for (int i=0; i<9; i++){
			if (a[i]==0) return i;
		}
	}
	
	//epistrefei neo state allazontas ta stoixeia i kai j metaksi tous
	State exch(int i, int j){
		State b;
		for (int k=0; k<9; k++)
			b.a[k]=a[k];
		int t=b.a[i];
		b.a[i]=b.a[j];
		b.a[j]=t;
		return b;
	}

	//elegzei an to state einai idio me to s
	int equal(State s){
		for(int i=0; i<9; i++){
			if (a[i]!=s.a[i]) return 0;
		}
		return 1;
	}
	
	//elegxei an to state einai to state stoxou
	int goal(){
		int g[9] = {1,2,3,8,0,4,7,6,5};
		for (int i=0; i<9; i++){
			if (a[i]!=g[i]) return 0; 
		}
		return 1;
	}
	
};

class Node{
public:
	State s;
	Node *father;
	int action, cost, depth;
	
	//dimiourgia rizas dentrou(arxikis katastasis)
	Node(){
		State s();
		father=NULL;
		action=-1;
		cost=1;
		depth=0;
	}
	
	//dimiourgia komvou meso expand
	Node(State _s, Node *_father, int _action, int _depth){
		s=_s;
		father=_father;
		action=_action;
		depth=_depth;
	}
	
	//antigrafi komvou
	Node copy(){
		Node b;
		for (int i=0; i<9; i++)
			b.s.a[i]=s.a[i];		
		b.father=father;
		b.action=action;
		b.depth=depth;
		
		return b;
	}
	
	//ektypwsi cost+depth
	void print(){
		cout << "Cost: \t" << cost << endl;
		cout << "Depth: \t" << depth << endl;
	}
	
	//expand komvou
	void expand(deque<Node> *deque){
		int p = s.findzero();
		//moveup	(0)
		if ((p!=0 && p!=1 && p!=2) && action!=1){
			Node n(s.exch(p,p-3), this, 0, depth+1);
			(*deque).push_back(n);
		}
		//movedown 	(1)
		if ((p!=6 && p!=7 && p!=8) && action!=0){
			Node n(s.exch(p,p+3), this, 1, depth+1);
			(*deque).push_back(n);
		}
		//moveright (2)
		if ((p!=2 && p!=5 && p!=8) && action!=3){
			Node n(s.exch(p,p+1), this, 2, depth+1);
			(*deque).push_back(n);
		}
		//moveleft 	(3)
		if ((p!=0 && p!=3 && p!=6) && action!=2){
			Node n(s.exch(p,p-1), this, 3, depth+1);
			(*deque).push_back(n);
		}
	}
	
	//elegxos gia to an o komvos exei epektathei
	int expanded(deque<State> *deque){
		int max=(*deque).size()>depth?depth:(*deque).size();
		for (int i=0; i<max; i++){
			if ( s.equal( (*deque)[i] ) ){
				return 1;
			}
		}
		return 0;
	}

		
	int bfs(){
		deque<Node> toexpand;
		deque<State> expanded;
		
		toexpand.push_back(*this);
		while ( !toexpand.empty() ){
			if ( toexpand.front().s.goal()==1 ){ 
				cout << "------|BFS|------" << endl;
				cout << "Solution found!" << endl;
				toexpand.front().print();
				cost = toexpand.front().cost;
				toexpand.clear();
				return cost;
			}
			else{
				if ( !(toexpand.front().expanded(&expanded)) ){
					toexpand.front().expand(&toexpand);
					expanded.push_front( toexpand.front().s );
					toexpand[1].cost=toexpand[0].cost+1;
				}
				toexpand.pop_front();
			}
		}
		if ( toexpand.empty() ) cout << endl << "Solution NOT found!" << endl;
		return 0;
	}
	
	int dfs(int idsdepth){
		deque<Node> toexpand;
		
		if (idsdepth==-1) idsdepth = sizeof(int);
		
		toexpand.push_back(*this);
		while ( !toexpand.empty() ){
				if (toexpand.back().depth < idsdepth){
					if ( toexpand.back().s.goal()==1 ){ 
						if (idsdepth == sizeof(int)) 
							cout << "------|DFS|------" << endl;
						else 
							cout << "------|IDS|------" << endl;
						cout << "Solution found!" << endl;
						toexpand.back().print();
						toexpand.clear();
						return cost;
					}
					else{
						Node t;
						t= toexpand.back().copy();
						toexpand.pop_back();
						t.expand(&toexpand);
					}
				}
				else return 0;
		}
		if ( toexpand.empty() ) cout << endl << "Solution NOT found!" << endl;
		return 0;
	}
	
	int ids(){
		for(int i=0;;i++){
			int t = dfs(i);
			if (t!=0) return t; 
		}
	}
};

	
int main(int argc, char *argv[]){
	int num=1;
	int bfscost=0;
	int bfsfail=0;
	int dfscost=0;
	int dfsfail=0;
	int idscost=0;
	int idsfail=0;

	
	cout << "\nEnter number of problems to solve: ";
	cin >> num;
    cout << endl;
	
	if (argc == 2){
		cout << "-------DEMO------\n\n";
	}
	
	for(int i=0;i<num;i++){
		int _bfs=0;
		int _dfs=0;
		int _ids=0;		
		
		cout << "\n    Problem " << i+1 << endl;
		srand(argc==2?(2*i+1):time(NULL)+i);
        Node n;
		n.s.print();
		
		_bfs=n.bfs();
		// _dfs=n.dfs(-1);
		// _ids=n.ids();
		
		if (_bfs>0) 
			bfscost+=_bfs;
		else
			bfsfail+=1;
		
		if (_dfs>0) 
			dfscost+=_dfs;
		else
			dfsfail+=1;
		
		if (_ids>0) 
			idscost+=_ids;
		else
			idsfail+=1;
	}
	if (num > 1){
		cout << "\n\n------|BFS|-------";
		cout << "\nAverage cost = " << bfscost/(num-bfsfail);
		cout << "\nSolved " << num-bfsfail << "/" << num;
		cout << "\n------------------";
		// cout << "\n\n------|DFS|-------";
		// cout << "\nAverage cost = " << dfscost/(num-dfsfail);
		// cout << "\nSolved " << num-dfsfail << "/" << num;
		// cout << "\n------------------";
		// cout << "\n\n------|IDS|-------";
		// cout << "\nAverage cost = " << idscost/(num-idsfail);
		// cout << "\nSolved " << num-idsfail << "/" << num;
		// cout << "\n------------------";
		cout << endl;
	}
	
	return 0;
}
