#include "adf.h"
#include "kernels.h"

using namespace adf;

class MemTileGraph : public graph
{
public:
    input_plio  in;
    output_plio  out;
    kernel diff, k1, k2, k3, k4, k5;

    MemTileGraph()
    {
        in = input_plio::create("ImageIn", plio_32_bits,"data/input.txt");
        out = output_plio::create("ImageOut", plio_32_bits,"data/output.txt");

        diff = kernel::create(sobel_gradient);
        runtime<ratio>(diff) = 0.1;
        source(diff) = "src/kernels.cpp";
        connect<stream>(in.out[0], diff.in[0]);

        k1 = kernel::create(multiply);
        runtime<ratio>(k1) = 0.1;
        source(k1) = "src/kernels.cpp";
        connect<stream>(diff.out[0], k1.in[0]);
        connect<stream>(diff.out[1], k1.in[1]);

        k2 = kernel::create(multiply_and_add);
        runtime<ratio>(k2) = 0.1;
        source(k2) = "src/kernels.cpp";
        connect<stream>(k1.out[0], k2.in[0]);

        k3 = kernel::create(square);
        runtime<ratio>(k3) = 0.1;
        source(k3) = "src/kernels.cpp";
        connect<stream>(k1.out[1], k3.in[0]);

        k4 = kernel::create(black);
        runtime<ratio>(k4) = 0.1;
        source(k4) = "src/kernels.cpp";
        connect<stream>(k2.out[1], k4.in[0]);
        connect<stream>(k3.out[0], k4.in[1]);

        k5 = kernel::create(threshold);
        runtime<ratio>(k5) = 0.1;
        source(k5) = "src/kernels.cpp";
        connect<stream>(k2.out[0], k5.in[0]);
        connect<stream>(k4.out[0], k5.in[1]);

        connect<stream>(k5.out[0], out.in[0]);
    }
};
