#include "Sparse_Matrix.h"

#include <gtest.h>

TEST(Sparse_Matrix, can_create_matrix_with_positive_length)
{
  ASSERT_NO_THROW(Sparse_Matrix<int> m(5));
}

TEST(Sparse_Matrix, cant_create_too_large_matrix)
{
  ASSERT_ANY_THROW(Sparse_Matrix<int> m(MAX_MATRIX_SIZE + 1));
}

TEST(Sparse_Matrix, throws_when_create_matrix_with_negative_length)
{
  ASSERT_ANY_THROW(Sparse_Matrix<int> m(-5));
}

TEST(Sparse_Matrix, can_create_copied_matrix)
{
  Sparse_Matrix<int> m(5);

  ASSERT_NO_THROW(Sparse_Matrix<int> m1(m));
}

TEST(Sparse_Matrix, copied_matrix_is_equal_to_source_one)
{
  Sparse_Matrix<int>m(3);
  int tmp;
  for(int i = 0;i<3;i++){
      for(int j = 0;j<3;j++){
          tmp = (i+j+2)*i;
          m.set(i,j,tmp);
      }
  }
  Sparse_Matrix<int>m1(m);
  EXPECT_EQ(true, m1 == m);
}

TEST(Sparse_Matrix, copied_matrix_has_its_own_memory)
{
    int tmp;
    Sparse_Matrix<int>m(3);
    for(int i = 0;i<3;i++){
        for(int j = 0;j<3;j++){
            tmp = (i+j + 2)*i;
            m.set(i,j,tmp);
        }
    }
    Sparse_Matrix<int>m1(m);
    tmp = 10;
    m1.set(0,0,tmp);
    EXPECT_NE(m.get(0,0),m1.get(0,0));
}

TEST(Sparse_Matrix, can_get_size)
{
    Sparse_Matrix<int>m(3);
    EXPECT_EQ(3,m.getsize());
}

TEST(Sparse_Matrix, can_set_and_get_element)
{
    Sparse_Matrix<int>m(3);
    int tmp = 3;
    m.set(2,2,tmp);
    EXPECT_EQ(3,m.get(2,2));
}

TEST(Sparse_Matrix, throws_when_set_element_with_negative_index)
{
    Sparse_Matrix<int>m(3);
    int tmp =0;
    ASSERT_ANY_THROW(m.set(2,-1,tmp));
    ASSERT_ANY_THROW(m.set(-1,2,tmp));
}

TEST(Sparse_Matrix, throws_when_set_element_with_too_large_index)
{
    Sparse_Matrix<int>m(3);
    int tmp =0;
    ASSERT_ANY_THROW(m.set(2,3,tmp));
    ASSERT_ANY_THROW(m.set(3,2,tmp));
}

TEST(Sparse_Matrix, can_assign_matrix_to_itself)
{
    Sparse_Matrix<int>m(2);
    int tmp;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = (i+j)%2;
            m.set(i,j,tmp);
        }
    }
    Sparse_Matrix<int>m1(m);
    m1 = m1;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = (i+j)%2;
            EXPECT_EQ(m.get(i,j),m1.get(i,j));
        }
    }
}

TEST(Sparse_Matrix, can_assign_matrices_of_equal_size)
{
    Sparse_Matrix<int>m(2);
    int tmp;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = (i+j)%2;
            m.set(i,j,tmp);
        }
    }
    Sparse_Matrix<int>m1(2);
    m1 = m;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = (i+j)%2;
            EXPECT_EQ(m.get(i,j),m1.get(i,j));
        }
    }
}

TEST(Sparse_Matrix, assign_operator_change_matrix_size)
{
    Sparse_Matrix<int>m(2);
    int tmp;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = (i+j)%2;
            m.set(i,j,tmp);
        }
    }
    Sparse_Matrix<int>m1(3);
    int privious_size = 3;
    m1 = m;
    int now_size = m1.getsize();
    EXPECT_NE(now_size,privious_size);

}

