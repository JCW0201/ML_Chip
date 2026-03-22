#include <systemc.h>
#include <string>
#include "Pattern.h"
#include "AlexNetTop.h"

using namespace std;

int sc_main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: ./run <cat.txt|dog.txt>" << endl;
        return 1;
    }

    string img_name = argv[1];

    sc_clock clk("clk", 1, SC_NS);

    sc_signal<bool> rst;
    sc_signal<bool> in_valid;
    sc_signal<bool> out_valid;

    sc_vector<sc_signal<double>> img_sig("img_sig", 150528);
    sc_vector<sc_signal<double>> output_linear_sig("output_linear_sig", 1000);
    sc_vector<sc_signal<double>> output_softmax_sig("output_softmax_sig", 1000);

    Pattern pattern("pattern", img_name);
    AlexNet dut("dut");

    // Bind Pattern
    pattern.clock(clk);
    pattern.rst(rst);
    pattern.in_valid(in_valid);
    pattern.out_valid(out_valid);

    // Bind DUT
    dut.clk(clk);
    dut.rst(rst);
    dut.in_valid(in_valid);
    dut.out_valid(out_valid);

    for (int i = 0; i < 150528; i++)
    {
        pattern.img[i](img_sig[i]);
        dut.img[i](img_sig[i]);
    }

    for (int i = 0; i < 1000; i++)
    {
        pattern.output_linear[i](output_linear_sig[i]);
        pattern.output_softmax[i](output_softmax_sig[i]);

        dut.output_linear[i](output_linear_sig[i]);
        dut.output_softmax[i](output_softmax_sig[i]);
    }

    sc_start();
    return 0;
}