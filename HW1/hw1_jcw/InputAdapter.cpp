#include "InputAdapter.h"

void InputAdapter::run()
{
    if (rst.read())
    {
        return;
    }

    if (in_valid.read())
    {
        Tensor3DPtr out = std::make_shared<Tensor3D>(224, 224, 3);

        int idx = 0;

        // Raster scan order:
        // [1] width -> [2] height -> [3] channel
        // Therefore:
        // for channel
        //   for height
        //     for width
        for (int c = 0; c < 3; c++)
        {
            for (int h = 0; h < 224; h++)
            {
                for (int w = 0; w < 224; w++)
                {
                    out->at(h, w, c) = img[idx++].read();
                }
            }
        }

        out_fifo.write(out);
    }
}