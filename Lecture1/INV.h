#ifndef INV_H
#define INV_H

#include <systemc.h>

SC_MODULE(INV)
{
    sc_in<bool> a;
    sc_out<bool> o;

    void do_inv()
    {
        o.write(!a.read());
    }

    SC_CTOR(INV)
    {
        SC_METHOD(do_inv);
        sensitive << a;
    }
};

#endif