#include "adf.h"
#include "kernels.h"

using namespace adf;

class MemTileGraph : public graph
{
public:
    input_plio  in;
    output_plio  out;
    kernel diff, k2, sq, sq2;

    MemTileGraph()
    {
        in = input_plio::create("ImageIn", plio_32_bits,"data/input.txt");
        out = output_plio::create("ImageOut", plio_32_bits,"data/output.txt");

        diff = kernel::create(sobel_gradient);
        runtime<ratio>(diff) = 0.1;
        source(diff) = "src/kernels.cpp";
        connect<stream>(in.out[0], diff.in[0]);

        sq = kernel::create(square);
        runtime<ratio>(sq) = 0.1;
        source(sq) = "src/kernels.cpp";
        connect<stream>(diff.out[0], sq.in[0]);

        sq2 = kernel::create(square);
        runtime<ratio>(sq2) = 0.1;
        source(sq2) = "src/kernels.cpp";
        connect<stream>(diff.out[1], sq2.in[0]);

        k2 = kernel::create(black);
        runtime<ratio>(k2) = 0.1;
        source(k2) = "src/kernels.cpp";
        connect<stream>(sq.out[0], k2.in[0]);
        connect<stream>(sq2.out[0], k2.in[1]);

        connect<stream>(k2.out[0], out.in[0]);
    }
};