#include "Ea_MemMap.h"
#include "Ea.h"
#include "SchM_Ea.h"
#include "Det.h"
#include "Ea_Cbk.h"


static uint16 local_BlockNumber;
static uint16 local_BlockOffset;
static uint8* local_DataBufferPtr;
static uint16 local_Length;

static MemIf_StatusType MemIf_Status = MEMIF_UNINIT;
static MemIf_JobResultType MemIf_JobResult ;


/**
*@func	init function 
*@brief Initializes the EEPROM abstraction module.
*@param ConfigPtr : Pointer to the selected configuration set..
*/
extern void Ea_Init(const Ea_ConfigType* ConfigPtr)
{
	MemIf_Status = MEMIF_BUSY_INTERNAL;
	
	local_BlockNumber = 0;
	local_BlockOffset = 0;
	local_DataBufferPtr = NULL_PTR;
	local_Length = 0;
	
	MemIf_Status = MEMIF_IDLE;
}

extern void Ea_SetMode(MemIf_ModeType Mode)
{

}




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
	if(MemIf_Status == MEMIF_UNINIT)
	{
		return E_NOT_OK;
	}
	#if EaDevErrorDetect == true
		if(MemIf_Status == MEMIF_UNINIT)
		{
			//raise the development error EA_E_UNINIT
			return E_NOT_OK;
		}
		if(MemIf_Status == MEMIF_BUSY)
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
}
extern Std_ReturnType Ea_Write(uint16 BlockNumber,const uint8* DataBufferPtr)
{
}
extern void Ea_Cancel(void)
{
}
extern MemIf_StatusType Ea_GetStatus(void)
{
}
extern MemIf_JobResultType Ea_GetJobResult(void)
{
}
extern Std_ReturnType Ea_InvalidateBlock(uint16 BlockNumber)
{
}
extern void Ea_GetVersionInfo(Std_VersionInfoType* VersionInfoPtr)
{
}
extern Std_ReturnType Ea_EraseImmediateBlock(uint16 BlockNumber)
{
}
extern void Ea_JobEndNotification(void)
{
}
extern void Ea_JobErrorNotification(void)
{
}
extern void Ea_MainFunction(void)
{
}