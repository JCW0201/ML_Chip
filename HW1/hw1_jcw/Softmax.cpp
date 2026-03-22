#include "Softmax.h"
#include <cmath>
#include <algorithm>

void Softmax::run()
{
    while (true)
    {
        wait();

        if (rst.read())
            continue;

        Tensor1DPtr in = in_fifo.read();
        Tensor1DPtr out = std::make_shared<Tensor1D>(in->N);

        double max_val = in->at(0);
        for (int i = 1; i < in->N; i++)
        {
            if (in->at(i) > max_val)
                max_val = in->at(i);
        }

        double sum = 0.0;
        for (int i = 0; i < in->N; i++)
        {
            out->at(i) = std::exp(in->at(i) - max_val);
            sum += out->at(i);
        }

        for (int i = 0; i < in->N; i++)
        {
            out->at(i) /= sum;
        }

        out_fifo.write(out);
    }
}