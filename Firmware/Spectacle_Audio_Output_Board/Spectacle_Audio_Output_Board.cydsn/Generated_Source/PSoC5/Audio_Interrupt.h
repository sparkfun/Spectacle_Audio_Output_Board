/*******************************************************************************
* File Name: Audio_Interrupt.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_Audio_Interrupt_H)
#define CY_ISR_Audio_Interrupt_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Audio_Interrupt_Start(void);
void Audio_Interrupt_StartEx(cyisraddress address);
void Audio_Interrupt_Stop(void);

CY_ISR_PROTO(Audio_Interrupt_Interrupt);

void Audio_Interrupt_SetVector(cyisraddress address);
cyisraddress Audio_Interrupt_GetVector(void);

void Audio_Interrupt_SetPriority(uint8 priority);
uint8 Audio_Interrupt_GetPriority(void);

void Audio_Interrupt_Enable(void);
uint8 Audio_Interrupt_GetState(void);
void Audio_Interrupt_Disable(void);

void Audio_Interrupt_SetPending(void);
void Audio_Interrupt_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Audio_Interrupt ISR. */
#define Audio_Interrupt_INTC_VECTOR            ((reg32 *) Audio_Interrupt__INTC_VECT)

/* Address of the Audio_Interrupt ISR priority. */
#define Audio_Interrupt_INTC_PRIOR             ((reg8 *) Audio_Interrupt__INTC_PRIOR_REG)

/* Priority of the Audio_Interrupt interrupt. */
#define Audio_Interrupt_INTC_PRIOR_NUMBER      Audio_Interrupt__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Audio_Interrupt interrupt. */
#define Audio_Interrupt_INTC_SET_EN            ((reg32 *) Audio_Interrupt__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Audio_Interrupt interrupt. */
#define Audio_Interrupt_INTC_CLR_EN            ((reg32 *) Audio_Interrupt__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Audio_Interrupt interrupt state to pending. */
#define Audio_Interrupt_INTC_SET_PD            ((reg32 *) Audio_Interrupt__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Audio_Interrupt interrupt. */
#define Audio_Interrupt_INTC_CLR_PD            ((reg32 *) Audio_Interrupt__INTC_CLR_PD_REG)


#endif /* CY_ISR_Audio_Interrupt_H */


/* [] END OF FILE */
