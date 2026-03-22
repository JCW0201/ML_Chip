#include "ReLU1D.h"

void ReLU1D::run()
{
    while (true)
    {
        wait();

        if (rst.read())
            continue;

        Tensor1DPtr in = in_fifo.read();
        Tensor1DPtr out = std::make_shared<Tensor1D>(in->N);

        for (int i = 0; i < in->N; i++)
        {
            double x = in->at(i);
            out->at(i) = (x > 0.0) ? x : 0.0;
        }

        out_fifo.write(out);
    }
}