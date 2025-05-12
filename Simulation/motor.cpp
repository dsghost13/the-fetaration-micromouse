#include "motor.h"

Motor::Motor(GPIO_TypeDef* fwd_port, GPIO_TypeDef* bwd_port, int fwd_pin, int bwd_pin) {
    this->fwd_port = fwd_port;
    this->bwd_port = bwd_port;
    this->fwd_pin = fwd_pin;
    this->bwd_pin = bwd_pin;
}

void Motor::spin_fwd() {
    HAL_GPIO_WritePin(fwd_port,  fwd_pin, 1);    // sets MX_FWD as HIGH
    HAL_GPIO_WritePin(fwd_port, bwd_pin, 0);    // sets MX_BWD as LOW
}

void Motor::spin_bwd() {
    HAL_GPIO_WritePin(bwd_port,  fwd_pin, 0);    // sets MX_FWD as LOW
    HAL_GPIO_WritePin(bwd_port, bwd_pin, 1);    // sets MX_BWD as HIGH
}