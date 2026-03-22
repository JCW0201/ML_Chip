#ifndef FC_H
#define FC_H

#include <systemc.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Tensor.h"

SC_MODULE(FC)
{
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_fifo_in<Tensor1DPtr> in_fifo;
    sc_fifo_out<Tensor1DPtr> out_fifo;

    int in_dim;
    int out_dim;

    std::vector<double> weights; // raster: input dim fastest, output dim slowest
    std::vector<double> bias;

    void load_weights(const std::string &weight_file, const std::string &bias_file);
    double weight_at(int o, int i) const;
    void run();

    SC_HAS_PROCESS(FC);
    FC(sc_module_name name,
       int in_dim, int out_dim,
       const std::string &weight_file,
       const std::string &bias_file)
        : sc_module(name), in_dim(in_dim), out_dim(out_dim)
    {
        load_weights(weight_file, bias_file);
        SC_THREAD(run);
        sensitive << clk.pos();
    }
};

#endif