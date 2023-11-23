#include <bits/stdc++.h>
using namespace std;

struct Node {
  uint16_t up;
  uint16_t down;
  uint16_t left;
  uint16_t right;
  uint16_t col;
};

// suduko constraint table is 729 * 324, 730 * 325 for col row, root
struct DLX {
  array<uint8_t, 325> col_size;
  array<Node, 3241> nodes;
  int s;

  DLX() {
    col_size.fill(10);
    col_size[0] = 1;
    nodes[0] = {0, 0, 324, 1, 0}; // root
    for (uint16_t i=1; i<=324; i++) {
      nodes[i] = {i, i, i-1, i+1, i};
    }
    nodes[324].right = 0; // link back to root
    s = 325;
  }

  void add_row(int c1, int c2, int c3, int c4) {
    c1++, c2++, c3++, c4++; // increment by one for root
    int i1 = s;
    int i2 = s + 1;
    int i3 = s + 2;
    int i4 = s + 3;
    s += 4;
    nodes[i1] = {nodes[c1].up, c1, i4, i2, c1};
    nodes[nodes[c1].up].down = i1;
    nodes[c1].up = i1;
    
    nodes[i2] = {nodes[c2].up, c2, i1, i3, c2};
    nodes[nodes[c2].up].down = i2;
    nodes[c2].up = i2;

    nodes[i3] = {nodes[c3].up, c3, i2, i4, c3};
    nodes[nodes[c3].up].down = i3;
    nodes[c3].up = i3;

    nodes[i4] = {nodes[c4].up, c4, i3, i1, c4};
    nodes[nodes[c4].up].down = i4;
    nodes[c4].up = i4;
  }
};

DLX dlx;
array<array<char, 9>, 9> board;

#define U(x) dlx.nodes[x].up
#define D(x) dlx.nodes[x].down
#define L(x) dlx.nodes[x].left
#define R(x) dlx.nodes[x].right
#define C(x) dlx.nodes[x].col
#define S(x) dlx.col_size[x]
#define foreach(i, x, F) for (int i = F(x); i != x; i = F(i))

void cover(int col) {
  L(R(col)) = L(col);
  R(L(col)) = R(col);
  foreach (i, col, D) {
    foreach (j, i, R) {
      U(D(j)) = U(j);
      D(U(j)) = D(j);
      S(C(j))--;
    }
  }
};
void uncover(int col) {
  foreach (i, col, U) {
    foreach (j, i, L) {
      S(C(j))++;
      U(D(j)) = j;
      D(U(j)) = j;
    }
  }
  L(R(col)) = col;
  R(L(col)) = col;
};

bool dfs() {
  // Knuth
  if (R(0) == 0) return true;

  // choose column
  int c = R(0);
  int s = S(C(c));
  foreach (j, 0, R) {
    if (S(C(j)) < s) {
      c = j;
      s = S(C(j));
    }
  }
  cover(c);
  foreach (r, c, D) {
    foreach (j, r, R) {
      cover(dlx.nodes[j].col);
    }
    if (dfs()) {
      int k = r;
      while (C(k) > 81) k = L(k);
      int col = C(k) - 1; // root
      int r = col / 9;
      int c = col % 9;
      int d = (C(R(k)) - 81 - 1) / 27;
      board[r][c] = d + 1 + '0';

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
  array<char, 83> line;
  while (cin.getline(&line[0], 82)) {

    for (int r=0; r<9; r++) {
      for (int c=0; c<9; c++) {
        for (int d=0; d<9; d++) {
          int d_offset = 81 + 27 * d;
          dlx.add_row(9 * r + c, d_offset + r, d_offset + 9 + c, d_offset + 18 + r/3*3+c/3);
        }
      }
    }

    int k = 0;
    for (int i=0; i<9; i++) {
      for (int j=0; j<9; j++, k++) {
        char c = line[k];
        if (c == '.') continue;
        board[i][j] = c;

        int col = 1 + 9 * i + j;
        cover(col);
        foreach (r, col, D) {
          int d = (dlx.nodes[R(r)].col - 81 - 1) / 27;
          if (d + 1 == c - '0') {
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

    //for (int i=0; i<9; i++) {
    //  for (int j=0; j<9; j++) {
    //    printf("%c", board[i][j]);
    //  }
    //  printf("\n");
    //}
    //printf("\n");

    dlx = DLX{};

  }
}
