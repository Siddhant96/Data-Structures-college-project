#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include <sstream>
#include <graphics.h>
using namespace std;

#define gridSize 13
#define weight 1
#define INF 0x3f3f3f3f
#define maxSize 1000
#define mazeFile "Mazeq.txt"

typedef pair<int,int> iPair;

string to_string(int);
void swap(int&, int&);

class NQueens
{
    int size;
    vector<vector<char> > matrix;
    bool solvable(int);
    bool isSafe(int, int);
    void printMatrix();
    void video();
    void initiateGraphics();
public:
    NQueens(int);
    void solveNQueens();
};

void NQueens::video()
{
    setfillstyle(SOLID_FILL,LIGHTGREEN);
    floodfill(1,1,WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    setbkcolor(WHITE);
    setcolor(LIGHTRED);
    settextstyle(3,0,5);
    setbkcolor(LIGHTGREEN);
    outtextxy(250,30,"N Queens");
    setbkcolor(BLACK);
    settextstyle(0,0,2);
    setcolor(BLACK);
    int count=0;
    int left=100,right=640,top=100,bottom=640;
    int gap=(right-left)/size;
    for(int i=0;i<3;i++,left++,right--,top++,bottom--)
        rectangle(left,top,right,bottom);
    for(int i=100;i<620;i+=gap)
    {
        line(100,i,640,i);
        line(i,100,i,640);
    }
    for(int i=100+(gap/2);i<640;i+=gap)
    {
       for(int j=100+(gap/2);j<640;j+=gap)
        {
             if(count%2==0)
             {
                 setfillstyle(SOLID_FILL,LIGHTGRAY);
                 floodfill(j,i,BLACK);
                 setfillstyle(SOLID_FILL,WHITE);
             }
             else
             {
                 setfillstyle(SOLID_FILL,WHITE);
                 floodfill(j,i,BLACK);
             }
            ++count;
        }
        if(size%2==0)
            ++count;
    }
    setfillstyle(SOLID_FILL,WHITE);
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            if(matrix[i][j]=='Q')
            {
                setfillstyle(SOLID_FILL,RED);
                circle(100+(gap/2)*(2*j+1),100+(gap/2)*(2*i+1),gap/3);
                floodfill(100+(gap/2)*(2*j+1),100+(gap/2)*(2*i+1),BLACK);
                setfillstyle(SOLID_FILL,WHITE);
            }
        }
    }
}

void NQueens::initiateGraphics()
{
    int gd=DETECT,gm;
    initgraph(&gd,&gm,"");
    initwindow(800,700);
}

NQueens::NQueens(int size)
{
    this->size = size;
    matrix.assign(size,vector<char>(size,'-'));
}

void NQueens::solveNQueens()
{
    initiateGraphics();
    int initialRow = 0;
    if(solvable(initialRow))
        printMatrix();
    else
        cout << "No solution exists!";
}

bool NQueens::solvable(int row)
{
    //If we fill all rows, then problem is solved
    if(size==row)
        return true;

    for(int column=0;column<size;column++)
    {
        //If queen can be placed
        if(isSafe(row,column))
        {
            matrix[row][column] = 'Q'; //place queen
            //Recur for next row
                cleardevice();
                video();
                delay(500);
            if(solvable(row+1))
                return true;
            //If we're here that means solution didn't work, so backtrack
            matrix[row][column] = '-';
                cleardevice();
                video();
                delay(500);
        }
    }
    //If loop completes then we didn't find the solution
    return false;
}

bool NQueens::isSafe(int row, int column)
{
    //Check for column
    for(int i=0;i<row;i++)
        if(matrix[i][column]=='Q')
            return false;

    //Check for major diagonal (left side)
    for(int i=row,j=column;i>=0&&j>=0;i--,j--)
        if(matrix[i][j]=='Q')
            return false;

    //Check second diagonal (right side)
    for(int i=row,j=column;i>=0&&j<size;i--,j++)
        if(matrix[i][j]=='Q')
            return false;

    return true;
}

void NQueens::printMatrix()
{
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
            cout << matrix[i][j] << ' ';
        cout << '\n';
    }
}

class Grid
{
    static const int N = 9;
    string message;
    vector<vector<int> > matrix;
    vector<vector<int> > solution;
    vector<vector<int> > auxilliary;
    bool isSafe(int, int, int);
    bool solvable();
    void printMatrix(vector<vector<int> >);
    bool gridFilled(int&, int&);
    bool correct(int, int, int);
    void refreshSudoku();
    pair<int,int> getCordinates(int,int);
    void initiateGraphics();

public:
    Grid();
    void start();
};


