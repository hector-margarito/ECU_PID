// pwm.h
#ifndef PWM_H
#define PWM_H

#include <cmath>
#include <systemc-ams>
SCA_TDF_MODULE(pwm)
{
    sca_tdf::sca_in<double> in;
    sca_tdf::sca_out<double> out;

    pwm(sc_core::sc_module_name nm, double v0_ = 0.0, double v1_ = 1.0,
        const sca_core::sca_time& t_period_ = sca_core::sca_time(5.0, sc_core::SC_MS),
        const sca_core::sca_time& t_ramp_ = sca_core::sca_time(0.05, sc_core::SC_MS),
        const sca_core::sca_time& t_step_ = sca_core::sca_time(0.01, sc_core::SC_MS) )
        : in("in"), out("out"), v0(v0_), v1(v1_), t_period( t_period_.to_seconds() ),
        t_ramp( t_ramp_.to_seconds() ), t_duty_max( t_period - 2.0 * t_ramp ),
        t_duty( t_duty_max ), t_step( t_step_ ) {}
    
    void set_attributes()
    {
        // fixed time step for module activation
        set_timestep( t_step ); 
    }

    void processing()
    {
        // current time
        double t = get_time().to_seconds();
        // time position inside pulse period
        double t_pos = std::fmod( t, t_period );
        // initial value
        double val = v0; 

        if (t_pos < t_ramp) 
        {   
            // calculate and clamp duty time
            t_duty = in.read() * t_duty_max;
            if ( t_duty < 0.0 ) t_duty = 0.0;
            if ( t_duty > t_duty_max ) t_duty = t_duty_max;
        }
        // rising edge
        if ( t_pos < t_ramp )
            val = ( (v1 - v0) / t_ramp ) * t_pos + v0;
        // plateau
        else if ( t_pos < t_ramp + t_duty )
            val = v1;
        // falling edge
        else if ( t_pos < t_ramp + t_duty + t_ramp ) 
            val = ( (v0 - v1) / t_ramp ) * ( t_pos - t_ramp - t_duty ) + v1;
        // else return to initial value
        
        out.write(val);
    }

    private:
    double v0, v1;              // initial and plateau values
    double t_period, t_ramp;    // pulse period and ramp time
    double t_duty_max;          // maximum duty time
    double t_duty;              // current duty time
    sca_core::sca_time t_step;  // module time step
};


#endif
