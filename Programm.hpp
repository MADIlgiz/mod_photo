#pragma once 


#include <iostream>
#include <signal.h>
#include "dbc_client.hpp"
#include <sys/stat.h>
#include "retk2/core/Application.hpp"
#include "retk2/core/Time.hpp"
#include "retk2/core/Timer.hpp"
#include "retk2/core/Reporter.hpp"
#include "retk2/io/Terminal.hpp"
#include "retk2/io/Poller.hpp"
#include "retk2/ipc/DBCClient.hpp"
#include "retk2/proto/PacketMultiLayer.hpp"
#include "PWM.hpp"
#include "sys/stat.h"
#include <time.h>
#include <sys/time.h>

class Program: public retk::Application, public retk::Reporter, public retk::PacketMultiLayer {
    public:
    /// Конструтор
    Program();
    
    /// Деструктор
    ~Program(){};
    /// Запуск программы
    int run();
    /// Инициализация
    void init();
    /// Цикл работы 
    void cycle();

    /// отключится при завершении
    bool do_disconnect;
    /// режим ожидания пакетов 
    bool do_wait;
    /// Контроль работы ШИМ
    int PWM_Controle();

    ///< закрытые данные приложения 
    private:
    /// параметры приложения
    retk::VariableMap p_vars; /// Конфиг программы
    bool debug;
    uint64_t time_now;///текущая временная метка
    ///Транспорт 
    mod_photo_DBCclient client;
    ///ШИМ
    PWM *pwm2;
    std::string local_addr = "";
    std::string remote_addr = "";
    std::string rules = "";
    int pwmchipnum = 0;/// Номер ШИМ
    int pwmdevicenum = 0;
    int pwmperiod = 0;/// Период 
    int pwmdutycycle = 0;/// Заполнение
    int pwmpackduration = 0;/// длительность пачки импульсов
    uint8_t mod_photo_status = slp::SLP_SUBSYSTEM_STATUS::SDSS_UNDEFINED;
    uint8_t sender = slp::SLP_SUBSYSTEMS::SLPS_PHOTO1; /// идентификатор подсистемы отправителя
    slp::SLP_Message msg_income, msg_outcome; ///объекты для хранения сообщений
    bool msg_result; /// Результат получения сообщения
    bool has_answer; /// флаг наличия  ответного сообщения на полученное
    /// обработка сообщения с подготовкой ответа
    void proceedMsg(slp::SLP_Message & min, ///< Входящее сообщение
                    slp::SLP_Message & mout, ///< Записываемое исходящее сообщение
                    bool * has_answer_l ///< Было ли сгенерировано ответное сообщение
                    );
    /// Обработка сообщения с подготовкой ответа - команды управления, тип MSG*_CTRL
    void proceedMsgCTRL(slp::SLP_Message & min, ///< Входящее сообщение
                        slp::SLP_Message & mout, ///< Записываемое исходящее сообщение
                        bool * has_answer_l ///< Было ли сгенерировано ответное сообщение
                        );

    /// Запись данных измерений текущего источника данных в сообщение
    void writeData(slp::SLP_Message & mout ///< Записываемое исходящее сообщение
                   );

    /// Отправка статуса 
    slp::SLP_SUBSYSTEM_STATUS Status = slp::SLP_SUBSYSTEM_STATUS::SDSS_UNDEFINED; 
    int sendStatus();

    ///таймер для управления ШИМ
    //struct itimerval timer;
    struct itimerval timer;
    /// Флаг о появлении сообщения SHUTTERS
    int ShutterFlag = 0;
};