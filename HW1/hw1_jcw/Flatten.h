#ifndef FLATTEN_H
#define FLATTEN_H

#include <systemc.h>
#include "Tensor.h"

SC_MODULE(Flatten)
{
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_fifo_in<Tensor3DPtr> in_fifo;
    sc_fifo_out<Tensor1DPtr> out_fifo;

    void run();

    SC_CTOR(Flatten)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
    }
};

#endif