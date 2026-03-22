#include "ZeroPad.h"

void ZeroPad::run()
{
    while (true)
    {
        wait();

        // Ignore all transactions during reset.
        if (rst.read())
            continue;

        // Read one input tensor from the upstream FIFO.
        Tensor3DPtr in = in_fifo.read();

        // Allocate the output tensor with padded spatial dimensions.
        // Channel count is preserved.
        Tensor3DPtr out = std::make_shared<Tensor3D>(
            in->H + pad_top + pad_bottom,
            in->W + pad_left + pad_right,
            in->C);

        // Copy the original tensor into the shifted region of the output tensor.
        // All other locations remain zero due to Tensor3D initialization.
        for (int h = 0; h < in->H; h++)
        {
            for (int w = 0; w < in->W; w++)
            {
                for (int c = 0; c < in->C; c++)
                {
                    out->at(h + pad_top, w + pad_left, c) = in->at(h, w, c);
                }
            }
        }

        out_fifo.write(out);
    }
}