#ifndef SPLITTER1D_H
#define SPLITTER1D_H

#include <systemc.h>
#include "Tensor.h"

SC_MODULE(Splitter1D)
{
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_fifo_in<Tensor1DPtr> in_fifo;
    sc_fifo_out<Tensor1DPtr> out_fifo_a;
    sc_fifo_out<Tensor1DPtr> out_fifo_b;

    void run();

    SC_CTOR(Splitter1D)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
    }
};

#endif