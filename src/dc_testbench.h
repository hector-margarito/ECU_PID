#ifndef DC_TESTBENCH_H
#define DC_TESTBENCH_H

#include <systemc.h>
#include "initiator.h"
#include "registers.h"

#if 0

    TODO!!
    Make this file pure C++, no SYSTEMC and implement functions
    such as PID_configuration, PID_GetStatus, PID_GetCoefficients
    the file target.h should do the translation to TLM

    tip move the wave file creation to main.cpp, there you will 
    wait to execute next function




#endif
SC_MODULE(DC_tb)
{
    sc_in<bool> clk;
    sca_tdf::sca_in<double> imeas,vdrv,ctrl;
    
    Initiator *initiator;
    sc_signal<sc_lv<32> > data;
    //sc_signal<int> data;
    sc_signal<int> oper;

    sc_lv<32>  data2;

    SC_CTOR(DC_tb)
    {
        cout << "Creating DC_tb object" << endl;
        SC_THREAD(do_tb);
        sensitive << clk.pos();

        initiator = new Initiator("initiator");
        initiator->data(data);
        initiator->oper(oper);
    }

    void do_tb()
    {
        cout << "@" << sc_time_stamp() << " Creating VCD file" << endl;
        sca_util::sca_trace_file* wf = sca_util::sca_create_vcd_trace_file("DC_control_trace");
        cout << "@" << sc_time_stamp() << " Adding the desired signals" << endl;
        /* Add ports to vcd traces */
        sca_trace(wf, imeas, "imeas");
        sca_trace(wf, vdrv, "vdrv");
        sca_trace(wf, ctrl, "c(t)");

        wait(20,SC_MS);

        // Reset and start config
        cout << "@" << sc_time_stamp() << " First operation start" << endl;
        data.write("00000000000000000000000000000011");
        oper.write(PID_CR);
        cout << "@" << sc_time_stamp() << " First operation end" << endl;

        wait(20,SC_MS);
        cout << "@" << sc_time_stamp() << " Second operation start" << endl;


    }

};

#endif
