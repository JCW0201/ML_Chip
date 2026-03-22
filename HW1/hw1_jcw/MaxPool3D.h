#ifndef MAXPOOL3D_H
#define MAXPOOL3D_H

#include <systemc.h>
#include "Tensor.h"

SC_MODULE(MaxPool3D)
{
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_fifo_in<Tensor3DPtr> in_fifo;
    sc_fifo_out<Tensor3DPtr> out_fifo;

    int kernel;
    int stride;

    void run();

    SC_HAS_PROCESS(MaxPool3D);
    MaxPool3D(sc_module_name name, int kernel, int stride)
        : sc_module(name), kernel(kernel), stride(stride)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
    }
};

#endif