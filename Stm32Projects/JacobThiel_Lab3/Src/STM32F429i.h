/*
 * STM32F429i.h
 *
 *  Created on: Jan 28, 2025
 *      Author: Jacob
 */

#ifndef STM32F429I_H_
#define STM32F429I_H_

#include <stdint.h>

#endif /* STM32F429I_H_ */

#define AHB1_BASE_ADDR 0x40020000
#define APB2_BASE_ADDR 0x40010000

#define RCC_BASE_ADDR (AHB1_BASE_ADDR + 0x3800)//

#define EXTI_BASE_ADDR (APB2_BASE_ADDR + 0x3C00)
#define SYSCFG_BASE_ADDR (APB2_BASE_ADDR + 0x3800)

#define SYSCFG ((SYSCFG_RegDef_t*)SYSCFG_BASE_ADDR)
#define EXTI ((EXTI_RegDef_t*)EXTI_BASE_ADDR)

#define RCC_SYSCFG_OFFSET 14
#define SYSCFG_CLOCK_ENABLE (RCC->RCC_APB2LPENR |= (1 << RCC_SYSCFG_OFFSET))
#define SYSCFG_CLOCK_DISABLE (RCC->RCC_APB2LPENR &= ~(1 << RCC_SYSCFG_OFFSET))

#define GPIOA_BASE_ADDR (AHB1_BASE_ADDR)
#define GPIOB_BASE_ADDR (AHB1_BASE_ADDR + 0x0400)
#define GPIOC_BASE_ADDR (AHB1_BASE_ADDR + 0x0800)
#define GPIOD_BASE_ADDR (AHB1_BASE_ADDR + 0x0C00)
#define GPIOE_BASE_ADDR (AHB1_BASE_ADDR + 0x1000)
#define GPIOF_BASE_ADDR (AHB1_BASE_ADDR + 0x1400)
#define GPIOG_BASE_ADDR (AHB1_BASE_ADDR + 0x1800)
#define GPIOH_BASE_ADDR (AHB1_BASE_ADDR + 0x1C00)
#define GPIOI_BASE_ADDR (AHB1_BASE_ADDR + 0x2000)
#define GPIOJ_BASE_ADDR (AHB1_BASE_ADDR + 0x2400)
#define GPIOK_BASE_ADDR (AHB1_BASE_ADDR + 0x2800)


#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASE_ADDR)
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASE_ADDR)
#define GPIOC ((GPIO_RegDef_t*)GPIOC_BASE_ADDR)
#define GPIOD ((GPIO_RegDef_t*)GPIOD_BASE_ADDR)
#define GPIOE ((GPIO_RegDef_t*)GPIOE_BASE_ADDR)
#define GPIOF ((GPIO_RegDef_t*)GPIOF_BASE_ADDR)
#define GPIOG ((GPIO_RegDef_t*)GPIOG_BASE_ADDR)
#define GPIOH ((GPIO_RegDef_t*)GPIOH_BASE_ADDR)
#define GPIOI ((GPIO_RegDef_t*)GPIOI_BASE_ADDR)
#define GPIOJ ((GPIO_RegDef_t*)GPIOJ_BASE_ADDR)
#define GPIOK ((GPIO_RegDef_t*)GPIOK_BASE_ADDR)

#define RCC ((RCC_RegDef_t*)RCC_BASE_ADDR)

#define RCC_AHB1_GPIOA_OFFSET 0
#define RCC_AHB1_GPIOB_OFFSET 1
#define RCC_AHB1_GPIOC_OFFSET 2
#define RCC_AHB1_GPIOD_OFFSET 3
#define RCC_AHB1_GPIOE_OFFSET 4
#define RCC_AHB1_GPIOF_OFFSET 5
#define RCC_AHB1_GPIOG_OFFSET 6
#define RCC_AHB1_GPIOH_OFFSET 7
#define RCC_AHB1_GPIOI_OFFSET 8

#define GPIO_CLOCK_ENABLE(x) (RCC->RCC_AHB1ENR |= (1 << x))
#define GPIO_CLOCK_DISABLE(x) (RCC->RCC_AHB1ENR &= ~(1 << x))

#define NVIC_ISER0 ((volatile uint32_t*) 0xE000E100) //SET-ENABLE REGISTER
#define NVIC_ICER0 ((volatile uint32_t*) 0xE000E180) //CLEAR ENABLE REGISTER
#define NVIC_ISPR0 ((volatile uint32_t*) 0xE000E200) //SET PENDING REGISTER
#define NVIC_ICPR0 ((volatile uint32_t*) 0xE000E280) //CLEAR PENDING REGISTER
#define NVIC_IABR0 ((volatile uint32_t*) 0xE000E300) //ACTIVE BIT REGISTERS
#define NVIC_IPR0 ((volatile uint32_t*) 0xE000E400) //INTERUPT PRIORITY REGISTER
#define STIR ((volatile uint32_t*) 0xE000EF00) //SOFTWARE TRIGGER INTERUOPT REIGSTER

