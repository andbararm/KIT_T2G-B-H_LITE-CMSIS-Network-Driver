#include "RTE_Components.h"             // Component selection
#include CMSIS_device_header            // Device header
#include "cy_pdl.h"
#include "cybsp.h"
#include <stdlib.h>
#include <stdio.h>


void start_CM7_0( void )
{
  uint32_t value ;
  
  CPUSS->CM7_0_VECTOR_TABLE_BASE = BASE_CODE_FLASH_CM7_0 ;
  CPUSS->CM7_0_PWR_CTL = ( 0x05FA << CPUSS_UDB_PWR_CTL_VECTKEYSTAT_Pos ) | ( 3 << CPUSS_UDB_PWR_CTL_PWR_MODE_Pos ) ;
  value = CPUSS->CM7_0_CTL ;
  CPUSS->CM7_0_CTL = ( value & ~( CPUSS_CM7_0_CTL_CPU_WAIT_Msk ) ) ;
  value = SRSS->CLK_ROOT_SELECT[ 1 ] ;
  SRSS->CLK_ROOT_SELECT[ 1 ] = ( value | ( 1 << SRSS_CLK_ROOT_SELECT_ENABLE_Pos ) ) ;
}


void start_CM7_1( void )
{
  uint32_t value ;
  
  CPUSS->CM7_1_VECTOR_TABLE_BASE = BASE_CODE_FLASH_CM7_1 ;
  CPUSS->CM7_1_PWR_CTL = ( 0x05FA << CPUSS_UDB_PWR_CTL_VECTKEYSTAT_Pos ) | ( 3 << CPUSS_UDB_PWR_CTL_PWR_MODE_Pos ) ;
  value = CPUSS->CM7_1_CTL ;
  CPUSS->CM7_1_CTL = ( value & ~( CPUSS_CM7_1_CTL_CPU_WAIT_Msk ) ) ;
  value = SRSS->CLK_ROOT_SELECT[ 1 ] ;
  SRSS->CLK_ROOT_SELECT[ 1 ] = ( value | ( 1 << SRSS_CLK_ROOT_SELECT_ENABLE_Pos ) ) ;
}


int app_main( void )
{
  uint32_t count ;
  
  init_cycfg_all( ) ;
  start_CM7_0( ) ;
  start_CM7_1( ) ;

  while( 1 )
  {
    Cy_GPIO_Inv( LED1_PORT, LED1_PIN ) ;
    for( count = 0 ; count < 0x100000 ; count++ )
    {
      __NOP() ;
    }
  }
}
