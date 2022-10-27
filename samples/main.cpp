#include <iostream>
#include "Sparse_Matrix.h"
using namespace std;
int main() {
    Sparse_Matrix<int>a(3);
    Sparse_Matrix<int>b(3);
    for(int i = 0;i<3;i++){
        for(int j = 0;j<3;j++){
            a.set(i,j,(i+j)%2);
            b.set(i,j,(i+j+1)%2);
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
