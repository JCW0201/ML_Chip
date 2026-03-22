#ifndef AND3_H
#define AND3_H

#include <systemc.h>

SC_MODULE(AND3)
{
    sc_in<bool> a;
    sc_in<bool> b;
    sc_in<bool> c;
    sc_out<bool> o;

    void do_and3()
    {
        o.write(a.read() && b.read() && c.read());
    }

    SC_CTOR(AND3)
    {
        SC_METHOD(do_and3);
        sensitive << a << b << c;
    }
};

#endif