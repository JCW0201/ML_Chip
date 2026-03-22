#include "ReLU3D.h"

void ReLU3D::run()
{
    while (true)
    {
        wait();

        if (rst.read())
            continue;

        Tensor3DPtr in = in_fifo.read();
        Tensor3DPtr out = std::make_shared<Tensor3D>(in->H, in->W, in->C);

        for (int h = 0; h < in->H; h++)
        {
            for (int w = 0; w < in->W; w++)
            {
                for (int c = 0; c < in->C; c++)
                {
                    double x = in->at(h, w, c);
                    out->at(h, w, c) = (x > 0.0) ? x : 0.0;
                }
            }
        }

        out_fifo.write(out);
    }
}