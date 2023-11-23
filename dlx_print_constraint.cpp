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

vector<int> row_starts;

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
    row_starts.push_back(0);
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

    row_starts.push_back(i1);
  }
};

DLX dlx{81 * 4};

int main() {
  ios_base::sync_with_stdio(false); cin.tie(nullptr);

  // columns:
  // [0, 81]: 9 * r + c filled
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
  
  for (int i=0; i<729; i++) {
    int j = row_starts[i];
    for (int k=0; k<=324; k++) {
      if (k == dlx.nodes[j].col) {
        printf("X");
        j = dlx.nodes[j].right;
      }
      else {
        printf(" ");
      }
    }
    printf("\n");
  }
  

}