Grid::Grid()
{

    //cout << "Enter the values of the 9x9 grid.\n";
    matrix.assign(N,vector<int>(N,0));
    solution.assign(N,vector<int>(N,0));
    auxilliary.assign(N,vector<int>(N,0));

    ifstream fin("sudoku.txt");

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            fin >> matrix[i][j];
            solution[i][j] = auxilliary[i][j] = matrix[i][j];
        }
    }
}

void Grid::start()
{
    cout << "\nProblem:\n";
    printMatrix(matrix);
    if(solvable()) //start from 0th row
    {
        cout << "\nSolution:\n";
        printMatrix(solution);
    }
    else
        cout << "No solution exists.";
    initiateGraphics();
}

bool Grid::solvable()
{
    int row, col;
    if(gridFilled(row,col)) //Find a 0, if not found, then solved
        return true;

    for(int num=1;num<=9;num++) //For all 9 values
    {
        if(isSafe(row,col,num)) //if not filled
        {
            solution[row][col] = num;

            if(solvable()) //Recursively fill other blocks and see if solved
                return true;

            //If code is here, then our above soltion failed
            solution[row][col] = 0;
        }
    }
    //If all numbers have been tried and none satisfy, then our grid is not solvable
    return false;
}

bool Grid::gridFilled(int &row, int&col)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(solution[i][j]==0)
            {
                row = i;
                col = j;
                return false;
            }
        }
    }
    return true;
}

bool Grid::isSafe(int row, int col, int num)
{
    //Check for current row
    for(int i=0;i<N;i++)
        if(solution[row][i]==num)
            return false;

    //Check for current column
    for(int i=0;i<N;i++)
        if(solution[i][col]==num)
            return false;

    //Check for current box
    int box_row = row - row%3;
    int box_col = col - col%3;

    for(int i=box_row;i<box_row+3;i++)
        for(int j=box_col;j<box_col+3;j++)
            if(solution[i][j]==num)
                return false;

    //If code is here then it's safe to fill the number
    return true;
}

void Grid::printMatrix(vector<vector<int> > sudokuMatrix)
{
    for(int i=0;i<N;i++)
    {
        if(i!=0 && i%3==0)
            cout << "- - - | - - - | - - -\n";
        for(int j=0;j<N;j++)
        {
            if(sudokuMatrix[i][j])
                cout << sudokuMatrix[i][j] << ' ';
            else
                cout << "  ";
            if((j+1!=N) && (j+1)%3==0)
                cout << '|' << ' ';
        }
        cout << '\n';
    }
}

bool Grid::correct(int i, int j, int value)
{
    if(solution[i][j] == value)
    {
        auxilliary[i][j] = value;
        message = "";
    }
    else
        message = "Error! Wrong move!";
}


pair<int,int> Grid::getCordinates(int x1, int y1){
    int y = (x1 - 105)/60;
    int x = (y1 - 105)/60;
    //cout<<x<<" "<<y<<endl;
    return make_pair(x,y);
}

