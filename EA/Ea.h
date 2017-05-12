#ifndef __EA_H
#define __EA_H

#include "Std_Types.h"
#include "Ea_Cfg.h"


#ifdef EaNvmJobEndNotification
	#define EaNvmEnableJobEndNotification true
#else
	#define EaNvmEnableJobEndNotification false 	
#endif


#ifndef EaDevErrorDetect
#warning "EaDevErrorDetect is not defined by user and set to its default value (false)"
#define EaDevErrorDetect false
#endif

#if EaPollingMode == false
extern void Ea_JobEndNotification(void);
extern void Ea_JobErrorNotification(void);
#define EaPollingModeEnabled true

#else
	#define EaPollingModeEnabled false
#endif


#ifndef EaSetModeSupported
#warning "EaSetModeSupported is not defined by user and set to its default value (true)"
#define EaSetModeSupported true
#endif

#ifndef EaVersionInfoApi
#warning "EaVersionInfoApi is not defined by user and set to its default value (false)"
#define EaVersionInfoApi false
#endif

#if EaVirtualPageSize > (EcucIntegerParamDef)65535
#error "EaVirtualPageSize is over limit (65535)"
#endif

#if EaBlockNumber > (EcucIntegerParamDef)65534
#error "EaBlockNumber is over limit (65535)"
#endif

#if EaBlockSize > (EcucIntegerParamDef)65534
#error "EaBlockSize is over limit"
#endif

#if EaBlockOverhead > (EcucIntegerParamDef)65534
#error "EaBlockOverhead is over limit"
#endif


#if EaPageOverhead > (EcucIntegerParamDef)65534
#error "EaPageOverhead is over limit"
#endif

extern void Ea_Init(const Ea_ConfigType* ConfigPtr);
extern void Ea_SetMode(MemIf_ModeType Mode);
extern Std_ReturnType Ea_Read(uint16 BlockNumber,uint16 BlockOffset,uint8* DataBufferPtr,uint16 Length);
extern Std_ReturnType Ea_Write(uint16 BlockNumber,const uint8* DataBufferPtr);
extern void Ea_Cancel(void);
extern MemIf_StatusType Ea_GetStatus(void);
extern MemIf_JobResultType Ea_GetJobResult(void);
extern Std_ReturnType Ea_InvalidateBlock(uint16 BlockNumber);
extern void Ea_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr);
extern Std_ReturnType Ea_EraseImmediateBlock(uint16 BlockNumber);
extern void Ea_MainFunction(void);

#endif