//
// Created by ztlxx202422802 on 2026/11/5.
//
#include "Power_Check_Task.h"
#include "CAN_bsp.h"
#include "cmsis_os.h"
float chassis_power_w = 0.0f;    // 底盘功率 (单位：W)
float buffer_energy_j = 0.0f;    // 缓冲能量 (单位：J)

void Power_Rx_Callback(can_device_receive *dev, uint8_t *data) {
    //解析底盘功率
    int16_t raw_power = (int16_t)((data[1] << 8) | data[0]);
    chassis_power_w = raw_power * 0.1f;

    //解析缓冲能量
    int16_t raw_energy = (int16_t)((data[3] << 8) | data[2]);
    buffer_energy_j = raw_energy * 0.1f;
}
void StartPowerTask(void *argument)
{
    static can_device_receive Power_Check_rx(&hcan1, 0x31, Power_Rx_Callback);
    for (;;)
    {
    osDelay(1);
    }
}