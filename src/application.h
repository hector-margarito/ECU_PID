#ifndef APPLICATION_H
#define APPLICATION_H

#include "initiator.h"
#include <string.h>

class ECU
{
public:
    Initiator initiator;
    char* ptr_name;

    ECU(char* name): initiator("initiator")
    {
        ptr_name = new char[25];
        strcpy(ptr_name,name);
        ptr_name[24] = 0;
    }
    ~ECU()
    {
        delete[] ptr_name;
    }

    void PID_configuration(unsigned int addr, unsigned int value)
    {
        cout << "[ECU] PID_configuration" << endl;
        initiator.execute(addr,value,true);
    }

    int PID_GetCoefficients(unsigned int addr, unsigned int value)
    {
        int data;
        cout << "[ECU] PID_GetCoefficients" << endl;
        data = initiator.execute(addr,value,false);
        return data;
    }

    int PID_GetStatus(void)
    {
        int data;
        cout << "[ECU] PID_GetCoefficients" << endl;
        data = initiator.execute(PID_CHSR,data,false);
        return data;
    }
};

#endif
