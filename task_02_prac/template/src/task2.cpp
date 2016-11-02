#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <memory>

#include "../include/gradient.h"

#define EPS 0.00001

using std::unique_ptr;
using std::cout;
using std::endl;
using std::ofstream;

int main(int argc, char** argv) {

    SetEasyBMPwarningsOff();

    if(argc != 2){
        cout << "Usage: task2 <img_filename>";
        return 0;
    }
    const char *input_filename = argv[1];

    std::unique_ptr<BMP> image(new BMP());

    image->ReadFromFile(input_filename);

    Matrix<float> img = BMPToGrayscale(image.get());

    cout << img;

    auto g1 = simple::gradient(img);

    cout << endl;
    cout << g1.n_rows << ' ' << g1.n_cols << endl;
    cout << g1;

    auto g2 = sse::gradient(img);

    cout << endl;
    cout << g2.n_rows << ' ' << g2.n_cols << endl;
    cout << g2;

    cout << endl;
    cout << g1(0,0) << ' ' << g1(0,1) << endl;
    cout << g2(0,0) << ' ' << g2(0,1) << endl;

    if(abs(g1(0,0) - g2(0,0)) < EPS){
        cout << "OK" << endl;
    } else {
        cout << "BAD" << endl;
    }
    cout << endl;
}