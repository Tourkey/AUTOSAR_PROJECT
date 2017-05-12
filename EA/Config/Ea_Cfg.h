#include "NvM_Cbk.h"
#include "MemIf_Types.h"
#include "Eap.h"


#ifndef EA_GENERAL_H
#define EA_GENERAL_H

/*General configuration of the EEPROM abstraction module. This container 
lists block independent configuration parameters.*/


#define false 0
#define true 1

/*Switches the development error detection and notification on or off.
  true: detection and notification is enabled. 
  false: detection and notification is disabled */

#define EaDevErrorDetect	false


/*The period between successive calls to the main function in seconds.*/
#define EaMainFunctionPeriod 	10



/*Mapped to the job end notification routine provided by the upper layer 
module (NvM_JobEndNotification).*/
#define EaNvmJobEndNotification  


/*Mapped to the job error notification routine provided by the upper layer 
module (NvM_JobErrorNotification).*/
#define EaNvmJobErrorNotification


/*Pre-processor switch to enable and disable the polling mode for this 
module.
true: Polling mode enabled, callback functions (provided to EEP module) 
disabled.
false: Polling mode disabled, callback functions (provided to EEP module) 
enabled.*/
#define EaPollingMode false

/*Compile switch to enable / disable the function Ea_SetMode.*/
#define EaSetModeSupported true

/*Pre-processor switch to enable / disable the API to read out the modules
version information.
true: Version info API enabled.
false: Version info API disabled
*/
#define EaVersionInfoApi  false




/*The size in bytes to which logical blocks shall be aligned.*/
#define EaVirtualPageSize  50

#endif /* EA_GENERAL_H */

/////////////////////////////////////////






/*Block identifier (handle).
0x0000 and 0xFFFF shall not be used for block numbers (see EA006).
Range: 
min = 2^NVM_DATASET_SELECTION_BITS 
max = 0xFFFF -2^NVM_DATASET_SELECTION_BITS
Note: Depending on the number of bits set aside for dataset selection 
several other block numbers shall also be left out to ease implementation.*/
#define EaBlockConfiguration	50


/*Size of a logical block in bytes*/
#define EaBlockSize  50



/*Marker for high priority data.
true: Block contains immediate data.
false: Block does not contain immediate data.*/
#define EaImmediateData false


/*Number of write cycles required for this block*/
#define EaNumberOfWriteCycles

/*Reference to the device this block is stored in*/
#define EaDeviceIndex


//////////////////////////////////////



/*Management overhead per logical block in bytes.
Note: If the management overhead depends on the block size or block 
location a formula has to be provided that allows the configurator to 
calculate the management overhead correctly.*/


#define EaBlockOverhead

/*Management overhead per logical block in bytes.
Note: If the management overhead depends on the block size or block 
location a formula has to be provided that allows the configurator to 
calculate the management overhead correctly.*/

#define EaPageOverhead



/*Management overhead per page in bytes.
Note: If the management overhead depends on the block size or block 
location a formula has to be provided that allows the configurator to 
calculate the management overhead correctly*/
#define EaPageOverhead

//////////////////////////////////////////////
