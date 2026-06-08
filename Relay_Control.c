#include "Relay_Control.h"
#include "tim.h"

static float Get_Distance_ByPins(GPIO_TypeDef *trig_port,
                                 uint16_t trig_pin,
                                 GPIO_TypeDef *echo_port,
                                 uint16_t echo_pin)
{
    uint32_t wait_start;
    uint32_t start_time;
    uint32_t end_time;
		float temperature = 25.0f; //先把温度写死
		float sound_speed = 331.3f + 0.606f * temperature;
		float distance_per_us = sound_speed * 100.0f / 1000000.0f;
    /* 在新的trig发出前，ECHO引脚应该是低的 */
    wait_start = __HAL_TIM_GET_COUNTER(&htim2);
    while (HAL_GPIO_ReadPin(echo_port, echo_pin) == GPIO_PIN_SET)
    {
        if ((__HAL_TIM_GET_COUNTER(&htim2) - wait_start) > 5000U)
        {
            return -3.0f;
        }
    }

    /* 发送一个TRIG引脚 */
    HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_RESET);
    Delay_us(5);
    HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_SET);
    Delay_us(20);
    HAL_GPIO_WritePin(trig_port, trig_pin, GPIO_PIN_RESET);

    /* Wait for echo rising edge. */
    wait_start = __HAL_TIM_GET_COUNTER(&htim2);
    while (HAL_GPIO_ReadPin(echo_port, echo_pin) == GPIO_PIN_RESET)
    {
        if ((__HAL_TIM_GET_COUNTER(&htim2) - wait_start) > 30000U)
        {
            return -1.0f;
        }
    }

    start_time = __HAL_TIM_GET_COUNTER(&htim2);

    /* Wait for echo falling edge. */
    while (HAL_GPIO_ReadPin(echo_port, echo_pin) == GPIO_PIN_SET)
    {
        if ((__HAL_TIM_GET_COUNTER(&htim2) - start_time) > 40000U)
        {
            return -2.0f;
        }
    }

    end_time = __HAL_TIM_GET_COUNTER(&htim2);

    return (float)(end_time - start_time) * distance_per_us / 2.0f;   //之前是 0.034 cm/us光速，除以2 变成 0.017f cm/s
}

float Get_Distance(void)
{
    return Get_Distance_ByPins(ULTRASONIC1_TRIG_GPIO_Port,
                               ULTRASONIC1_TRIG_Pin,
                               ULTRASONIC1_ECHO_GPIO_Port,
                               ULTRASONIC1_ECHO_Pin);
}

float Get_Distance_2(void)
{
    return Get_Distance_ByPins(ULTRASONIC2_TRIG_GPIO_Port,
                               ULTRASONIC2_TRIG_Pin,
                               ULTRASONIC2_ECHO_GPIO_Port,
                               ULTRASONIC2_ECHO_Pin);
}

void Bed_Updown(uint8_t dir)
{
    if (dir == 1)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
    }
    else if (dir == 2)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
    }
}