TEST(Sparse_Matrix, can_assign_matrices_of_different_size)
{
    Sparse_Matrix<int>m(2);
    int tmp;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = (i+j)%2;
            m.set(i,j,tmp);
        }
    }
    Sparse_Matrix<int>m1(3);
    m1 = m;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = (i+j)%2;
            EXPECT_EQ(m.get(i,j),m1.get(i,j));
        }
    }
}

TEST(Sparse_Matrix, compare_equal_matrices_return_true)
{
    Sparse_Matrix<int>m(2);
    int tmp;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = (i+j)%2;
            m.set(i,j,tmp);
        }
    }
    Sparse_Matrix<int>m1(2);
    m1 = m;
    EXPECT_EQ(true , m==m1);
}

TEST(Sparse_Matrix, compare_matrix_with_itself_return_true)
{
    Sparse_Matrix<int>m(2);
    int tmp;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = (i+j)%2;
            m.set(i,j,tmp);
        }
    }
    EXPECT_EQ(true , m==m);
}

TEST(Sparse_Matrix, matrices_with_different_size_are_not_equal)
{
    Sparse_Matrix<int>m(2);
    Sparse_Matrix<int>m1(3);
    EXPECT_EQ(false, m==m1);
}

TEST(Sparse_Matrix, can_add_matrices_with_equal_size)
{
    Sparse_Matrix<int>m(2);
    Sparse_Matrix<int>m1(2);
    int tmp;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = i+j;
            m.set(i,j,tmp);
            tmp = 2 + i + j;
            m1.set(i,j,tmp);
        }
    }
    Sparse_Matrix<int>m2 = m+m1;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            EXPECT_EQ(m.get(i,j) + m1.get(i,j), m2.get(i,j));
        }
    }
}

TEST(Sparse_Matrix, cant_add_matrices_with_not_equal_size)
{
    Sparse_Matrix<int>m(2);
    Sparse_Matrix<int>m1(3);
    ASSERT_ANY_THROW(Sparse_Matrix<int>m2 = m+m1);
}

TEST(Sparse_Matrix, can_subtract_matrices_with_equal_size)
{
    Sparse_Matrix<int>m(2);
    Sparse_Matrix<int>m1(2);
    int tmp;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = i+j;
            m.set(i,j,tmp);
            tmp = 2 + i + j;
            m1.set(i,j,tmp);
        }
    }
    Sparse_Matrix<int>m2 = m-m1;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            EXPECT_EQ(m.get(i,j) - m1.get(i,j), m2.get(i,j));
        }
    }
}

TEST(Sparse_Matrix, cant_subtract_matrixes_with_not_equal_size)
{
    Sparse_Matrix<int>m(2);
    Sparse_Matrix<int>m1(3);
    ASSERT_ANY_THROW(Sparse_Matrix<int>m2 = m-m1);
}

TEST(Sparse_Matrix, can_multiply_matrices_with_equal_size)
{
    Sparse_Matrix<int>m(2);
    Sparse_Matrix<int>m1(2);
    int tmp;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            tmp = i+j;
            m.set(i,j,tmp);
            tmp = 2 + i + j;
            m1.set(i,j,tmp);
        }
    }
    Sparse_Matrix<int>m2 = m*m1;
    for(int i = 0;i<2;i++){
        for(int j = 0;j<2;j++){
            int val = 0;
            for(int k = 0;k<2;k++){
                val += m.get(i,k) * m1.get(k,j);
            }
            EXPECT_EQ(val , m2.get(i,j));
        }
    }
}

TEST(Sparse_Matrix, cant_multyplay_matrixes_with_not_equal_size)
{
    Sparse_Matrix<int>m(2);
    Sparse_Matrix<int>m1(3);
    ASSERT_ANY_THROW(Sparse_Matrix<int>m2 = m*m1);
}
