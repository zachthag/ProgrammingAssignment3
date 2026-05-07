// main.cpp
// PA3 - Maze Escape Using DFS
//
// IMPORTANT: The assignment provides starter code for generateMaze(),
// chooseBoundaryCell(), printMaze(), printPath(), and the global data
// structures. The functions below are a reconstruction that matches the
// assignment description so the file compiles and runs end-to-end.
// When submitting, keep your professor's starter implementations and
// only paste in the dfs() function and the DFS call inside main().

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// ---------- Globals (provided by starter code) ----------
int N, M;
vector<vector<int>>  maze;
vector<vector<bool>> visited;
vector<vector<int>>  parent_r;
vector<vector<int>>  parent_c;

int start_r, start_c;
int end_r,   end_c;

// Direction arrays: up, down, left, right
int dr[4] = {-1, 1,  0, 0};
int dc[4] = { 0, 0, -1, 1};

// ---------- Starter helpers (DO NOT MODIFY in your submission) ----------

void generateMaze() {
    maze.assign(N, vector<int>(M, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            // ~30% walls feels solvable for small mazes
            maze[i][j] = (rand() % 10 < 3) ? 1 : 0;
        }
    }
}

void chooseBoundaryCell(int &r, int &c) {
    int side = rand() % 4;
    if      (side == 0) { r = 0;     c = rand() % M; }
    else if (side == 1) { r = N - 1; c = rand() % M; }
    else if (side == 2) { r = rand() % N; c = 0;     }
    else                { r = rand() % N; c = M - 1; }
    maze[r][c] = 0; // boundary cell must be open
}

void printMaze() {
    cout << "Maze:" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (i == start_r && j == start_c)      cout << "S ";
            else if (i == end_r && j == end_c)     cout << "E ";
            else                                   cout << maze[i][j] << ' ';
        }
        cout << '\n';
    }
}

void printPath() {
    // Walk parents from exit back to entrance, then reverse.
    vector<pair<int,int>> path;
    int r = end_r, c = end_c;
    while (!(r == start_r && c == start_c)) {
        path.push_back({r, c});
        int pr = parent_r[r][c];
        int pc = parent_c[r][c];
        r = pr; c = pc;
    }
    path.push_back({start_r, start_c});

    cout << "Path from S to E:" << endl;
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        cout << "(" << it->first << ", " << it->second << ")" << endl;
    }
}

bool dfs(int r, int c) {
    visited[r][c] = true;

    if (r == end_r && c == end_c) {
        return true;
    }

    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        if (nr < 0 || nr >= N || nc < 0 || nc >= M) continue;
        if (maze[nr][nc] == 1) continue;
        if (visited[nr][nc])   continue;

        parent_r[nr][nc] = r;
        parent_c[nr][nc] = c;

        if (dfs(nr, nc)) return true;
    }

    return false;
}

// ---------- Main ----------
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "Enter maze dimensions N M: ";
    cin >> N >> M;
    if (N <= 0 || M <= 0) {
        cout << "Invalid dimensions." << endl;
        return 1;
    }

    generateMaze();

    visited.assign(N,  vector<bool>(M, false));
    parent_r.assign(N, vector<int>(M, -1));
    parent_c.assign(N, vector<int>(M, -1));

    // Pick distinct boundary cells for S and E.
    chooseBoundaryCell(start_r, start_c);
    do {
        chooseBoundaryCell(end_r, end_c);
    } while (end_r == start_r && end_c == start_c);

    printMaze();

    if (dfs(start_r, start_c)) {
        printPath();
    } else {
        cout << "No path exists." << endl;
    }

    return 0;
}
