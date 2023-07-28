
/**
* @file           : Nvic_config.h
* @autor          : liyou
* @date           : 2022/01/16
* All rights reserved.;
* @attention
*/

/*
 * SystemReset();
 * Nvic_GetPriorityGrouping();
 * NVIC_SetPriorityGrouping(PriorityGroup); PriorityGroup: 0, 1, 2, 3,
 * NVIC_GetPriority(IRQn, PriorityGroup, pPreemptPriority, pSubPriority) 
 * NVIC_SetPriority(IRQn_Type IRQn, unsigned int PreemptPriority, unsigned int SubPriority)   
 * NVIC_EnableIRQ(EXTI2_IRQn);
 * NVIC_DisableIRQ(E_IRQn);
 * NVIC_GetPendingIRQ(IRQn_Type IRQn);
 * NVIC_SetPendingIRQ(IRQn_Type IRQn);
 * NVIC_ClearPendingIRQ(IRQn);
 * NVIC_GetActive(IRQn);
 *
*/

#ifndef __NVIC_CONFIG__
#define __NVIC_CONFIG__

#include "gpio_register.h"
#include "Nvic_compiler.h"

#define __is_PriorityGroup(PriorityGroup)	\
    (PriorityGroup == 0 || PriorityGroup == 1 || \
     PriorityGroup == 2 || PriorityGroup == 3 || PriorityGroup == 4)

/**
  @brief   Get Priority Grouping
  @details Reads the priority grouping field from the NVIC Interrupt Controller.
  @return                Priority grouping field (SCB->AIRCR [10:8] PRIGROUP field).
 */
#define Nvic_GetPriorityGrouping()  \
    (~__GetPriorityGrouping() & 0x07UL)

