/*
 * Eep.c
 *
 *  Created on: May 10, 2017
 *      Author: power
 */
#include <stdint.h>
#include <stdbool.h>
#include "Eep.h"
#include "Eep_Private.h"
#include "driverlib/sysctl.h"
//#include "Eep_MemMap.h"
//#include "Dem.h"
//#include "SchM_Eep.h"
//#include "Det.h"
//#include "Eep_Cbk.h"
#define NULL 0
static uint8_t FLAG_Init=0;
extern uint8_t EEP_Error=0;/*send this parameter at error message page 23*/
//***********************************************************************************************************************************************************
//************************************************************************************************************************************************************
/*static function to for eeprom done*/
//***********************************************************************************************************************************************************
/*void Eep_init (const Eep_ConfigType* ConfigPtr)
{
    if(ConfigPtr==NULL)
    {
        EEP_Error=EEP_E_PARAM_POINTER;
    }
    //***********************************
    /*check null pointer*/
    //**********************************
  /*  else if(MEM_State!=MEMIF_IDLE)
    {
        EEP_Error=EEP_E_BUSY;
    }
    //*********************************
    /*check memory state*/
    //********************************
    /*else
    {
        SysCtlDelay(2);
        if (EEPROM_Done()!=INIT_Error)
        {
            SysCtlPeripheralReset(SYSCTL_PERIPH_EEPROM0);
             SysCtlDelay(2);
             if (EEPROM_Done()!=INIT_Error)
             {
                 EEP_Error=EEP_NOTERROR;
                 EEP_Global.config=ConfigPtr;
             }
             else
             {
                 EEP_Error=EEP_E_INIT_FAILED;
             }
        }
        else
        {
            EEP_Error=EEP_E_INIT_FAILED;
        }
    }


}
*/
typedef struct{
     Eep_ConfigType *config;
    Eep_Arc_JobType JOB_Type;
    MemIf_JobResultType JOBResultType;
    MemIf_StatusType status;
    MemIf_ModeType mode;
}Eep_GlobalType;
Eep_GlobalType EepGlobal;
//*************************************************************
typedef struct{
    Eep_AddressType Offset;
    uint8_t* Buffer;
    Eep_LengthType Size;
}Job;
Job EEP_JobDetails;
static void COPY_Cfg (const Eep_ConfigType* ConfigPtr)
{
    EepGlobal.status=MEMIF_BUSY;/*will be MEMIF_IDLE after init of eeprom finish totally*/
    EepGlobal.config=ConfigPtr;
    EepGlobal.config->EepBaseAddress=EEP_Address;
    EepGlobal.config->EepDefaultMode=MEM_Mode;
    EepGlobal.config->EepFastReadBlockSize=EEPFAST_Read;
    EepGlobal.config->EepFastWriteBlockSize=EEPFAST_Write;
    EepGlobal.config->EepNormalReadBlockSize=EEPNORMAL_Read;
    EepGlobal.config->EepNormalWriteBlockSize=EEPNORMAL_Write;
    EepGlobal.config->EepSize=EEP_Size;
    EepGlobal.JOB_Type=EEP_Init;

}
//**********************************************************************************************************************************************************
uint8_t TEST_Init (void)
{
    if (FLAG_Init==0)
    {
        return EEP_E_UNINIT;
    }
    else
    {
        return True;
    }

}
void Eep_init ( Eep_ConfigType* ConfigPtr)
{
#if EepDevErrorDetect==True
    if(ConfigPtr==NULL)
    {

        EEP_Error=EEP_E_PARAM_POINTER;
    }

    else if(EepGlobal.status==MEMIF_IDLE)
    {
        COPY_Cfg (ConfigPtr);
        FLAG_Init=1;

    }
    else
    {
        if(EepGlobal.status==MEMIF_BUSY)
        {
            EEP_Error=EEP_E_INIT_FAILED;

        }
    }
#else
    if(EepGlobal.status==MEMIF_IDLE)
    {
    COPY_Cfg (const Eep_ConfigType* ConfigPtr);
    FLAG_Init=1;
    }
#endif

}
//**********************************************************************************************************************************************************
/*TNIT OF EEPROM*/
//**********************************************************************************************************************************************************

