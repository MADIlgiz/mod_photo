#include "PWM.hpp"
#include <fstream>
#include <iostream>
using namespace std;
///Конструктор
PWM::PWM(uint8_t device_number, uint8_t chip_number): chipnumber{chip_number}, devicenumber{device_number}{
    string pwmchip = "pwmchip" + std::to_string(chipnumber);
    ExportStr =  pwm_exp_str + pwmchip + "/export";
    cout << ExportStr.c_str() << endl;
    PeriodStr = pwm_exp_str + pwmchip + "/pwm" + std::to_string(devicenumber) + "/period" + "\0";
    //string PeriodStr = "/sys/class/pwm/pwmchip2/pwm1/period";
    cout << PeriodStr.c_str() << endl;
    DutyCycleStr = pwm_exp_str + pwmchip + "/pwm" + std::to_string(devicenumber) + "/duty_cycle";
    cout << DutyCycleStr << endl;
    EnableStr = pwm_exp_str + pwmchip + "/pwm" + std::to_string(devicenumber) + "/enable";
    cout << EnableStr << endl;
    UnexportStr = pwm_exp_str + pwmchip + "/unexport";
    cout << UnexportStr << endl;
    int fd;
    try{
    fd = open(ExportStr.c_str(),O_WRONLY);
    if(fd == -1){
        cout << "PWM export error" << endl;
        //return EXIT_FAILURE;
    }
    write(fd, SYSFS_PWM_ENAMBLE_ST, sizeof(SYSFS_PWM_ENAMBLE_ST));
    close(fd);
    }
    catch(...){
        cout << "PWM export error" << endl;
    }
}

int PWM::PWM_set_duty_cycle(int newdutycycle){
    int fd;
    try
    {
        fd = open(DutyCycleStr.c_str(),O_WRONLY);
        char NewDuty[10] = {'\0'};
        sprintf(NewDuty,"%d",newdutycycle);
        write(fd,DUTY_CYCLE,sizeof(DUTY_CYCLE));
        close(fd);


    }
    catch(...)
    {
        cout << "set Duty error" << endl;
    }
    


}

int PWM::PWM_set_period(int newperiod){
   int fd;
   try{
       fd = open(PeriodStr.c_str(),O_WRONLY);
        if(fd == -1){
        cout << "PWM period error" << endl;
        return EXIT_FAILURE;
    }
       string NewPeriod = to_string(newperiod);
       write(fd,NewPeriod.c_str(),sizeof(NewPeriod.c_str()));
       close(fd);
   }
   catch(...){
       cout << "set period error" << endl;
   }
}

int PWM::PWM_get_period(int & pwm_period){
   int fd;
   try{
       fd = open(PeriodStr.c_str(),O_RDONLY);
       //string Period(10,'x');
       char Period[10] = {0};
       read(fd, Period,sizeof(Period));
       close(fd);
       pwm_period = atoi(Period);
   }
   catch(...){
       cout << "get period error" << endl;
   }
}

int PWM::PWM_get_duty_cycle(int & pwm_duty_cycle){
    int fd;
    try{
       fd = open(DutyCycleStr.c_str(),O_RDONLY);
       char Dutycycle[10] = {0};
       read(fd, Dutycycle,sizeof(Dutycycle));
       close(fd);
       pwm_duty_cycle = atoi(Dutycycle);
    }
    catch(...){
       cout << "get period error" << endl;
    }
}

int PWM::PWM_enable(){
    int fd;
    try{
        cout << "=======PWM enable func========" << endl;
        fd = open(EnableStr.c_str(),O_WRONLY);
        write( fd, SYSFS_PWM_ENAMBLE_ST, sizeof(SYSFS_PWM_ENAMBLE_ST));
        close(fd);
    }
    catch(...){
        cout << "enable error" << endl;
        return 1;
    }
    pwmstate = ENABLE;
    return 0;
}

int PWM::PWM_disable(){
    int fd;
    try{
        cout << "=======PWM disable func========" << endl;
        fd = open(EnableStr.c_str(),O_WRONLY);
        write( fd, SYSFS_PWM_DISABLE_ST, sizeof(SYSFS_PWM_DISABLE_ST));
        close(fd);
    }
    catch(...){
        cout << "disable error" << endl;
        return 1;
    }
    pwmstate = DISABLE;
    return 0;
}

PWM::~PWM(){
    if(pwmstate == DISABLE){
        PWM_unexport();
        cout << "Unexport PWM" << endl;
    }
    else{
        PWM_disable();
        cout << "disable PWM" << endl;
        PWM_unexport();
        cout << "unexport PWM" << endl;
    }
}

int PWM::PWM_unexport(){
    if(pwmstate == DISABLE){
    int fd;
    try{
    fd = open(UnexportStr.c_str(),O_WRONLY);
    write(fd, SYSFS_PWM_ENAMBLE_ST, sizeof(SYSFS_PWM_ENAMBLE_ST));
    close(fd);
    }
    catch(...){
        cout << "PWM unexport error" << endl;
        return 1;
    }
    return 0;
    }
    else{
        cout << "PWM enabled, cant unexport" << endl;
    }

}


int PWMcFunc(){
    int fd;
    fd = open(SYSFS_PWM_EXPORT,O_WRONLY);
    if(fd == -1){
        cout << "PWM export error" << endl;
        return EXIT_FAILURE;
    }
    write(fd, SYSFS_PWM_ENAMBLE_ST, sizeof(SYSFS_PWM_ENAMBLE_ST));
    close(fd);

    fd = open(SYSFS_PWM_PERIOD,O_WRONLY);
    if(fd == -1){
        cout << "PWM period error" << endl;
        return EXIT_FAILURE;
    }
    char Period[] = "2500000";
    write(fd,Period, sizeof(Period));
    close(fd);

    fd = open(SYSFS_PWM_DUTY_CYCLE,O_WRONLY);
    if(fd == -1){
        cout << "PWM duty cycle error" << endl;
        return EXIT_FAILURE;
    }
    char Duty_cycle[] = "2000000";
    write(fd, Duty_cycle, sizeof(Duty_cycle));
    close(fd);

    fd = open(SYSFS_PWM_ENABLE,O_WRONLY);
     if(fd == -1){
        cout << "PWM duty cycle error" << endl;
        return EXIT_FAILURE;
    }
     write(fd, SYSFS_PWM_ENAMBLE_ST, sizeof(SYSFS_PWM_ENAMBLE_ST));
     close(fd);
    int flag = 0;
     while(flag == 0){
         sleep(10);
         flag = 1;
     }

     fd = open(SYSFS_PWM_ENABLE,O_WRONLY);
     if(fd == -1){
        cout << "PWM duty cycle error" << endl;
        return EXIT_FAILURE;
    }
     write(fd, SYSFS_PWM_DISABLE_ST, sizeof(SYSFS_PWM_DISABLE_ST));
     close(fd);

     fd = open(SYSFS_PWM_UNEXPORT,O_WRONLY);
     if(fd == -1){
         cout << "PWM unexport error" << endl;
         return EXIT_FAILURE;
     }
     write(fd,SYSFS_PWM_ENAMBLE_ST,sizeof(SYSFS_PWM_ENAMBLE_ST));
     close(fd);






}







