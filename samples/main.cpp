#include <iostream>
#include "Sparse_Matrix.h"
using namespace std;
int main() {
    Sparse_Matrix<int>m;
    cin>>m;
    Sparse_Matrix<int>m1(m);
    int val = 0;
    m.set(1,0,val);
    m.set(0,2,val);
    val = -2;
    m.set(0,2,val);
    cout<<m<<'\n';
    cout<<'\n';
    cout<<m1<<'\n';
    cout<<'\n';
    Sparse_Matrix<int>m2 = m - m1;
    cout<<m2<<'\n';
    return 0;
}