Std_ReturnType Eep_Read( Eep_AddressType EepromAddress, uint8_t* DataBufferPtr, Eep_LengthType Length )
{
#if EepDevErrorDetect==True
    if((TEST_Init()==EEP_E_UNINIT)|(EepGlobal.status==MEMIF_BUSY)|(DataBufferPtr==NULL)|(EepromAddress>(EepGlobal.config->EepSize))|(((EepGlobal.config->EepSize)-EepromAddress)< Length))
    {
        return E_NOT_OK;
    }


#endif
    EepGlobal.status=MEMIF_BUSY;
    EEP_JobDetails.Offset=EepromAddress+EepGlobal.config->EepBaseAddress;
    EEP_JobDetails.Buffer=DataBufferPtr;
    EEP_JobDetails.Size=Length;
    EepGlobal.JOB_Type=EEP_Read;
    EepGlobal.JOBResultType=MEMIF_JOB_PENDING;
    return E_OK;

}
//**********************************************************************************************************************************************************
/*Read function*/
//**********************************************************************************************************************************************************
Std_ReturnType Eep_Write(Eep_AddressType EepromAddress,const uint8_t* DataBufferPtr,Eep_LengthType Length)
{
#if EepDevErrorDetect==True
    if((TEST_Init()==EEP_E_UNINIT)|(EepGlobal.status==MEMIF_BUSY)|(DataBufferPtr==NULL)|(EepromAddress>(EepGlobal.config->EepSize))|((EepGlobal.config->EepSize - EepromAddress) < Length ))
    {
        return E_NOT_OK;
    }
#endif
    EEP_JobDetails.Offset=EepromAddress+(EepGlobal.config->EepBaseAddress);
    EEP_JobDetails.Buffer=DataBufferPtr;
    EEP_JobDetails.Size=Length;
    EepGlobal.JOB_Type=EEP_Write;
    EepGlobal.JOBResultType=MEMIF_JOB_PENDING;
}
//**********************************************************************************************************************************************************
/*write function*/
//**********************************************************************************************************************************************************
Std_ReturnType Eep_Compare(Eep_AddressType EepromAddress,const uint8_t* DataBufferPtr,Eep_LengthType Length)
{
#if EepDevErrorDetect==True
    if((TEST_Init()==EEP_E_UNINIT)|(EepGlobal.status==MEMIF_BUSY)|(DataBufferPtr==NULL)|(EepromAddress>(EepGlobal.config->EepSize))|((EepGlobal.config->EepSize-EepromAddress)<Length))
    {
        return E_NOT_OK;
    }
#endif
    EEP_JobDetails.Offset=EepromAddress+(EepGlobal.config->EepBaseAddress);
    EEP_JobDetails.Buffer=DataBufferPtr;
    EEP_JobDetails.Size=Length;
    EepGlobal.JOB_Type=EEP_Compare;
    EepGlobal.JOBResultType=MEMIF_JOB_PENDING;
}
//**********************************************************************************************************************************************************
/*compare function*/
//**********************************************************************************************************************************************************
void Eep_SetMode(MemIf_ModeType Mode)
{
#if EepDevErrorDetect==True
    if((TEST_Init()!=EEP_E_UNINIT)|(EepGlobal.status!=MEMIF_BUSY))
            {
    EepGlobal.mode=Mode;
            }
#else
    EepGlobal.mode=Mode;
#endif
}
//**********************************************************************************************************************************************************
/*set mode*/
//**********************************************************************************************************************************************************
Std_ReturnType Eep_Erase( Eep_AddressType EepromAddress,Eep_LengthType Length )
{
#if EepDevErrorDetect==True
    if((TEST_Init()==EEP_E_UNINIT)|(EepGlobal.status==MEMIF_BUSY)|(EepromAddress>(EepGlobal.config->EepSize))|((EepGlobal.config->EepSize - EepromAddress) < Length ))
    {
        return E_NOT_OK;
    }
#endif
    EepGlobal.status=MEMIF_BUSY;
    EEP_JobDetails.Offset=EepromAddress+EepGlobal.config->EepBaseAddress;
    EEP_JobDetails.Size=Length;
    EepGlobal.JOB_Type=EEP_Read;
    EepGlobal.JOBResultType=MEMIF_JOB_PENDING;
    return E_OK;

}
//**********************************************************************************************************************************************************
/*Erase function*/
//**********************************************************************************************************************************************************
MemIf_StatusType Eep_GetStatus( void )
{
    return EepGlobal.status;
}
//**********************************************************************************************************************************************************
/*get status of eeprom*/
//*********************************************************************************************************************************************************
MemIf_JobResultType Eep_GetJobResult( void )
{
    return EepGlobal.JOBResultType;
}
//**********************************************************************************************************************************************************
/*get job result*/
//**********************************************************************************************************************************************************
void Eep_GetVersionInfo( Std_VersionInfoType* versioninfo )
{
    if (versioninfo==NULL)
    {
        EEP_Error=EEP_E_PARAM_ADDRESS;
    }

}
//**********************************************************************************************************************************************************
/*not finished*/
//**********************************************************************************************************************************************************

#define EEP_JOB_END_NOTIFICATION() \
  if (Eep_Global.config->Eep_JobEndNotification!=NULL) { \
    Eep_Global.config->Eep_JobEndNotification(); \
  }
//**********************************************************************************************************************************************************
#define EEP_JOB_Error_NOTIFICATION() \
  if (Eep_Global.config->Eep_JobErrorNotification!=NULL) { \
    Eep_Global.config->Eep_JobErrorNotification(); \
  }
