#include "Splitter1D.h"

void Splitter1D::run()
{
    while (true)
    {
        wait();

        if (rst.read())
            continue;

        Tensor1DPtr in = in_fifo.read();

        Tensor1DPtr a = std::make_shared<Tensor1D>(in->N);
        Tensor1DPtr b = std::make_shared<Tensor1D>(in->N);

        for (int i = 0; i < in->N; i++)
        {
            a->at(i) = in->at(i);
            b->at(i) = in->at(i);
        }

        out_fifo_a.write(a);
        out_fifo_b.write(b);
    }
}