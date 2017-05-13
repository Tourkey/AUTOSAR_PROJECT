/*
 * Eep_Cfg.c
 *
 *  Created on: May 14, 2017
 *      Author: power
 */
#include "Eep_Cfg.h"
void Eep_InitCfg (void)
{
#if (EepDevErrorDetect==True)
    {
    //
    }
#else
    {
    //
    }
#endif

#if (EepUseInterrupts==True)
{
    //
}
#else
{  //
}
#endif

#if (EepVersionInfoApi==True)
{

}
#else
{

}
#endif
#if (EepWriteCycleReduction==True)
{

}
#else
{

}
#endif
}


