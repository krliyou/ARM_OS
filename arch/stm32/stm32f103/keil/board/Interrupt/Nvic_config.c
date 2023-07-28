
/**
* @file           : Nvic_config.c
* @autor          : liyou
* @date           : 2022/01/16
* All rights reserved.;
* @attention
*/

#include "Nvic_config.h"

/**
  @brief   Encode Priority
  @details Encodes the priority for an interrupt with the given priority group,
           preemptive priority value, and subpriority value.
           In case of a conflict between priority grouping and available
           priority bits (__NVIC_PRIO_BITS), the smallest possible priority group is set.
  @param [in]   PreemptPriority  Preemptive priority value (starting from 0).
  @param [in]       SubPriority  Subpriority value (starting from 0).
  @return                        Encoded priority. Value can be used in the function \ref NVIC_SetPriority().
 */
unsigned int __NVIC_EncodePriority(unsigned int PriorityGroup,
		unsigned int PreemptPriority, unsigned int SubPriority)
{
    unsigned int PriorityGroupTmp = \
    (PriorityGroup & (unsigned int)0x07UL);   /* only values 0..7 are used          */
    unsigned int PreemptPriorityBits;
    unsigned int SubPriorityBits;

    PreemptPriorityBits = \
    ((7UL - PriorityGroupTmp) > \
    (unsigned int)(__NVIC_PRIO_BITS)) ? \
    (unsigned int)(__NVIC_PRIO_BITS) :  \
    (unsigned int)(7UL - PriorityGroupTmp);    
    
    SubPriorityBits =   \
    ((PriorityGroupTmp + (unsigned int)(__NVIC_PRIO_BITS)) <    \
    (unsigned int)7UL) ? (unsigned int)0UL :    \
    (unsigned int)((PriorityGroupTmp - 7UL) +   \
    (unsigned int)(__NVIC_PRIO_BITS));

    return 
    (((PreemptPriority & (unsigned int) \
    ((1UL << (PreemptPriorityBits)) - 1UL)) << SubPriorityBits) | \
    ((SubPriority & (unsigned int)((1UL << (SubPriorityBits)) - 1UL))));
}

/**
  @brief   Disable Interrupt
  @details Disables a device specific interrupt in the NVIC interrupt controller.
  @param [in]      IRQn  Device specific interrupt number.
  @note    IRQn must not be negative.
 */
void __NVIC_DisableIRQ(IRQn_Type IRQn)
{
	NVIC->ICER[(((unsigned int)IRQn) >> 5UL)] = \
		(unsigned int)(1UL << (((unsigned int)IRQn) & 0x1FUL));
    __DSB();
    __ISB();
}

/**
  @brief   Enable Interrupt
  @details Enables a device specific interrupt in the NVIC interrupt controller.
  @param [in]      IRQn  Device specific interrupt number.
  @note    IRQn must not be negative.
 */
void __NVIC_EnableIRQ(IRQn_Type IRQn)
{
	NVIC->ISER[(((unsigned int)IRQn) >> 5UL)] = \
    (unsigned int)(1UL << (((unsigned int)IRQn) & 0x1FUL));
}

/**
  @brief   Set Interrupt Priority
  @details Sets the priority of a device specific interrupt or a processor exception.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  @param [in]      IRQn  Interrupt number.
  @param [in]  priority  Priority to set.
  @note    The priority cannot be set for every processor exception.
 */
void __NVIC_SetPriority(IRQn_Type IRQn, unsigned int priority)
{
	if ((int32_t)(IRQn) < 0)
    	SCB->SHP[(((unsigned int)IRQn) & 0xFUL)-4UL] = \
		(unsigned char)((priority << (8U - __NVIC_PRIO_BITS)) & (unsigned int)0xFFUL);
  
  	NVIC->IP[((unsigned int)IRQn)] = \
			(unsigned char)((priority << (8U - __NVIC_PRIO_BITS)) & (unsigned int)0xFFUL);
}

/**
  @brief   Get Interrupt Priority
  @details Reads the priority of a device specific interrupt or a processor exception.
           The interrupt number can be positive to specify a device specific interrupt,
           or negative to specify a processor exception.
  @param [in]   IRQn  Interrupt number.
  @return             Interrupt Priority.
                      Value is aligned automatically to the implemented priority bits of the microcontroller.
 */
unsigned int __T_NVIC_GetPriority(IRQn_Type IRQn)
{
	if ((int32_t)(IRQn) < 0)
		return (((unsigned int)SCB->SHP[(((unsigned int)IRQn) & 0xFUL)-4UL] >> 	\
			(8U - __NVIC_PRIO_BITS)));
  
    return(((unsigned int)NVIC->IP[((unsigned int)IRQn)] >> \
		(8U - __NVIC_PRIO_BITS)));
}

/**
  @brief   Set Priority Grouping
  @details Sets the priority grouping field using the required unlock sequence.
           The parameter PriorityGroup is assigned to the field SCB->AIRCR [10:8] PRIGROUP field.
           Only values from 0..7 are used.
           In case of a conflict between priority grouping and available
           priority bits (__NVIC_PRIO_BITS), the smallest possible priority group is set.
  @param [in]      PriorityGroup  Priority grouping field.
 */

