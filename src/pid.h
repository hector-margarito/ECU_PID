#ifndef PID_H
#define PID_H


#include <systemc.h>
#include <systemc-ams>
#include <cmath>

#if 0
const double Kp = (1/15);
const double Ki = M_PI * 4;
const double Kd = 1;
#endif
SCA_TDF_MODULE(pid_control)
{
    /* inputs and outputs declaration */
    sca_tdf::sca_in<double> imeas;
    sca_tdf::sca_out<double> out;
    sca_tdf::sc_in<sc_lv<32> > CHER;
    sca_tdf::sc_in<sc_lv<32> > CHGR1;
    sca_tdf::sc_in<sc_lv<32> > CHGR2;
    sca_tdf::sc_in<sc_lv<32> > CHGR3;
    sca_tdf::sc_in<sc_lv<8> > CR;

    /* to compute transfer function */
    sca_tdf::sca_ltf_nd ltf_i, ltf_d;
    /* coeficients */
    sca_util::sca_vector<double> num_i, num_d, den_i, den_d;
    sca_util::sca_vector<double> s_int, s_der;
    double iref;

    void set_attributes()
    {
        /* time between activations */
        set_timestep(0.01, SC_MS);
        out.set_delay(1);
        cout << "[pid_control] set_attributes() " << endl;
    }
    
    void initialize()
    {
        iref = 10.0;
        cout << "[pid_control] initialize() " << endl;

        /* initial values */
        Kp = (1/15);
        Ki = M_PI * 4;
        Kd = (1/4) * M_PI;
        cher[0] = '1';
        cher[1] = '1';
        cher[2] = '1';

    }

    void processing()
    {
        double i_err;
        double s_p, s_i, s_d, s_out;

        // copy signal values to local variable
        cr = CR;
        chgr1 = CHGR1;
        chgr2 = CHGR2;
        chgr3 = CHGR3;
        /* Set all Coefficients to 0 */
        if(true == cr[0])
        {
            Kp = 0;
            Ki = 0;
            Kd = 0;
        }else if(true == cr[1])
        {
            Kp = chgr1.to_uint();
            Ki = chgr2.to_uint();
            Kd = chgr3.to_uint();

            cher = CHER;
        }

        /* get signal error */
        i_err = iref - imeas;

        /* filling equations */
#if 0
        num_i(0) = Ki;
        den_i(0) = 1.0;
        num_d(0) = Kd;
        den_d(0) = 1.0;

        /* solve */
        s_p = Kp * i_err;
        s_i = ltf_i(num_i,den_i,s_int,i_err);
        s_d = ltf_d(num_d,den_d,s_der,std::pow(i_err,2));
#endif
        num_i(0) = Ki;
        den_i(0) = 0.0;
        den_i(1) = 1.0;
        num_d(0) = 0.0;
        num_d(1) = Kd;
        den_d(0) = 1.0;

        s_p = Kp*i_err;
        s_i = ltf_i(num_i,den_i,s_int,i_err);
        s_d = ltf_d(num_d,den_d,s_der,i_err);

        if((true == cher[0]) && (true == cher[1]) && (true == cher[2]))
        {
            s_out = s_p + s_i + s_d;
        }else if((true == cher[0]) && (true == cher[1]) && (false == cher[2]))
        {
            s_out = s_p + s_i;
        }else if((true == cher[0]) && (false == cher[1]) && (true == cher[2]))
        {
            s_out = s_p + s_d;
        }else if((true == cher[0]) && (false == cher[1]) && (false == cher[2]))
        {
            s_out = s_p;
        }else if((false == cher[0]) && (true == cher[1]) && (true == cher[2]))
        {
            s_out = s_i + s_d;
        }else if((false == cher[0]) && (true == cher[1]) && (false == cher[2]))
        {
            s_out = s_i;
        }else if((true == cher[0]) && (false == cher[1]) && (true == cher[2]))
        {
            s_out = s_d;
        }else if((true == cher[0]) && (false == cher[1]) && (false == cher[2]))
        {
            s_out = 0;
        }

        /* add signals and write */
        out.write(s_out);
    }

    SCA_CTOR(pid_control) {}
private:
    sc_lv<8> cr;
    sc_lv<32> chgr1, chgr2, chgr3;
    sc_lv<32> cher;
    double Kp, Ki, Kd;
};

#endif
