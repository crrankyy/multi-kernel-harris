#include <adf.h>
// #include "coefficients.h"

int IMG_WIDTH = 8;
int IMG_HEIGHT = 8;
int K = 3;

const int Ix[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}};

const int Iy[3][3] = {
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}};

int window[3][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

void sobel_gradient(input_stream<int32> *in, output_stream<int32> *out1, output_stream<int32> *out2)
{

    for (int i = 1; i < K; i++)
        chess_prepare_for_pipelining
            chess_loop_range(2, )
        {
            for (int j = 0; j < IMG_WIDTH; j++)
                chess_prepare_for_pipelining
                    chess_loop_range(4, )
                {
                    int val = readincr(in);
                    window[i][j] = val;
                }
        }

    int count = 0;

    for (int index = 0; index < IMG_HEIGHT - 2; index++)
        chess_prepare_for_pipelining
            chess_loop_range(6, )
        {
            for (int i = 0; i < K - 1; i++)
                chess_prepare_for_pipelining
                    chess_loop_range(2, )
                {
                    for (int j = 0; j < IMG_WIDTH; j++)
                        chess_prepare_for_pipelining
                            chess_loop_range(4, )
                        {
                            window[i][j] = window[i + 1][j];
                        }
                }

            for (int j = 0; j < IMG_WIDTH; j++)
                chess_prepare_for_pipelining
                    chess_loop_range(4, )
                {
                    int val = readincr(in);
                    window[K - 1][j] = val;
                }

            for (int k = 0; k < IMG_WIDTH - 2; k++)
                chess_prepare_for_pipelining
                    chess_loop_range(6, )
                {
                    int grad_x = 0, grad_y = 0;
                    count++;
                    for (int i = 0; i < K; i++)
                        chess_prepare_for_pipelining
                            chess_loop_range(3, )
                        {
                            for (int j = 0; j < K; j++)
                                chess_prepare_for_pipelining
                                    chess_loop_range(3, )
                                {
                                    grad_x += window[i][j + k] * Ix[i][j];
                                    grad_y += window[i][j + k] * Iy[i][j];
                                }
                        }
                    writeincr(out1, grad_x);
                    writeincr(out2, grad_y);
                    // printf("Pixel %2d Row %2d Col %2d: dx = %4d, dy = %4d\n", count, index, k, grad_x, grad_y);
                }
        }
}

void black(input_stream<int32> *in1, input_stream<int32> *in2, output_stream<int32> *out)
{

    for (int i = 0; i < (IMG_HEIGHT - 2) * (IMG_WIDTH - 2); i++)
        chess_prepare_for_pipelining
            chess_loop_range(36, )
        {
            int val1 = readincr(in1);
            int val2 = readincr(in2);

            int val = val1 + val2;

            writeincr(out, val);
        }
}

void square(input_stream<int32> *ixy, output_stream<int32> *det2)
{

    for (int i = 0; i < (IMG_HEIGHT - 2) * (IMG_WIDTH - 2); i++)
        chess_prepare_for_pipelining
            chess_loop_range(36, )
        {
            int val = readincr(ixy);

            int valsq = val * val;

            writeincr(det2, valsq);
        }
}

void multiply_and_add(input_stream<int32> *ixxiyy, output_stream<int32> *trc, output_stream<int32> *det1)
{

    for (int i = 0; i < (IMG_HEIGHT - 2) * (IMG_WIDTH - 2); i++)
        chess_prepare_for_pipelining
            chess_loop_range(36, )
        {
            int val1 = readincr(ixxiyy);
            int val2 = readincr(ixxiyy);

            int val = val1 + val2;
            int val3 = val1 * val2;

            writeincr(trc, val);
            writeincr(det1, val3);
        }
}

void multiply(input_stream<int32> *ix, input_stream<int32> *iy, output_stream<int32> *ixxiyy, output_stream<int32> *ixy)
{

    for (int i = 0; i < (IMG_HEIGHT - 2) * (IMG_WIDTH - 2); i++)
        chess_prepare_for_pipelining
            chess_loop_range(36, )
        {
            int val1 = readincr(ix);
            int val2 = readincr(iy);

            int sq1 = val1 * val1;
            int sq2 = val2 * val2;
            int val = val1 * val2;

            writeincr(ixxiyy, sq1);
            writeincr(ixxiyy, sq2);
            writeincr(ixy, val);
        }
}

void threshold(input_stream<int32> *trc, input_stream<int32> *det, output_stream<int32> *out)
{

    for (int i = 0; i < (IMG_HEIGHT - 2) * (IMG_WIDTH - 2); i++)
        chess_prepare_for_pipelining
            chess_loop_range(36, )
        {
            int val1 = readincr(trc);
            int val2 = readincr(det);

            int val = val2 + 0.04 * val1 * val1;

            if (val > 10000)
            {
                writeincr(out, 1);
            }
            else
            {
                writeincr(out, 0);
            }
        }
}
