#ifndef INITIATOR_H
#define INITIATOR_H

using namespace tlm;
using namespace std;

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include "registers.h"

struct Initiator: sc_module
{
    tlm_utils::simple_initiator_socket<Initiator> socket;

    SC_CTOR(Initiator): socket("socket"){ }

    int execute(unsigned int addr, unsigned int data, bool rw)
    {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            sc_time delay = SC_ZERO_TIME;
            tlm::tlm_command cmd;

            if(rw) cmd = TLM_WRITE_COMMAND;
            else cmd = TLM_READ_COMMAND;

            tmp = data;
            printf("[Initiator] Address: %#x \n", addr);
            cout << "[Initiator] Data: " << tmp << endl;

            trans->set_command( cmd );
            trans->set_address(addr);
            trans->set_data_ptr( reinterpret_cast<unsigned char*>(&tmp) );
            trans->set_data_length( 4 );
            trans->set_streaming_width( 4 ); // = data_length to indicate no streaming
            trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
            trans->set_dmi_allowed( false ); // Mandatory initial value
            trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

            // Blocking call
            socket->b_transport(*trans, delay);

            /* check error response */
            if ( trans->is_response_error() )
                SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
            else
                cout << "[Initiator] block call finished\n";

            // call wait if delay was specified by the target

            return tmp;
    }
    private:
    unsigned int tmp;
};

#endif
