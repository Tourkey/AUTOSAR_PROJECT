#include "Ea_MemMap.h"
#include "Ea.h"
#include "SchM_Ea.h"
#include "Det.h"
#include "Ea_Cbk.h"



/////////////////////// Local Variables ////////////////////////

typedef struct
{
	EcucBooleanParamDef local_IsInvalidateRequest;
	EcucBooleanParamDef local_IsEraseRequest;

}local_flags_type;

typedef struct
{
	uint8* local_DataBufferPtr;
	uint16 local_BlockNumber;
	uint16 local_BlockOffset;
	uint16 local_Length;
	MemIf_StatusType MemIf_Status = MEMIF_UNINIT;
	MemIf_JobResultType MemIf_JobResult ;
	MemIf_ModeType MemIf_Mode;
	uint16 Local_PhysicalAddress;
	EcucBooleanParamDef Local_ChangeModeFlag;
	uint8 Local_NumberOfPhysicalPagesPerBlock;
	
}local_variable_type;

static local_flags_type local_flags;
static local_variable_type local_variable={NULL_PTR,0};





/*version info*/
Std_VersionInfoType local_VersionInfo;


//////////////////// Ea_Init Funftion ////////////////////////////

/**
*@func	init function 
*@brief Initializes the EEPROM abstraction module.
*@param ConfigPtr : Pointer to the selected configuration set..
*/
extern void Ea_Init(const Ea_ConfigType* ConfigPtr)
{
	local_variable.local_variable.MemIf_Status = 	MEMIF_BUSY_INTERNAL;
	
	local_variable.local_BlockNumber = 0;
	local_variable.local_BlockOffset = 0;
	local_variable.local_DataBufferPtr = NULL_PTR;
	local_variable.local_Length = 0;
	local_variable.Local_PhysicalAddress=0;
	local_variable.MemIf_Mode = local_variable.MemIf_Mode_SLOW;
	local_variable.Local_ChangeModeFlag=0;
	local_flags.local_IsInvalidateRequest=false;
	local_flags.local_IsEraseRequest=false;
	local_variable.Local_NumberOfPhysicalPagesPerBlock=(EaBlockSize / EaVirtualPageSize);	
	
	/* init the version info */
	local_VersionInfo.vendorID=0;
	local_VersionInfo.moduleID=0;
	local_VersionInfo.sw_major_version=0;
	local_VersionInfo.sw_minor_version=0;
	local_VersionInfo.sw_patch_version=0;
	
	
	if ( (EaBlockSize > EaVirtualPageSize) && ( (EaBlockSize % EaVirtualPageSize) > 0 ) )
	{
	local_variable.Local_NumberOfPhysicalPagesPerBlock+=1;
	}
	
	local_variable.local_variable.MemIf_Status = MEMIF_IDLE;
}


//////////////////// Ea_SetMode Funftion ////////////////////////////

#ifdef EaSetModeSupported

/**
*@func	SetMode function 
*@brief Sets the mode of reading and writing operations.
*@param Mode : The mode of opertaion, type local_variable.MemIf_ModeType, Range:[local_variable.MemIf_Mode_SLOW,local_variable.MemIf_Mode_FAST].
*@return 	Void
*/

extern void Ea_SetMode(local_variable.MemIf_ModeType Mode)
{
	switch (local_variable.local_variable.MemIf_Status)
	{

	#if EaDevErrorDetect == true
	
	case MEMIF_UNINIT:
	//raise the development error EA_E_UNINIT
	break;
	
	case MEMIF_BUSY:
	//raise the development error EA_E_BUSY
	break;
	
	#endif
	
	case MEMIF_IDLE:
		local_variable.MemIf_Mode = Mode ;
		local_variable.Local_ChangeModeFlag =	true; // to be excecuted asynchronously inside the main function 

	break;
	
	case MEMIF_BUSY_INTERNAL:
		local_variable.MemIf_Mode = Mode ;
		local_variable.Local_ChangeModeFlag =	true; // to be excecuted asynchronously inside the main function 
	break;
	
	default:
	break;
	}
}


#endif

//////////////////// Ea_Read Funftion ////////////////////////////

