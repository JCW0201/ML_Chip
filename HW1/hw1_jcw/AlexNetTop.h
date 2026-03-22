#ifndef ALEXNET_H
#define ALEXNET_H

#include <systemc.h>
#include "Tensor.h"
#include "InputAdapter.h"
#include "ZeroPad.h"
#include "Conv3D.h"
#include "ReLU3D.h"
#include "MaxPool3D.h"
#include "Flatten.h"
#include "FC.h"
#include "ReLU1D.h"
#include "Softmax.h"
#include "Splitter1D.h"

SC_MODULE(AlexNet)
{
    // External interface (must match Pattern)
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in<bool> in_valid;

    sc_vector<sc_in<double>> img{"img", 150528};

    sc_out<bool> out_valid;
    sc_vector<sc_out<double>> output_linear{"output_linear", 1000};
    sc_vector<sc_out<double>> output_softmax{"output_softmax", 1000};

    // Submodules
    InputAdapter *input_adapter;
    ZeroPad *pad0;

    Conv3D *conv1;
    ReLU3D *relu1;
    MaxPool3D *pool1;

    Conv3D *conv2;
    ReLU3D *relu2;
    MaxPool3D *pool2;

    Conv3D *conv3;
    ReLU3D *relu3;

    Conv3D *conv4;
    ReLU3D *relu4;

    Conv3D *conv5;
    ReLU3D *relu5;
    MaxPool3D *pool5;

    Flatten *flatten;

    FC *fc6;
    ReLU1D *relu6;

    FC *fc7;
    ReLU1D *relu7;

    FC *fc8;
    Splitter1D *splitter;
    Softmax *softmax;

    // Internal FIFOs
    sc_fifo<Tensor3DPtr> fifo_input;
    sc_fifo<Tensor3DPtr> fifo_pad0;

    sc_fifo<Tensor3DPtr> fifo_conv1;
    sc_fifo<Tensor3DPtr> fifo_relu1;
    sc_fifo<Tensor3DPtr> fifo_pool1;

    sc_fifo<Tensor3DPtr> fifo_conv2;
    sc_fifo<Tensor3DPtr> fifo_relu2;
    sc_fifo<Tensor3DPtr> fifo_pool2;

    sc_fifo<Tensor3DPtr> fifo_conv3;
    sc_fifo<Tensor3DPtr> fifo_relu3;

    sc_fifo<Tensor3DPtr> fifo_conv4;
    sc_fifo<Tensor3DPtr> fifo_relu4;

    sc_fifo<Tensor3DPtr> fifo_conv5;
    sc_fifo<Tensor3DPtr> fifo_relu5;
    sc_fifo<Tensor3DPtr> fifo_pool5;

    sc_fifo<Tensor1DPtr> fifo_flatten;
    sc_fifo<Tensor1DPtr> fifo_fc6;
    sc_fifo<Tensor1DPtr> fifo_relu6;
    sc_fifo<Tensor1DPtr> fifo_fc7;
    sc_fifo<Tensor1DPtr> fifo_relu7;
    sc_fifo<Tensor1DPtr> fifo_fc8;

    sc_fifo<Tensor1DPtr> fifo_fc8_linear;
    sc_fifo<Tensor1DPtr> fifo_fc8_softmax_in;
    sc_fifo<Tensor1DPtr> fifo_softmax_out;

    void output_stage();

    SC_CTOR(AlexNet)
        : fifo_input(1),
          fifo_pad0(1),
          fifo_conv1(1),
          fifo_relu1(1),
          fifo_pool1(1),
          fifo_conv2(1),
          fifo_relu2(1),
          fifo_pool2(1),
          fifo_conv3(1),
          fifo_relu3(1),
          fifo_conv4(1),
          fifo_relu4(1),
          fifo_conv5(1),
          fifo_relu5(1),
          fifo_pool5(1),
          fifo_flatten(1),
          fifo_fc6(1),
          fifo_relu6(1),
          fifo_fc7(1),
          fifo_relu7(1),
          fifo_fc8(1),
          fifo_fc8_linear(1),
          fifo_fc8_softmax_in(1),
          fifo_softmax_out(1)
    {
        // Instantiate submodules
        input_adapter = new InputAdapter("input_adapter");
        pad0 = new ZeroPad("pad0", 2, 1, 2, 1);

        conv1 = new Conv3D("conv1", 3, 64, 11, 4, 0,
                           "data/conv1_weight.txt", "data/conv1_bias.txt");
        relu1 = new ReLU3D("relu1");
        pool1 = new MaxPool3D("pool1", 3, 2);

        conv2 = new Conv3D("conv2", 64, 192, 5, 1, 2,
                           "data/conv2_weight.txt", "data/conv2_bias.txt");
        relu2 = new ReLU3D("relu2");
        pool2 = new MaxPool3D("pool2", 3, 2);

        conv3 = new Conv3D("conv3", 192, 384, 3, 1, 1,
                           "data/conv3_weight.txt", "data/conv3_bias.txt");
        relu3 = new ReLU3D("relu3");

        conv4 = new Conv3D("conv4", 384, 256, 3, 1, 1,
                           "data/conv4_weight.txt", "data/conv4_bias.txt");
        relu4 = new ReLU3D("relu4");

        conv5 = new Conv3D("conv5", 256, 256, 3, 1, 1,
                           "data/conv5_weight.txt", "data/conv5_bias.txt");
        relu5 = new ReLU3D("relu5");
        pool5 = new MaxPool3D("pool5", 3, 2);

        flatten = new Flatten("flatten");

        fc6 = new FC("fc6", 9216, 4096,
                     "data/fc6_weight.txt", "data/fc6_bias.txt");
        relu6 = new ReLU1D("relu6");

        fc7 = new FC("fc7", 4096, 4096,
                     "data/fc7_weight.txt", "data/fc7_bias.txt");
        relu7 = new ReLU1D("relu7");

        fc8 = new FC("fc8", 4096, 1000,
                     "data/fc8_weight.txt", "data/fc8_bias.txt");
        splitter = new Splitter1D("splitter");
        softmax = new Softmax("softmax");

        // Bind clocks/resets
        input_adapter->clk(clk);
        input_adapter->rst(rst);
        input_adapter->in_valid(in_valid);

        pad0->clk(clk);
        pad0->rst(rst);

        conv1->clk(clk);
        conv1->rst(rst);
        relu1->clk(clk);
        relu1->rst(rst);
        pool1->clk(clk);
        pool1->rst(rst);

        conv2->clk(clk);
        conv2->rst(rst);
        relu2->clk(clk);
        relu2->rst(rst);
        pool2->clk(clk);
        pool2->rst(rst);

        conv3->clk(clk);
        conv3->rst(rst);
        relu3->clk(clk);
        relu3->rst(rst);

        conv4->clk(clk);
        conv4->rst(rst);
        relu4->clk(clk);
        relu4->rst(rst);

        conv5->clk(clk);
        conv5->rst(rst);
        relu5->clk(clk);
        relu5->rst(rst);
        pool5->clk(clk);
        pool5->rst(rst);

        flatten->clk(clk);
        flatten->rst(rst);

        fc6->clk(clk);
        fc6->rst(rst);
        relu6->clk(clk);
        relu6->rst(rst);

        fc7->clk(clk);
        fc7->rst(rst);
        relu7->clk(clk);
        relu7->rst(rst);

        fc8->clk(clk);
        fc8->rst(rst);
        splitter->clk(clk);
        splitter->rst(rst);
        softmax->clk(clk);
        softmax->rst(rst);

        // Bind image ports
        for (int i = 0; i < 150528; i++)
        {
            input_adapter->img[i](img[i]);
        }

        // Bind internal FIFOs
        input_adapter->out_fifo(fifo_input);

        pad0->in_fifo(fifo_input);
        pad0->out_fifo(fifo_pad0);

        conv1->in_fifo(fifo_pad0);
        conv1->out_fifo(fifo_conv1);
        relu1->in_fifo(fifo_conv1);
        relu1->out_fifo(fifo_relu1);
        pool1->in_fifo(fifo_relu1);
        pool1->out_fifo(fifo_pool1);

        conv2->in_fifo(fifo_pool1);
        conv2->out_fifo(fifo_conv2);
        relu2->in_fifo(fifo_conv2);
        relu2->out_fifo(fifo_relu2);
        pool2->in_fifo(fifo_relu2);
        pool2->out_fifo(fifo_pool2);

        conv3->in_fifo(fifo_pool2);
        conv3->out_fifo(fifo_conv3);
        relu3->in_fifo(fifo_conv3);
        relu3->out_fifo(fifo_relu3);

        conv4->in_fifo(fifo_relu3);
        conv4->out_fifo(fifo_conv4);
        relu4->in_fifo(fifo_conv4);
        relu4->out_fifo(fifo_relu4);

        conv5->in_fifo(fifo_relu4);
        conv5->out_fifo(fifo_conv5);
        relu5->in_fifo(fifo_conv5);
        relu5->out_fifo(fifo_relu5);
        pool5->in_fifo(fifo_relu5);
        pool5->out_fifo(fifo_pool5);

        flatten->in_fifo(fifo_pool5);
        flatten->out_fifo(fifo_flatten);

        fc6->in_fifo(fifo_flatten);
        fc6->out_fifo(fifo_fc6);
        relu6->in_fifo(fifo_fc6);
        relu6->out_fifo(fifo_relu6);

        fc7->in_fifo(fifo_relu6);
        fc7->out_fifo(fifo_fc7);
        relu7->in_fifo(fifo_fc7);
        relu7->out_fifo(fifo_relu7);

        fc8->in_fifo(fifo_relu7);
        fc8->out_fifo(fifo_fc8);

        splitter->in_fifo(fifo_fc8);
        splitter->out_fifo_a(fifo_fc8_linear);
        splitter->out_fifo_b(fifo_fc8_softmax_in);

        softmax->in_fifo(fifo_fc8_softmax_in);
        softmax->out_fifo(fifo_softmax_out);

        SC_THREAD(output_stage);
        sensitive << clk.pos();
    }

    ~AlexNet();
};

#endif