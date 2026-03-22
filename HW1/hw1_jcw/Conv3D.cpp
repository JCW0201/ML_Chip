#include "Conv3D.h"
#include <cmath>

void Conv3D::load_weights(const std::string &weight_file, const std::string &bias_file)
{
    int total = out_ch * in_ch * kernel * kernel;
    weights.resize(total);
    bias.resize(out_ch);

    std::ifstream wf(weight_file);
    std::ifstream bf(bias_file);

    if (!wf.is_open())
    {
        std::cerr << "Weight open fail\n";
        exit(1);
    }

    if (!bf.is_open())
    {
        std::cerr << "Bias open fail\n";
        exit(1);
    }

    // 直接照檔案順序讀
    for (int i = 0; i < total; i++)
    {
        wf >> weights[i];
    }

    for (int i = 0; i < out_ch; i++)
    {
        bf >> bias[i];
    }
}

double Conv3D::weight_at(int oc, int kh, int kw, int ic) const
{
    int idx =
        (((oc * in_ch + ic) * kernel + kh) * kernel + kw);
    return weights[idx];
}

void Conv3D::run()
{
    while (true)
    {
        wait();

        if (rst.read())
            continue;

        Tensor3DPtr in = in_fifo.read();

        int in_h = in->H;
        int in_w = in->W;

        int out_h = (in_h + 2 * padding - kernel) / stride + 1;
        int out_w = (in_w + 2 * padding - kernel) / stride + 1;

        Tensor3DPtr out = std::make_shared<Tensor3D>(out_h, out_w, out_ch);

        for (int oh = 0; oh < out_h; oh++)
        {
            for (int ow = 0; ow < out_w; ow++)
            {
                for (int oc = 0; oc < out_ch; oc++)
                {
                    double sum = bias[oc];

                    for (int kh = 0; kh < kernel; kh++)
                    {
                        for (int kw = 0; kw < kernel; kw++)
                        {
                            for (int ic = 0; ic < in_ch; ic++)
                            {
                                int ih = oh * stride + kh - padding;
                                int iw = ow * stride + kw - padding;

                                if (ih >= 0 && ih < in_h && iw >= 0 && iw < in_w)
                                {
                                    sum += in->at(ih, iw, ic) * weight_at(oc, kh, kw, ic);
                                }
                            }
                        }
                    }

                    out->at(oh, ow, oc) = sum;
                }
            }
        }

        out_fifo.write(out);
    }
}