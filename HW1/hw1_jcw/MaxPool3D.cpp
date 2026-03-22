#include "MaxPool3D.h"
#include <limits>

void MaxPool3D::run()
{
    while (true)
    {
        wait();

        if (rst.read())
            continue;

        Tensor3DPtr in = in_fifo.read();

        int out_h = (in->H - kernel) / stride + 1;
        int out_w = (in->W - kernel) / stride + 1;

        Tensor3DPtr out = std::make_shared<Tensor3D>(out_h, out_w, in->C);

        for (int oh = 0; oh < out_h; oh++)
        {
            for (int ow = 0; ow < out_w; ow++)
            {
                for (int c = 0; c < in->C; c++)
                {
                    double mx = -std::numeric_limits<double>::infinity();

                    for (int kh = 0; kh < kernel; kh++)
                    {
                        for (int kw = 0; kw < kernel; kw++)
                        {
                            int ih = oh * stride + kh;
                            int iw = ow * stride + kw;
                            double val = in->at(ih, iw, c);
                            if (val > mx)
                                mx = val;
                        }
                    }

                    out->at(oh, ow, c) = mx;
                }
            }
        }

        out_fifo.write(out);
    }
}