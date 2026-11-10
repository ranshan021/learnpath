//
// Created by ztlxx202422802 on 2026/11/5.
//

#ifndef HOMEWORK5_2_DBUS_H
#define HOMEWORK5_2_DBUS_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//遥控器数据结构体
typedef struct {
    struct {
        uint16_t ch0;
        uint16_t ch1;
        uint16_t ch2;
        uint16_t ch3;
        uint8_t s1;
        uint8_t s2;
    } button;
} dbus_data_t;
extern volatile dbus_data_t dbus_data;
// FreeRTOS 任务入口
void StartDBUS_Task(void *argument);

#ifdef __cplusplus
}
#endif

#endif //HOMEWORK5_2_DBUS_H
