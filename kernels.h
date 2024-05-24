#ifndef FUNCTION_KERNELS_H
#define FUNCTION_KERNELS_H

void sobel_gradient(input_stream<int32> *in, output_stream<int32> *out1, output_stream<int32> *out2);
void multiply(input_stream<int32> *ix, input_stream<int32> *iy, output_stream<int32> *ixxiyy, output_stream<int32> *ixy);
void multiply_and_add(input_stream<int32> *ixxiyy, output_stream<int32> *trc, output_stream<int32> *det1);
void square(input_stream<int32> *ixy, output_stream<int32> *det2);
void black(input_stream<int32> *in1, input_stream<int32> *in2, output_stream<int32> *out);
void threshold(input_stream<int32> *trc, input_stream<int32> *det, output_stream<int32> *out);

#endif
