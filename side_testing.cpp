#include <iostream>
#include <string>
#include <queue>
#include <stringstream>
using namespace std;

static const string EMPTY = "EMPTY";
static const int deltaAdj[4] = {-4,-1,4,1};

int find_other_empty(int empty,string (&board)[20]);
bool add_ten(int& x) { x += 10; return x;}
bool above_ten(int& x) { return 10 < x; }
bool remove_ten(int& x) {x -= 10; return x;}

int main() {
    string board[20] = 
    {
    "0","1","2","3",
    "4","5","6","7",
    "8","9","10","11",
    "12",EMPTY,"14","15",
    "16","17","18",EMPTY,    
    };
    
    cout << find_other_empty(19, board) << endl;
    
    int a = 4;
    if (add_ten(a) && remove_ten(a) && above_ten(a))
        cout << "ADD FIRST YAYY" << endl;
    else 
        cout << "BOOO" << endl;
        
    return 0;
}


int find_other_empty(int empty, string (&board)[20]) {
    bool visited[20];
    memset(visited, false, sizeof visited);
    
    queue<int> q;
    q.push(empty);
    visited[empty] = true;
    
    while (!q.empty()) {
        int root = q.front();
        cerr << root << " "; 
        for (int i = 0; i < 4; i++) {
            int curr = root + deltaAdj[i];
            if (not visited[curr] && curr < 20 && curr >= 0) {
                if (board[curr] == EMPTY) {
                    cerr << "\n";
                    return curr;
                }
                q.push(curr);
                visited[curr] = true;
            }
        }
        q.pop();
    }
    return -1;
}