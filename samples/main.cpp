#include <iostream>
#include "Sparse_Matrix.h"
using namespace std;
int main() {
    Sparse_Matrix<int>m;
    cin>>m;
    int val = 0;
    m.set(1,0,val);
    m.set(0,2,val);
    val = 6;
    m.set(0,2,val);
    cout<<m<<'\n';
    return 0;
}
