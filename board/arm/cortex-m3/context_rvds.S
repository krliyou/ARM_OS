

SCB_VTOR        EQU     0xE000ED08               ; Vector Table Offset Register
NVIC_INT_CTRL   EQU     0xE000ED04               ; interrupt control state register
NVIC_SYSPRI2    EQU     0xE000ED20               ; system priority register (2)
NVIC_PENDSV_PRI EQU     0xFFFF0000               ; PendSV and SysTick priority value (lowest)
NVIC_PENDSVSET  EQU     0x10000000               ; value to trigger PendSV exception

    AREA |.text|, CODE, READONLY, ALIGN=2
    THUMB
    REQUIRE8
    PRESERVE8

    IMPORT ly_thread_switch_interrupt_flag
    IMPORT ly_interrupt_from_thread
    IMPORT ly_interrupt_to_thread

interrupt_disable	PROC
	EXPORT	interrupt_disable
	MRS		r0,	PRIMASK
	CPSID	I
	BX		LR
	ENDP

interrupt_enable	PROC
	EXPORT	interrupt_enable
	MSR		PRIMASK,	r0
	BX		LR
	ENDP

;
;  void rt_hw_context_switch(rt_uint32 from, rt_uint32 to);
;  r0 -- from
;  r1 -- to
; 
ly_context_switch_interrupt
    EXPORT ly_context_switch_interrupt
ly_context_switch    PROC
    EXPORT ly_context_switch

    ; set rt_thread_switch_interrupt_flag to 1
    LDR     r2, =ly_thread_switch_interrupt_flag
    LDR     r3, [r2]
    CMP     r3, #1
    BEQ     _reswitch
    MOV     r3, #1
    STR     r3, [r2]

    LDR     r2, =ly_interrupt_from_thread  
    STR     r0, [r2]

_reswitch
    LDR     r2, =ly_interrupt_to_thread     
    STR     r1, [r2]

    LDR     r0, =NVIC_INT_CTRL              
    LDR     r1, =NVIC_PENDSVSET
    STR     r1, [r0]
    BX      LR
    ENDP

; r0 -- switch from thread stack
; r1 -- switch to thread stack
; psr, pc, lr, r12, r3, r2, r1, r0 are pushed into [from] stack
PendSV_Handler   PROC
    EXPORT PendSV_Handler

    MRS     r2, PRIMASK
    CPSID   I

    LDR     r0, =ly_thread_switch_interrupt_flag
    LDR     r1, [r0]
    CBZ     r1, pendsv_exit         

    MOV     r1, #0x00
    STR     r1, [r0]

    LDR     r0, =ly_interrupt_from_thread
    LDR     r1, [r0]
    CBZ     r1, switch_to_thread    

    MRS     r1, psp                
    STMFD   r1!, {r4 - r11}        
    LDR     r0, [r0]
    STR     r1, [r0]                

switch_to_thread
    LDR     r1, =ly_interrupt_to_thread
    LDR     r1, [r1]
    LDR     r1, [r1]                

    LDMFD   r1!, {r4 - r11}        
    MSR     psp, r1              

pendsv_exit
    MSR     PRIMASK, r2

    ORR     lr, lr, #0x04
    BX      lr
    ENDP

;
;  void rt_hw_context_switch_to(rt_uint32 to);
;  r0 -- to
;  this fucntion is used to perform the first thread switch
; 
ly_context_switch_to        PROC
    EXPORT ly_context_switch_to    

    LDR     r1, =ly_interrupt_to_thread
    STR     r0, [r1]

    LDR     r1, =ly_interrupt_from_thread
    MOV     r0, #0x0
    STR     r0, [r1]

    LDR     r1, =ly_thread_switch_interrupt_flag
    MOV     r0, #1
    STR     r0, [r1]

    LDR     r0, =NVIC_SYSPRI2
    LDR     r1, =NVIC_PENDSV_PRI
    LDR.W   r2, [r0,#0x00]      
    ORR     r1,r1,r2            
    STR     r1, [r0]           

    LDR     r0, =NVIC_INT_CTRL
    LDR     r1, =NVIC_PENDSVSET
    STR     r1, [r0]

    LDR     r0, =SCB_VTOR
    LDR     r0, [r0]
    LDR     r0, [r0]
    MSR     msp, r0

    CPSIE   F
    CPSIE   I

    DSB
    ISB

    ENDP


ly_interrupt_thread_switch PROC
    EXPORT ly_interrupt_thread_switch
    BX      lr
    ENDP

   
    ALIGN   4

    END