/**
*@func	read function 
*@brief Reads Length bytes of block Blocknumber at offset BlockOffset into the buffer DataBufferPtr. 
*@param BlockNumber : Number of logical block, also denoting start address of that block in EEPROM.
*@param BlockOffset : Read address offset inside the block.
*@param DataBufferPtr : Pointer to data buffer.
*@param Length : Number of bytes to read.
*@return 	E_OK: The requested job has been accepted by the module.
*			E_NOT_OK: The requested job has not been accepted by the EA module.
*/
extern Std_ReturnType Ea_Read(uint16 BlockNumber,uint16 BlockOffset,uint8* DataBufferPtr,uint16 Length)
{
	if((local_variable.local_variable.MemIf_Status == MEMIF_IDLE) || (local_variable.local_variable.MemIf_Status == MEMIF_BUSY_INTERNAL))
	{
		local_variable.Local_PhysicalAddress = ( (BlockNumber - 1 ) * EaVirtualPageSize * local_variable.Local_NumberOfPhysicalPagesPerBlock ) ;
		local_variable.local_BlockNumber = BlockNumber;
		local_variable.local_BlockOffset = BlockOffset;
		local_variable.local_DataBufferPtr = DataBufferPtr;
		local_variable.local_Length = Length;
		
		local_variable.local_variable.MemIf_Status = MEMIF_BUSY;
		local_variable.MemIf_JobResult = MEMIF_JOB_PENDING;
		
		return E_OK;
	}
	
	else
	{
		#if EaDevErrorDetect == true
			if(local_variable.local_variable.MemIf_Status == MEMIF_UNINIT)
			{
				//raise the development error EA_E_UNINIT
				return E_NOT_OK;
			}
			else if(local_variable.local_variable.MemIf_Status == MEMIF_BUSY)
			{
				// raise the development error EA_E_BUSY 
				return E_NOT_OK;
			}
			if((blockNumber == 0) || (blockNumber == 0xffff) )
			{
				// raise the development error EA_E_INVALID_BLOCK_NO
				return E_NOT_OK;
			}
			if(BlockOffset >= EaBlockNumber )
			{
				// raise the development error EA_E_INVALID_BLOCK_OFS
				return E_NOT_OK;
			}
		/*	if((Length+BlockOffset) >= EaBlockNumber )
			{
				// raise the development error EA_E_INVALID_BLOCK_LEN
				return E_NOT_OK;
			}*/
			if(DataBufferPtr == NULL_PTR  )
			{
				// raise the development error EA_E_PARAM_POINTER
				return E_NOT_OK;
			}
		#endif
		
		return E_NOT_OK;

	}
}

//////////////////// Ea_Write Funftion ////////////////////////////

/**
*@func	Write function 
*@brief Writes the contents of the DataBufferPtr to the block BlockNumber. 
*@param BlockNumber : Number of logical block, also denoting start address of that block in EEPROM.
*@param DataBufferPtr : Pointer to data buffer.
*@return 	E_OK: The requested job has been accepted by the EA module.
*			E_NOT_OK: The requested job has not been accepted by the EA module.
*/


extern Std_ReturnType Ea_Write(uint16 BlockNumber,const uint8* DataBufferPtr)
{
	if((local_variable.local_variable.MemIf_Status == MEMIF_IDLE) || (local_variable.local_variable.MemIf_Status == MEMIF_BUSY_INTERNAL))
	{
		local_variable.Local_PhysicalAddress = ( (BlockNumber - 1 ) * EaVirtualPageSize * local_variable.Local_NumberOfPhysicalPagesPerBlock ) ;
		local_variable.local_Length = EaBlockSize;
		local_variable.local_DataBufferPtr = DataBufferPtr ;
		
		local_variable.local_variable.MemIf_Status = MEMIF_BUSY;
		local_variable.MemIf_JobResult = MEMIF_JOB_PENDING ;
		
		return E_OK;
	}
	
	else
	{
		#if EaDevErrorDetect == true
			if(local_variable.local_variable.MemIf_Status == MEMIF_UNINIT)
			{
				//raise the development error EA_E_UNINIT
				return E_NOT_OK;
			}
			else if(local_variable.local_variable.MemIf_Status == MEMIF_BUSY)
			{
				// raise the development error EA_E_BUSY 
				return E_NOT_OK;
			}
			if((blockNumber == 0) || (blockNumber == 0xffff) )
			{
				// raise the development error EA_E_INVALID_BLOCK_NO
				return E_NOT_OK;
			}
			if(DataBufferPtr == NULL_PTR  )
			{
				// raise the development error EA_E_PARAM_POINTER
				return E_NOT_OK;
			}
			
		#endif

		return E_NOT_OK;

	}
}

