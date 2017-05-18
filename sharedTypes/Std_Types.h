#include "Compiler.h"
#include "Platform_Types.h"

typedef enum
{
	E_OK, 			// The requested job has been accepted. 
	E_NOT_OK		// The requested job has not been accepted.
	
}Std_ReturnType;


/*This type shall be used to request the version of a BSW module using the <Module
name>_GetVersionInfo() function.*/
typedef struct
{
	uint16 vendorID;
	uint16 moduleID;
	uint8 sw_major_version;
	uint8 sw_minor_version;
	uint8 sw_patch_version;
}Std_VersionInfoType;