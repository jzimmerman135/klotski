#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
using namespace std;

static const int SIZE = 20;
static const string EMPTY = "  ";
static const int deltaAdj[4] = {-4,-1,4,1};

bool is_solved(string (&board)[SIZE]);

string board_to_string(string (&board)[SIZE], bool generic=false);
void string_to_board(string board_str, string (&board)[SIZE]);

void print_board(string (&board)[SIZE]);
void print_board(string board_str);

bool move_left_in(int to, string (&board)[SIZE]);
bool move_right_in(int to, string (&board)[SIZE]);
bool move_above_in(int to, string (&board)[SIZE]);
bool move_below_in(int to, string (&board)[SIZE]);

int find_other_empty(int empty,string (&board)[SIZE]);
void copy_array(string (&a)[SIZE], string (&b)[SIZE]);

void clear_queue(queue<string>& q) {
    queue<string>empty;
    q.swap(empty);
}

typedef bool (*moveFunction) (int to, string (&board)[SIZE]);

int main() {
    //set up board and pieces
    // string board[SIZE] = {  "v1", "g1", "g1", "v2",
    //                         "v1", "g1", "g1", "v2",
    //                         "v3", "h1", "h1", "v4",
    //                         "v3", "s2", "s3", "v4",
    //                         "s1", EMPTY,EMPTY,"s4" };

    string board[20] = {  EMPTY, "g1", "g1", EMPTY,
                            "s1",  "g1", "g1", "s4",
                            "s2",  "s3", "h1", "h1",
                            "h2",  "h2", "v1", "v2",
                            "h3",  "h3", "v1", "v2" };
                
    string start_board_str = board_to_string(board);
    string final_board_str = "";
    
    print_board(board);
    bool solved = false;
    bool generic = true;
    moveFunction move[] = {  move_left_in,
                             move_right_in,
                             move_above_in,
                             move_below_in };   
                                       
    unordered_set<string> visited;
    unordered_map<string, string> to_from;
    queue<string> q;
    q.push(start_board_str);
    visited.insert(board_to_string(board, generic));
    
    while (not q.empty() && not solved) {
        string base[SIZE];
        string_to_board(q.front(), base);
        q.pop();
        int empty[2];
        empty[0] = find_other_empty(0, base);
        empty[1] = find_other_empty(empty[0], base);
        for (int i = 0; i < 8; i++) {
            string curr[SIZE];
            copy_array(curr, base);
            if (move[i % 4](empty[i/4], curr)) {
                if (visited.count(board_to_string(curr, generic)))
                    continue;
                
                to_from[board_to_string(curr)] = board_to_string(base);
                q.push(board_to_string(curr));
                visited.insert(board_to_string(curr, generic));
                if (is_solved(curr)){
                    solved = true;
                    final_board_str = board_to_string(curr);
                }
            }
        }
    }
    
    if (solved) {
        cout << "solved\n\n";
        visited.clear();
        clear_queue(q);
    }
    else {
        cout << "failed\n";
        return 1;
    }
    
    int moves = 0;
    stack<string> sol;    
    string to = final_board_str;
    while (to != start_board_str) {
        sol.push(to);
        to = to_from[to];
    } 
    sol.push(start_board_str);
    cout << "SOLUTION:" << endl;
    cout << "in " << moves << " moves" << endl;
    
    while (!sol.empty()) {
        getline(cin, start_board_str);
        print_board(sol.top());
        sol.pop();
    }
    return 0;
}

bool is_solved(string (&board)[SIZE]) {
    return (board[13] == board[18] && board[14] == board[17]);
}

string board_to_string(string (&board)[SIZE], bool generic) {
    string res = "";
    for (int i = 0; i < SIZE; i++) {
        if (generic)
            res += board[i][0] + ' ';    
        else 
            res += board[i] + "_";
    }
    return res;
}

void string_to_board(string board_str, string (&board)[SIZE]) {
    int j = 0;
    for (int i = 0; i < SIZE; i++){
        while (board_str[j] != '_') {
            board[i] += board_str[j];
            j++;
        }
        j++;
    }
}

void print_board(string (&board)[SIZE]) {
    string res = "\n";
    for (int i = 0; i < SIZE; i++) {
        res += board[i] + "  ";
        if ((i+1) % 4 == 0) {
            res += "\n";
        }
    }
    cout << res << "\n";
}

void print_board(string board_str) {
    int j = 0;
    cout << "\n";
    for (int i = 0; i < SIZE; i++){
        while (board_str[j] != '_') {
            cout << board_str[j];
            j++;
        }
        cout << "  ";
        j++;
        if (i % 4 == 3) {
            cout << "\n";
        }
    }
    cout << "\n";
}

bool move_left_in(int to, string (&board)[SIZE]) {
    if (to % 4 == 0) { //empty is already at leftmost column
        return false;
    }
    if (board[to - 1][0] == 's') { //left of to is single block
        board[to] = board[to - 1];
        board[to - 1] = EMPTY;
        return true;
    }
    if (board[to - 1][0] == 'h') { //left is horizontal
        board[to] = board[to - 2];
        board[to - 2] = EMPTY;
        return true;
    }
    if (to > 15) { //empty is at top row of board
        return false;
    }
    if (board[to - 1] == board[to + 3] && board[to + 4] == EMPTY) {
        if (board[to - 1][0] == 'v') { 
            board[to] = board[to - 1];
            board[to + 4] = board[to + 3];
            board[to - 1] = EMPTY;
            board[to + 3] = EMPTY;
            return true;
        }
        else if (board[to - 1][0] == 'g') {
            board[to] = board[to - 2];
            board[to + 4] = board[to + 2];
            board[to - 2] = EMPTY;
            board[to + 2] = EMPTY;
            return true;
        }
    }
    return false;
}

