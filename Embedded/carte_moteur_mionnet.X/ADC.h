/* 
 * File:   ADC.h
 * Author: TP-EO-5
 *
 * Created on 11 février 2021, 14:14
 */

#ifndef ADC_H
#define	ADC_H

void InitADC1(void);

/* This is ADC interrupt routine */
void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void);

void ADC1StartConversionSequence();

unsigned int * ADCGetResult(void);

unsigned char ADCIsConversionFinished(void);

void ADCClearConversionFinishedFlag(void);

#endif	/* ADC_H */

