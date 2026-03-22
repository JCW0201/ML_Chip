#include <systemc.h>
#include "AND2.h"
#include "AND3.h"
#include "OR2.h"
#include "OR3.h"
#include "NOR2.h"
#include "INV.h"

int sc_main(int argc, char *argv[])
{
    // Step 1: instantiate the gate objects
    OR2  or1("or1"), or8("or8");
    OR3  or2("or2");
    AND2 and3("and3"), and4("and4"), and5("and5");
    AND3 and6("and6");
    NOR2 nor7("nor7");
    INV  inv9("inv9");

    // Step 2: instantiate the signal objects
    sc_signal<bool> A, B, CI;                // input nets
    sc_signal<bool> CO, SUM;                 // output nets
    sc_signal<bool> or_1, or_2, and_3, and_4;
    sc_signal<bool> and_5, and_6, nor_7;

    // Step 3: connect the gates to the signal nets
    or1.a(A);      or1.b(B);      or1.o(or_1);
    or2.a(A);      or2.b(B);      or2.c(CI);    or2.o(or_2);
    and3.a(or_1);  and3.b(CI);    and3.o(and_3);
    and4.a(A);     and4.b(B);     and4.o(and_4);
    and5.a(nor_7); and5.b(or_2);  and5.o(and_5);
    and6.a(A);     and6.b(B);     and6.c(CI);   and6.o(and_6);
    nor7.a(and_3); nor7.b(and_4); nor7.o(nor_7);
    or8.a(and_5);  or8.b(and_6);  or8.o(SUM);
    inv9.a(nor_7); inv9.o(CO);

    // waveform
    sc_trace_file *tf = sc_create_vcd_trace_file("trace");// 產生 trace.vcd 波形檔，可以用 gtkwave 開來看
    sc_trace(tf, A,   "A");
    sc_trace(tf, B,   "B");
    sc_trace(tf, CI,  "CI");
    sc_trace(tf, SUM, "SUM");
    sc_trace(tf, CO,  "CO");

    // input stimulus
    A = 0; B = 0; CI = 0;
    sc_start(10, SC_NS);

    for (int i = 0; i < 8; i++)
    {
        A  = ((i & 0x1) != 0); // i 的 bit0 決定 A
        B  = ((i & 0x2) != 0); 
        CI = ((i & 0x4) != 0);
        sc_start(10, SC_NS);// 等 10ns 再改下一組輸入，讓波形圖看起來比較清楚
    }

    sc_close_vcd_trace_file(tf);
    return 0;
}