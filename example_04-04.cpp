#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

// A better way to print a sparse matrix
//
template <class T> void print_matrix( const cv::SparseMat_<T>* sm ) {
  cv::SparseMatConstIterator_<T> it = sm->begin();
  cv::SparseMatConstIterator_<T> it_end = sm->end();

  for(; it != it_end; ++it) {
    const typename cv::SparseMat_<T>::Node* node = it.node();
    cout <<"( " <<node->idx[0] <<", " <<node->idx[1]
         <<" ) = " <<*it <<endl;
  }
}

void calling_function1( void ) {

  int ndim = 2;
  int size[] = {4,4};

  cv::SparseMat_<float> sm( ndim, size );

  // Create a sparse matrix with a few nonzero elements
  //
  for( int i=0; i<4; i++ ) { // Fill the array
    int idx[2];
    idx[0] = size[0] * rand();
    idx[1] = size[1] * rand();
    sm.ref( idx ) += 1.0f;
  }

  print_matrix<float>( &sm );
}

void calling_function2( void ) {

  int ndim = 2;
  int size[] = {4,4};

  cv::SparseMat sm( ndim, size, CV_32F );

  // Create a sparse matrix with a few nonzero elements
  //
  for( int i=0; i<4; i++ ) { // Fill the array
    int idx[2];
    idx[0] = size[0] * rand();
    idx[1] = size[1] * rand();
    sm.ref<float>( idx ) += 1.0f;
  }

  print_matrix<float>( (cv::SparseMat_<float>*) &sm );

}

int main( int argc, char** argv ) {

  cout <<"Case 1:" <<endl;
  calling_function1();
  cout <<endl;

  cout <<"Case 2:" <<endl;
  calling_function2();
  cout <<endl;

}

