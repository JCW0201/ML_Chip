#ifndef SOFTMAX_H
#define SOFTMAX_H

#include <systemc.h>
#include "Tensor.h"

SC_MODULE(Softmax)
{
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_fifo_in<Tensor1DPtr> in_fifo;
    sc_fifo_out<Tensor1DPtr> out_fifo;

    void run();

    SC_CTOR(Softmax)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
    }
};

#endif