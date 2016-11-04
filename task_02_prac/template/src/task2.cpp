#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <memory>

#include "../include/gradient.h"
#include "../include/Timer.h"
#define EPSILON 0.00001

using std::unique_ptr;
using std::cout;
using std::endl;
using std::ofstream;

int main(int argc, char** argv) {

    SetEasyBMPwarningsOff();

    if(argc < 3 || argc > 4 || (argc == 4 && std::string(argv[1]) != "--sse")){
        cout << "Usage: task2 [--sse] FILENAME RUN_COUNT" << endl;
        cout << "Calculate gradient of image FILENAME, run RUN_COUNT"
             << " times and print average time" << endl;
        return 0;
    }
    const char *input_filename;
    bool use_sse;
    int RUN_COUNT;

    input_filename = argv[argc-2];
    use_sse = (argc == 4);
    RUN_COUNT = atoi(argv[argc-1]);

    std::unique_ptr<BMP> image(new BMP());

    cout << input_filename << endl;
    image->ReadFromFile(input_filename);

    Matrix<float> img = BMPToGrayscale(image.get());
    Matrix<float> grad;
    Timer t;


    if(use_sse){
        t.start();
        for(int i = 0; i < RUN_COUNT; ++i)
            grad = sse::gradient(img);
        t.check("SSE   ");
    } else {
        t.start();
        for (int i = 0; i < RUN_COUNT; ++i)
            grad = simple::gradient(img);
        t.check("Simple");
    }
}