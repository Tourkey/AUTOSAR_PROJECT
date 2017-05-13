/*
 * Eep_Cfg.h
 *
 *  Created on: May 10, 2017
 *      Author: power
 */

#ifndef EEP_CFG_H_
#define EEP_CFG_H_
#include "Memlf_Types.h"
#define EepDevErrorDetect True
#define EepDriverIndex 1
#define EepUseInterrupts True
#define EepVersionInfoApi True
#define EepWriteCycleReduction False
//****************************************************************************************************************************
//eep init config container
//****************************************************************************************************************************
typedef struct{
    Eep_AddressType EepBaseAddress;
    MemIf_ModeType EepDefaultMode;
    Eep_LengthType EepFastReadBlockSize;
    Eep_LengthType EepFastWriteBlockSize;
    //float EepJobCallCycle;
    void (*EepJobEndNotification)(void);
    void (*EepJobErrorNotification)(void);
    Eep_LengthType EepNormalReadBlockSize;
    Eep_LengthType EepNormalWriteBlockSize;
    Eep_LengthType EepSize;
}Eep_ConfigType;
//*****************************************************************************************************************************
//EepPublishedInformation
//****************************************************************************************************************************
#define EepAllowedWriteCycles 0//Specified maximum number of write cycles
#define EepEraseTime (float)0//Maximum time for erasing one EEPROM data unit.
#define EepEraseUnitSize 0//Size of smallest erasable EEPROM data unit in bytes.
#define EepEraseValue 0//Value of an erased EEPROM cell.
#define EepMinimumAddressType 0//Minimum expected size of Eep_AddressType.
#define EepMinimumLengthType 0//Minimum expected size of Eep_LengthType.
#define EepReadUnitSize 0//Size of smallest readable EEPROM data unit in bytes.
#define EepSpecifiedEraseCycles 0//Number of erase cycles specified for the EEP device
#define EepTotalSize 0//Total size of EEPROM in bytes.
#define EepWriteTime (float)0//Maximum time for writing one EEPROM data unit.
#define EepWriteUnitSize 0//Size of smallest writeable EEPROM data unit in bytes.
//****************************************************************************************************************************









#endif /* EEP_CFG_H_ */
