#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

class State{ 
public:
	int Size, *a, Cost;
	
	State(int _Size) : Size(_Size){
		Cost=0;
		if (!(a = new int [Size])){
			cout << "Allocation error!\n";
			exit(1);
		}
		for(int i=0; i<Size; i++){
			a[i] = rand() % Size;
		}
		Calc_cost();
	}
	
	void Print(){
		for ( int i=0; i<Size; i++){
			cout << " ";
			for ( int k=0; k<Size ; k++) cout << "----";
			cout << endl << "|";
			for ( int j=0; j<Size; j++){
				if ( a[j]==i ) cout << " Q |";
				else cout << "   |";
			}
			cout << endl;
		}
		cout << " ";
		for ( int k=0; k<Size ; k++) cout << "----"; 
		cout << endl;
	}
	
	int Calc_cost(){
		Cost=0;
		for ( int i=0; i<Size; i++){
			for ( int j=i+1; j<Size; j++){
				if ( a[i]==a[j] || abs(a[i]-a[j])==j-i )
					Cost=Cost+1;
			}
		}
		return Cost;
	}

	State& operator= (const State &Source){
		if ( this == &Source)
			return *this;

		for (int i=0; i<Size; i++)
			a[i]=Source.a[i];
		Cost=Source.Cost;
		
		return *this;
	}	
	
	int Hill( int Moves_Limit){
		int temp = Restarts( Moves_Limit, 0);
		return temp;
	}
	
	int Restarts( int Moves_Limit, int Restarts_Limit){
		return Sideway( Moves_Limit, Restarts_Limit, 0);
	}
	
	int Sideway( int Moves_Limit, int Restarts_Limit, int SidewayMoves_Limit){
		State next(Size);
		
		int k, moves=0;
		int notFound=Cost;
		int T_Cost;
		int update=0;
		int column, line;
		int SidewayMoves_Counter;

		for ( int l=0; l<=Restarts_Limit && notFound; l++){			
			
			if (l>0){
				State restart(Size);
				*this = restart;
			}
			T_Cost=Cost;
			SidewayMoves_Counter=0;
				
			for ( k=0; k<Moves_Limit && notFound; k++){
				for ( int i=0; i<Size; i++){
					for ( int j=0; j<Size; j++){
						next = *this;
						if ( next.a[i]!=j ){
							next.a[i]=j;
							next.Calc_cost();
							if( next.Cost <= T_Cost){
								update = 1;
								column = i;
								line = j;
								T_Cost = next.Cost;
							}
						}	
					}
				}
				
				if (update){
					update=0;	
					
					if(T_Cost==Cost){
						SidewayMoves_Counter++;	
					}
					
					if(SidewayMoves_Counter<=SidewayMoves_Limit){
						a[column]=line;
						Cost=T_Cost;
						if ( Cost==0) notFound=0;
					}
				}
			}
			moves+=k;	
		}
		
		return moves;
	}

	int FirstChoice( int Moves_Limit, int Restarts_Limit, int SidewayMoves_Limit){
		State next(Size);

		int k, moves=0;
		int notFound=Cost;
		int update;
		int SidewayMoves_Counter;


		for ( int l=0; l<=Restarts_Limit && notFound; l++){
		
			if (l>0){
				State restart(Size);
				*this = restart;
			}
			
			for ( k=0; k<Moves_Limit && notFound; k++){
				int checked[Size][Size];
				for( int i=0; i<Size; i++){
					for( int j=0; j<Size; j++){
						checked[i][j]=0;
					}
				}
				
				update=0;
				
				while (!update && SidewayMoves_Counter<SidewayMoves_Limit){
					int i=rand() % Size;
					int j=rand() % Size;
					next = *this;
					if ( next.a[i]!=j && checked[i][j]==1){
						checked[i][j]=1;
						next.a[i]=j;
						next.Calc_cost();
						if( next.Cost <= Cost){
							if(next.Cost==Cost){
								SidewayMoves_Counter++;	
							}
		
							update = 1;
							a[i]=j;
							Cost = next.Cost;
							if ( Cost==0) notFound=0;
						}	
					}
					
				}
			}
			moves+=k;
		}
		return moves;
	}
};
	
