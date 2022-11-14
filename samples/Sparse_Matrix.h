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
        val = obj.val;
        row = obj.row;
        col = obj.col;
        row_index = obj.row_index;
    }
    Sparse_Matrix(Sparse_Matrix&& obj) noexcept
    {
        size = 0;
        nz = 0;
        swap(*this, obj);
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
    void set(int i , int j,T t){
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
        val = obj.val;
        row = obj.row;
        col = obj.col;
        row_index = obj.row_index;
        return *this;
    }
    Sparse_Matrix& operator=(Sparse_Matrix&& obj) noexcept
    {
        this->clear();
        swap(*this, obj);
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
        int l,r,index,maxsize;l = r = index =0;
        maxsize = min(size*size , nz + m.nz);
        t.row.resize(maxsize);
        t.col.resize(maxsize);
        t.val.resize(maxsize);
        T Neutral_Element{};
        while(true){
            if(l == nz && r == m.nz)
                break;
            if(l == nz){
                t.val[index] = (m.val[r]);
                t.row[index] = (m.row[r]);
                t.col[index] = (m.col[r]);
                t.nz++;r++;
            }
            else if(r == m.nz){
                t.val[index] = (val[l]);
                t.row[index] = (row[l]);
                t.col[index] = (col[l]);
                t.nz++;l++;
            }
            else if(row[l] == m.row[r] && col[l] == m.col[r]){
                T value = val[l] + m.val[r];
                if(value != Neutral_Element) {
                    t.val[index] = (value);
                    t.row[index] = (row[l]);
                    t.col[index] = (col[l]);
                    t.nz++;
                }
                r++;
                l++;
            }
            else if(row[l] < m.row[r] || (row[l] == m.row[r] && col[l] < m.col[r])){
                t.val[index] = (val[l]);
                t.row[index] = (row[l]);
                t.col[index] = (col[l]);
                t.nz++;l++;
            }
            else{
                t.val[index] = (m.val[r]);
                t.row[index] = (m.row[r]);
                t.col[index] = (m.col[r]);
                t.nz++;r++;
            }
            index++;
        }
        Sparse_Matrix<T>res(size);
        res.nz = t.nz;
        res.val.resize(nz);
        res.row.resize(nz);
        res.col.resize(nz);
        index = 0;
        for(int i = 0;i<res.nz;i++){
            if(t.val[i] == Neutral_Element)continue;
            res.val[index] = t.val[i];
            res.row[index] = t.row[i];
            res.col[index] = t.col[i];
            index++;
        }
        index = 0;
        res.row_index[0] = 0;
        for(int i = 0;i<res.size;i++){
            while(index < res.nz && res.row[index] < i) {
                index++;
            }
            res.row_index[i] = index;
        }
        res.row_index[size] = t.nz;
        return res;
    }
    Sparse_Matrix<T> operator-(Sparse_Matrix<T>&m ){
        Sparse_Matrix<T> tmp = m * (-1.0);
        return ( (*this) + (tmp) );
    }
    Sparse_Matrix<T> transposition(){
        //Транспонирование за О(nz) без выделения лишней памяти ¯\_(ツ)_/¯
        Sparse_Matrix<T> AT(*this);
        for(int i = 0;i<=size;i++){
            AT.row_index[i] = 0;
        }
        for(int i = 0;i<nz;i++){
            AT.row_index[col[i]+1]++;
        }
        int s = 0;int tmp;
        for(int i = 1;i<=size;i++){
            tmp = AT.row_index[i];
            AT.row_index[i] = s;
            s+=tmp;
        }
        for(int i = 0;i<size;i++){
            int start = row_index[i];
            int finish = row_index[i+1];
            for(int j = start;j<finish;j++){
                int index = AT.row_index[col[j]+1];
                AT.val[index] = val[j];
                AT.row[index] = col[j];
                AT.col[index] = row[j];
                AT.row_index[col[j]+1]++;
            }
        }
        return AT;
    }
    Sparse_Matrix<T> operator*(Sparse_Matrix<T>& B){
        //Алгоритмическая оптимизация c портретом строки матрицы this
        if(B.size != size)
            throw exception();
        Sparse_Matrix<T>BT = B.transposition();
        vector<int>tmp;
        Sparse_Matrix<T>result(size);
        int start,finish,aind;
        T Neutral_element{};
        for(int i = 0;i<size;i++){
            tmp.resize(size,-1);
            start = row_index[i];
            finish = row_index[i+1];
            for(int j = start;j<finish;j++){
                tmp[col[j]] = j;
            }
            //Построен портрет строки i
            //надо её умножить на каждую из строк матрицы BT
            for(int j = 0;j<size;j++){
                T sum{};
                for(int k = BT.row_index[j];k<BT.row_index[j+1];k++){
                    aind = tmp[BT.col[k]];
                    if(tmp[BT.col[k]] != -1)
                        sum+= val[aind] * BT.val[k];
                }
                if(sum!=Neutral_element){
                    result.nz++;
                    result.row.push_back(i);
                    result.col.push_back(j);
                    result.val.push_back(sum);
                }
            }
            result.row_index[i+1] = result.nz;
        }
        return result;
    }
    void SimbolicMult(Sparse_Matrix<T>&A ,Sparse_Matrix<T>&B){
        //Строим портрет матрицы умножения матриц А,B
        if(B.size != A.size)
            throw exception();
        this->clear();
        row_index.resize(A.size+1,0);
        size = A.size;
        Sparse_Matrix<T>BT = B.transposition();
        vector<int>tmp(size,-1);
        int start,finish,aind;
        for(int i = 0;i<A.size;i++){
            for(int j = 0;j<size;j++)
                tmp[j] = -1;
            start = A.row_index[i];
            finish = A.row_index[i+1];
            for(int j = start;j<finish;j++){
                tmp[A.col[j]] = j;
            }
            //Построен портрет строки i
            //надо её умножить на каждую из строк матрицы BT
            for(int j = 0;j<A.size;j++){
                bool Find = false;
                int k = BT.row_index[j];aind = -1;
                while(aind == -1 && k < BT.row_index[j+1]){
                    aind = tmp[BT.col[k]];
                    k++;
                }
                if(aind != -1){
                    nz++;
                    row.push_back(i);
                    col.push_back(j);
                }
            }
            row_index[i+1] = nz;
        }
    }
    void NumericMult(Sparse_Matrix<T>&A ,Sparse_Matrix<T>&B){
        //Матрицы А,В - это матрицы которые надо перемножить
        //this - уже сформированный партрет умножения этих матриц
        if(B.size != A.size || B.size != size)
            throw exception();
        Sparse_Matrix<T>BT = B.transposition();
        Sparse_Matrix res(*this);
        vector<int>tmp(A.size,-1);
        res.val.resize(nz);
        int start,finish,aind,index,cnt_non_neutral;
        T Neutral_elem{};
        cnt_non_neutral = index = 0;
        for(int i = 0;i<res.size;i++){
            start = res.row_index[i];
            finish = res.row_index[i+1];
            if(start<finish){
                for(int j = 0;j<size;j++)
                    tmp[j] = -1;
            }
            for(int j = A.row_index[i];j<A.row_index[i+1];j++){
                tmp[A.col[j]] = j;
            }
            //Портрет строки i матрицы А построен
            //Умножаем строку i матрицы А на столбцы матрицы В
            for(int j = start;j<finish;j++){
                T sum{};
                for(int k = BT.row_index[res.col[j]];k<BT.row_index[res.col[j]+1];k++){
                    aind = tmp[BT.col[k]];
                    if(aind!=-1)
                        sum+=A.val[aind]*B.val[k];
                }
                if(sum!=Neutral_elem)
                    cnt_non_neutral++;
                res.val[index] = sum;
                index++;
            }
        }
        nz = cnt_non_neutral;
        val.resize(cnt_non_neutral);
        col.resize(cnt_non_neutral);
        row.resize(cnt_non_neutral);
        for(int i = 0;i<=size;i++){
            row_index[i] = 0;
        }
        index = 0;
        for(int i = 0;i<res.nz;i++){
            if(res.val[i] != Neutral_elem){
                val[index] = res.val[i];
                col[index] = res.col[i];
                row[index] = res.row[i];
                row_index[res.row[i] + 1]++;
                index++;
            }
        }
        for(int i = 1;i<=size;i++){
            row_index[i]+=row_index[i-1];
        }
    }
    void SimbolicAdd(Sparse_Matrix<T>&A , Sparse_Matrix<T>&B){
        //Строим шаблон сложения матриц
        if(A.size!=B.size)
            throw exception();
        this->clear();
        size = A.size;nz = 0;
        row_index.resize(size+1);
        int maxsize = min(A.size+B.size , size*size);
        col.resize(maxsize);row.resize(maxsize);
        int l = 0;int r = 0;
        while(true){
            if(l== A.nz && r==B.nz){
                break;
            }
            if(l == A.nz){
                row[nz] = B.row[r];
                col[nz] = B.col[r];
                r++;
            }
            else if(r == B.nz) {
                row[nz] = (A.row[l]);
                col[nz] = (A.col[l]);
                l++;
            }
            else if(A.row[l] == B.row[r] && A.col[l] == B.col[r]){
                row[nz] = A.row[l];
                col[nz] = A.col[l];
                l++;
                r++;
            }
            else if(A.row[l] < B.row[r] || (A.row[l] == B.row[r] && A.col[l]<B.col[r])){
                row[nz] = (A.row[l]);
                col[nz] = (A.col[l]);
                l++;
            }
            else if(A.row[l] > B.row[r] ||(A.row[l] == B.row[r] && A.col[l]>B.col[r])){
                row[nz] = (B.row[r]);
                col[nz] = (B.col[r]);
                r++;
            }
            nz++;
        }
        row_index[0] = 0;
        int index = 0;
        for(int i = 0;i<size;i++){
            while(index < nz && row[index] < i) {
                index++;
            }
            row_index[i] = index;
        }
        row_index[size] = nz;
    }
    void NumericAdd(Sparse_Matrix<T>&A , Sparse_Matrix<T>&B){
        //this - это уже построенный портрет матрицы
        Sparse_Matrix<T>res(*this);
        res.val.resize(res.nz);int l,r,cnt_non_zero;
        int index = 0;cnt_non_zero = 0;
        for(int i = 0;i<size;i++){
            l = A.row_index[i];r = B.row_index[i];
            while(true){
                if(l == A.row_index[i+1] && r == B.row_index[i+1]){
                    break;
                }
                if(l == A.row_index[i+1]){
                    res.val[index]=B.val[r];
                    r++;
                }
                else if(r == B.row_index[i+1]){
                    res.val[index]=A.val[l];
                    l++;
                }
                else if(A.col[l] == B.col[r]){
                    res.val[index] = A.val[l] + B.val[r];
                    l++;r++;
                }
                else if(A.col[l] > B.col[r]){
                    res.val[index] = B.val[r];
                    r++;
                }
                else{
                    res.val[index] = A.val[l];
                    l++;
                }
                index++;
            }
        }
        T Neutral_Element{};
        for(int i = 0;i<res.nz;i++){
            if(res.val[i]!=Neutral_Element)
                cnt_non_zero++;
        }
        index = 0;nz = cnt_non_zero;
        val.resize(cnt_non_zero);
        row.resize(cnt_non_zero);
        col.resize(cnt_non_zero);
        for(int i = 0;i<=size;i++){
            row_index[i] = 0;
        }
        index = 0;
        for(int i = 0;i<res.nz;i++){
            if(res.val[i] != Neutral_Element){
                val[index] = res.val[i];
                col[index] = res.col[i];
                row[index] = res.row[i];
                row_index[res.row[i] + 1]++;
                index++;
            }
        }
        for(int i = 1;i<=size;i++){
            row_index[i]+=row_index[i-1];
        }
    }
    friend void swap(Sparse_Matrix& lhs, Sparse_Matrix& rhs) noexcept
    {
        std::swap(lhs.size, rhs.size);
        std::swap(lhs.nz, rhs.nz);
        std::swap(lhs.val, rhs.val);
        std::swap(lhs.row, rhs.row);
        std::swap(lhs.col, rhs.col);
        std::swap(lhs.row_index, rhs.row_index);
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