bool move_right_in(int to, string (&board)[SIZE]) {
    if (to % 4 == 3) { //empty is already at rightmost column
        return false;
    }
    if (board[to + 1][0] == 's') { //right of to is single block
        board[to] = board[to + 1];
        board[to + 1] = EMPTY;
        return true;
    }
    if (board[to + 1][0] == 'h') { //left is horizontal
        board[to] = board[to + 2];
        board[to + 2] = EMPTY;
        return true;
    }
    if (to > 15) { //empty is at top row of board
        return false;
    }
    if (board[to + 1] == board[to + 5] && board[to + 4] == EMPTY) {
        if (board[to + 1][0] == 'v') {
            board[to] = board[to + 1];
            board[to + 4] = board[to + 5];
            board[to + 1] = EMPTY;
            board[to + 5] = EMPTY;
            return true;
        }
        else if (board[to + 1][0] == 'g') {
            board[to] = board[to + 2];
            board[to + 4] = board[to + 6];
            board[to + 2] = EMPTY;
            board[to + 6] = EMPTY;
            return true;
        }
    }
    return false;
}

bool move_above_in(int to, string (&board)[SIZE]) {
    if (to < 4) { //empty is already at topmost row
        return false;
    }
    if (board[to - 4][0] == 's') { //right of to is single block
        board[to] = board[to - 4];
        board[to - 4] = EMPTY;
        return true;
    }
    if (board[to - 4][0] == 'v' && to > 7) { //above is vertical   
        board[to] = board[to - 4];
        board[to - 4] = board[to - 8];
        board[to - 8] = EMPTY;
        return true;
    }
    if (to % 4 == 3) { //empty is at rightmost column of board
        return false;
    }
    if (board[to - 4] == board[to - 3] && board[to + 1] == EMPTY) {
        if (board[to - 4][0] == 'h') { //horizontal
            board[to] = board[to - 4];
            board[to + 1] = board[to - 3];
            board[to - 4] = EMPTY;
            board[to - 3] = EMPTY;
            return true;
        }
        else if (board[to - 4][0] == 'g') { //big square
            board[to] = board[to - 8];
            board[to + 1] = board[to - 7];
            board[to - 8] = EMPTY;
            board[to - 7] = EMPTY;
            return true;
        }
    }
    return false;
}

bool move_below_in(int to, string (&board)[SIZE]) {
    if (to > 15) { //empty is already at bottommost row
        return false;
    }
    if (board[to + 4][0] == 's') { //right of to is single block
        board[to] = board[to + 4];
        board[to + 4] = EMPTY;
        return true;
    }
    if (board[to + 4][0] == 'v' && to < 12) { //above is vertical   
        board[to] = board[to + 4];
        board[to + 4] = board[to + 8];
        board[to + 8] = EMPTY;
        return true;
    }
    if (to % 4 == 3) { //empty is at rightmost column of board
        return false;
    }
    if (board[to + 4] == board[to + 5] && board[to + 1] == EMPTY) {
        if (board[to + 4][0] == 'h') {
            board[to] = board[to + 4];
            board[to + 1] = board[to + 5];
            board[to + 4] = EMPTY;
            board[to + 5] = EMPTY;
            return true;
        }
        else if (board[to + 4][0] == 'g') {
            board[to] = board[to + 8];
            board[to + 1] = board[to + 9];
            board[to + 8] = EMPTY;
            board[to + 9] = EMPTY;
            return true;
        }
    }
    return false;
}

int find_other_empty(int empty, string (&board)[20]) {
    bool visited[20];
    memset(visited, false, sizeof visited);
    
    queue<int> q;
    q.push(empty);
    visited[empty] = true;
    
    while (!q.empty()) {
        int root = q.front();
        for (int i = 0; i < 4; i++) {
            int curr = root + deltaAdj[i];
            if (not visited[curr] && curr < 20 && curr >= 0) {
                if (board[curr] == EMPTY) {
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

void solve(string (&board)[SIZE]) {
    cout << "Failed" << board[0];
}

void copy_array(string (&a)[SIZE], string (&b)[SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        a[i] = b[i];
    }
}

// bool solve_util(string (&board)[SIZE], unordered_set<string> prev_boards, int& empty) {
//     if (is_solved(board))
//         return true;
//     if (prev_boards.count(board_to_string(board)))
//         return false;
// 
//     curr_prev_boards.insert(board_to_string(board));
//     int other_empty = find_other_empty(empty, board);
// 
//     if (move_left_in(empty, board) && solve_util(board, curr_prev_boards, other_empty))
//         return true;    
//     else if (move_right_in(empty, board) && solve_util(board, curr_prev_boards, other_empty))
//         return true;    
//     else if (move_above_in(empty, board) && solve_util(board, curr_prev_boards, other_empty))
//         return true;    
//     else if (move_below_in(empty, board) && solve_util(board, curr_prev_boards, other_empty))
//         return true;
//     else if (solve_util(board, curr_prev_boards, other_empty))
//         return true;
//     else 
//         prev_boards.clear(board_to_string(board));
// 
//     return false;
// }