//#define GPIOG_CLOCK_ENABLE()	(RCC->RCC_AHB1ENR |= (1 << 6))
//#define GPIOG_CLOCK_DISABLE()	(RCC->RCC_AHB1ENR &= ~(1 << 6))

//#define GPIOA_CLOCK_ENABLE()	(RCC->RCC_AHB1ENR |= (1))
//#define GPIOA_CLOCK_DISABLE()	(RCC->RCC_AHB1ENR &= ~(1))


#define ACTIVE 1
#define NON_ACTIVE 0
#define SET 1
#define RESET 0
#define ENABLE SET
#define DISABLE RESET

typedef struct { //9 member
 volatile uint32_t GPIOG_MODER; //offset 00
 volatile uint32_t GPIOG_OTYPER; //offset 04
 volatile uint32_t GPIOG_OSPEEDR; //offset 08
 volatile uint32_t GPIOG_PUPDR; //offset 9C
 volatile uint32_t GPIOG_IDR; //offset 0x10
 volatile uint32_t GPIOG_ODR; //offset 0x14
 volatile uint32_t GPIOG_BSRR; //offset 0x18
 volatile uint32_t GPIOG_LCKR; //offset 0x1c
 volatile uint32_t GPIOG_AFRL[2]; //offset 0x20
}GPIO_RegDef_t;

typedef struct {
 volatile uint32_t RCC_CR; //offset 00
 volatile uint32_t RCC_PLLCFGR; //offset 04
 volatile uint32_t RCC_CFGR; //offset 08
 volatile uint32_t RCC_CIR; //offset 0c
 volatile uint32_t RCC_AHB1RSTR; //offset 0x10
 volatile uint32_t RCC_AHB2RSTR; //offset 0x14
 volatile uint32_t RCC_AHB3RSTR; //offset 0x18
 volatile uint32_t RCC_RES1C; //offset 1c
 volatile uint32_t RCC_APB1RSTR; //offset 0x20
 volatile uint32_t RCC_APB2RSTR; //ofset 0x24
 volatile uint32_t RCC_RES28[2]; //offset 28->2C
 volatile uint32_t RCC_AHB1ENR; //offset 0x30
 volatile uint32_t RCC_AHB2ENR; //offset 0x34
 volatile uint32_t RCC_AHB3ENR; //offset 0x38
 volatile uint32_t RCC_RES3C; //offset 0x3c
 volatile uint32_t RCC_APB1ENR; //offset 0x40
 volatile uint32_t RCC_APB2ENR; //offset 0x44
 volatile uint32_t RCC_RES48[2]; //offset 0x48->4c
 volatile uint32_t RCC_AHB1LPENR; //offset 0x50
 volatile uint32_t RCC_AHB2LPENR; //offset 0x54
 volatile uint32_t RCC_AHB3LPENR; //offset 0x58
 volatile uint32_t RCC_RES5C; //offset 0x5c
 volatile uint32_t RCC_APB1LPENR; //offset 0x60
 volatile uint32_t RCC_APB2LPENR; //offset 0x64
 volatile uint32_t RCC_RES68[2]; //offset 0x68->6c
 volatile uint32_t RCC_BDCR; //offset 0x70
 volatile uint32_t RCC_CSR; //offset 0x74
 volatile uint32_t RCC_RES78[2]; //offset 78 ->7c
 volatile uint32_t RCC_SSCGR; //offset 0x80
 volatile uint32_t RCC_PLLI2SCFGR; //offset 0x84
 volatile uint32_t RCC_PLLSAICFGR; //offset 0x88
 volatile uint32_t RCC_DCKCFGR; //offset 8c
}RCC_RegDef_t;

typedef struct {
 volatile uint32_t SYSCFG_MEMRMP; //offset 0x00
 volatile uint32_t SYSCFG_PMC; //offset 0x04
 volatile uint32_t SYSCFG_EXTICR[3]; //offset 0x08 - 0x14
 volatile uint32_t SYSCFG_CMPCR; //offset 0x20
}SYSCFG_RegDef_t;

typedef struct {
 volatile uint32_t EXTI_IMR; //offset 0x00
 volatile uint32_t EXTI_EMR; //offset 0x04
 volatile uint32_t EXTI_RTSR; //offset 0x08
 volatile uint32_t EXTI_FTSR; //offset 0x0C
 volatile uint32_t EXTI_SWIER; //offset 0x10
 volatile uint32_t EXTI_PR; //offset 0x14
}EXTI_RegDef_t;
