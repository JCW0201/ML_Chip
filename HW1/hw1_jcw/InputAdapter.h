#ifndef INPUT_ADAPTER_H
#define INPUT_ADAPTER_H

#include <systemc.h>
#include "Tensor.h"

SC_MODULE(InputAdapter)
{
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in<bool> in_valid;

    sc_vector<sc_in<double>> img{"img", 150528};

    sc_fifo_out<Tensor3DPtr> out_fifo;

    void run();

    SC_CTOR(InputAdapter)
    {
        SC_METHOD(run);
        sensitive << clk.pos();
    }
};

#endif