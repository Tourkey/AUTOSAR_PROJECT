#ifndef __EA_H
#define __EA_H

#include "Std_Types.h"
#include "Ea_Cfg.h"

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
extern void Ea_JobEndNotification(void);
extern void Ea_JobErrorNotification(void);
extern void Ea_MainFunction(void);

#endif