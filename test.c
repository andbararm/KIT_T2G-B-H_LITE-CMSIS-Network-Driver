#include "RTE_Components.h"             // Component selection
#include CMSIS_device_header            // Device header
#include <stdlib.h>
#include "cy_pdl.h"
#include "cybsp.h"
#include "rl_net.h"                     // Network:CORE
#include <stdio.h>

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)

__asm( ".global __ARM_use_no_argv" ) ;

#endif



/* IP address change notification */
void netDHCP_Notify( uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len )
{
  static uint8_t ip_addr[NET_ADDR_IP6_LEN];
  static char    ip_ascii[40];

  (void)if_num;
  (void)val;
  (void)len;

  if( option == NET_DHCP_OPTION_IP_ADDRESS )
  {
    netIF_GetOption( NET_IF_CLASS_ETH, netIF_OptionIP4_Address, ip_addr, sizeof( ip_addr ) ) ;
    netIP_ntoa( NET_ADDR_IP4, ip_addr, ip_ascii, sizeof( ip_ascii ) ) ;

    printf( "IP4:%-16s\n", ip_ascii ) ;
  }
}

#include "Driver_ETH_MAC.h"             // CMSIS Driver:Ethernet MAC


void netETH_Notify (uint32_t if_num, netETH_Event event, uint32_t val) {
  NET_ETH_LINK_INFO *info;

  int32_t status,arg;
  
  switch (event) {
    case netETH_LinkDown:
      printf ("Link is down\n");
      break;
    case netETH_LinkUp:
      printf ("Link is up\n");
      arg = 0;
      info = (NET_ETH_LINK_INFO *)&val;
      switch (info->speed) {
        case 0:
          arg |= ARM_ETH_MAC_SPEED_10M;
          printf ("10 MBit\n");
          break;
        case 1:
          arg |= ARM_ETH_MAC_SPEED_100M;
          printf ("100 MBit\n");
          break;
        case 2:
          arg |= ARM_ETH_MAC_SPEED_1G;
          printf ("1 GBit\n");
          break;
      }
      switch (info->duplex) {
        case 0:
          printf ("Half duplex\n");
          arg |= ARM_ETH_MAC_DUPLEX_HALF;
          break;
        case 1:
          arg |= ARM_ETH_MAC_DUPLEX_FULL;
          printf ("Full duplex\n");
          break;
      }
      break;
    case netETH_Wakeup:
      printf ("Wakeup frame received\n");
      break;
    case netETH_TimerAlarm:
      printf ("Timer alarm\n");
      break;
  }
}







void DoSomething( void )
{
  
}


#if defined ( RTE_CMSIS_RTOS2 )

#include <cmsis_os2.h>

extern osThreadId_t MainThreadId ;
osThreadId_t MainThreadId ;


extern void MainThread( void *Argument ) ;
__NO_RETURN void MainThread( void *Argument )
{
  ( void )Argument ;
  DoSomething( ) ;

  netInitialize( ) ;

  while( 1 )
  {
#ifdef CORE_NAME_CM0P
    if( Cy_GPIO_Read( CYBSP_USER_BTN1_PORT, CYBSP_USER_BTN1_PIN ) )
    {
      Cy_GPIO_Inv( LED1_PORT, LED1_PIN ) ;
    }
#endif
#ifdef CORE_NAME_CM7_0
    if( Cy_GPIO_Read( CYBSP_USER_BTN2_PORT, CYBSP_USER_BTN2_PIN ) )
    {
      Cy_GPIO_Inv( LED2_PORT, LED2_PIN ) ;
    }
#endif
    osDelay( 100 ) ;
  }
}


void start_CM7_0( void )
{
  uint32_t value ;
  
  CPUSS->CM7_0_PWR_CTL = ( 0x05FA << CPUSS_UDB_PWR_CTL_VECTKEYSTAT_Pos ) | ( 3 << CPUSS_UDB_PWR_CTL_PWR_MODE_Pos ) ;
  value = CPUSS->CM7_0_CTL ;
  CPUSS->CM7_0_CTL = ( value & ~( CPUSS_CM7_0_CTL_CPU_WAIT_Msk ) ) ;
  value = SRSS->CLK_ROOT_SELECT[ 1 ] ;
  SRSS->CLK_ROOT_SELECT[ 1 ] = ( value | ( 1 << SRSS_CLK_ROOT_SELECT_ENABLE_Pos ) ) ;
}
void stop_CM7_0( void )
{
  uint32_t value ;
  
  CPUSS->CM7_0_PWR_CTL = ( 0x05FA << CPUSS_UDB_PWR_CTL_VECTKEYSTAT_Pos ) | ( 0 << CPUSS_UDB_PWR_CTL_PWR_MODE_Pos ) ;
  value = CPUSS->CM7_0_CTL ;
  CPUSS->CM7_0_CTL = ( value | CPUSS_CM7_0_CTL_CPU_WAIT_Msk ) ;
  value = SRSS->CLK_ROOT_SELECT[ 1 ] ;
  SRSS->CLK_ROOT_SELECT[ 1 ] = ( value &~ ( 1 << SRSS_CLK_ROOT_SELECT_ENABLE_Pos ) ) ;
}


int retarget_stdio_init (void) ;

int main( void )
{
#ifdef CORE_NAME_CM0P
  init_cycfg_all( ) ;
  //start_CM7_0( ) ;
  stop_CM7_0( ) ;
#else
  SCB_DisableDCache(); // Disables, Because DMA in ETH will also read/write SRAM
#endif
  
  //SCB_DisableDCache( ) ; // Disables, Because DMA in ETH will also read/write SRAM
  __enable_irq( ) ;
  retarget_stdio_init( ) ;
  osKernelInitialize( ) ;
  MainThreadId = osThreadNew( MainThread, NULL, NULL ) ;
  if( MainThreadId )
  {
    osKernelStart( ) ;
  }
  return( 0 ) ;
}

#else

int main( void )
{
  DoSomething( ) ;
  return( 0 ) ;
}

#endif
