#include "AlexNetTop.h"

AlexNet::~AlexNet()
{
    delete input_adapter;
    delete pad0;

    delete conv1;
    delete relu1;
    delete pool1;

    delete conv2;
    delete relu2;
    delete pool2;

    delete conv3;
    delete relu3;

    delete conv4;
    delete relu4;

    delete conv5;
    delete relu5;
    delete pool5;

    delete flatten;

    delete fc6;
    delete relu6;

    delete fc7;
    delete relu7;

    delete fc8;
    delete splitter;
    delete softmax;
}

void AlexNet::output_stage()
{
    out_valid.write(false);

    for (int i = 0; i < 1000; i++)
    {
        output_linear[i].write(0.0);
        output_softmax[i].write(0.0);
    }

    while (true)
    {
        wait();

        if (rst.read())
        {
            out_valid.write(false);
            for (int i = 0; i < 1000; i++)
            {
                output_linear[i].write(0.0);
                output_softmax[i].write(0.0);
            }
            continue;
        }

        if (fifo_fc8_linear.num_available() > 0 &&
            fifo_softmax_out.num_available() > 0)
        {

            Tensor1DPtr linear = fifo_fc8_linear.read();
            Tensor1DPtr prob = fifo_softmax_out.read();

            for (int i = 0; i < 1000; i++)
            {
                output_linear[i].write(linear->at(i));
                output_softmax[i].write(prob->at(i));
            }

            out_valid.write(true);
        }
        else
        {
            out_valid.write(false);
        }
    }
}