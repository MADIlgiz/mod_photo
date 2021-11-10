#include "Programm.hpp"

using namespace slp;
using namespace retk;
using namespace std;

int PWMTIMERFLAG = 0;


void timer_handler (int signum)
{
 //PWMCOUNT++;
 static size_t count = 0;
 cout  << "timer expired " << ++count << endl;
 //GLOBALPWMFLAG = 1;
 PWMTIMERFLAG = 1;
}

Program::Program()
{
    version="0.0.1";
    do_disconnect = true;
    do_wait = true;
    p_vars.setName("mod_photo");
    //p_vars.add("local_addr",'l',"Local address").link(local_addr);
    //p_vars.add("remote_addr",'r',"Remote address").link(remote_addr);
    p_vars.add("rules",'u',"Rules in form key1:mask1 key2:mask2 ...").link(rules);
    p_vars.add("debug",'g',"Debug (0 - off, 1 - on)").link(debug);
    p_vars.add("pwmchipnum",'n',"PWM chip sys fs number").link(pwmchipnum);
    p_vars.add("pwmdevice",'x',"PWM device number sys fs").link(pwmdevicenum);
    p_vars.add("pwmperiod",'t',"PWM period sys fs").link(pwmperiod);
    p_vars.add("pwmdutycycle",'d',"PWM duty cycle in sys fs").link(pwmdutycycle);
    p_vars.add("pwmduration",'j',"PWM pack duration").link(pwmpackduration);
    time_now = 0;


    ///Reporter
    debug = false;
    setReporterName("MOD_PHOTO");
    setReporterTTYColor(Terminal::Colors::Cyan);
    ///p_vars.add("sender",'s',"Sender subsystem id (SLP_SUBSYSTEMS), dec").link(sender);
    conf.push_back(&p_vars);
    confPacketLayer(conf);
};

void Program::init(){
    /// вывод аргументов приложения
    if (debug)
    {
        report('i',"Init Parameters:");
        //report('i',"Local address: %s",local_addr.c_str());
        //report('i',"Remote address: %s",remote_addr.c_str());
        report('i',"DBC rules: %s",rules.c_str());
        report('i',"Debug: %d", debug);
        report('i',"Sender subsystem: %u", sender);
        report('i',"PWM chip number: %d", pwmchipnum);

        report('i',"PWM period: %d", pwmperiod);
        report('i',"PWM duty cycle: %d",pwmdutycycle);
        report('i',"PWM pack duration: %d", pwmpackduration);
    }
    msg_outcome.sender = (SLP_SUBSYSTEMS)sender;

}

void Program::cycle(){
    uint8_t layer = PacketMultiLayer::layerUnix;
    Poller poll;
    
    initPacketLayer(layer,poll);
    client.setPacketLayer(pl);
    client.setRules(rules);
    client.connect();

    /// Указываем обработчик для сигнала SIGALARM
    signal(SIGALRM,timer_handler);
    /// Отправка статуса: Запущен, еще  нет связи с датчиком
    Status = slp::SLP_SUBSYSTEM_STATUS::SDSS_INIT;
    sendStatus();
    ///Создаем объект для управления ШИМ
    PWM mod_photo_pwm(pwmdevicenum,pwmchipnum);
    pwm2 = &mod_photo_pwm;
    /// Отправить сообщение о текущем статусе, есть подключение к ШИМ
    if(pwm2 != nullptr){
    Status = slp::SLP_SUBSYSTEM_STATUS::SDSS_CONNECT;
    sendStatus();
    pwm2->PWM_set_period(pwmperiod);
    //pwm2->PWM_enable();
    pwm2->PWM_set_duty_cycle(pwmdutycycle);
    //pwm2->PWM_enable();
    }
    while(is_running && do_wait)
    {

        
        time_now = rtime_now();
        if(poll.wait(10)){
            if(client.hasMessage()){ 
                cout << "Client has nessage" << endl;
                client.recvMessage(&msg_income);
                proceedMsg(msg_income,msg_outcome,&has_answer);
            }
        }
        PWM_Controle();
    }
    if(do_disconnect) { 
        client.disconnect();
        cout << "Disconnect" << endl;
    }
    shutdownPacketLayer();

}


int Program::run(){
    init();
    cycle();
    return 0;
}

void Program::proceedMsg(slp::SLP_Message &min, slp::SLP_Message &mout, bool *has_answer_l){
   
    *has_answer_l = false;

    try{/// обработка сообщений controle
        if(min.code == MSGR_CTRL){
            proceedMsgCTRL(min,mout,has_answer_l);
        }
    }catch(...){

    }

}

void Program::proceedMsgCTRL(slp::SLP_Message & min, slp::SLP_Message & mout, bool * has_answer_l)
{
    switch(min.obj_id) {
        case slp::SLPC_SUBS_SHUTTERS:
            report("SHUTTERRECIEVED\n");
            /// Установка флага о получении сообщения на начало измерения
            ShutterFlag = 1;
            /// Завожу таймер и включаю ШИМ на 50 мс
            /// При первом получении меняем статус на Запущен, есть связь с датчиком, инициализирован SDSS_CONNECT
            
            break;
        case slp::SLPC_SUBS_STATE:


            sendStatus();
            /// Отправка текущего статуса
            break;
        case slp::SLPC_SUBS_OFF:

            /// Отключить ШИМ
            break;
        case slp::SLPC_SUBS_ON:
            /// Не знаю пока что делать с ШИМ при команде на включение
            break;
        
        default:

            break;

    }
}
int Program::sendStatus(){
    
    bool res = false;
    //msg_outcome.sender = sender;
    msg_outcome.receiver = SLP_SUBSYSTEMS::SLPS_NONE;
    msg_outcome.code = MSGA_CTRL;
    msg_outcome.obj_id = SLP_CTRL::SLPC_SUBS_STATE;
    uint32_t Datasize = 1;
    //msg_outcome.setData(&Status,Datasize);   
    uint64_t statusRtime = rtime_now();
    res = slp::SLP_DataFormatter::pack(msg_outcome,&Status,Datasize,statusRtime);
    if(res == true){
        client.sendMessage(&msg_outcome);
    }
    return res;
}

int Program::PWM_Controle(){
    cout << "PWM controle func" << endl;
    if(this->pwm2 == nullptr){
        cout << "PWM not init" << endl;
    }
    else{
        switch (pwm2->pwmstate)
        {
        case PWM::STATES::ENABLE/* constant-expression */:
            if(PWMTIMERFLAG == 1){
                pwm2->PWM_disable();
                //pwm2->PWM_get_state();
                PWMTIMERFLAG = 0;
                pwm2->pwmstate = PWM::STATES::DISABLE;
                report("PWM disabled");
            }
            /* code */
            break;
        case PWM::STATES::DISABLE:
            if((PWMTIMERFLAG == 0) && (ShutterFlag == 1)){
                timer.it_value.tv_sec = 0;
                timer.it_value.tv_usec = pwmpackduration;
                timer.it_interval.tv_sec = 0;
                timer.it_interval.tv_usec = 0;
                setitimer (ITIMER_REAL, &timer, NULL);
                pwm2->PWM_enable();
                report("PWM enabled");
                pwm2->pwmstate = PWM::STATES::ENABLE;
                ShutterFlag = 0;
                
            }
            break;
        default:
            break;
        }
    }
}