void Grid::initiateGraphics()
{
    int gd=DETECT,gm;
    initgraph(&gd,&gm,"");
    initwindow(800,700);
    setfillstyle(SOLID_FILL,LIGHTGREEN);
    floodfill(1,1,WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    setbkcolor(WHITE);
    setcolor(LIGHTRED);
    settextstyle(4,0,5);
    setbkcolor(LIGHTGREEN);
    outtextxy(160,30,"Sudoku Game");
    setbkcolor(BLACK);
    settextstyle(0,0,2);
    setcolor(BLACK);
    int left=100,right=640,top=100,bottom=640;
    for(int i=0;i<3;i++,left++,right--,top++,bottom--)
        rectangle(left,top,right,bottom);
    for(int i=100;i<=640;i+=60)
    {
        line(100,i,640,i);
        line(i,100,i,640);
    }
    for(int i=0;i<3;i++)
        line(279+i,100,279+i,640);
    for(int i=0;i<3;i++)
        line(459+i,100,459+i,640);
    for(int i=0;i<3;i++)
        line(100,279+i,640,279+i);
    for(int i=0;i<3;i++)
        line(100,459+i,640,459+i);
    string level="1";
    setbkcolor(LIGHTGREEN);
    for(int i=130;i<=610;i+=60)
    {

        outtextxy(i,80,(char *)level.c_str());
        outtextxy(80,i,(char *)level.c_str());
        char ch = level[0];
        ++ch;
        level[0] = ch;
    }
    setcolor(LIGHTRED);
    for(int i=0;i<3;i++)
        rectangle(200+i,650+i,540-i,690-i);
    setcolor(BLACK);
    outtextxy(300,660,"STATEMENT");
    setbkcolor(BLACK);
    setcolor(LIGHTBLUE);
    left=105;
    right=155;
    top=105;
    bottom=155;
    rectangle(left,top,right,bottom);
    while(true){


        if(GetAsyncKeyState(VK_LEFT)){
            setfillstyle(SOLID_FILL,LIGHTGREEN);
            floodfill(left+1,top+1,BLACK);
            PlaySound("C:\\Users\\kaust\\Documents\\Ds_Project\\pacman_eatfruit.wav",NULL,SND_ASYNC);
            Grid::refreshSudoku();
            if(left >= 165){
                left -= 60; right -= 60;
            }
        }
        else if(GetAsyncKeyState(VK_RIGHT)){
            setfillstyle(SOLID_FILL,LIGHTGREEN);
            floodfill(left+1,top+1,BLACK);
            PlaySound("C:\\Users\\kaust\\Documents\\Ds_Project\\pacman_eatfruit.wav",NULL,SND_ASYNC);
            Grid::refreshSudoku();
            if(right <= 575){
                right += 60; left += 60;
            }
        }
        else if(GetAsyncKeyState(VK_UP)){
            setfillstyle(SOLID_FILL,LIGHTGREEN);
            floodfill(left+1,top+1,BLACK);
            PlaySound("C:\\Users\\kaust\\Documents\\Ds_Project\\pacman_eatfruit.wav",NULL,SND_ASYNC);
            Grid::refreshSudoku();
            if(top >= 165){
                top -= 60; bottom -= 60;
            }
        }
        else if(GetAsyncKeyState(VK_DOWN)){
            setfillstyle(SOLID_FILL,LIGHTGREEN);
            floodfill(left+1,top+1,BLACK);
            PlaySound("C:\\Users\\kaust\\Documents\\Ds_Project\\pacman_eatfruit.wav",NULL,SND_ASYNC);
            Grid::refreshSudoku();
            if(bottom <= 575){
                bottom += 60; top += 60;
            }
        }
        else
        {

            char c = getch();
            if (c>='1' && c<='9')
            {
                pair<int,int> cordinates = getCordinates(right,bottom);
                int i = cordinates.first;
                int j = cordinates.second;
                //cout << "Number entered " << c;
                if(correct(i,j,c-'0'))
                {
                    printMatrix(auxilliary);
                    Grid::refreshSudoku();
                }
                else
                    message = "Invalid keypress!";
            }
        }

        delay(140);
        int tleft = left;
        int tright = right;
        int ttop = top;
        int tbottom = bottom;
        for(int i = 0; i<2; i++, tleft--, ttop--, tright++, tbottom++)
            rectangle(tleft,ttop,tright,tbottom);

    }
    setfillstyle(SOLID_FILL,WHITE);
    setcolor(BLACK);
}

void Grid::refreshSudoku()
{
    setfillstyle(SOLID_FILL,LIGHTGREEN);
    floodfill(1,1,WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    setbkcolor(WHITE);
    setcolor(LIGHTRED);
    settextstyle(4,0,5);
    setbkcolor(LIGHTGREEN);
    outtextxy(160,30,"Sudoku Game");
    setbkcolor(BLACK);
    settextstyle(0,0,2);
    setcolor(BLACK);
    int left=100,right=640,top=100,bottom=640;
    for(int i=0;i<3;i++,left++,right--,top++,bottom--)
        rectangle(left,top,right,bottom);
    for(int i=100;i<=640;i+=60)
    {
        line(100,i,640,i);
        line(i,100,i,640);
    }
    for(int i=0;i<3;i++)
        line(279+i,100,279+i,640);
    for(int i=0;i<3;i++)
        line(459+i,100,459+i,640);
    for(int i=0;i<3;i++)
        line(100,279+i,640,279+i);
    for(int i=0;i<3;i++)
        line(100,459+i,640,459+i);
    string level="1";
    setbkcolor(LIGHTGREEN);
    for(int i=130;i<=610;i+=60)
    {

        outtextxy(i,80,(char *)level.c_str());
        outtextxy(80,i,(char *)level.c_str());
        char ch = level[0];
        ++ch;
        level[0] = ch;
    }
    setcolor(LIGHTRED);
    for(int i=0;i<3;i++)
        rectangle(200+i,650+i,540-i,690-i);
    setcolor(BLACK);
    outtextxy(250,660,(char*)message.c_str());
    setbkcolor(BLACK);
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            int data = auxilliary[i][j];
            if(data!=0)
            {
                string strData = to_string(data);
                setbkcolor(LIGHTGREEN);
                int x=120+i*60;
                int y=120+j*60;
                outtextxy(y,x,(char*)(strData.c_str()));
                setbkcolor(BLACK);
            }
        }
    }
    setcolor(LIGHTBLUE);
}

