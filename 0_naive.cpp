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

bool dfs(int i, int j) {
  uint16_t opts = ~(row[i] | col[j] | box[i/3*3+j/3]) & filled;

  for (int d=1; d<=9; d++) {
    if (opts & (1 << d)) {
      mark(i, j, d);

      int next_i = i, next_j = j;
      while (true) {
        if (next_j == 8) {
          next_i++;
          next_j = 0;
        }
        else {
          next_j++;
        }
        if (next_i == 9) return true;
        if (board[next_i][next_j] == 0) break;
      }
      if (dfs(next_i, next_j)) return true;

      unmark(i, j);
    }
  }
  return false;
}


int main() {
  ios_base::sync_with_stdio(false); cin.tie(nullptr);
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

  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++) {
      if (board[i][j] != 0) continue;
      dfs(i, j);
      goto done;
    }
  }
  done: ;

  for (int i=0; i<9; i++) {
    for (int j=0; j<9; j++) {
      printf("%d", board[i][j]);
    }
    printf("\n");
  }
}
