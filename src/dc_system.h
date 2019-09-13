#ifndef DC_SYSTEM_H
#define DC_SYSTEM_H

#include "pid.h"
#include "pwm.h"
#include "dc_motor.h"
#include "target.h"

SC_MODULE(DCsystem){
    sca_tdf::sca_out<double> imeas,vdrv,ctrl;

    pid_control* pid_i;
    pwm* pwm_i;
    dc_motor* motor_i;
    Target* target;

    //sc_core::sc_signal <double> Kp,Ki,Kd;
    sc_core::sc_signal <sc_lv<8> > cr;
    sc_core::sc_signal <sc_lv<32> > cher;
    sc_core::sc_signal <sc_lv<32> > chgr1;
    sc_core::sc_signal <sc_lv<32> > chgr2;
    sc_core::sc_signal <sc_lv<32> > chgr3;

    SC_CTOR(DCsystem){
        target = new Target("target");
        target->CHGR1(chgr1);
        target->CHGR2(chgr2);
        target->CHGR3(chgr3);
        target->CR(cr);
        target->CHER(cher);

        pid_i = new pid_control("pid_i");
        pid_i->imeas(imeas);
        pid_i->out(ctrl);
        pid_i->CHER(cher);
        pid_i->CHGR1(chgr1);
        pid_i->CHGR2(chgr2);
        pid_i->CHGR3(chgr3);
        pid_i->CR(cr);

        pwm_i  = new pwm("pwm_i");
        pwm_i->in(ctrl);
        pwm_i->out(vdrv);

        motor_i = new dc_motor("motor_i");
        motor_i->in(vdrv);
        motor_i->out(imeas);
    }
};

#endif
