
/* ECU types def */

typedef enum{
	false,
	true
}EcucBooleanParamDef;




typedef enum{
	MEMIF_UNINIT=0,		// The EA module has not been initialized (yet).
	MEMIF_IDLE,			// The EA module is currently idle.
	MEMIF_BUSY,			// The EA module is currently busy.
	MEMIF_BUSY_INTERNAL,// The EA module is currently busy with internal management operations.
}MemIf_StatusType;


typedef enum{
	MEMIF_JOB_OK=0, 			// The last job has been finished successfully.
	MEMIF_JOB_PENDING, 			// The last job is waiting for execution or currently being executed.
	MEMIF_JOB_CANCELED, 		// The last job has been canceled(which means it failed).
	MEMIF_JOB_FAILED,			// The last job was not finished successfully (it failed).
	MEMIF_BLOCK_INCONSISTENT, 	// The requested block is inconsistent, it may contain corrupted data.
}MemIf_JobResultType;
	


typedef float EcucFloatParamDef ;

typedef int EcucIntegerParamDef ;