// #define NVIC_PRIORITYGROUP_0         0x00000007U /*!< 0 bits for pre-emption priority
//                                                       4 bits for subpriority */
// #define NVIC_PRIORITYGROUP_1         0x00000006U /*!< 1 bits for pre-emption priority
//                                                       3 bits for subpriority */
// #define NVIC_PRIORITYGROUP_2         0x00000005U /*!< 2 bits for pre-emption priority
//                                                       2 bits for subpriority */
// #define NVIC_PRIORITYGROUP_3         0x00000004U /*!< 3 bits for pre-emption priority
//                                                       1 bits for subpriority */
// #define NVIC_PRIORITYGROUP_4         0x00000003U /*!< 4 bits for pre-emption priority
//                                                       0 bits for subpriority */

void __NVIC_SetPriorityGrouping(unsigned int PriorityGroup)
{
	uint32_t reg_value;
  	uint32_t PriorityGroupTmp = \
    	(~PriorityGroup & (uint32_t)0x07UL);          

  	reg_value	=	SCB->AIRCR;                                                   
  	reg_value	&=	~((uint32_t)(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk));          
  	reg_value   =	(reg_value | \
					((uint32_t)0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
					(PriorityGroupTmp << SCB_AIRCR_PRIGROUP_Pos) );      
				         
	SCB->AIRCR  =  reg_value;
}

/**
  @brief   Decode Priority
  @details Decodes an interrupt priority value with a given priority group to
           preemptive priority value and subpriority value.
           In case of a conflict between priority grouping and available
           priority bits (__NVIC_PRIO_BITS) the smallest possible priority group is set.
  @param [in]         Priority   Priority value, which can be retrieved with the function \ref NVIC_GetPriority().
  @param [in]     PriorityGroup  Used priority group.
  @param [out] pPreemptPriority  Preemptive priority value (starting from 0).
  @param [out]     pSubPriority  Subpriority value (starting from 0).
 */
void __NVIC_DecodePriority 	
		(unsigned int Priority, unsigned int PriorityGroup,
		unsigned int* const pPreemptPriority, unsigned int* const pSubPriority)
{
	unsigned int PriorityGroupTmp = (PriorityGroup & (unsigned int)0x07UL);  
	unsigned int PreemptPriorityBits;
	unsigned int SubPriorityBits;

	PreemptPriorityBits = \
		((7UL - PriorityGroupTmp) > (unsigned int)(__NVIC_PRIO_BITS)) ? \
		(unsigned int)(__NVIC_PRIO_BITS) : (unsigned int)(7UL - PriorityGroupTmp);
  
  	SubPriorityBits     = \
  		((PriorityGroupTmp + (unsigned int)(__NVIC_PRIO_BITS)) < \
		(unsigned int)7UL) ? (unsigned int)0UL : (unsigned int)((PriorityGroupTmp - 7UL) + \
		(unsigned int)(__NVIC_PRIO_BITS));

	*pPreemptPriority = \
		(Priority >> SubPriorityBits) & \
		(unsigned int)((1UL << (PreemptPriorityBits)) - 1UL);
  
	*pSubPriority     = \
		(Priority) & (unsigned int)((1UL << (SubPriorityBits)) - 1UL);
}

/**
  @brief   Set Pending Interrupt
  @details Sets the pending bit of a device specific interrupt in the NVIC pending register.
  @param [in]      IRQn  Device specific interrupt number.
  @note    IRQn must not be negative.
 */
void __NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
	NVIC->ISPR[(((unsigned int)IRQn) >> 5UL)] = \
		(unsigned int)(1UL << (((unsigned int)IRQn) & 0x1FUL));
}

/**
  @brief   Get Pending Interrupt
  @details Reads the NVIC pending register and returns the pending bit for the specified device specific interrupt.
  @param [in]      IRQn  Device specific interrupt number.
  @return             0  Interrupt status is not pending.
  @return             1  Interrupt status is pending.
  @note    IRQn must not be negative.
 */
unsigned int __NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
    return ((unsigned int)(((NVIC->ISPR[(((unsigned int)IRQn) >> 5UL)] & \
		(1UL << (((unsigned int)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
}

/**
  @brief   Clear Pending Interrupt
  @details Clears the pending bit of a device specific interrupt in the NVIC pending register.
  @param [in]      IRQn  Device specific interrupt number.
  @note    IRQn must not be negative.
 */
void __NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
	NVIC->ICPR[(((unsigned int)IRQn) >> 5UL)] = \
		(unsigned int)(1UL << (((unsigned int)IRQn) & 0x1FUL));
}

/**
  @brief   Get Active Interrupt
  @details Reads the active register in the NVIC and returns the active bit for the device specific interrupt.
  @param [in]      IRQn  Device specific interrupt number.
  @return             0  Interrupt status is not active.
  @return             1  Interrupt status is active.
  @note    IRQn must not be negative.
 */
unsigned int __NVIC_GetActive(IRQn_Type IRQn)
{
	return((unsigned int)(((NVIC->IABR[(((unsigned int)IRQn) >> 5UL)] & \
		(1UL << (((unsigned int)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
}

