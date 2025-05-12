#ifndef MOTOR_H
#define MOTOR_H

struct Motor {
    GPIO_TypeDef* fwd_port; 
    GPIO_TypeDef* bwd_port;
    int fwd_pin;
    int bwd_pin;

    Motor(GPIO_TypeDef* fwd_port, GPIO_TypeDef* bwd_port, int fwd_pin, int bwd_pin);
    void spin_fwd();
    void spin_bwd();
};

#endif 