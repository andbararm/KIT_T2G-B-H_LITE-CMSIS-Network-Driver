int app_main( void ) ;
int retarget_stdio_init (void) ;

int main( void )
{
  retarget_stdio_init( ) ;
  app_main( ) ;
}
