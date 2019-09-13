#ifndef TARGET_H
#define TARGET_H

// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc.h>
#include <systemc-ams>
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
#include "registers.h"

// Target module representing a simple memory

struct Target: sc_module
{
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_target_socket<Target> socket;
    sc_out<sc_lv<8> > CR;
    sc_out<sc_lv<32> > CHER;
    sc_out<sc_lv<32> > CHGR1;
    sc_out<sc_lv<32> > CHGR2;
    sc_out<sc_lv<32> > CHGR3;

    SC_CTOR(Target): socket("socket")
    {
        cout << "[Target] Creating module\n";
        // Register callback for incoming b_transport interface method call
        socket.register_b_transport(this, &Target::b_transport);

    }

    // TLM-2 blocking transport method
    virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
    {
        sc_lv<32> data;
        /* get info from generic payload */
        tlm::tlm_command cmd = trans.get_command();
        unsigned int    adr = trans.get_address();
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();
        unsigned char*   byt = trans.get_byte_enable_ptr();
        unsigned int     wid = trans.get_streaming_width();

        if (byt != 0 || len > 4 || wid < len)
           SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction");

        // Obliged to implement read and write commands
        if ( cmd == tlm::TLM_READ_COMMAND )
        {
            unsigned int val;
            cout << "[Target] TLM_READ_COMMAND" << endl;
            switch(adr)
            {
                case PID_CHGR1:
                    val = pid_chgr1.to_uint();
                    memcpy(ptr, &val, len);
                    break;
                case PID_CHGR2:
                    val = pid_chgr2.to_uint();
                    memcpy(ptr, &val, len);
                    break;
                case PID_CHGR3:
                    val = pid_chgr3.to_uint();
                    memcpy(ptr, &val, len);
                    break;
                case PID_CHSR:
                    val = pid_cher.to_uint();
                    memcpy(ptr, &val, len);
                    break;
                default:
                    cout << "[Target] Unknown command" << endl;
            }
            data = val;
        }
        else if ( cmd == tlm::TLM_WRITE_COMMAND )
        {
            cout << "[Target] TLM_WRITE_COMMAND" << endl;
            data = *(reinterpret_cast<unsigned int *>(ptr));
            switch(adr)
            {
                case PID_CR:
                    pid_cr = data.range(7,0);
                    CR.write(pid_cr);
                    break;
                case PID_CHER:
                    pid_cher = data;
                    CHER.write(pid_cher);
                case PID_CHGR1:
                    pid_chgr1 = data;
                    CHGR1.write(pid_chgr1);
                    break;
                case PID_CHGR2:
                    pid_chgr2 = data;
                    CHGR2.write(pid_chgr2);
                    break;
                case PID_CHGR3:
                    pid_chgr3 = data;
                    CHGR3.write(pid_chgr3);
                    break;
                case PID_CHDR:
                    pid_chdr = data;
                    if(true == pid_chdr[0])
                        pid_cher[0] = '0';
                    if(true == pid_chdr[1])
                        pid_cher[1] = '0';
                    if(true == pid_chdr[2])
                        pid_cher[2] = '0';
                    CHER.write(pid_cher);
                    break;
                default:
                    cout << "[Target] Unknown command\n";
                    break;
            }
        }

        printf("[Target] Register: %#x \n",adr);
        cout << "[Target] Data: " << data << endl;


        // Set response status to indicate successful completion
        trans.set_response_status( tlm::TLM_OK_RESPONSE );
    }
    private:
    sc_lv<8> pid_cr;
    sc_lv<32> pid_chgr1;
    sc_lv<32> pid_chgr2;
    sc_lv<32> pid_chgr3;
    sc_lv<32> pid_cher;
    sc_lv<32> pid_chdr;

};


#endif
