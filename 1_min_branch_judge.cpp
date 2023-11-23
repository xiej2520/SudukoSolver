#include <bits/stdc++.h>
using namespace std;

array<array<uint8_t, 9>, 9> board;

array<uint16_t, 9> row;
array<uint16_t, 9> col;
array<uint16_t, 9> box;

constexpr uint16_t filled = (1 << 10) - 2;

void mark(uint8_t i, uint8_t j, uint8_t c) {
  board[i][j] = c;
  uint16_t set = (1 << c);
  row[i] |= set;
  col[j] |= set;
  box[i/3*3+j/3] |= set;
}

void unmark(uint8_t i, uint8_t j) {
  uint16_t unset = ~(1 << board[i][j]);
  board[i][j] = 0;
  row[i] &= unset;
  col[j] &= unset;
  box[i/3*3+j/3] &= unset;
}

bool dfs() {
  int min_pc = 10;
  uint16_t min_opts = 0;
  int x = -1;
  int y = -1;
  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++) {
      if (board[i][j] != 0) continue;

      uint16_t opts = ~(row[i] | col[j] | box[i/3*3+j/3]) & filled;
      if (opts == 0) return false;
      int pc = __builtin_popcount(opts);
      if (pc < min_pc) {
        min_pc = pc;
        min_opts = opts;
        x = i, y = j;
      }
    }
  }
  if (min_pc == 10) return true; // done

  for (int d=1; d<=9; d++) {
    if (min_opts & (1 << d)) {
      mark(x, y, d);
      if (dfs()) return true;
      unmark(x, y);
    }
  }
  return false;
}


int main() {
  ios_base::sync_with_stdio(false); cin.tie(nullptr);
  int T; cin >> T;
  char nl[3];
  cin.getline(&nl[0], 3);
  for (int t=1; t<=T; t++) {
    board = {};
    row = {};
    col = {};
    box = {};
    cin.getline(&nl[0], 3);
    for (int i=0; i<9; i++) {
      array<char, 10> line;
      cin.getline(&line[0], 10);
      for (int j=0; j<9; j++) {
        char c = line[j];
        if (c == '.') {
          board[i][j] = 0;
        }
        else {
          mark(i, j, c - '0');
        }
      }
    }
    
    dfs();
    
    printf("Case %d:\n", t);
    for (int i=0; i<9; i++) {
      for (int j=0; j<9; j++) {
        printf("%d", board[i][j]);
      }
      printf("\n");
    }
  }
}
