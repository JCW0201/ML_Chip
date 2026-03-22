#ifndef RELU3D_H
#define RELU3D_H

#include <systemc.h>
#include "Tensor.h"

SC_MODULE(ReLU3D)
{
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_fifo_in<Tensor3DPtr> in_fifo;
    sc_fifo_out<Tensor3DPtr> out_fifo;

    void run();

    SC_CTOR(ReLU3D)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
    }
};

#endif