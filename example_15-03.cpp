//Example 15-3. Computing the on and off-diagonal elements of a variance/covariance model

vector<cv::Mat> planes(3);
vector<cv::Mat> sums(3);
vector<cv::Mat> xysums(6);
int image_count = 0;

//A function to accumulate
// the information we need for our variance computation:
void accumulateVariance(
	cv::Mat& I
) {
	if( sum.empty ) {
		sum = cv::Mat::zeros( I.size(), CV_32FC(I.channels()) );
		sqsum = cv::Mat::zeros( I.size(), CV_32FC(I.channels()) );
	}
	cv::accumulate( I, sum );
	cv::accumulateSquare( I, sqsum );
	image_count++;
}

//The associated variance computation function would then be:
// (note that 'variance' is sigma^2)
//
void computeVariance(
	cv::Mat& variance
	) {
	double one_by_N = 1.0 / image_count;
	variance = one_by_N * sqsum – (one_by_N * one_by_N) * sum.mul(sum);
}




void accumulateCovariance(
cv::Mat& I
) {
	int i, j, n;

	if( sum.empty ) {
		for( i=0; i<3; i++ ) {
			// the r, g, and b sums
			sums[i]
			= cv::Mat::zeros( I.size(), CV::F32C1 );
		}
		for( n=0; n<6; n++ ) {
			// the rr, rg, rb, gg, gb, and bb elements
			xysums[n] = cv::Mat::zeros( I.size(), CV::F32C1 ) );
		}
	}
	cv::split( I, planes );
	for( i=0; i<3; i++ ) {
		cv::accumulate( planes[i], sums[i] );
	}
	n = 0;
	for( i=0; i<3; i++ ) {
		// "row" of Sigma
		for( j=i; j<3; j++ ) {
			// "column" of Sigma
			n++;
			cv::accumulateProduct( planes[i], planes[j], xysums[n] );
		}
	}
	image_count++;
}

//The corresponding compute function is also just a slight extension of the compute
//function for the variances we saw earlier.
//   note that 'variance' is sigma^2
//
void computeVariance(
	cv::Mat& covariance
	// a six-channel array, channels are the
	// rr, rg, rb, gg, gb, and bb elements of Sigma_xy
) {
	double one_by_N = 1.0 / image_count;
	
	// reuse the xysum arrays as storage for individual entries
	//
	int n = 0;
	for( int i=0; i<3; i++ ) {
		// "row" of Sigma
		for( int j=i; j<3; j++ ) {
			// "column" of Sigma
			n++;
			xysums[n] = one_by_N * xysums[n]
			– (one_by_N * one_by_N) * sums[i].mul(sums[j]);
		}
	}
	
	// reassemble the six individual elements into a six-channel array
	//
	cv::merge( xysums, covariance );
}
