#include "Flatten.h"

void Flatten::run()
{
    while (true)
    {
        wait();

        if (rst.read())
            continue;

        Tensor3DPtr in = in_fifo.read();
        Tensor1DPtr out = std::make_shared<Tensor1D>(in->H * in->W * in->C);

        int idx = 0;

        // Flatten order:
        // width fastest, then height, then channel
        // => c outer, h middle, w inner
        for (int c = 0; c < in->C; c++)
        {
            for (int h = 0; h < in->H; h++)
            {
                for (int w = 0; w < in->W; w++)
                {
                    out->at(idx++) = in->at(h, w, c);
                }
            }
        }

        out_fifo.write(out);
    }
}