//////////////////// Ea_Cancel Funftion ////////////////////////////
/**
*@func	Cancel function 
*@brief Cancels the ongoing asynchronous operation.
*/


extern void Ea_Cancel(void)
{
	switch(local_variable.local_variable.MemIf_Status)
	{
	case MEMIF_BUSY:
		Eep_Cancel();
		local_variable.local_variable.MemIf_Status = MEMIF_IDLE;
		local_variable.MemIf_JobResultType = MEMIF_JOB_CANCELED ;
	break;
	
	#if EaDevErrorDetect == true
	
	case MEMIF_UNINIT:
		//raise the development error EA_E_UNINIT
	break;
	
	default:
	// raise the development error EA_E_INVALID_CANCEL.
	break;
	
	#endif
	
	}
}

//////////////////// Ea_GetStatus Funftion ////////////////////////////
/**
*@func	GetStatus function 
*@brief get the status of MEMIF.
*@return 	return local_variable.local_variable.MemIf_StatusType 
*MEMIF_UNINIT: The EA module has not been initialized (yet).
*MEMIF_IDLE: The EA module is currently idle.
*MEMIF_BUSY: The EA module is currently busy.
*MEMIF_BUSY_INTERNAL: The EA module is currently busy with internal management operations.
*/

extern local_variable.local_variable.MemIf_StatusType Ea_GetStatus(void)
{
	return local_variable.local_variable.MemIf_Status;		
}

//////////////////// Ea_GetJobResult Funftion ////////////////////////////

/**
*@func	GetJobResult function 
*@brief Service to return the JobResult.
*@return 	local_variable.MemIf_JobResultType 
*MEMIF_JOB_OK: The last job has been finished successfully. 
*MEMIF_JOB_PENDING: The last job is waiting for execution or currently being executed. 
*MEMIF_JOB_CANCELED: The last job has been canceled (which means it failed). 
*MEMIF_JOB_FAILED: The last job was not finished successfully (it failed). 
*MEMIF_BLOCK_INCONSISTENT: The requested block is inconsistent, it may contain corrupted data.
*/

extern local_variable.MemIf_JobResultType Ea_GetJobResult(void)
{
	switch(local_variable.local_variable.MemIf_Status)
	{
	
	#if EaDevErrorDetect == true

	case MEMIF_UNINIT:
		// raise the development error EA_E_UNINIT
		return MEMIF_JOB_FAILED ;
	break;
	#endif
	
	default:
	return local_variable.MemIf_JobResult;
	break;	
	}
	
}

//////////////////// Ea_InvalidateBlock Funftion ////////////////////////////
/**
*@func	InvalidateBlock function 
*@brief Invalidates the block BlockNumber.
*@param BlockNumber Number of logical block, also denoting start address of that block in EEPROM.
*@return 	E_OK: The requested job has been accepted by the module.
*			E_NOT_OK - only if DET is enabled: The requested job has not been accepted by the EA module.
*/
extern Std_ReturnType Ea_InvalidateBlock(uint16 BlockNumber)
{
	if((local_variable.local_variable.MemIf_Status == MEMIF_IDLE) || (local_variable.local_variable.MemIf_Status == MEMIF_BUSY_INTERNAL))
	{
		local_flags.local_IsInvalidateRequest=true;

		local_variable.local_BlockNumber=BlockNumber;
				
		return E_OK;
	}
	else
	{
		if(local_variable.local_variable.MemIf_Status == MEMIF_UNINIT)
		{
			#if EaDevErrorDetect == true
				//raise the development error EA_E_UNINIT
			#endif
			return E_NOT_OK;
		}
		if(local_variable.local_variable.MemIf_Status == MEMIF_BUSY)
		{
			#if EaDevErrorDetect == true
				//raise the development error EA_E_BUSY
			#endif
			return E_NOT_OK;
		}
	}
}

