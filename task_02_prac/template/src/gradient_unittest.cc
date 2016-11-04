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
    ASSERT_EQ(m, sse::gradient(m));
}

TEST(GradientTest, Zero_2x2){
    Matrix<float> m = {{0, 0},
                       {0, 0}};
    ASSERT_EQ(m, sse::gradient(m)
    );
}

TEST(GradientTest, Zero_5x5){
    Matrix<float> m = {{0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0}};
    ASSERT_EQ(m, sse::gradient(m));
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

    ASSERT_EQ(z, sse::gradient(m));
}

TEST(GradientTest, Const_4x8){
    int height = 4;
    int width = 8;
    float c = -3.1415;

    Matrix<float> m(height, width);
    Matrix<float> z(height, width);

    for(int y = 0; y < height; ++y)
        for(int x = 0; x < width; ++x){
            m(y, x) = c;
            z(y, x) = 0.;
        }

    ASSERT_EQ(z, sse::gradient(m));
}

char **argv;
int argc;

TEST(GradientTest, Lenna){
    ASSERT_GE(argc, 2);
    char *filename = argv[1];
    std::unique_ptr<BMP> image(new BMP());
    image->ReadFromFile(filename);
    Matrix<float> img = BMPToGrayscale(image.get());

    ASSERT_EQ(simple::gradient(img), sse::gradient(img));
}


int main(int argc, char **argv) {
    ::argv = argv;
    ::argc = argc;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}