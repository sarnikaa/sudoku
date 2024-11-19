#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>
#define UNASSIGNED 0

using namespace std;

class Sudoku
{
        private:
                int grid[9][9];
                int solnGrid[9][9];
                int guessNum[9];
                int gridPos[81];
                int conRC[9][9];
        public:
                Sudoku ();
                bool checkEqual();
                int checkPos();
                void fillDiagonalBox(int);
                void createSeed();
                void printGrid();
                bool solveGrid();
                void countSoln(int &number);
                void genPuzzle();
                void printSoln();
                void con();
};

int genRandNum(int);
bool FindUnassignedLocation(int grid[9][9],int&,int&);
bool UsedInRow(int[9][9],int,int);
bool UsedInCol(int[9][9],int,int);
bool UsedInBox(int[9][9],int,int,int);
bool isSafe(int grid[9][9],int,int,int);
Sudoku* create();
Sudoku* create()
{
        srand(time(NULL));
        Sudoku *puzzle = new Sudoku();
        puzzle->createSeed();
        puzzle->genPuzzle();
        puzzle->printGrid();
        return puzzle;
}

//Generate random number
int genRandNum(int x)
{
        return rand()%x;
}

//Functions for solving grid
bool FindUnassignedLocation(int grid[9][9], int &row, int &col)
{
        for (row = 0; row < 9; row++)
        {
                for (col = 0; col < 9; col++)
                {
                        if (grid[row][col] == UNASSIGNED)
                        return true;
                }
        }

        return false;
}

bool UsedInRow(int grid[9][9], int row, int num)
{
        for (int col = 0; col < 9; col++)
        {
                if (grid[row][col] == num)
                        return true;
        }
        return false;
}

bool UsedInCol(int grid[9][9], int col, int num)
{
        for (int row = 0; row < 9; row++)
        {
                if (grid[row][col] == num)
                        return true;
        }
        return false;
}
bool UsedInBox(int grid[9][9], int boxStartRow, int boxStartCol, int num)
{
        for (int row = 0; row < 3; row++)
        {
                for (int col = 0; col < 3; col++)
                {
                        if (grid[row+boxStartRow][col+boxStartCol] == num)
                                return true;
                }
        }
        return false;
}

bool isSafe(int grid[9][9], int row, int col, int num)
{
        return !UsedInRow(grid, row, num) && !UsedInCol(grid, col, num) && !UsedInBox(grid, row - row%3 , col - col%3, num);
}
//Intialising
Sudoku::Sudoku()
{
  // Randomly shuffling the array for removing grid positions
        for(int i=0;i<81;i++)
        {
                this->gridPos[i] = i;
        }

        random_shuffle(this->gridPos, (this->gridPos) + 81, genRandNum);

  // Randomly shuffling the guessing number array
        for(int i=0;i<9;i++)
        {
                this->guessNum[i]=i+1;
        }

        random_shuffle(this->guessNum, (this->guessNum) + 9, genRandNum);

  // Initialising the grid
        for(int i=0;i<9;i++)
        {
                for(int j=0;j<9;j++)
                {
                        this->grid[i][j]=0;
                }
        }
}
//To find if the user has solved the puzzle
bool Sudoku::checkEqual()
{
        int c=0;
        for(int i=0;i<9;i++)
        {
                for(int j=0;j<9;j++)
                {
                        if(grid[i][j]==solnGrid[i][j])
                                c++;
                }
        }
        if(c==81)
                return true;
        else
                return false;
}
//Playing
int Sudoku::checkPos()
{
        int r,c,num;
        cout<<"Enter row and column value as -1 to view the solution\n";
        cout<<"Enter row value : ";
        cin>>r;
        if(!((r>=1 && r<=9)||r==-1))
        {
           cout<<"Enter a valid value. Try again!"<<endl<<endl;
           return 0;
        }
        cout<<"Enter column value : ";
        cin>>c;
        if(!((c>=1 && c<=9)||c==-1))
        {
           cout<<"Enter a valid value. Try again!"<<endl<<endl;
           return 0;
        }
        r-=1;
        c-=1;
        if(conRC[r][c]==1)
        {
           cout<<"It is a constant value. Try again!"<<endl<<endl;
           return 0;
        }
        if(r==-2 && c==-2)
        {
                printSoln();
                return 2;
        }
        cout<<"Enter the number to be placed at ("<< r+1<<","<<c+1<<") : ";
        cin>>num;
        system("clear");
	  if(num!=solnGrid[r][c])
        {
                if(UsedInRow(grid, r, num))
                        cout << "\033[31mThis number is already present in Row.\033[0m\n";
                if(UsedInCol(grid, c, num))
                        cout << "\033[31mThis number is already present in Column.\033[0m\n";
                if(UsedInBox(grid, r - r%3 , c - c%3, num))
                        cout << "\033[31mThis number is already present in Box .\033[0m\n";
                grid[r][c]=num;
                printGrid();
                return 0;
        }
        else
        {
                grid[r][c]=num;
                printGrid();
                return 1;
        }
}
// Create a random grid
void Sudoku::fillDiagonalBox(int x)
{
        int start = x*3;
        random_shuffle(this->guessNum, (this->guessNum) + 9, genRandNum);
        for (int i = 0; i < 3; ++i)
        {
                for (int j = 0; j < 3; ++j)
                {
                        this->grid[start+i][start+j] = guessNum[i*3+j];
                }
        }
}

