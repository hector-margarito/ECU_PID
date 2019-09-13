#ifndef TLM_INITIATOR_H
#define TLM_INITIATOR_H

//#define SC_INCLUDE_DYNAMIC_PROCESSES // is it necessary?
#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

struct Initiator: sc_module, tlm::tlm_bw_transport_if<>
{
    tlm::tlm_initiator_socket<> init_socket;

    SC_CTOR(Initiator): init_socket("init_socket"){
        SC_THREAD(thread);
        init_socket.bind(*this);
    }
    void thread(){
        tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload;
        sc_core::sc_time delay = SC_ZERO_TIME;

        init_socket->b_transport(*trans,delay);
    }
};
#endif
