#include <string>
#include <iostream>
#include <memory>

#include "EasyBMP.h"
#include "gradient.h"
#include "Timer.h"
#include "matrix.h"

/** @file task2.cpp
 * Main file of the project.
 */

/** @file matrix.h
 * Matrix operations.
 */

/** @file matrix.hpp
 * Matrix operations implementations.
 */

/*! \mainpage Gradient Project Index Page
 *
 * \section desc_sec Description
 *
 * This project is about gradient calculation.
 * There are two implemetnations of function gradient: simple one and with SSE.
 *
 * \section install_sec How to run
 * __Step 1:__ Download the .tar.gz archive and extract the project\n
 * __Step 2:__ cd into the directory\n
 * \subsection install_python A) Using python
 * __Step 3:__ add .bmp files to data folder (optional)\n
 * __Step 4:__ run
 * \code{.unparsed}
 * make all
 * \endcode
 * to check time of gradient functions\n
 * __Step 5:__ run
 * \code{.unparsed}
 * make test
 * \endcode
 * to run the tests\n
 * \subsection install_easy B) Without python
 * __Step 3:__ run
 * \code{.unparsed}
 * cd template
 * make all
 * build/bin/task2 --sse ../data/Lenna.bmp 100
 * build/bin/task2 ../data/Lenna.bmp 100
 * \endcode
 * to check time of gradient functions\n
 * __Step 4:__ run
 * \code{.unparsed}
 * make test
 * \endcode
 * to run the tests\n
 * \section explain_sec Details
 * __make all__ in the template directory builds the project and creates __task2__ executable.
 *
 * __make all__ in the root directory runs __make all__ in the template directory,
 * and runs __task2__ on all the .bmp files in data directory. This is performed by python script __run.py__.
 */

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