void Sudoku::createSeed()
{
//To fill diagonal boxes
        this->fillDiagonalBox(0);
        this->fillDiagonalBox(1);
        this->fillDiagonalBox(2);


//To fill the remaining boxes
        this->solveGrid();
// Saving the solution grid
        for(int i=0;i<9;i++)
        {
                 for(int j=0;j<9;j++)
                 {
                        this->solnGrid[i][j] = this->grid[i][j];
                 }
        }
}
//To print the puzzle
void Sudoku::printGrid()
{
        cout<<endl<<endl;
        for (int row = 0; row < 9; row++)
        {
                cout<<"\t\t\t\t\t\t   ";
                for (int col = 0; col < 9; col++)
                {
                        if(grid[row][col]==0)
                                cout << grid[row][col] << " ";
                        else if(grid[row][col]!=solnGrid[row][col])
                        {
                                cout <<"\033[1;36;41m";
                                cout<<grid[row][col];
                                cout<<"\033[0m ";
                        }
                        else if(conRC[row][col]==1)
                                cout <<"\033[32m"<< grid[row][col]<<"\033[0m" << " ";
                        else
                                cout <<"\033[36m"<< grid[row][col]<<"\033[0m" << " ";
                        if ((col+1) % 3 == 0)
                                cout << "  ";  // Print extra space every 3 columns
                }
                cout << endl;
                if ((row+1) % 3 == 0)
                        cout << endl;  // Print extra newline every 3 rows
        }
}
//To print the solutuin grid
void Sudoku::printSoln()
{
        cout<<endl<<endl;
        for (int row = 0; row < 9; row++)
        {
                cout<<"\t\t\t\t\t\t   ";
                for (int col = 0; col < 9; col++)
                {
                        if(conRC[row][col]==1)
                                cout <<"\033[32m"<< grid[row][col]<<"\033[0m" << " ";
                        else
                        {
                                cout <<"\033[36m"<< solnGrid[row][col]<<"\033[0m" << " ";
                                grid[row][col]=solnGrid[row][col];
                        }
                        if ((col+1) % 3 == 0)
                                cout << "  ";  // Print extra space every 3 columns
                }
                cout << endl;
                if ((row+1) % 3 == 0)
                        cout << endl;  // Print extra newline every 3 rows
        }
}
//Sudoku solver
bool Sudoku::solveGrid()
{
        int row, col;

    //To check if there is no unassigned location
        if (!FindUnassignedLocation(this->grid, row, col))
                return true;

        for (int num = 0; num < 9; num++)
        {
                if (isSafe(this->grid, row, col, this->guessNum[num]))
                {
                        this->grid[row][col] = this->guessNum[num];

                        if (solveGrid())
                                return true;

                        this->grid[row][col] = UNASSIGNED;
                }
        }
        return false; // this triggers backtracking
}
// To check if the grid is uniquely solvable
void Sudoku::countSoln(int &number)
{
        int row, col;
        if(!FindUnassignedLocation(this->grid, row, col))
        {
                number++;
                return ;
        }
        for(int i=0;i<9 && number<2;i++)
        {
                if( isSafe(this->grid, row, col, this->guessNum[i]) )
                {
                        this->grid[row][col] = this->guessNum[i];
                        countSoln(number);
                }

                this->grid[row][col] = UNASSIGNED;
        }

}


