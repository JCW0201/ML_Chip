#ifndef ZERO_PAD_H
#define ZERO_PAD_H

#include <systemc.h>
#include "Tensor.h"

// [Module] ZeroPad
// Applies zero padding to an input 3D tensor.
// The module preserves the original spatial values and shifts them
// to the padded location in the output tensor.
//
// Input  : Tensor3DPtr with shape (H, W, C)
// Output : Tensor3DPtr with shape
//          (H + pad_top + pad_bottom, W + pad_left + pad_right, C)
//
// This module is used before Conv1 to convert 224x224x3 into 227x227x3
// with asymmetric padding required by the assignment.

SC_MODULE(ZeroPad)
{
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_fifo_in<Tensor3DPtr> in_fifo;
    sc_fifo_out<Tensor3DPtr> out_fifo;

    // Padding configuration.
    // These values define how many zero rows/columns
    // are added to the feature map
    int pad_top, pad_bottom, pad_left, pad_right;

    void run();

    SC_HAS_PROCESS(ZeroPad);
    ZeroPad(sc_module_name name, int pt, int pb, int pl, int pr)
        : sc_module(name),
          pad_top(pt), pad_bottom(pb), pad_left(pl), pad_right(pr)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
    }
};

#endif