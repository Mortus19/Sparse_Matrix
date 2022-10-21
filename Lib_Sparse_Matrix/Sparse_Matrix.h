#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

constexpr int MAX_MATRIX_SIZE = 1000000;

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
        row_index.resize(_size+1, 0);
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
    bool is_neutral(int i ,int j){
        if(i < 0 || i>=size || j < 0 || j >= size)
            throw exception();
        for(int k = row_index[i];k<row_index[i+1];k++){
            if(col[k] == j)
                return false;
        }
        return true;
    }
    T get(int i , int j){
        if(i < 0 || i>=size || j < 0 || j >= size)
            throw exception();
        for(int k = row_index[i];k<row_index[i+1];k++){
            if(col[k] == j)
                return val[k];
        }
        T tmp{};
        return tmp;
    }
    void set(int i , int j,T& t){
        if(i < 0 || i>=size || j < 0 || j >= size)
            throw exception();
        T Neutral_element{};
        if(t == Neutral_element && is_neutral(i,j))
            return;
        if(t != Neutral_element && !is_neutral(i,j)){
            for(int k = row_index[i];k<row_index[i+1];k++){
                if(col[k] == j) {
                    val[k] = t;
                    return;
                }
            }
        }
        if(t == Neutral_element){
            //То мы не нейтральный заменяем на нейтральный элемент
            //nz уменьшается
            int index = 0;
            for(int k = row_index[i];k<row_index[i+1];k++){
                if(col[k] == j){
                    index = k;
                    break;
                }
            }
            for(int k = index;k<nz-1;k++){
                swap(val[k] , val[k+1]);
                swap(row[k] , row[k+1]);
                swap(col[k] , col[k+1]);
            }
            val.pop_back();
            row.pop_back();
            col.pop_back();
            nz--;
            for(int k = i+1;k<=size;k++)
                row_index[k]--;
            return;
        }
        if(t != Neutral_element){
            //То мы нейтральный заменяем на нейтральный
            //nz увеличивается
            val.push_back(t);
            row.push_back(i);
            col.push_back(j);
            nz++;
            for(int k =nz-2;k>=0;k--){
                if(row[k] > row[k+1]){
                    swap(val[k] , val[k+1]);
                    swap(row[k] , row[k+1]);
                    swap(col[k] , col[k+1]);
                }
                if(row[k] == row[k+1] && col[k] >col[k+1]){
                    swap(val[k] , val[k+1]);
                    swap(row[k] , row[k+1]);
                    swap(col[k] , col[k+1]);
                }
            }
            for(int k = i+1;k<=size;k++)
                row_index[k]++;
            return;
        }
    }
    bool operator==(const Sparse_Matrix<T>& m ){
        if(nz != m.nz || size != m.size)
            return false;
        for(int i = 0;i<=size;i++){
            if(row_index[i] != m.row_index[i])
                return false;
        }
        for(int i = 0;i<nz;i++){
            if(val[i]!=m.val[i])
                return false;
            if(row[i]!=m.row[i])
                return false;
            if(col[i]!=m.col[i])
                return false;
        }
        return true;
    }
    bool operator!=(const Sparse_Matrix<T>& m){
        if(*this == m)
            return false;
        return true;
    }
    Sparse_Matrix<T>& operator=(const Sparse_Matrix<T>&obj){
        if(*this == obj)
            return *this;
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
    Sparse_Matrix<T> operator+(T& value){
        Sparse_Matrix<T>t(*this);
        for(int i = 0;i<nz;i++){
            t.val[i] += value;
        }
        return t;
    }
    Sparse_Matrix<T> operator-(T& value){
        return *this + (-value);
    }
    Sparse_Matrix<T> operator*(double value){
        if(value == 0) {
            Sparse_Matrix<T>t(size);
            return t;
        }
        Sparse_Matrix<T>t(*this);
        for(int i = 0;i<nz;i++){
            t.val[i] *= value;
        }
        return t;
    }
    Sparse_Matrix<T> operator+(Sparse_Matrix<T>&m ){
        if(size != m.size)
            throw exception();
        Sparse_Matrix<T>t(size);
        int l = 0;
        int r = 0;
        t.row_index.resize(size+1,0);
        while(l < nz && r<m.nz){
            if(row[l] == m.row[r] && col[l] == m.col[r]){
                T value = val[l] + m.val[r];
                if(value != T{}) {
                    t.val.push_back(value);
                    t.row.push_back(row[l]);
                    t.col.push_back(col[l]);
                    t.nz++;
                }
                r++;
                l++;
            }
            else if(row[l] < m.row[r] || (row[l] == m.row[r] && col[l] < m.col[r])){
                t.val.push_back(val[l]);
                t.row.push_back(row[l]);
                t.col.push_back(col[l]);
                t.nz++;
                l++;
            }
            else{
                t.val.push_back(m.val[r]);
                t.row.push_back(m.row[r]);
                t.col.push_back(m.col[r]);
                t.nz++;
                r++;
            }
        }
        while(l < nz){
            t.val.push_back(val[l]);
            t.row.push_back(row[l]);
            t.col.push_back(col[l]);
            t.nz++;
            l++;
        }
        while(r < m.nz){
            t.val.push_back(m.val[r]);
            t.row.push_back(m.row[r]);
            t.col.push_back(m.col[r]);
            t.nz++;
            r++;
        }
        t.row_index[0] = 0;
        int index = 0;
        for(int i = 0;i<t.size;i++){
            while(index < t.nz && t.row[index] < i) {
                index++;
            }
            t.row_index[i] = index;
        }
        t.row_index[size] = t.nz;
        return t;
    }
    Sparse_Matrix<T> operator-(Sparse_Matrix<T>&m ){
        Sparse_Matrix<T> tmp = m * (-1.0);
        return ( (*this) + (tmp) );
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
        t.row_index[0] = 0;
        int index = 0;
        for(int i = 0;i<t.size;i++){
            while(index < t.nz && t.row[index] < i) {
                index++;
            }
            t.row_index[i] = index;
        }
        t.row_index[size] = t.nz;
        return t;
    }
    Sparse_Matrix<T> operator*(Sparse_Matrix<T>& m){
        if(m.size != size)
            throw exception();
        Sparse_Matrix<T> tm = m.transposition();
        Sparse_Matrix<T> result(size);
        T Neutral_element{};
        for(int i = 0;i<size;i++){
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
        result.row_index[size] = result.nz;
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