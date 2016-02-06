/*
* This file is part of the stmbl project.
*
* Copyright (C) 2013-2015 Rene Hopf <renehopf@mac.com>
* Copyright (C) 2013-2015 Nico Stute <crinq@crinq.de>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stm32f4xx_conf.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "usb_cdc.h"

volatile uint64_t systime = 0.0;
void Wait(unsigned int ms);

void SysTick_Handler(void)
{
  systime++;
}

void setup(){
   //Enable clocks
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
   
   //LEDs as output
   GPIO_Init(GPIOD,  &(GPIO_InitTypeDef){
      .GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
    	.GPIO_Mode  = GPIO_Mode_OUT,
      .GPIO_OType = GPIO_OType_PP,
      .GPIO_Speed = GPIO_Speed_2MHz,
      .GPIO_PuPd  = GPIO_PuPd_NOPULL,
   });
   
   //Button
   GPIO_Init(GPIOA,  &(GPIO_InitTypeDef){
      .GPIO_Pin   = GPIO_Pin_0,
    	.GPIO_Mode  = GPIO_Mode_IN,
      .GPIO_Speed = GPIO_Speed_2MHz,
      .GPIO_PuPd  = GPIO_PuPd_NOPULL,
   });

   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

   usb_init();
   
   RCC_ClocksTypeDef RCC_Clocks;
   RCC_GetClocksFreq(&RCC_Clocks);
   SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
   
   //systick prio
   NVIC_SetPriority(SysTick_IRQn, 14);
}


int main(void)
{
   setup();
   while(1){
      printf("hallo\n");
      if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
         GPIO_SetBits(GPIOD, GPIO_Pin_12);
      }else{
         GPIO_ResetBits(GPIOD, GPIO_Pin_12);
      }
      GPIO_SetBits(GPIOD, GPIO_Pin_13);
      GPIO_SetBits(GPIOD, GPIO_Pin_14);
      GPIO_SetBits(GPIOD, GPIO_Pin_15);
      Wait(100);
      GPIO_ResetBits(GPIOD, GPIO_Pin_13);
      GPIO_ResetBits(GPIOD, GPIO_Pin_14);
      GPIO_ResetBits(GPIOD, GPIO_Pin_15);
      Wait(100);
   }
}

void Wait(unsigned int ms){
   uint64_t t = systime + ms;
   while(t >= systime){}
}
