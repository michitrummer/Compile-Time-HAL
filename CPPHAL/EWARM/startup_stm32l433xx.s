;********************************************************************************
;* File Name          : startup_stm32l433xx.s
;* Author             : MCD Application Team
;* Description        : STM32L433xx Ultra Low Power Devices vector
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == _iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR
;*                        address.
;*                      - Branches to main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the Cortex-M4 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;********************************************************************************
;*
;* <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
;* All rights reserved.</center></h2>
;*
;* This software component is licensed by ST under BSD 3-Clause license,
;* the "License"; You may not use this file except in compliance with the
;* License. You may obtain a copy of the License at:
;*                        opensource.org/licenses/BSD-3-Clause
;*
;*******************************************************************************
;
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler             ; Reset Handler

        DCD     Default_Handler               ; NMI Handler
        DCD     Default_Handler         ; Hard Fault Handler
        DCD     Default_Handler         ; MPU Fault Handler
        DCD     Default_Handler          ; Bus Fault Handler
        DCD     Default_Handler        ; Usage Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     Default_Handler               ; SVCall Handler
        DCD     Default_Handler          ; Debug Monitor Handler
        DCD     0                         ; Reserved
        DCD     Default_Handler            ; PendSV Handler
        DCD     Default_Handler           ; SysTick Handler

         ; External Interrupts
        DCD     Default_Handler                   ; Window WatchDog
        DCD     Default_Handler                ; PVD/PVM1/PVM2/PVM3/PVM4 through EXTI Line detection
        DCD     Default_Handler             ; Tamper and TimeStamps through the EXTI line
        DCD     Default_Handler               ; RTC Wakeup through the EXTI line
        DCD     Default_Handler                  ; FLASH
        DCD     Default_Handler                    ; RCC
        DCD     Default_Handler                  ; EXTI Line0
        DCD     Default_Handler                  ; EXTI Line1
        DCD     Default_Handler                  ; EXTI Line2
        DCD     Default_Handler                  ; EXTI Line3
        DCD     Default_Handler                  ; EXTI Line4
        DCD     Default_Handler          ; DMA1 Channel 1
        DCD     Default_Handler          ; DMA1 Channel 2
        DCD     Default_Handler          ; DMA1 Channel 3
        DCD     Default_Handler          ; DMA1 Channel 4
        DCD     Default_Handler          ; DMA1 Channel 5
        DCD     Default_Handler          ; DMA1 Channel 6
        DCD     Default_Handler          ; DMA1 Channel 7
        DCD     Default_Handler                   ; ADC1
        DCD     Default_Handler                ; CAN1 TX
        DCD     Default_Handler               ; CAN1 RX0
        DCD     Default_Handler               ; CAN1 RX1
        DCD     Default_Handler               ; CAN1 SCE
        DCD     Default_Handler                ; External Line[9:5]s
        DCD     Default_Handler         ; TIM1 Break and TIM15
        DCD     Default_Handler          ; TIM1 Update and TIM16
        DCD     Default_Handler           ; TIM1 Trigger and Commutation
        DCD     Default_Handler                ; TIM1 Capture Compare
        DCD     Default_Handler                   ; TIM2
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     Default_Handler                ; I2C1 Event
        DCD     Default_Handler                ; I2C1 Error
        DCD     Default_Handler                ; I2C2 Event
        DCD     Default_Handler                ; I2C2 Error
        DCD     Default_Handler                   ; SPI1
        DCD     Default_Handler                   ; SPI2
        DCD     Default_Handler                 ; USART1
        DCD     Default_Handler                 ; USART2
        DCD     Default_Handler                 ; USART3
        DCD     Default_Handler              ; External Line[15:10]
        DCD     Default_Handler              ; RTC Alarm (A and B) through EXTI Line
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     Default_Handler                 ; SDMMC1
        DCD     0                                 ; Reserved
        DCD     Default_Handler                   ; SPI3
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     Default_Handler               ; TIM6 and DAC1&2 underrun errors
        DCD     Default_Handler                   ; TIM7
        DCD     Default_Handler          ; DMA2 Channel 1
        DCD     Default_Handler          ; DMA2 Channel 2
        DCD     Default_Handler          ; DMA2 Channel 3
        DCD     Default_Handler          ; DMA2 Channel 4
        DCD     Default_Handler          ; DMA2 Channel 5
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     0                                 ; Reserved
        DCD     Default_Handler                   ; COMP Interrupt
        DCD     Default_Handler                 ; LP TIM1 interrupt
        DCD     Default_Handler                 ; LP TIM2 interrupt
        DCD     Default_Handler                    ; USB FS
        DCD     Default_Handler          ; DMA2 Channel 6
        DCD     Default_Handler          ; DMA2 Channel 7
        DCD     Default_Handler                ; LP UART 1 interrupt
        DCD     Default_Handler                ; Quad SPI global interrupt
        DCD     Default_Handler                ; I2C3 event
        DCD     Default_Handler                ; I2C3 error
        DCD     Default_Handler                   ; Serial Audio Interface 1 global interrupt
        DCD     0                                 ; Reserved
        DCD     Default_Handler                 ; Serial Wire Interface global interrupt
        DCD     Default_Handler                    ; Touch Sense Controller global interrupt
        DCD     Default_Handler                    ; LCD global interrupt
        DCD     0                                 ; Reserved
        DCD     Default_Handler                    ; RNG global interrupt
        DCD     Default_Handler                    ; FPU interrupt
        DCD     Default_Handler                    ; CRS interrupt

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        PUBWEAK Default_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Default_Handler
Infinite_Loop
        B     Infinite_Loop
        
        THUMB
        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
        LDR     R0, =__iar_program_start
        BX      R0

        END
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
