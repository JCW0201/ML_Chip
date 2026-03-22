#include "FC.h"

void FC::load_weights(const std::string &weight_file, const std::string &bias_file)
{
    weights.resize(out_dim * in_dim);
    bias.resize(out_dim);

    std::ifstream wf(weight_file.c_str());
    std::ifstream bf(bias_file.c_str());

    if (!wf.is_open())
    {
        std::cerr << "Cannot open FC weight file: " << weight_file << std::endl;
        exit(1);
    }
    if (!bf.is_open())
    {
        std::cerr << "Cannot open FC bias file: " << bias_file << std::endl;
        exit(1);
    }

    for (int i = 0; i < out_dim * in_dim; i++)
    {
        wf >> weights[i];
    }

    for (int i = 0; i < out_dim; i++)
    {
        bf >> bias[i];
    }
}

double FC::weight_at(int o, int i) const
{
    // raster assumption for 2D weight matrix:
    // input dim fastest, output dim slowest
    return weights[o * in_dim + i];
}

void FC::run()
{
    while (true)
    {
        wait();

        if (rst.read())
            continue;

        Tensor1DPtr in = in_fifo.read();
        Tensor1DPtr out = std::make_shared<Tensor1D>(out_dim);

        for (int o = 0; o < out_dim; o++)
        {
            double sum = bias[o];
            for (int i = 0; i < in_dim; i++)
            {
                sum += in->at(i) * weight_at(o, i);
            }
            out->at(o) = sum;
        }

        out_fifo.write(out);
    }
}