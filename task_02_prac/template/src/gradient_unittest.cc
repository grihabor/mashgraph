#include "../include/gradient.h"
#include "../include/matrix.h"
#include "gtest/gtest.h"

#define EPSILON 1e-5

bool operator==(const Matrix<float> &m1, const Matrix<float> &m2) {
    if (m1.n_cols != m2.n_cols || m1.n_rows != m2.n_rows) {
        return false;
    }

    for (uint y = 0; y < m1.n_rows; ++y) {
        for (uint x = 0; x < m1.n_cols; ++x) {
            if (abs(m1(y, x) - m2(y, x)) > EPSILON) {
                return false;
            }
        }
    }
    return true;
}


TEST(GradientTest, Zero_1x1){
    Matrix<float> m(1, 1);
    m(0, 0) = 0.;
    EXPECT_EQ(m, sse::gradient(m));
}

TEST(GradientTest, Zero_2x2){
    Matrix<float> m = {{0, 0},
                       {0, 0}};
    EXPECT_EQ(m, sse::gradient(m)
    );
}

TEST(GradientTest, Zero_5x5){
    Matrix<float> m = {{0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0}};
    EXPECT_EQ(m, sse::gradient(m)
    );
}

TEST(GradientTest, Const_2x2){
    Matrix<float> m = {{5, 5},
                       {5, 5}};
    Matrix<float> z = {{0, 0},
                       {0, 0}};
}


TEST(GradientTest, Const_5x5){
    Matrix<float> m = {{3.4, 3.4, 3.4, 3.4, 3.4},
                       {3.4, 3.4, 3.4, 3.4, 3.4},
                       {3.4, 3.4, 3.4, 3.4, 3.4},
                       {3.4, 3.4, 3.4, 3.4, 3.4},
                       {3.4, 3.4, 3.4, 3.4, 3.4}};
    Matrix<float> z = {{0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0}};

    EXPECT_EQ(z, sse::gradient(m));
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}