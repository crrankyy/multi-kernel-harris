#ifndef FUNCTION_KERNELS_H
#define FUNCTION_KERNELS_H

void sobel_gradient(input_stream<int32> * __restrict in, output_stream<int32> * __restrict out1, output_stream<int32> * __restrict out2);
void multiply(input_stream<int32> * __restrict ix, input_stream<int32> * __restrict iy, output_stream<int32> * __restrict ixxiyy, output_stream<int32> * __restrict ixy);
void multiply_and_add(input_stream<int32> * __restrict ixxiyy, output_stream<int32> * __restrict trc, output_stream<int32> * __restrict det1);
void square(input_stream<int32> * __restrict ixy, output_stream<int32> * __restrict det2);
void black(input_stream<int32> * __restrict in1, input_stream<int32> * __restrict in2, output_stream<int32> * __restrict out);
void threshold(input_stream<int32> * __restrict trc, input_stream<int32> * __restrict det, output_stream<int32> * __restrict out);

#endif