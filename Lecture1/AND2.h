#ifndef AND2_H
#define AND2_H

#include <systemc.h>

SC_MODULE(AND2)
{
    sc_in<bool> a;
    sc_in<bool> b;
    sc_out<bool> o;

    void do_and2()
    {
        o.write(a.read() && b.read());
    }

    SC_CTOR(AND2)
    {
        SC_METHOD(do_and2);
        sensitive << a << b;
    }
};

#endif