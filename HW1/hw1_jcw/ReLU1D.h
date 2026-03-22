#ifndef RELU1D_H
#define RELU1D_H

#include <systemc.h>
#include "Tensor.h"

SC_MODULE(ReLU1D)
{
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_fifo_in<Tensor1DPtr> in_fifo;
    sc_fifo_out<Tensor1DPtr> out_fifo;

    void run();

    SC_CTOR(ReLU1D)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
    }
};

#endif