#define __GetPriorityGrouping()   \
    (unsigned int)((SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos)

#define __is_PreemptPriority(PreemptPriority)   \
    (PreemptPriority < 0x10U)

#define __is_SubPriority(SubPriority)   \
    (SubPriority < 0x10U)

#define __is_ExTI2_IRQn(IRQn)     \
    ((IRQn) >= (IRQn_Type)0x00U)

/**
  * @brief  Sets the priority grouping field (preemption priority and subpriority)
  *         using the required unlock sequence.
  * @param  PriorityGroup: The priority grouping bits length. 
  *         This parameter can be one of the following values:
  *         @arg NVIC_PRIORITYGROUP_0: 0 bits for preemption priority
  *                                    4 bits for subpriority
  *         @arg NVIC_PRIORITYGROUP_1: 1 bits for preemption priority
  *                                    3 bits for subpriority
  *         @arg NVIC_PRIORITYGROUP_2: 2 bits for preemption priority
  *                                    2 bits for subpriority
  *         @arg NVIC_PRIORITYGROUP_3: 3 bits for preemption priority
  *                                    1 bits for subpriority
  *         @arg NVIC_PRIORITYGROUP_4: 4 bits for preemption priority
  *                                    0 bits for subpriority
  * @note   When the NVIC_PriorityGroup_0 is selected, IRQ preemption is no more possible. 
  *         The pending IRQ priority will be managed only by the subpriority. 
  * @retval None
  */
#define NVIC_SetPriorityGrouping(PriorityGroup) \
({\
    unsigned char __PriorityGroup = (PriorityGroup);    \
    (__is_PriorityGroup(__PriorityGroup)) ? \
    __NVIC_SetPriorityGrouping(__PriorityGroup) : \
    -1; \
})

/**
  * @brief  Sets the priority of an interrupt.
  * @param  IRQn: External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *  
  * @param  PreemptPriority: The preemption priority for the IRQn channel.
  *         This parameter can be a value between 0 and 15
  *         A lower priority value indicates a higher priority.     
  * @param  SubPriority: the subpriority level for the IRQ channel.
  *         This parameter can be a value between 0 and 15
  *         A lower priority value indicates a higher priority.          
  * @retval None
  */
#define NVIC_SetPriority(IRQn, PreemptPriority, SubPriority)    \
({  \
    IRQn_Type __IRQn = (IRQn);  \
    unsigned int __PreemptPriority = (PreemptPriority);    \
    unsigned int __SubPriority = (SubPriority);    \
    unsigned int __PriorityGroup = __GetPriorityGrouping();  \
    (__is_PreemptPriority(__PreemptPriority) &&  \
    __is_SubPriority(__SubPriority)) ?  \
    ({  \
        unsigned int __Priority = \
        __NVIC_EncodePriority(__PriorityGroup, __PreemptPriority, __SubPriority);   \
        __NVIC_SetPriority(__IRQn, __Priority);    \
    }) : \
    -1; \
})

/**
  * @brief  Enables a device specific interrupt in the NVIC interrupt controller.
  * @note   To configure interrupts priority correctly, the NVIC_PriorityGroupConfig()
  *         function should be called before. 
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete STM32 Devices IRQ Channels list, please refer to the appropriate CMSIS device file (stm32f10xxx.h))
  * @retval None
  */
#define NVIC_EnableIRQ(E_IRQn)  \
({  \
    IRQn_Type __E_IRQn = (E_IRQn);  \
    __is_ExTI2_IRQn(__E_IRQn) ?  \
    __NVIC_EnableIRQ(__E_IRQn) : \
    -1; \
})

/**
  * @brief  Disables a device specific interrupt in the NVIC interrupt controller.
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete STM32 Devices IRQ Channels list, please refer to the appropriate CMSIS device file (stm32f10xxx.h))  
  * @retval None
  */
#define NVIC_DisableIRQ(E_IRQn)   \
({  \
    IRQn_Type __E_IRQn = (E_IRQn);  \
    __is_ExTI2_IRQn(__E_IRQn) ?  \
    __NVIC_DisableIRQ(__E_IRQn) : \
    -1; \
})

/**
  @brief   System Reset
  @details Initiates a system reset request to reset the MCU.
 */
#define SystemReset() \
({ \
	__DSB(); \
	SCB->AIRCR = \
	(unsigned int)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |	\
	(SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |	\
    (SCB_AIRCR_SYSRESETREQ_Msk));	\
  	__DSB(); \
  	for(;;) \
  	{\
    	__NOP();	\
   	}\
})

/**
  * @brief  Gets the priority of an interrupt.
  * @param  IRQn: External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete STM32 Devices IRQ Channels list, please refer to the appropriate CMSIS device file (stm32f10xxx.h))
  * @param   PriorityGroup: the priority grouping bits length.
  *         This parameter can be one of the following values:
  *           @arg NVIC_PRIORITYGROUP_0: 0 bits for preemption priority
  *                                      4 bits for subpriority
  *           @arg NVIC_PRIORITYGROUP_1: 1 bits for preemption priority
  *                                      3 bits for subpriority
  *           @arg NVIC_PRIORITYGROUP_2: 2 bits for preemption priority
  *                                      2 bits for subpriority
  *           @arg NVIC_PRIORITYGROUP_3: 3 bits for preemption priority
  *                                      1 bits for subpriority
  *           @arg NVIC_PRIORITYGROUP_4: 4 bits for preemption priority
  *                                      0 bits for subpriority
  * @param  pPreemptPriority: Pointer on the Preemptive priority value (starting from 0).
  * @param  pSubPriority: Pointer on the Subpriority value (starting from 0).
  * @retval None
  */
#define NVIC_GetPriority(IRQn, PriorityGroup, pPreemptPriority, pSubPriority)   \
({  \
    IRQn_Type __IRQn = (IRQn_Type); \
    unsigned int __PriorityGroup = (PriorityGroup); \
    typeof(pPreemptPriority + 1) __pPreemptPriority = (pPreemptPriority);   \
    typeof(pSubPriority + 1) __pSubPriority = (pSubPriority);   \
    __is_PriorityGroup(__PriorityGroup) ?   \
    __NVIC_DecodePriority(__T_NVIC_GetPriority(IRQn), __PriorityGroup, __pPreemptPriority, __pSubPriority) :  \
    -1; \
})

/**
  * @brief  Sets Pending bit of an external interrupt.
  * @param  IRQn External interrupt number
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete STM32 Devices IRQ Channels list, please refer to the appropriate CMSIS device file (stm32f10xxx.h))  
  * @retval None
  */
#define NVIC_SetPendingIRQ(IRQn) \
({  \
    IRQn_Type __IRQn = (IRQn); \
    (__is_ExTI2_IRQn(__IRQn)) ?  \
    __NVIC_SetPendingIRQ(__IRQn) : \
    -1; \
})

/**
  * @brief  Gets Pending Interrupt (reads the pending register in the NVIC 
  *         and returns the pending bit for the specified interrupt).
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete STM32 Devices IRQ Channels list, please refer to the appropriate CMSIS device file (stm32f10xxx.h))  
  * @retval status: - 0  Interrupt status is not pending.
  *                 - 1  Interrupt status is pending.
  *  Return 1 if pending else 0
  */
#define NVIC_GetPendingIRQ(IRQn)    \
({  \
    IRQn_Type __IRQn = (IRQn); \
    (__is_ExTI2_IRQn(__IRQn)) ?  \
    __NVIC_GetPendingIRQ(__IRQn) : \
    -1; \
})

/**
  * @brief  Clears the pending bit of an external interrupt.
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete STM32 Devices IRQ Channels list, please refer to the appropriate CMSIS device file (stm32f10xxx.h))  
  * @retval None
  */
#define NVIC_ClearPendingIRQ(IRQn)  \
({  \
    IRQn_Type __IRQn = (IRQn); \
    (__is_ExTI2_IRQn(__IRQn)) ?  \
    __NVIC_ClearPendingIRQ(__IRQn) : \
    -1; \
})

/**
  * @brief Gets active interrupt ( reads the active register in NVIC and returns the active bit).
  * @param IRQn External interrupt number
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete STM32 Devices IRQ Channels list, please refer to the appropriate CMSIS device file (stm32f10xxx.h))  
  * @retval status:  0  Interrupt status is not pending.
  *                  1  Interrupt status is pending.
  */
#define NVIC_GetActive(IRQn)    \
({  \
    IRQn_Type __IRQn = (IRQn); \
    (__is_ExTI2_IRQn(__IRQn)) ?  \
    __NVIC_GetActive(__IRQn) : \
    0; \
})

unsigned int __NVIC_EncodePriority 
    (unsigned int PriorityGroup, unsigned int PreemptPriority, unsigned int SubPriority);

void __NVIC_EnableIRQ(IRQn_Type IRQn);

void __NVIC_DisableIRQ(IRQn_Type IRQn);

void __NVIC_SetPriority(IRQn_Type IRQn, unsigned int priority);

void __NVIC_SetPriorityGrouping(unsigned int PriorityGroup);

void __NVIC_DecodePriority 	
		(unsigned int Priority, unsigned int PriorityGroup,
		unsigned int* const pPreemptPriority, unsigned int* const pSubPriority);

unsigned int __T_NVIC_GetPriority(IRQn_Type IRQn);

void __NVIC_SetPendingIRQ(IRQn_Type IRQn);

unsigned int __NVIC_GetPendingIRQ(IRQn_Type IRQn);

void __NVIC_ClearPendingIRQ(IRQn_Type IRQn);

#endif

