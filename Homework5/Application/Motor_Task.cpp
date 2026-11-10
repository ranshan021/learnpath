//
// Created by ztlxx202422802 on 2026/11/4.
//
#include "Motor_Task.h"
    #include "CAN_bsp.h"
    #include "cmsis_os.h"
#include <math.h>
#include "DJI_motor_driver.h"
void StartMotor_Task(void *argument)
{
    //大疆电机对象
    DJI_motor motor(&hcan1, nullptr, 1, DJI_M3508, 0);
    //设置PID参数
    //位置环
    motor.posPid.pid_reset(9600.0f, 500.0f, 2.5f, 0.06f, 18.0f);
    //速度环
    motor.velPid.pid_reset(16384.0f, 500.0f, 2.4f, 0.45f, 0.1f);

    const float base_speed = 60.0f;
    const float sine_amp = 40.0f;
    const float freq = 0.5f;
    const float w = 2.0f * 3.1415926f * freq;

    float target_rounds = 0.0f;
    uint32_t last_tick = osKernelGetTickCount();

    for(;;)
    {
        //dt
        uint32_t now = osKernelGetTickCount();
        float dt = (now - last_tick) / 1000.0f;
        if (dt <= 0.0f) dt = 0.001f;
        last_tick = now;
        //积分
        float time_s = now / 1000.0f;
        float v_target_rpm = base_speed + sine_amp * sinf(w * time_s);
        target_rounds += (v_target_rpm / 60.0f) * dt;
        motor.motor_set_rounds(target_rounds);
        osDelay(1);
    }
}
// #define ENCODER_RANGE 8191
//
// typedef struct {
//   float Kp, Ki, Kd;
//   float target;
//   float err, err_last, err_sum;
//   float out;
//   float limit_out, limit_errsum;
// } PIDTypedef;
//
// PIDTypedef anglePID;
// PIDTypedef speedPID;
//
// int16_t moto_raw_angle = 0;
// int16_t moto_raw_speed = 0;
// int16_t moto_raw_current = 0;
// int16_t moto_last_raw_angle = 0;
// float moto_total_angle = 0.0f;
// uint8_t tx_buf[8] = {0};
//
// // CAN 接收回调函数
// void Motor1_Rx_Callback(can_device_receive *dev, uint8_t *data)
// {
//     moto_raw_angle   = (int16_t)((data[0] << 8) | data[1]);
//     moto_raw_speed   = (int16_t)((data[2] << 8) | data[3]);
//     moto_raw_current = (int16_t)((data[4] << 8) | data[5]);
//
//     int16_t delta = moto_raw_angle - moto_last_raw_angle;
//     if (delta > ENCODER_RANGE / 2) {
//         delta -= ENCODER_RANGE;
//     } else if (delta < -ENCODER_RANGE / 2) {
//         delta += ENCODER_RANGE;
//     }
//     moto_total_angle += delta;
//     moto_last_raw_angle = moto_raw_angle;
// }
//
// /**
//   * @brief PID 计算
//   * @param  pid: PID 结构体指针
//   * @param  feedback: 当前实际反馈值
//   * @retval PID 输出
//   */
// float PID_Calc(PIDTypedef *pid, float feedback)
// {
//     pid->err = pid->target - feedback;
//
//     // 积分项
//     pid->err_sum+=pid->err;
//     //积分限幅
//     if (pid->err_sum > pid->limit_errsum) pid->err_sum=pid->limit_errsum;
//     if (pid->err_sum< -pid->limit_errsum)pid->err_sum=-pid->limit_errsum;
//
//     // 总和输出
//     pid->out = pid->Kp * pid->err  +  pid->Ki*pid->err_sum  +  pid->Kd * (pid->err - pid->err_last);
//
//     // 保存上一次误差
//     pid->err_last = pid->err;
//     //输出限幅
//     if (pid->out > pid->limit_out)pid->out = pid->limit_out;
//     if (pid->out < -pid->limit_out)pid->out = -pid->limit_out;
//
//     return pid->out;
// }
// /**
//   * @brief  PID初始化
//   * @param  pid: PID 结构体指针
//   * @param  kp
//   * @param  ki
//   * @param  kd
//   * @param  limit_errsum:积分限幅
//   * @param  limit_out:输出限幅
//   */
// void PID_Init(PIDTypedef *pid, float kp, float ki, float kd,float limit_errsum, float limit_out)
// {
//     pid->Kp = kp;
//     pid->Ki = ki;
//     pid->Kd = kd;
//     pid->limit_errsum = limit_errsum;
//     pid->limit_out = limit_out;
//
//     pid->target   = 0.0f;
//     pid->err      = 0.0f;
//     pid->err_last = 0.0f;
//     pid->err_sum = 0.0f;
//     pid->out      = 0.0f;
//
// }
// void StartMotor_Task(void *argument)
// {
//     //初始化PID
//     PID_Init(&speedPID,2.4,0.45,0.1,500.0,16384.0);
//     PID_Init(&anglePID,2.5,0.06,18,500.0,9600.0);
//     //创建模板对象
//     static can_device_receive motor1_rx(&hcan1, 0x201, Motor1_Rx_Callback);
//     static can_device_transmit motor_tx(&hcan1, 0x200, 8, tx_buf);
//     //速度参数
//     const float base_speed = 60.0f;
//     const float sine_amp = 40.0f;
//     const float freq = 0.5f;
//     const float omega = 2.0f * 3.1415926f * freq;
//     float target_angle = 0.0f;
//     uint8_t pos_cnt = 2;
//
//     for(;;) {
//         if (pos_cnt >= 2) {
//             pos_cnt = 0;
//             float time_s = osKernelGetTickCount() / 1000.0f;
//             float v_target_rpm = base_speed + sine_amp * sinf(omega * time_s);
//             float v_target_units = v_target_rpm * (8192.0f / 60.0f);
//             target_angle += v_target_units * 0.002f;
//
//             anglePID.target = target_angle;
//             speedPID.target = PID_Calc(&anglePID, moto_total_angle);
//         }
//         pos_cnt++;
//
//         int16_t iq1 = (int16_t)PID_Calc(&speedPID, moto_raw_speed);
//         tx_buf[0] = (iq1 >> 8) & 0xFF;
//         tx_buf[1] = iq1 & 0xFF;
//         tx_buf[2] = 0; tx_buf[3] = 0; tx_buf[4] = 0; tx_buf[5] = 0; tx_buf[6] = 0; tx_buf[7] = 0;
//
//         motor_tx.can_send_msg();
//         osDelay(1);
//     }
// }