string to_string(int x)
{
    stringstream ss;
    ss << x;
    return ss.str();
}



template<class T>
class List
{
    T *ar;
    int size;
public:
    List();
    void add(T);
    int getSize();
    T get(int);
};

template <class T>
List<T>::List()
{
    ar = new T[maxSize];
    size = 0;
}

template <class T>
void List<T>::add(T element)
{
    if(size==maxSize)
        return;

    ar[size] = element;
    size++;
}

template <class T>
T List<T>::get(int index)
{
    return ar[index];
}

template <class T>
int List<T>::getSize()
{
    return size;
}

template <class T>
class MinHeap
{
    T *heap;
    int size;
    int parent(int i) {return (i-1)/2;}
    int left(int i) { return 2*i + 1;}
    int right(int i) { return 2*i + 2;}
    void minHeapify(int);

public:
    MinHeap();
    void push(T);
    T pop();
    bool isEmpty();
};

template <class T>
MinHeap<T>::MinHeap()
{
    heap = new T[maxSize];
    size = 0;
}

template <class T>
void MinHeap<T>::push(T element)
{
    if(size==maxSize)
        return;
    int i = size;
    heap[i] = element;
    while(i>0 && heap[parent(i)] > heap[i])
    {
        swap(heap[i],heap[parent(i)]);
        i = parent(i);
    }
    size++;
}

template <class T>
T MinHeap<T>::pop()
{
    if(size==1)
    {
        size--;
        return heap[0];
    }
    else
    {
        T root = heap[0];
        heap[0] = heap[size-1];
        size--;
        minHeapify(0);
        return root;
    }
}

template <class T>
void MinHeap<T>::minHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if(l<size && heap[l] < heap[smallest])
        smallest = l;
    if(r<size && heap[r] < heap[smallest])
        smallest = r;
    if(i != smallest)
    {
        swap(heap[i],heap[smallest]);
        minHeapify(smallest);
    }
}

template <class T>
bool MinHeap<T>::isEmpty()
{
    if(size==0)
        return true;
    return false;
}

class Rat
{
    int size;
    int vertices;
    int source;
    int destination;
    string maze;
    vector<vector<int> > matrix;
    List<int> *adj;
    vector<int> distance;
    vector<int> solution;
    vector<int> parent;
    vector<bool> visited;
    void loadGrid();
    void inputMatrix();
    void generateList();
    bool djikstra();
    int hash(int i, int j){return ((i-1)*gridSize + j);}
public:
    Rat(int);
    void start();
    void video();
    void generateSolution(int);
    void initiateGraphics();

};

Rat::Rat(int n)
{
    this->size = n;
    this->vertices = n*n;
    source = hash(8,6);
    destination = hash(5,1);
    maze = mazeFile;
    adj = new List<int>[vertices+1];
    solution.resize(vertices+1);
    distance.resize(vertices+1, INF);
    parent.resize(vertices+1, INF);
    visited.resize(vertices+1, false);
    matrix.resize(n+2,vector<int>(n+2));
    inputMatrix();
    generateList();
}

void Rat::generateList()
{
    for(int i=1;i<=size;i++)
    {
        for(int j=1;j<=size;j++)
        {
            int node = hash(i,j);
            int up = hash(i-1,j);
            int left = hash(i,j-1);
            int right = hash(i,j+1);
            int down = hash(i+1,j);
            int neighbour1 = matrix[i-1][j];
            int neighbour2 = matrix[i][j-1];
            int neighbour3 = matrix[i][j+1];
            int neighbour4 = matrix[i+1][j];
            if(neighbour1)
                adj[node].add(up);
            if(neighbour2)
                adj[node].add(left);
            if(neighbour3)
                adj[node].add(right);
            if(neighbour4)
                adj[node].add(down);
        }
    }
}

