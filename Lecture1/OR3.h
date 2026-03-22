#ifndef OR3_H
#define OR3_H

#include <systemc.h>

SC_MODULE(OR3)
{
    sc_in<bool> a;
    sc_in<bool> b;
    sc_in<bool> c;
    sc_out<bool> o;

    void do_or3()
    {
        o.write(a.read() || b.read() || c.read());
    }

    SC_CTOR(OR3)
    {
        SC_METHOD(do_or3);
        sensitive << a << b << c;
    }
};

#endif