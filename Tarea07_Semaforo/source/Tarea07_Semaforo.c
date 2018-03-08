/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    Tarea07_Semaforo.c
 * By: Gustavo Araiza Obeso
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

SemaphoreHandle_t g_led_semaphore;
SemaphoreHandle_t counter_semaphore;

// SW3
void PORTA_IRQHandler()
{
	BaseType_t xHigherPriorityTaskWoken;
	PORT_ClearPinsInterruptFlags(PORTA, 1<<4);
	xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR( g_led_semaphore, &xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

//SW2
void PORTC_IRQHandler()
{
	BaseType_t xHigherPriorityTaskWoken;
	PORT_ClearPinsInterruptFlags(PORTC, 1<<6);
	xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR( counter_semaphore, &xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void led_task(void *arg)
{
	for(;;)
	{
		xSemaphoreTake(g_led_semaphore,portMAX_DELAY);
		GPIO_TogglePinsOutput(GPIOB,1<<21);
	}

}

void Counter_task(void *arg)
{
	//uint8_t counter ;
	counter_semaphore = xSemaphoreCreateCounting(10,1);
	for(;;)
	{
		//xSemaphoreTake(counter_semaphore,portMAX_DELAY);
		if (uxSemaphoreGetCount(counter_semaphore) == 10)
		{
			//counter = 0;
			counter_semaphore = xSemaphoreCreateCounting(10,0);
			GPIO_TogglePinsOutput(GPIOE,1<<26);
		}
	}
}
int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortA); //Enable SW3
	CLOCK_EnableClock(kCLOCK_PortC); //Enable SW2
	CLOCK_EnableClock(kCLOCK_PortE); //enable led green

	port_pin_config_t config_led =
	{ kPORT_PullDisable, kPORT_SlowSlewRate, kPORT_PassiveFilterDisable,
			kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAsGpio,
			kPORT_UnlockRegister, };

	PORT_SetPinConfig(PORTB, 21, &config_led); //led blue
	PORT_SetPinConfig(PORTE, 26, &config_led); //led green

	port_pin_config_t config_switch =
	{ kPORT_PullDisable, kPORT_SlowSlewRate, kPORT_PassiveFilterDisable,
			kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAsGpio,
			kPORT_UnlockRegister};
	PORT_SetPinInterruptConfig(PORTA, 4, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTC, 6, kPORT_InterruptFallingEdge);

	PORT_SetPinConfig(PORTA, 4, &config_switch);
	PORT_SetPinConfig(PORTC, 6, &config_switch);

	gpio_pin_config_t led_config_gpio =
	{ kGPIO_DigitalOutput, 1 };

	GPIO_PinInit(GPIOB, 21, &led_config_gpio);
	GPIO_PinInit(GPIOE, 26, &led_config_gpio);

	gpio_pin_config_t switch_config_gpio =
	{ kGPIO_DigitalInput, 1 };

	GPIO_PinInit(GPIOA, 4, &switch_config_gpio);
	GPIO_PinInit(GPIOC, 6, &switch_config_gpio);

	NVIC_EnableIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTC_IRQn);
	NVIC_SetPriority(PORTA_IRQn,5);
	NVIC_SetPriority(PORTC_IRQn,6);

	GPIO_WritePinOutput(GPIOB,21,0);
	g_led_semaphore = xSemaphoreCreateBinary();


	xTaskCreate(led_task, "LED task", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
	xTaskCreate(Counter_task, "Counter task", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-2, NULL);

	vTaskStartScheduler();
	while (1)
	{

	}
	return 0;
}
