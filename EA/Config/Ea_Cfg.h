#ifndef EA_CFG_H
#define EA_CFG_H

#include "NvM_Cbk.h"
#include "MemIf_Types.h"
#include "Eep.h"



/*version info*/

#define cfg_vendorID	1
#define cfg_moduleID	1		
#define cfg_sw_major_version	1		
#define cfg_sw_minor_version	1	
#define cfg_sw_patch_version	1


/*General configuration of the EEPROM abstraction module. This container 
lists block independent configuration parameters.*/

/*Switches the development error detection and notification on or off.
  true: detection and notification is enabled. 
  false: detection and notification is disabled */

#define EaDevErrorDetect	(EcucBooleanParamDef)false


/*The period between successive calls to the main function in seconds.*/
#define EaMainFunctionPeriod 	(EcucFloatParamDef)0.1



/*Mapped to the job end notification routine provided by the upper layer 
module (NvM_JobEndNotification).*/


/* NVM Pooling mode disabled and interface provided to EA */
#if NvMPollingMode == (EcucBooleanParamDef)false 
#define EaNvmJobEndNotification NvM_JobEndNotification
#endif



/*Mapped to the job error notification routine provided by the upper layer module (NvM_JobErrorNotification)*/
#if NvMPollingMode == (EcucBooleanParamDef)false 
#define EaNvmJobErrorNotification NvM_JobErrorNotification
#endif

/*Pre-processor switch to enable and disable the polling mode for this 
module.
true: Polling mode enabled, callback functions (provided to EEP module) 
disabled.
false: Polling mode disabled, callback functions (provided to EEP module) 
enabled.*/
#define EaPollingMode (EcucBooleanParamDef)false

/*Compile switch to enable / disable the function Ea_SetMode.*/
#define EaSetModeSupported (EcucBooleanParamDef)true

/*Pre-processor switch to enable / disable the API to read out the modules
version information.
true: Version info API enabled.
false: Version info API disabled
*/
#define EaVersionInfoApi  (EcucBooleanParamDef)false




/*The size in bytes to which logical blocks shall be aligned.*/
#define EaVirtualPageSize  (EcucIntegerParamDef)50

///////////////////////////////////////////////

/*Block identifier (handle).
0x0000 and 0xFFFF shall not be used for block numbers (see EA006).
Range: 
min = 2^NVM_DATASET_SELECTION_BITS 
max = 0xFFFF -2^NVM_DATASET_SELECTION_BITS
Note: Depending on the number of bits set aside for dataset selection 
several other block numbers shall also be left out to ease implementation.*/
#define EaBlockNumber	(EcucIntegerParamDef)50


/*Size of a logical block in bytes*/
#define EaBlockSize  (EcucIntegerParamDef)50



/*Marker for high priority data.
true: Block contains immediate data.
false: Block does not contain immediate data.*/
#define EaImmediateData  (EcucBooleanParamDef)false


/*Number of write cycles required for this block*/
#define EaNumberOfWriteCycles (EcucIntegerParamDef)0

/*Reference to the device this block is stored in*/
#define EaDeviceIndex


//////////////////////////////////////


/*Management overhead per logical block in bytes.
Note: If the management overhead depends on the block size or block 
location a formula has to be provided that allows the configurator to 
calculate the management overhead correctly.*/


#define EaBlockOverhead (EcucIntegerParamDef)0

/*Management overhead per logical block in bytes.
Note: If the management overhead depends on the block size or block 
location a formula has to be provided that allows the configurator to 
calculate the management overhead correctly.*/
#define EaPageOverhead (EcucIntegerParamDef)0



/*Management overhead per page in bytes.
Note: If the management overhead depends on the block size or block 
location a formula has to be provided that allows the configurator to 
calculate the management overhead correctly*/
#define EaPageOverhead (EcucIntegerParamDef)0

//////////////////////////////////////////////

#endif /* EA_CFG_H */