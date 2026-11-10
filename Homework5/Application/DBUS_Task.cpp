//
// Created by ztlxx202422802 on 2026/11/5.
//
#include "DBUS_Task.h"
#include "usart.h"
#include "cmsis_os.h"
#include "CAN_bsp.h"
int16_t a=0;
volatile dbus_data_t dbus_data;
volatile uint8_t dbus_rx_flag = 0;
uint8_t dbus_rx_buffer[18];

//DMA接收完成回调
extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance == USART3 && Size == 18)
    {
        dbus_rx_flag = 1;
        // 重新开启 DMA 接收
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, dbus_rx_buffer, 18);
    }
}

//解析函数
void dbus_parse_frame(void)
{
    dbus_data.button.ch0 = (dbus_rx_buffer[0] | (dbus_rx_buffer[1] << 8)) & 0x07ff;
    dbus_data.button.ch1 = ((dbus_rx_buffer[1] >> 3) | (dbus_rx_buffer[2] << 5)) & 0x07ff;
    dbus_data.button.ch2 = ((dbus_rx_buffer[2] >> 6) | (dbus_rx_buffer[3] << 2) | (dbus_rx_buffer[4] << 10)) & 0x07ff;
    dbus_data.button.ch3 = ((dbus_rx_buffer[4] >> 1) | (dbus_rx_buffer[5] << 7)) & 0x07ff;
    dbus_data.button.s1 = ((dbus_rx_buffer[5] >> 4) & 0x000C) >> 2;
    dbus_data.button.s2 = ((dbus_rx_buffer[5] >> 4) & 0x0003);
}

//任务入口
void StartDBUS_Task(void *argument)
{

    //初始化串口 DMA 接收
    __HAL_UART_CLEAR_OREFLAG(&huart3);
    HAL_UART_AbortReceive(&huart3);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, dbus_rx_buffer, 18);
    for(;;)
    {
        a++;
        if (dbus_rx_flag == 1)
        {
            dbus_rx_flag = 0;
            dbus_parse_frame();
        }
        osDelay(2);
    }
}