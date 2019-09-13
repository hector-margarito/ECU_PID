#include "dc_system.h"
#include "application.h"
#include <systemc.h>
#include <systemc-ams>
#include <bitset>

int sc_main(int argc, char* argv[]){
    ECU *ecu = new ECU((char *)"ecu");
    DCsystem *device = new DCsystem("device");
    int result;
    
    ecu->initiator.socket.bind(device->target->socket);
    
    /* SIGNALS FOR VDC TRACE FILE */
    sca_tdf::sca_signal<double> imeas;
    sca_tdf::sca_signal<double> ct;
    sca_tdf::sca_signal<double> vdrv;
    device->imeas(imeas);
    device->ctrl(ct);
    device->vdrv(vdrv);

    cout << "@" << sc_time_stamp() << " Creating VCD file" << endl;
    sca_util::sca_trace_file* wf = sca_util::sca_create_vcd_trace_file("DC_control_trace");
    cout << "@" << sc_time_stamp() << " Adding the desired signals" << endl;

    /* Add ports to vcd traces */
    sca_trace(wf, imeas, "imeas");
    sca_trace(wf, vdrv, "vdrv");
    sca_trace(wf, ct, "c(t)");

    cout << "Starting simulation" << endl;
    sc_start(500,SC_MS);
    cout << "@" << sc_time_stamp() << " Perform Software Reset" << endl;
    ecu->PID_configuration(PID_CR,1);
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Enable Kp, Ki, Kd, " << endl;
    ecu->PID_configuration(PID_CHER,7);
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Write Kp channel gain of 1" << endl;
    ecu->PID_configuration(PID_CHGR1,1);
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Write Ki channel gain of 2" << endl;
    ecu->PID_configuration(PID_CHGR2,2);
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Write Kd channel gain of 3 " << endl;
    ecu->PID_configuration(PID_CHGR3,3);
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Start configuration" << endl;
    ecu->PID_configuration(PID_CR,2);
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Stop configuration" << endl;
    ecu->PID_configuration(PID_CR,0);
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Read Kp channel gain" << endl;
    result = ecu->PID_GetCoefficients(PID_CHGR1,1);
    cout << "@" << sc_time_stamp() << " Kp channel gain: " << result << endl;
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Read Ki channel gain" << endl;
    result = ecu->PID_GetCoefficients(PID_CHGR2,2);
    cout << "@" << sc_time_stamp() << " Ki channel gain: " << result << endl;
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Read Kd channel gain" << endl;
    result = ecu->PID_GetCoefficients(PID_CHGR3,3);
    cout << "@" << sc_time_stamp() << " Kd channel gain: " << result << endl;
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Read current enabled coefficients" << endl;
    result = ecu->PID_GetStatus();
    {
    std::bitset<32> bits((unsigned int)result);
    cout << "@" << sc_time_stamp() << " Kp: ";
    if(bits.test(0)) cout << "enabled\n"; else cout << "disabled\n"; 
    cout << "@" << sc_time_stamp() << " Ki: ";
    if(bits.test(1)) cout << "enabled\n"; else cout << "disabled\n"; 
    cout << "@" << sc_time_stamp() << " Kd: ";
    if(bits.test(2)) cout << "enabled\n"; else cout << "disabled\n"; 
    }
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Disable Ki and Kd" << endl;
    ecu->PID_configuration(PID_CHDR,6);
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Start configuration" << endl;
    ecu->PID_configuration(PID_CR,1);
    sc_start(100,SC_MS);
    cout << "@" << sc_time_stamp() << " Read current enabled coefficients" << endl;
    result = ecu->PID_GetStatus();
    {
    std::bitset<32> bits((unsigned int)result);
    cout << "@" << sc_time_stamp() << " Kp: ";
    if(bits.test(0)) cout << "enabled\n"; else cout << "disabled\n"; 
    cout << "@" << sc_time_stamp() << " Ki: ";
    if(bits.test(1)) cout << "enabled\n"; else cout << "disabled\n"; 
    cout << "@" << sc_time_stamp() << " Kd: ";
    if(bits.test(2)) cout << "enabled\n"; else cout << "disabled\n"; 
    }
    sc_start(100,SC_MS);

    cout << "@" << sc_time_stamp() << " END of test" << endl;

    return 0;
}

