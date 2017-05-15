/*
 * Eep.h
 *
 *  Created on: May 10, 2017
 *      Author: power
 */

#ifndef EEP_H_
#define EEP_H_
#include "Eep_Cfg.h"
#include "Std_Types.h"
//optional//
//#include "Spi.h"
//Eep_ConfigType//

//Eep_AddressType//
typedef uint32_t Eep_AddressType;
//Eep_LengthType//
typedef uint32_t Eep_LengthType;
//Init//
extern void Eep_Init( const Eep_ConfigType* ConfigPtr );
extern void Eep_SetMode( MemIf_ModeType Mode );
extern Std_ReturnType Eep_Read( Eep_AddressType EepromAddress, uint8* DataBufferPtr, Eep_LengthType Length );
extern Std_ReturnType Eep_Write( Eep_AddressType EepromAddress, const uint8* DataBufferPtr, Eep_LengthType Length );
extern Std_ReturnType Eep_Erase( Eep_AddressType EepromAddress, Eep_LengthType Length );
extern Std_ReturnType Eep_Compare( Eep_AddressType EepromAddress, const uint8* DataBufferPtr, Eep_LengthType Length );
extern void Eep_Cancel( void );
extern MemIf_StatusType Eep_GetStatus( void );
extern MemIf_JobResultType Eep_GetJobResult( void );
extern void Eep_GetVersionInfo( Std_VersionInfoType* versioninfo );
//Scheduled_FUnctions//
extern void Eep_MainFunction( void );
//End Job Notification//
extern void Eep_JobEndNotification( void );
//Error Job Notification//
extern void Eep_JobErrorNotification( void );
#endif /* EEP_H_ */
