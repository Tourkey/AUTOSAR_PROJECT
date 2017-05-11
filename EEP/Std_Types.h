/*
 * Std_Types.h
 *
 *  Created on: May 10, 2017
 *      Author: power
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_
//#include "Compiler.h"
//#include "Platform_Types.h"
typedef uint8 Std_ReturnType;
typedef struct {
   uint16 vendorID;
   uint16 moduleID;
   uint8 sw_major_version;
   uint8 sw_minor_version;
   uint8 sw_patch_version;
   }Std_VersionInfoType;




#endif /* STD_TYPES_H_ */
