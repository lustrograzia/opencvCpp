
#define SQR(x) ((x)*(x))

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

// recursive 가우시안 필터의 계수계산;
void compute_coefs3(double c[5], double sigma) {
/*
	"recursive Implementation of the gaussian filter."
	lan T. Young, Lucas J. Van Vliet, Signal Processing 44. Elsevier 1995.
*/
	double q = 0;
	if (sigma >= 2.5)
		q = 0.98711 * sigma - 0.96330;
	else if ((sigma >= 0.5) && (sigma < 2.5))
		q = 3.97156 - 4.14554 * (double)sqrt((double)1 - 0.26891 * sigma);
	else
		q = 0.1147705018520355224609375;

	double q2 = q * q;
	double q3 = q * q2;
	c[0] = (1.57825 + (2.44413 * q) + (1.4281 * q2) + (0.422205 * q3));
	c[1] = ((2.44413 * q) + (2.85619 * q2) + (1.26661 * q3));
	c[2] = (-((1.4281 * q2) + (1.26661 * q3)));
	c[3] = ((0.422205 * q3));
	c[4] = 1.0 - (c[1] + c[2] + c[3]) / c[0]; //=B*;
}

// 멀티스케일 설정;
void retinex_scale_distribution(const int nscales/*=3*/,
	const int s/*=240*/, double scales[]) {

	// ASSERT(nscales >= 3);
	double size_step = (double)s / (double)nscales;
	for (int i = 0; i < nscales; ++i)
		scales[i] = 2. + (double)i * size_step;
}

// 가우시안 convolution(horizontal)
void gauss_smooth(double* src, int width, int height, double* out,
	double b[5]) {

	int bufsize = width + 3;
	std::vector<double> w1(bufsize);
	std::vector<double> w2(bufsize);
	double invb0 = 1. / b[0];
	for (int y = 0; y < height; y++) {
		/* forward pass */
		double* src_ptr = &src[y * width];
		w1[0] = src_ptr[0];
		w1[1] = src_ptr[0];
		w1[2] = src_ptr[0];
		for (int x = 0; x < width; x++) {
			w1[x + 3] = b[4] * src_ptr[x]
				+ ((b[1] * w1[x + 2] + b[2] * w1[x + 1] + b[3] * w1[x])
					* invb0);
		}
		/* backward pass : out ==> transposed; */
		w2[width + 0] = w1[width + 2];
		w2[width + 1] = w1[width + 2];
		w2[width + 2] = w1[width + 2];
		for (int x = width - 1; x >= 0; x--) {
			w2[x] = out[x * height + y] = b[4] * w1[x]
				+ ((b[1] * w2[x + 1] + b[2] * w2[x + 2] + b[3] * w2[x + 3])
					* invb0);
		}
	}
}

// 이미지의 평균과 표준편차 계산;
void image_statistics(double* img, int size /* width*height */,
	double* mean, double* std) {

	double s = 0, ss = 0;
	for (int i = 0; i < size; i++) {
		double a = img[i];
		s += a;
		ss += SQR(a);
	}
	*mean = s / size;
	*std = sqrt((ss - s * s / size) / size);
}

//
void rescale_range(double* data, int size) {
	
	double mean, sig;
	image_statistics(&data[0], size, &mean, &sig);

	double max_val = mean + 1.2 * sig;
	double min_val = mean - 1.2 * sig;
	double range = max_val - min_val;
	if (!range) range = 1.0;
	range = 255. / range;
	// change the range;
	for (int i = 0; i < size; i++) {
		data[i] = (data[i] - min_val) * range;
	}
}

// 메인
void retinex_process(double* src, int width, int height, double* dst) {
	const int nfilter = 3;
	double sigma[nfilter];
	int default_scale = 240;
	retinex_scale_distribution(nfilter, default_scale, sigma);
	TRACE("sigma::%f, %f, %f\n", sigma[0], sigma[1], sigma[2]);
	std::vector<double> scaleimg[nfilter];
	// allocate filterd image;
	for (int i = 0; i < nfilter; i++) scaleimg[i].resize(width * height);

	// scale-space gauss_smooth;
	for (int i = 0; i < nfilter; i++) {
		double c[5];
		compute_coefs3(c, sigma[i]);
		// seperable gaussian convolution;
		// (1) hrizontal convolution; (wxh --> hxw); and dst as a temp-buffer;
		gauss_smooth(&src[0], width, height, &dst[0], c);
		// (2) vertical convolution; (hxw --> wxh);
		gauss_smooth(&dst[0], height, width, &((scaleimg[i])[0]), c);
	}
	// reset dst-image;
	memset(dst, 0, width * height * sizeof(double));

	// accumulate dst-image;
	for (int i = 0; i < nfilter; i++) {
		std::vector<double>& filtered = scaleimg[i];
		for (int k = 0; k < width * height; k++)
			dst[k] += log(src[k] + 1.) - log(filtered[k] + 1.);
	}

	//scale to [0, 255];
	rescale_range(&dst[0], width * height);
}


int main()
{
	VideoCapture capture(-1);
	if (!capture.isOpened())
	{
		cout << "didn't connect camera" << endl;
		exit(1);
	}

	for (;;) {
		Mat frame;
		capture.read(frame);

		put_string(frame, "EXPOS: ", Point(10, 40), capture.get(CAP_PROP_EXPOSURE));
		imshow("cam", frame);
		if (waitKey(10) >= 0) break;
	}
	return 0;
}