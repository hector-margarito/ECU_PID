#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include <systemc.h>
#include <systemc-ams>
#include <cmath>

const double h0 = 15.0;
const double w0 = 20*M_PI;
SCA_TDF_MODULE(dc_motor)
{
    sca_tdf::sca_in<double> in;
    sca_tdf::sca_out<double> out;

    /* to compute transfer function */
    sca_tdf::sca_ltf_nd ltf;
    /* coeficients */
    sca_util::sca_vector<double> num, den;
    sca_util::sca_vector<double> s;

    void set_attributes()
    {
        /* time between activations */
        set_timestep(0.01, SC_MS);
        cout << "[dc_motor] set_attributes()" << endl;
    }
    void initialize()
    {
        num(0) = h0;
        den(0) = 1.0;
        den(1) = 1.0/w0;
        cout << "[dc_motor]initialize() " << endl;
    }

    void processing()
    {
        out.write( ltf(num,den,s,in));
    }

    SCA_CTOR(dc_motor) {}

};
#endif
