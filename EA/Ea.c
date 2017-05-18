#include "Ea_MemMap.h"
#include "Ea.h"
#include "SchM_Ea.h"
#include "Det.h"
#include "Ea_Cbk.h"



/////////////////////// Local Variables ////////////////////////

static uint16 local_BlockNumber;
static uint16 local_BlockOffset;
static uint8* local_DataBufferPtr;
static uint16 local_Length;
static EcucBooleanParamDef local_IsInvalidateRequest;
static MemIf_StatusType MemIf_Status = MEMIF_UNINIT;
static MemIf_JobResultType MemIf_JobResult ;
static MemIf_ModeType MemIf_Mode;
static EcucBooleanParamDef Local_ChangeModeFlag;
static uint16 Local_PhysicalAddress;
static uint8 Local_NumberOfPhysicalPagesPerBlock;


//////////////////// Ea_Init Funftion ////////////////////////////

/**
*@func	init function 
*@brief Initializes the EEPROM abstraction module.
*@param ConfigPtr : Pointer to the selected configuration set..
*/
extern void Ea_Init(const Ea_ConfigType* ConfigPtr)
{
	MemIf_Status = 	MEMIF_BUSY_INTERNAL;
	
	local_BlockNumber = 0;
	local_BlockOffset = 0;
	local_DataBufferPtr = NULL_PTR;
	local_Length = 0;
	Local_PhysicalAddress=0;
	MemIf_Mode = MEMIF_MODE_SLOW;
	Local_ChangeModeFlag=0;
	local_IsInvalidateRequest=false;
	Local_NumberOfPhysicalPagesPerBlock=(EaBlockSize / EaVirtualPageSize);	
	
	if ( (EaBlockSize > EaVirtualPageSize) && ( (EaBlockSize % EaVirtualPageSize) > 0 ) )
	{
	Local_NumberOfPhysicalPagesPerBlock+=1;
	}
	
	MemIf_Status = MEMIF_IDLE;
}


//////////////////// Ea_SetMode Funftion ////////////////////////////

#ifdef EaSetModeSupported

/**
*@func	SetMode function 
*@brief Sets the mode of reading and writing operations.
*@param Mode : The mode of opertaion, type MemIf_ModeType, Range:[MEMIF_MODE_SLOW,MEMIF_MODE_FAST].
*@return 	Void
*/

extern void Ea_SetMode(MemIf_ModeType Mode)
{
	switch (MemIf_Status)
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
		MemIf_Mode = Mode ;
		Local_ChangeModeFlag =	true; // to be excecuted asynchronously inside the main function 

	break;
	
	case MEMIF_BUSY_INTERNAL:
		MemIf_Mode = Mode ;
		Local_ChangeModeFlag =	true; // to be excecuted asynchronously inside the main function 
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
	if((MemIf_Status == MEMIF_IDLE) || (MemIf_Status == MEMIF_BUSY_INTERNAL))
	{
		local_BlockNumber = BlockNumber;
		local_BlockOffset = BlockOffset;
		local_DataBufferPtr = DataBufferPtr;
		local_Length = Length;
		
		MemIf_Status = MEMIF_BUSY;
		MemIf_JobResult = MEMIF_JOB_PENDING;
		
		return E_OK;
	}
	
	else
	{
		#if EaDevErrorDetect == true
			if(MemIf_Status == MEMIF_UNINIT)
			{
				//raise the development error EA_E_UNINIT
				return E_NOT_OK;
			}
			else if(MemIf_Status == MEMIF_BUSY)
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
	if((MemIf_Status == MEMIF_IDLE) || (MemIf_Status == MEMIF_BUSY_INTERNAL))
	{
		Local_PhysicalAddress = ( (BlockNumber - 1 ) * EaVirtualPageSize * Local_NumberOfPhysicalPagesPerBlock ) ;
		local_Length = EaBlockSize;
		local_DataBufferPtr = DataBufferPtr ;
		
		MemIf_Status = MEMIF_BUSY;
		MemIf_JobResult = MEMIF_JOB_PENDING ;
		
		return E_OK;
	}
	
	else
	{
		#if EaDevErrorDetect == true
			if(MemIf_Status == MEMIF_UNINIT)
			{
				//raise the development error EA_E_UNINIT
				return E_NOT_OK;
			}
			else if(MemIf_Status == MEMIF_BUSY)
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
	switch(MemIf_Status)
	{
	case MEMIF_BUSY:
		Eep_Cancel();
		MemIf_Status = MEMIF_IDLE;
		MemIf_JobResultType = MEMIF_JOB_CANCELED ;
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
*@return 	return MemIf_StatusType 
*MEMIF_UNINIT: The EA module has not been initialized (yet).
*MEMIF_IDLE: The EA module is currently idle.
*MEMIF_BUSY: The EA module is currently busy.
*MEMIF_BUSY_INTERNAL: The EA module is currently busy with internal management operations.
*/

extern MemIf_StatusType Ea_GetStatus(void)
{
	return MemIf_Status;		
}

//////////////////// Ea_GetJobResult Funftion ////////////////////////////

/**
*@func	GetJobResult function 
*@brief Service to return the JobResult.
*@return 	MemIf_JobResultType 
*MEMIF_JOB_OK: The last job has been finished successfully. 
*MEMIF_JOB_PENDING: The last job is waiting for execution or currently being executed. 
*MEMIF_JOB_CANCELED: The last job has been canceled (which means it failed). 
*MEMIF_JOB_FAILED: The last job was not finished successfully (it failed). 
*MEMIF_BLOCK_INCONSISTENT: The requested block is inconsistent, it may contain corrupted data.
*/

extern MemIf_JobResultType Ea_GetJobResult(void)
{
	switch(MemIf_Status)
	{
	
	#if EaDevErrorDetect == true

	case MEMIF_UNINIT:
		// raise the development error EA_E_UNINIT
		return MEMIF_JOB_FAILED ;
	break;
	#endif
	
	default:
	return MemIf_JobResult;
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
	if((MemIf_Status == MEMIF_IDLE) || (MemIf_Status == MEMIF_BUSY_INTERNAL))
	{
		local_IsInvalidateRequest=true;

		local_BlockNumber=BlockNumber;
				
		return E_OK;
	}
	else
	{
		if(MemIf_Status == MEMIF_UNINIT)
		{
			#if EaDevErrorDetect == true
				//raise the development error EA_E_UNINIT
			#endif
			return E_NOT_OK;
		}
		if(MemIf_Status == MEMIF_BUSY)
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
	
}

//////////////////// Ea_EraseImmediateBlock Funftion ////////////////////////////
extern Std_ReturnType Ea_EraseImmediateBlock(uint16 BlockNumber)
{
	
	
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
	if (MemIf_JobResult == MEMIF_JOB_PENDING)
	{
		MemIf_JobResult = MEMIF_JOB_OK;
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
	if (MemIf_JobResult == MEMIF_JOB_PENDING)
	{
		MemIf_JobResult = MEMIF_JOB_FAILED;
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