#ifndef OR2_H
#define OR2_H

#include <systemc.h>

SC_MODULE(OR2)
{
    sc_in<bool> a;
    sc_in<bool> b;
    sc_out<bool> o;

    void do_or2()
    {
        o.write(a.read() || b.read());
    }

    SC_CTOR(OR2)
    {
        SC_METHOD(do_or2);
        sensitive << a << b;
    }
};

#endif