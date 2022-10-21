#include <iostream>
#include "Sparse_Matrix.h"
using namespace std;
int main() {
    Sparse_Matrix<int>a(3);
    Sparse_Matrix<int>b(3);
    int tmp;
    for(int i = 0;i<3;i++){
        for(int j = 0;j<3;j++){
            tmp = (i+j)%2;
            a.set(i,j,tmp);
            tmp = (i+j+1)%2;
            b.set(i,j,tmp);
        }
    }
    cout<<"a:\n";
    cout<<a<<'\n';
    cout<<'\n';
    cout<<"b:\n";
    cout<<b<<'\n';
    cout<<'\n';
    cout<<"c = a + b:\n";
    Sparse_Matrix<int>c = a + b;
    cout<<c<<'\n';
    return 0;
}
