#ifndef CONV3D_H
#define CONV3D_H

#include <systemc.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Tensor.h"

SC_MODULE(Conv3D)
{
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_fifo_in<Tensor3DPtr> in_fifo;
    sc_fifo_out<Tensor3DPtr> out_fifo;

    int in_ch;
    int out_ch;
    int kernel;
    int stride;
    int padding;

    std::vector<double> weights; // [out_ch][kernel][kernel][in_ch]
    std::vector<double> bias;    // [out_ch]

    void load_weights(const std::string &weight_file, const std::string &bias_file);
    double weight_at(int oc, int kh, int kw, int ic) const;
    void run();

    SC_HAS_PROCESS(Conv3D);
    Conv3D(sc_module_name name,
           int in_ch, int out_ch, int kernel, int stride, int padding,
           const std::string &weight_file,
           const std::string &bias_file)
        : sc_module(name),
          in_ch(in_ch), out_ch(out_ch), kernel(kernel), stride(stride), padding(padding)
    {
        load_weights(weight_file, bias_file);
        SC_THREAD(run);
        sensitive << clk.pos();
    }
};

#endif