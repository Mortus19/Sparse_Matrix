#include <iostream>
#include "Sparse_Matrix.h"
using namespace std;
int main() {
    Sparse_Matrix<int>m;
    cin>>m;
    Sparse_Matrix<int>tm = m.transposition();
    vector<int>b = {1,2,3};
    vector<int>res = m*b;
    for(int i = 0;i<3;i++){
        cout<<res[i]<<' ';
    }
    cout<<'\n';
    cout<<'\n';
    cout<<m<<'\n';
    cout<<'\n';
    cout<<tm<<'\n';

    Sparse_Matrix<int>lol = m * m;
    cout<<'\n';
    cout<<lol<<'\n';
    return 0;
}
