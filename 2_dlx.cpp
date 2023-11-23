// https://arxiv.org/pdf/cs/0011047.pdf
// https://garethrees.org/2007/06/10/zendoku-generation/#section-4.2
// DLX 10ms vs Min Branch 57ms on https://lightoj.com/problem/sudoku-solver
#include <bits/stdc++.h>
using namespace std;

struct Node {
  int up;
  int down;
  int left;
  int right;
  int col;
};

struct Column {
  int count = 0;
};

struct DLX {
  vector<Column> columns;
  vector<Node> nodes;

  DLX(int n) {
    nodes.reserve(1 + 9 * 9 * 4 + (9 * 9 * 9) * 4);
    columns.reserve(n+1);
    nodes.push_back(Node{0, 0, n, 1, 0}); // root
    columns.push_back(Column{0}); // root
    for (int i=0; i<n; i++) {
      // 1st column node at index 1, left is 0, right is 2, column i -> index i
      nodes.push_back(Node{i+1, i+1, i, i + 2, i+1});
      columns.push_back(Column{0});
    }
    nodes.back().right = 0; // link back to root
  }
  void add_row(int c1, int c2, int c3, int c4) {
    c1++, c2++, c3++, c4++; // increment by one for root
    int i1 = nodes.size();
    int i2 = i1 + 1;
    int i3 = i2 + 1;
    int i4 = i3 + 1;
    nodes.push_back({Node{nodes[c1].up, c1, i4, i2, c1}});
    nodes[nodes[c1].up].down = i1;
    nodes[c1].up = i1;
    columns[c1].count++;
    
    nodes.push_back({Node{nodes[c2].up, c2, i1, i3, c2}});
    nodes[nodes[c2].up].down = i2;
    nodes[c2].up = i2;
    columns[c2].count++;

    nodes.push_back({Node{nodes[c3].up, c3, i2, i4, c3}});
    nodes[nodes[c3].up].down = i3;
    nodes[c3].up = i3;
    columns[c3].count++;

    nodes.push_back({Node{nodes[c4].up, c4, i3, i1, c4}});
    nodes[nodes[c4].up].down = i4;
    nodes[c4].up = i4;
    columns[c4].count++;
  }
};

DLX dlx{81 * 4};
vector<int> O;

#define U(x) dlx.nodes[x].up
#define D(x) dlx.nodes[x].down
#define L(x) dlx.nodes[x].left
#define R(x) dlx.nodes[x].right
#define C(x) dlx.columns[dlx.nodes[x].col]
#define foreach(i, x, F) for (int i = F(x); i != x; i = F(i))

void cover(int col) {
  //printf("covering col %d\n",col);
  L(R(col)) = L(col);
  R(L(col)) = R(col);
  foreach (i, col, D) {
    foreach (j, i, R) {
      U(D(j)) = U(j);
      D(U(j)) = D(j);
      C(j).count--;
      //printf("%d\n",j);
    }
  }
  //printf("done covering col %d\n",col);
};
void uncover(int col) {
  //printf("uncovering col %d\n",col);
  foreach (i, col, U) {
    foreach (j, i, L) {
      C(j).count++;
      U(D(j)) = j;
      D(U(j)) = j;
    }
  }
  L(R(col)) = col;
  R(L(col)) = col;
  //printf("done uncovering col %d\n",col);
};

bool dfs() {
  // Knuth
  if (R(0) == 0) return true;

  // choose column
  int c = R(0);
  int s = C(c).count;
  foreach (j, 0, R) {
    if (C(j).count < s) {
      c = j;
      s = C(j).count;
    }
  }
  cover(c);
  foreach (r, c, D) {
    foreach (j, r, R) {
      cover(dlx.nodes[j].col);
    }
    if (dfs()) {
      O.push_back(r);
      return true;
    }
    foreach (j, r, L) {
      uncover(dlx.nodes[j].col);
    }
  }
  uncover(c);
  return false;
}

int main() {
  ios_base::sync_with_stdio(false); cin.tie(nullptr);

  // columns:
  // [0, 81): 9 * r + c filled
  // then split into 27-size digit blocks
  // 9 row, 9 col, 9 box
  for (int r=0; r<9; r++) {
    for (int c=0; c<9; c++) {
      for (int d=1; d<=9; d++) {
        int d_offset = 81 + 27 * (d - 1);
        dlx.add_row(9 * r + c, d_offset + r, d_offset + 9 + c, d_offset + 18 + r/3*3+c/3);
      }
    }
  }
  O.reserve(81);

  array<array<char, 9>, 9> board{};

  for (int i=0; i<9; i++) {
    array<char, 10> line;
    cin.getline(&line[0], 10);
    for (int j=0; j<9; j++) {
      char c = line[j];
      if (c == '.') continue;
      board[i][j] = c;

      int col = 1 + 9 * i + j;
      cover(col);
      foreach (r, col, D) {
        int d = (dlx.nodes[R(r)].col - 81 - 1) / 27;
        if (d + 1 == c - '0') {
          O.push_back(r);
          foreach (j, r, R) {
            cover(dlx.nodes[j].col);
          }
          break;
        }
      }
    }
  }
  
  if (!dfs()) {
    printf("fail\n");
  }
  for (int i=0; i<81; i++) {
    while (dlx.nodes[O[i]].col > 81) {
      O[i] = dlx.nodes[O[i]].left;
    }
    int col = dlx.nodes[O[i]].col - 1; // root
    int r = col / 9;
    int c = col % 9;
    int d = (dlx.nodes[dlx.nodes[O[i]].right].col - 81 - 1) / 27;
    board[r][c] = d + 1 + '0';
  }

  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++) {
      printf("%c", board[i][j]);
    }
    printf("\n");
  }
  
}