int main(int argc, char *argv[]){
	int Size, Problems, Moves_Limit, Restarts_Limit, SidewayMoves_Limit;
	
	cout << "Size of problems: ";
	cin >> Size;
	cout << "Number of problems: ";
	cin >> Problems;
	cout << "Limit of moves: ";
	cin >> Moves_Limit;
	cout << "Limit of restarts: ";
	cin >> Restarts_Limit;
	cout << "Limit of sideway moves: ";
	cin >> SidewayMoves_Limit;
    cout << endl;
		
	
	
	int Hill_sum=0;
	int Hill_solved=0;
	int Restarts_sum=0;
	int Restarts_solved=0;
	int Sideway_sum=0;
	int Sideway_solved=0;
	int FirstChoice_sum=0;
	int FirstChoice_solved=0;

	for( int i=0; i<Problems; i++){
		srand(time(NULL)+i);
		State init(Size);
		State temp(Size);

		temp = init;
		int Hill_moves=temp.Hill(Moves_Limit);
		Hill_sum+=Hill_moves;
		int Hill_cost=temp.Cost;
		if (!Hill_cost) Hill_solved++;
		
		temp = init;
		int Restarts_moves=temp.Restarts(Moves_Limit, Restarts_Limit);
		Restarts_sum+=Restarts_moves;
		int Restarts_cost=temp.Cost;
		if (!Restarts_cost) Restarts_solved++;
		
		temp = init;
		int Sideway_moves=temp.Sideway(Moves_Limit, Restarts_Limit, SidewayMoves_Limit);
		Sideway_sum+=Sideway_moves;
		int Sideway_cost=temp.Cost;
		if (!Sideway_cost) Sideway_solved++;
		
		temp = init;
		int FirstChoice_moves=temp.FirstChoice(Moves_Limit, Restarts_Limit, SidewayMoves_Limit);
		FirstChoice_sum+=FirstChoice_moves;
		int FirstChoice_cost=temp.Cost;
		if (!FirstChoice_cost) FirstChoice_solved++;
		
		cout << "\nProblem " <<i+1<< "\t\t\t\t\t |   Kiniseis   |  Sygkrouseis  |  Lysi" << endl;
		cout << "-----------------------------------------------------------------------------------------" << endl;
		
		cout << "Greedy hill-climbing\t\t\t\t |";
			cout << "\t" << Hill_moves << "\t|";
			cout << "\t" << Hill_cost << "\t|";
			if (Hill_cost==0) cout << "   V";
			else cout << "   X";
		cout << endl;
		
		cout << "Greedy hill-climbing (+restarts)\t\t |" ;
			cout << "\t" << Restarts_moves << "\t|";
			cout << "\t" << Restarts_cost << "\t|";
			if (Restarts_cost==0) cout << "   V";
			else cout << "   X";
		cout << endl;
		
		cout << "Greedy hill-climbing (+restarts +sideways moves) |";
			cout << "\t" << Sideway_moves << "\t|";
			cout << "\t" << Sideway_cost << "\t|";
			if (Sideway_cost==0) cout << "   V";
			else cout << "   X";
		cout << endl;
		
		cout << "First-choice hill-climbing (+restarts)\t\t |";
			cout << "\t" << FirstChoice_moves << "\t|";
			cout << "\t" << FirstChoice_cost << "\t|";
			if (FirstChoice_cost==0) cout << "   V";
			else cout << "   X";
		cout << endl;
	}
	if (Problems > 1){
		cout << "\n\n\nStatistika\t\t\t\t\t |    Kiniseis   |     Elyse    |    %" << endl;
		cout << "-----------------------------------------------------------------------------------------" << endl;
		
		cout << "Greedy hill-climbing\t\t\t\t |";
			cout.precision(3);
			cout << "\t" << (float)Hill_sum/Problems << "\t |";
			cout << "\t" << Hill_solved << "/" << Problems << "\t|";
			cout.width(5);
			cout.precision(3);
			cout << (float)Hill_solved/Problems*100 << "%";
		cout << endl;
		
		cout << "Greedy hill-climbing (+restarts)\t\t |";
			cout.precision(3);
			cout << "\t" << (float)Restarts_sum/Problems << "\t |";
			cout << "\t" << Restarts_solved << "/" << Problems << "\t|";
			cout.width(5);
			cout.precision(3);
			cout << (float)Restarts_solved/Problems*100 << "%";
		cout << endl;
		
		cout << "Greedy hill-climbing (+restarts +sideways moves) |" ;
			cout.precision(3);
			cout << "\t" << (float)Sideway_sum/Problems << "\t |";
			cout << "\t" << Sideway_solved << "/" << Problems << "\t|";
			cout.width(5);
			cout.precision(3);
			cout << (float)Sideway_solved/Problems*100 << "%";
		cout << endl;
		
		cout << "First-choice hill-climbing (+restarts)\t\t |";
			cout.precision(3);
			cout << "\t" << (float)FirstChoice_sum/Problems << "\t |";
			cout << "\t" << FirstChoice_solved << "/" << Problems << "\t|";
			cout.width(5);
			cout.precision(3);
			cout << (float)FirstChoice_solved/Problems*100 << "%";
		cout << endl;
	}
	
	return 0;
}