void Rat::inputMatrix()
{
    ifstream fin(maze);
    for(int i=1;i<=size;i++)
    {
        for(int j=1;j<=size;j++)
        {
            fin >> matrix[i][j];
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void Rat::start()
{
    initiateGraphics();
    setfillstyle(SOLID_FILL,LIGHTBLUE);
    floodfill(1,1,WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    setbkcolor(WHITE);
    setcolor(LIGHTRED);
    settextstyle(3,0,5);
    setbkcolor(LIGHTGREEN);
    outtextxy(250,30,"");
    setbkcolor(BLACK);
    settextstyle(0,0,2);
    setcolor(BLACK);
    if(djikstra())
    {
        cout << endl << "Path found!" << endl;
        for(int i=1;i<=gridSize;i++)
        {
            for(int j=1;j<=gridSize;j++)
            {
                int node = hash(i,j);
                cout << solution[node] << " ";
            }
            cout << endl;
        }
    }
    else
        cout << "No path exists.";
}

bool Rat::djikstra()
{
    distance[source] = 0;
    visited[source] = true;
    MinHeap<iPair> heap;
    heap.push(make_pair(0,source));
    while(!heap.isEmpty())
    {
        int u = heap.pop().second;
        for(int i=0;i<adj[u].getSize();i++) //Adjacent to u
        {
            int v = adj[u].get(i);
            visited[v] = true;
            if(distance[u] + weight < distance[v])
            {
                distance[v] = distance[u] + weight;
                parent[v] = u;
                heap.push(make_pair(distance[v],v));
            }
        }
    }

    if(visited[destination])
    {
        generateSolution(destination);
        return true;
    }
    return false;
}

void Rat::generateSolution(int i)
{
    if(i==source)
    {
        solution[i] = 1;
        video();
        return;
    }
    generateSolution(parent[i]);
    solution[i] = 1;
    video();
}

void swap(int &a, int &b)
{
    int t = a;
    a = b;
    b = t;
}
void Rat::video()
{
    int left=100,right=640,top=100,bottom=640;
    int gap=(right-left)/size;
    for(int i=1;i<3;i++,left++,right--,top++,bottom--)
        rectangle(left,top,right,bottom);
    for(int i=100;i<620;i+=gap)
    {
        line(100,i,640,i);
        line(i,100,i,640);
    }
    for(int i=0;i<size;i++)
    {
       for(int j=0;j<size;j++)
        {
             if(matrix[i+1][j+1]==0)
             {
                 setfillstyle(SOLID_FILL,LIGHTGRAY);
                 floodfill(100+(gap/2)*(2*j+1),100+(gap/2)*(2*i+1),BLACK);
                 setfillstyle(SOLID_FILL,WHITE);
             }
             else
             {
                 setfillstyle(SOLID_FILL,WHITE);
                 floodfill(100+(gap/2)*(2*j+1),100+(gap/2)*(2*i+1),BLACK);

             }
        }
    }
     for(int i=0;i<gridSize;i++)
    {
        for(int j=0;j<gridSize;j++)
        {
            if(solution[hash(i+1,j+1)]==1)
            {
                setfillstyle(SOLID_FILL,RED);
                circle(100+(gap/2)*(2*j+1),100+(gap/2)*(2*i+1),gap/6);
                floodfill(100+(gap/2)*(2*j+1),100+(gap/2)*(2*i+1),BLACK);
                setfillstyle(SOLID_FILL,WHITE);
            }
        }
    }
    delay(300);
    //getch();
}


void Rat::initiateGraphics()
{
    int gd=DETECT,gm;
    initgraph(&gd,&gm,"");
    initwindow(800,700);
    setfillstyle(SOLID_FILL,LIGHTBLUE);
    floodfill(1,1,WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    setbkcolor(WHITE);
    setcolor(LIGHTRED);
    settextstyle(3,0,5);
    setbkcolor(LIGHTGREEN);
    outtextxy(250,30,"");
    setbkcolor(BLACK);
    settextstyle(0,0,2);
    setcolor(BLACK);
}

int main()
{
    int ch;
    cout << "Enter choice." << endl;
    cout << "1. N Queens." << endl;
    cout << "2. Sudoku." << endl;
    cout << "3. Rat in a maze." << endl;
    cin >> ch;
    if(ch==1)
    {
        cout << "Enter the value of n.\n";
        int size;
        cin >> size;
        NQueens Q(size);
        Q.solveNQueens();
    }
    else if(ch==2)
    {
        Grid G;
        G.start();
    }
    else if(ch==3)
    {
        Rat R(gridSize);
        R.start();
    }
    getch();
}
