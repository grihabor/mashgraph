#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <memory>

#include "../include/gradient.h"

#define EPSILON 0.00001

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

    cout << input_filename << endl;
    image->ReadFromFile(input_filename);

    Matrix<float> img = BMPToGrayscale(image.get());
    auto g2 = sse::gradient(img);
    auto g1 = simple::gradient(img);

    if(abs(g1(0,0) - g2(0,0)) < EPSILON){
        cout << "OK" << endl;
    } else {
        cout << "BAD" << endl;
    }
    cout << endl;
}