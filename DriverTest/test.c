#include "RTE_Components.h"             // Component selection
#include CMSIS_device_header            // Device header
#include <cmsis_os2.h>
#include <cmsis_dv.h>
#include "cy_pdl.h"
#include "cybsp.h"
#include <stdlib.h>
#include <stdio.h>

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)

__asm( ".global __ARM_use_no_argv" ) ;

#endif

extern osThreadId_t MainThreadId ;
osThreadId_t MainThreadId ;

__NO_RETURN void MainThread( void *Argument )
{
  ( void )Argument ;

  osThreadNew(cmsis_dv, NULL, NULL);

  while( 1 )
  {
    if( Cy_GPIO_Read( CYBSP_USER_BTN2_PORT, CYBSP_USER_BTN2_PIN ) )
    {
      Cy_GPIO_Inv( LED2_PORT, LED2_PIN ) ;
    }
    osDelay( 200 ) ;
  }
}



int app_main( void )
{
  uint32_t count ;
  
  osKernelInitialize( ) ;
  MainThreadId = osThreadNew( MainThread, NULL, NULL ) ;
  if( MainThreadId )
  {
    osKernelStart( ) ;
  }
  return( 0 ) ;
}
