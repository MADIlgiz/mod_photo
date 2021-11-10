#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>  //define O_WRONLY and O_RDONLY
#include <sys/stat.h>
#include <string>
#include <cstring>
#include <iostream>
using namespace std;

#define SYSFS_PWM_EXPORT "/sys/class/pwm/pwmchip2/export"
#define SYSFS_PWM_PERIOD "/sys/class/pwm/pwmchip2/pwm1/period"
#define SYSFS_PWM_DUTY_CYCLE "/sys/class/pwm/pwmchip2/pwm1/duty_cycle"
#define SYSFS_PWM_ENABLE "/sys/class/pwm/pwmchip2/pwm1/enable"
#define SYSFS_PWM_UNEXPORT "/sys/class/pwm/pwmchip2/unexport"
#define SYSFS_PWM_ENAMBLE_ST "1"
#define SYSFS_PWM_DISABLE_ST "0"
#define PERIOD "2500000"
#define DUTY_CYCLE "2000000"

int PWMcFunc();


//echo 1 > /sys/class/pwm/pwmchip2/export                                  
//echo 2500000 > /sys/class/pwm/pwmchip2/pwm1/period                       
//echo 2000000 > /sys/class/pwm/pwmchip2/pwm1/duty_cycle                   
//echo 1 > /sys/class/pwm/pwmchip2/pwm1/enable 
const std::string pwm_exp_str = "/sys/class/pwm/";
class PWM{
    ///
    uint8_t chipnumber;
    uint8_t devicenumber;
    int duty_cicly;
    int period;
    int sysfsfiledescr;
   // std::string ExportStr;
    std::string PeriodStr;
    std::string DutyCycleStr;
    std::string EnableStr;
    std::string UnexportStr;
    public:
    enum STATES{
        DISABLE,
        ENABLE,
    };
    std::string ExportStr;
    STATES pwmstate = DISABLE;
    ///export
    ///Конструктор
    PWM(uint8_t device_number, uint8_t chip_number);
    int PWM_enable();
    int PWM_set_period(int newperiod);
    int PWM_set_duty_cycle(int newdutycycle);
    int PWM_get_period(int & pwm_period);
    int PWM_get_duty_cycle(int & pwm_duty_cycle);
    int PWM_get_state();
    ///unexprt
    int PWM_disable();
    ///
    int PWM_unexport();
    ~PWM();


};

