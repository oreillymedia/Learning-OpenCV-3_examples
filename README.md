# Learning OpenCV 3


This is the example code that accompanies Learning OpenCV 3 by Adrian Kaehler and Gary Bradski (9781491937990). 
  
* ~~In progress May 24, 2017~~
* ~~In progress June 13, 2017~~
* ~~In progress June 20, 2017~~
* ~~In progress June 22, 2017~~
* ~~In progress July 02, 2017~~

**To do:**
* ~~Chapter 15~~
* ~~Chapter 16~~
* ~~Chapter 17~~
* ~~Chapter 18~~
* ~~Chapter 19~~
* ~~Chapter 20~~
* ~~Chapter 21~~
* ~~Chapter 22~~

Checking Chapters
~~2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15~~ 16 17 18 19 20 ~~21 22 23~~

* Submit associated errata for ~~15 16~~ 17 18 19 20 21 22
* Throw in some bonus material
  * ~~background subtraction~~
  * feature detector/descriptors
  * ~~More mac~~ machine learning(?) ... ongoing!
  * Some new stuff in opencv_contrib
  * Some DNN examples(?)
* ~~Throw in some exercises~~ ... ongoing!

For default suggestions of how the run the code, it assumes you put your build directory under Learning-OpenCV-3_examples directory. Thus, you

Previously, from the Learning-OpenCV-3_examples directory:

*  mkdir build
   * Then:
*  cd build
*  cmake ..
*  make -j

Note, for your interest, included here is an Ubuntu _Docker_ file that
* Shares a directory with the host operating system
* Shares the first camera between both systems
* Loads Ubuntu 16.04 
* Loads all dependencies for OpenCV 3.2 and opencv_contrib
* Loads and builds OpenCV 3.2 and opencv_contrib into a build directory 
  * executable files end up in opencv-3.2.0/build/bin
* Git clones the code (and Docker file) for Learning OpenCV 3 and builds it
  * executable files end up in Learning_OpenCV-3_examples/build
* To get to the top level directory, just type: `cd`


Click the Download Zip button to the right to download example code.

Visit the catalog page [here](http://shop.oreilly.com/product/0636920044765.do).

See an error? Report it [here](http://oreilly.com/catalog/errata.csp?isbn=0636920044765), or simply fork and send us a pull request
