#ifndef NOR2_H
#define NOR2_H

#include <systemc.h>

SC_MODULE(NOR2)
{
    sc_in<bool> a;
    sc_in<bool> b;
    sc_out<bool> o;

    void do_nor2()
    {
        o.write(!(a.read() || b.read()));
    }

    SC_CTOR(NOR2)
    {
        SC_METHOD(do_nor2);
        sensitive << a << b;
    }
};

#endif