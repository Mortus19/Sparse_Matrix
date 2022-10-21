#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

constexpr int MAX_MATRIX_SIZE = 1000;

bool cmp(const vector<int>&a , const vector<int>&b){
    //a[0] - index , a[1] - row[index], a[2] - col[index]
    if(a[1] != b[1])
        return a[1] < b[1];
    return a[2]<b[2];
    //Т.е сортируем сначала по убыванию строк, если строки равны, то по убыванию столбцов
}
template<class T>
class Sparse_Matrix{
    int size;
    int nz;
    vector<T>val;
    vector<int>col;
    vector<int>row;
    vector<int>row_index;
public:
    Sparse_Matrix(int _size = 1){
        if(_size <= 0  || _size>MAX_MATRIX_SIZE)
            throw exception();
        size = _size;
        nz = 0;
    }
    Sparse_Matrix(const Sparse_Matrix& obj){
        size = obj.size;
        nz = obj.nz;
        copy(obj.val.begin(),obj.val.end(), back_inserter(val));
        copy(obj.col.begin(),obj.col.end(),back_inserter(col));
        copy(obj.row.begin(),obj.row.end(),back_inserter(row));
        copy(obj.row_index.begin(),obj.row_index.end(),back_inserter(row_index));
    }
    void clear(){
        size = 0;
        nz = 0;
        val.clear();col.clear();row.clear();row_index.clear();
    }
    int getsize(){return size;}
    Sparse_Matrix<T>& operator=(const Sparse_Matrix<T>&obj){
        (*this).clear();
        size = obj.size;
        nz = obj.nz;
        copy(obj.val.begin(),obj.val.end(), back_inserter(val));
        copy(obj.col.begin(),obj.col.end(),back_inserter(col));
        copy(obj.row.begin(),obj.row.end(),back_inserter(row));
        copy(obj.row_index.begin(),obj.row_index.end(),back_inserter(row_index));
        return *this;
    }
    vector<T> operator*(vector<T>&b){
        if(b.size() != size)
            throw exception();
        vector<T> res(size);
        for(int i = 0;i<size;i++){
            res[i] = T{};
        }
        for(int i = 0;i<size;i++){
            int start = row_index[i];
            int finish = row_index[i+1];
            for(int j = start;j<finish;j++){
                res[i]+= val[j] * b[col[j]];
            }
        }
        return res;
    }
    Sparse_Matrix<T> operator+(const T& val){
        Sparse_Matrix<T>t(*this);
        for(int i = 0;i<size;i++){
            t.val[i] += val;
        }
        return t;
    }
    Sparse_Matrix<T> operator-(const T& val){
        return *this + (-val);
    }
    Sparse_Matrix<T> operator*(const T& val){
        Sparse_Matrix<T>t(*this);
        for(int i = 0;i<size;i++){
            t.val[i] *= val;
        }
        return t;
    }
    Sparse_Matrix<T> transposition(){
        //Моя реализация транспонирования, хочу чтобы были упорядочены по строкам, и по столбцам
        //Будет работать за O(nz * ln(nz) )
        //так как мы транспонируем матрицу , то row[index] будет col[index] , а col[index] будет row[index]
        vector<vector<int>>tmp(nz);// будет хранить 3 элемента {index , row[index], col[index]};
        for(int i = 0;i<nz;i++){
            tmp[i] = {i ,col[i], row[i]};
        }
        sort(tmp.begin() , tmp.end() , cmp);
        Sparse_Matrix<T>t(*this);
        for(int i = 0;i<nz;i++){
            t.val[i] = val[tmp[i][0]];
            t.row[i] = tmp[i][1];
            t.col[i] = tmp[i][2];
        }
        int cnt = 1;
        t.row_index[0] = 0;
        for(int i = 1;i<nz;i++){
            if(t.row[i]!=t.row[i-1])
                t.row_index[t.row[i]] = cnt;
            cnt++;
        }
        t.row_index[size] = cnt;
        return t;
    }
    Sparse_Matrix<T> operator*(Sparse_Matrix<T>& m){
        if(m.size != size)
            throw exception();
        Sparse_Matrix<T> tm = m.transposition();
        Sparse_Matrix<T> result(size);
        result.row_index.resize(size+1);
        T Neutral_element{};
        for(int i = 0;i<size;i++){
            //Оно робит!!!
            result.row_index[i] = result.nz;
            for(int j = 0;j<size;j++){
                T res_i_j{};
                int l = row_index[i];
                int r = tm.row_index[j];
                while(l < row_index[i+1] && r<tm.row_index[j+1]){
                    if(col[l] == tm.col[r]){
                        res_i_j+= val[l] * tm.val[r];
                        l++;
                        r++;
                    }
                    else if(col[l] < tm.col[r])
                        l++;
                    else if(col[l] > tm.col[r])
                        r++;
                }
                if(res_i_j == Neutral_element)continue;
                result.val.push_back(res_i_j);
                result.row.push_back(i);
                result.col.push_back(j);
                result.nz++;
            }
        }
        result.row_index[size] = nz;
        return result;
    }
    friend istream& operator>>(istream& in , Sparse_Matrix<T>&obj){
        int n;
        in>>n;
        obj.clear();
        obj.size = n;
        T Neutral_element{};
        obj.row_index.resize(n+1);
        for(int i = 0;i<n;i++){
            obj.row_index[i] = obj.nz;
            for(int j = 0;j<n;j++){
                T x;
                in>>x;
                if(x!= Neutral_element){
                    //То это не нулевой элемент, нам его надо добавить
                    obj.val.push_back(x);
                    obj.row.push_back(i);
                    obj.col.push_back(j);
                    obj.nz++;
                }
            }
        }
        obj.row_index[n] =obj.nz;
        return in;
    }
    friend ostream& operator<<(ostream& out , Sparse_Matrix<T>&obj){
        for(int i = 0;i<obj.size;i++){
            int start = obj.row_index[i];
            int finish = obj.row_index[i+1];
            int now = 0;
            for(int j = 0;j<obj.size;j++){
                if(now + start < finish && obj.col[now+start] == j){
                    out<<obj.val[now+start]<<' ';
                    now++;
                }
                else
                    out<<0<<' ';
            }
            out<<'\n';
        }
        return out;
    }


};