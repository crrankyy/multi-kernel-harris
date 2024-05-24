#include <adf.h>
// #include "coefficients.h"

int IMG_WIDTH = 8;
int IMG_HEIGHT = 8;
int K = 3;

const int Ix[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
};

const int Iy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
};

int window[3][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}               
};

void sobel_gradient(input_stream<int32> * __restrict in, output_stream<int32> * __restrict out1, output_stream<int32> * __restrict out2){

    for(int i = 1; i < K; i++)
    chess_prepare_for_pipelining
    chess_loop_range(2,)
    {
        for (int j = 0; j < IMG_WIDTH; j++)
        chess_prepare_for_pipelining
        chess_loop_range(4,)
        {
            int val = readincr(in);
            window[i][j] = val;
        }
    }

    int count = 0;

    for(int index = 0; index < IMG_HEIGHT - 2; index++)
    chess_prepare_for_pipelining
    chess_loop_range(6,)
    {
        for(int i = 0; i < K-1; i++)
        chess_prepare_for_pipelining
        chess_loop_range(2,)
        {
            for (int j = 0; j < IMG_WIDTH; j++)
            chess_prepare_for_pipelining
            chess_loop_range(4,)
            {
                window[i][j] = window[i+1][j];
            }
        }

        for (int j = 0; j < IMG_WIDTH; j++)
        chess_prepare_for_pipelining
        chess_loop_range(4,)
        {
            int val = readincr(in);
            window[K-1][j] = val;
        }

        for (int k = 0; k < IMG_WIDTH-2; k++) 
        chess_prepare_for_pipelining
        chess_loop_range(6,)
        {
            int grad_x = 0, grad_y = 0;
            count++;
            for (int i = 0; i < K; i++)
            chess_prepare_for_pipelining
            chess_loop_range(3,)
            {
                for (int j = 0; j < K; j++)
                chess_prepare_for_pipelining
                chess_loop_range(3,)
                {
                    grad_x += window[i][j+k] * Ix[i][j];
                    grad_y += window[i][j+k] * Iy[i][j];
                }
            }
            writeincr(out1, grad_x);
            writeincr(out2, grad_y);
            printf("Pixel %2d Row %2d Col %2d: dx = %4d, dy = %4d\n", count, index, k, grad_x, grad_y);
        }
    }
}

void black(input_stream<int32> * __restrict in1, input_stream<int32> * __restrict in2, output_stream<int32> * __restrict out){
    
    for(int i = 0; i < (IMG_HEIGHT-2)*(IMG_WIDTH-2); i++)
    chess_prepare_for_pipelining
    chess_loop_range(36,)
    {
        int val1 = readincr(in1);
        int val2 = readincr(in2);
        int val = val1 + val2;
        writeincr(out, val);
    }
}

void square(input_stream<int32> * __restrict ixy, output_stream<int32> * __restrict det2){
    
    for(int i = 0; i < (IMG_HEIGHT-2)*(IMG_WIDTH-2); i++)
    chess_prepare_for_pipelining
    chess_loop_range(36,)
    {
        int val = readincr(ixy);
        int valsq = val*val;
        writeincr(det2, valsq);
        printf("Pixel %2d: sq = %d\n", i+1, valsq);
    }
}

void multiply_and_add(input_stream<int32> * __restrict ixxiyy, output_stream<int32> * __restrict trc, output_stream<int32> * __restrict det1){
    
    for(int i = 0; i < (IMG_HEIGHT-2)*(IMG_WIDTH-2); i++)
    chess_prepare_for_pipelining
    chess_loop_range(36,)
    {
        int val1 = readincr(ixxiyy);
        int val2 = readincr(ixxiyy);
        writeincr(trc, val1 + val2);
        writeincr(det1, val1 * val2);
    }
}

void multiply(input_stream<int32> * __restrict ix, input_stream<int32> * __restrict iy, output_stream<int32> * __restrict ixxiyy, output_stream<int32> * __restrict ixy){
    
    for(int i = 0; i < (IMG_HEIGHT-2)*(IMG_WIDTH-2); i++)
    chess_prepare_for_pipelining
    chess_loop_range(36,)
    {
        int val1 = readincr(ix);
        int val2 = readincr(iy);
        writeincr(ixxiyy, val1 * val1);
        writeincr(ixxiyy, val2 * val2);
        writeincr(ixy, val1 * val2);
    }
}

void threshold(input_stream<int32> * __restrict trc, input_stream<int32> * __restrict det, output_stream<int32> * __restrict out){
    
    for(int i = 0; i < (IMG_HEIGHT-2)*(IMG_WIDTH-2); i++)
    chess_prepare_for_pipelining
    chess_loop_range(36,)
    {
        int val1 = readincr(trc);
        int val2 = readincr(det);

        int val = val2 + 0.04 * val1 * val1;

        if(val > 10000){
            writeincr(out, 1);
            printf("1\n");
        }else{
            writeincr(out, 0);
            printf("0\n");
        }
    }
}