//////////////////// Ea_GetVersionInfo Funftion ////////////////////////////
/**
*@func	GetVersionInfo function 
*@brief Service to get the version information of this module.
*@param  VersionInfoPtr Pointer to standard version information structure.
*@return 	None
*/
extern void Ea_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr)
{
	if(VersionInfoPtr == NULL_PTR)
	{
		#if EaDevErrorDetect == true
			//raise the development error EA_E_PARAM_POINTER.
		#endif
	}
	else
	{
		*VersionInfoPtr= local_VersionInfo;
	}
}

//////////////////// Ea_EraseImmediateBlock Funftion ////////////////////////////
/**
*@func	EraseImmediateBlock
*@brief Erases the block BlockNumber.
*@param BlockNumber Number of logical block, also denoting start address of that block in EEPROM.
*@return 	E_OK: The requested job has been accepted by the module.
*			E_NOT_OK - only if DET is enabled: The requested job has not been accepted by the EA module.
*/
extern Std_ReturnType Ea_EraseImmediateBlock(uint16 BlockNumber)
{
	if((local_variable.local_variable.MemIf_Status == MEMIF_IDLE) || (local_variable.local_variable.MemIf_Status == MEMIF_BUSY_INTERNAL))
	{
		#if EaDevErrorDetect == true
		if((blockNumber == 0) || (blockNumber == 0xffff) )
		{
			// raise the development error EA_E_INVALID_BLOCK_NO
			return E_NOT_OK;
		}

			#if EaImmediateData == false
				// raise the development error EA_E_INVALID_BLOCK_NO
				return E_NOT_OK;
			#endif
		#endif
		local_flags.local_IsEraseRequest =true;
		local_variable.Local_PhysicalAddress = ( (BlockNumber - 1 ) * EaVirtualPageSize * local_variable.Local_NumberOfPhysicalPagesPerBlock ) ;
		local_variable.local_BlockNumber=BlockNumber;

		return E_OK;
	}
	else
	{
		if(local_variable.local_variable.MemIf_Status == MEMIF_UNINIT)
		{
			#if EaDevErrorDetect == true
				//raise the development error EA_E_UNINIT
			#endif
			return E_NOT_OK;
		}
		if(local_variable.local_variable.MemIf_Status == MEMIF_BUSY)
		{
			#if EaDevErrorDetect == true
				//raise the development error EA_E_BUSY
			#endif
			return E_NOT_OK;
		}
	}
	
}

//////////////////// Ea_JobEndNotification Funftion ////////////////////////////

#if EaPollingMode == false

/**
*@func		JobEndNotification function 
*@brief 	Service to report to this module the successful end of an asynchronous operation.
*@param  	None
*@return 	None
*/
extern void Ea_JobEndNotification(void)
{
	if (local_variable.MemIf_JobResult == MEMIF_JOB_PENDING)
	{
		local_variable.MemIf_JobResult = MEMIF_JOB_OK;
		Ea_NvMJobEndNotification();
	}
	else
	{
		;
	}
	
	
}

#endif
//////////////////// Ea_JobErrorNotification Funftion ////////////////////////////

#if EaPollingMode == false

/**
*@func		JobErrorNotification function 
*@brief 	Service to report to this module the failure of an asynchronous operation.
*@param  	None
*@return 	None
*/

extern void Ea_JobErrorNotification(void)
{
	if (local_variable.MemIf_JobResult == MEMIF_JOB_PENDING)
	{
		local_variable.MemIf_JobResult = MEMIF_JOB_FAILED;
		Ea_NvMJobErrorNotification();
	}
	
	else
	{
		;
	}
	
}
#endif

//////////////////// Ea_MainFunction Funftion ////////////////////////////
extern void Ea_MainFunction(void)
{
	
	
}