#include <main.h>
#include "stm32f1xx_hal.h"
#include <stm32f1xx_hal_gpio.h>
#include <cmsis_os.h>
#include <algorithm>
#include <stm32f1xx_hal_tim.h>
#include "tim.h"
#include "space.h"
#include "animations.h"
#include "scheduler.h"


typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
} PIN;

PIN pins[] = {
		{LED_00_GPIO_Port, LED_00_Pin},
		{LED_01_GPIO_Port, LED_01_Pin},
		{LED_02_GPIO_Port, LED_02_Pin},

		{LED_10_GPIO_Port, LED_10_Pin},
		{LED_11_GPIO_Port, LED_11_Pin},
		{LED_12_GPIO_Port, LED_12_Pin},

		{LED_20_GPIO_Port, LED_20_Pin},
		{LED_21_GPIO_Port, LED_21_Pin},
		{LED_22_GPIO_Port, LED_22_Pin},
};

PIN rows[] = {
		{LAYER_0_GPIO_Port, LAYER_0_Pin},
		{LAYER_1_GPIO_Port, LAYER_1_Pin},
		{LAYER_2_GPIO_Port, LAYER_2_Pin},
};

Space *current_frame = new Space(3);
Space *compute_frame = new Space(3);

std::vector<Animation*> animations = {
		new Slide(Axis::X),
		new Slide(Axis::Y),
		new Slide(Axis::Z),
		new Frame(),
		new Random(3),
		new Increment(true),
		new Increment(false),
		new Rain(),
};


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim == &htim1) {
		HAL_GPIO_TogglePin(DEBUG_GPIO_Port, DEBUG_Pin);
	}
}


extern "C" void simulate_task(void const* argument) {
	TickType_t xLastWakeTime = 0;
	Scheduler scheduler(3000);
	scheduler.add(animations);

	int period = 200;

	for(;;) {
		compute_frame->clear();
		scheduler.schedule(period)->update(period, compute_frame);

		portENTER_CRITICAL();
		std::swap(current_frame, compute_frame);
		portEXIT_CRITICAL();

		vTaskDelayUntil(&xLastWakeTime, period);
	}
}

extern "C" void renderer_task(void const * argument) {
	HAL_TIM_Base_Start_IT(&htim1);

	for(;;) {
		for(int z = 0; z < 3; z++) {
			int i = 0;
			for(int x = 0; x < 3; x++) {
				for(int y = 0; y < 3; y++) {
					HAL_GPIO_WritePin(pins[i].port, pins[i].pin, current_frame->get(x, y, z) ? GPIO_PIN_SET : GPIO_PIN_RESET);
					i++;
				}
			}

			HAL_GPIO_WritePin(rows[z].port, rows[z].pin, GPIO_PIN_RESET);
			osDelay(1);
			HAL_GPIO_WritePin(rows[z].port, rows[z].pin, GPIO_PIN_SET);
		}
	}
}