//To generate puzzle
void Sudoku::genPuzzle()
{
        for(int i=0;i<81;i++)
        {
                int x = (this->gridPos[i])%9;
                int y = (this->gridPos[i])/9;
                int temp = this->grid[x][y];
                this->grid[x][y] = UNASSIGNED;

    //When there is more than 1 solution , replace the removed cell back.
                int check=0;
                countSoln(check);
                if(check!=1)
                        this->grid[x][y] = temp;
        }
    //After puzzle generation to keep initial values constant
        con();
}
void Sudoku::con()
{
        for(int i=0;i<9;i++)
        {
                for(int j=0;j<9;j++)
                {
                        if(grid[i][j]!=0)
                                conRC[i][j]=1;
                }
        }
}
int main()
{
        Sudoku *p;
        time_t start,end;
        int choice,goback,r,c;
        do
        {
                cout<<"\t\t\t\t  #####  ##     ## ######      #####   ##    ## ##     ##"<<endl;
                cout<<"\t\t\t\t ####### ##     ## ########   #######  ##    ## ##     ##"<<endl;
                cout<<"\t\t\t\t ##    # ##     ## ##     ## ##     ## ##   ##  ##     ##"<<endl;
                cout<<"\t\t\t\t ##      ##     ## ##     ## ##     ## ##  ##   ##     ##"<<endl;
                cout<<"\t\t\t\t ######  ##     ## ##     ## ##     ## ####     ##     ##"<<endl;
                cout<<"\t\t\t\t      ## ##     ## ##     ## ##     ## ##  ##   ##     ##"<<endl;
                cout<<"\t\t\t\t #    ## ##     ## ##     ## ##     ## ##   ##  ##     ##"<<endl;
                cout<<"\t\t\t\t #######  #######  ########   #######  ##    ##  #######"<<endl;
                cout<<"\t\t\t\t  #####    #####   ######      #####   ##    ##   #####"<<endl;

                cout<<endl<<endl<<endl<<endl;

                cout<<"\t\t\t\t   By  S.SARNIKA 22PD31\t\tS.AHALYANJUNA 22PD02"<<endl;

                cout<<endl<<endl<<endl<<endl<<endl;

                cout<<"\t\t\t\t\t\t\t  --------- "<<endl;
                cout<<"\t\t\t\t\t\t\t|  [1]PLAY  |"<<endl;
                cout<<"\t\t\t\t\t\t\t  --------- "<<endl;

                cout<<endl<<endl;


                cout<<"\t\t\t\t\t\t\t  ---------- "<<endl;
                cout<<"\t\t\t\t\t\t\t|  [2]ABOUT  |"<<endl;
                cout<<"\t\t\t\t\t\t\t  ---------- "<<endl;

                cout<<endl<<endl;


                cout<<"\t\t\t\t\t\t\t  ---------- "<<endl;
                cout<<"\t\t\t\t\t\t\t|  [3]RULES  |"<<endl;
                cout<<"\t\t\t\t\t\t\t  ---------- "<<endl;

                cout<<endl<<endl;

		    cout<<"\t\t\t\t\t\t\t  ---------   "<<endl;
                cout<<"\t\t\t\t\t\t\t|  [4]EXIT  | "<<endl;
                cout<<"\t\t\t\t\t\t\t  ---------   "<<endl;

                cout<<endl<<endl;

                cout<<"\t\t\t\t\t\t   Enter your choice : ";
                cin>>choice;

                system("clear");

                switch(choice)
                {
                        case 1:
                                p=create();
                                int val;
                                time(&start);
                                do
                                {
                                       val=p->checkPos();

                                }
                                while(!p->checkEqual());
                                if(val==1)
                                {
                                        time(&end);
                                   cout<<"Congratulations on solving the puzzle!\nYou have done it in "<<difftime(end,start)/60<<"minutes\n";
                                }
                                break;
				case 2:
                                cout<<"\t\t\t\t\t\t\t   ABOUT"<<endl<<endl;
                                cout<<"\t\tSudoku ,originally called Number Place is a logic-based, combinatorial number-placement puzzle.In its simplest and most common configuration, sudoku consists of a 9 × 9 grid with numbers appear";
                                break;
                        case 3:
                                cout<<"\t\t\t\t\t\t\t   RULES"<<endl<<endl;
                                cout<<"\t\t\t\t    All the empty boxes in the puzzle must be filled such that : \n\n";
                                cout<<"\t\t\t(1)Each row must contain the numbers from 1 to 9, without repetitions\n";
                                cout<<"\t\t\t(2)Each column must contain the numbers from 1 to 9, without repetitions\n";
                                cout<<"\t\t\t(3)The digits can only occur once per block (3x3 Matrix)\n";
                                break;
                        case 4:
                                cout<<"\t\t\t\t\t\t\tExiting...\n";
                                exit(1);
                                break;

                        default:
                                cout<<"\t\t\t\t\t\t   Inavlid choice!"<<endl;
                }
                cout<<endl<<endl<<"Press 1 to return to menu\n";
                cin>>goback;
                system("clear");
        }
        while(goback==1);
}
