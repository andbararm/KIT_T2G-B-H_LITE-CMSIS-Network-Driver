this is work in progress ...

Activating the VIO driver from the BSP 1.1 adds a define RTE_VIO_KIT_T2G-B-H_LITE, which gived compilation warnings.

There are separate board layers for each core.
There is a common Device Configurator project used in the board layers.
It contains also the EMAC and PHY drivers (maybe not great, but works for me)

About the differnt projects:

1) CM0pBoot
- runs on the Cortex-M0p core
- calls the init function for the generated code (includes pin configuration)
- starts the Cortex-M7-0 core
- starts the Cortex-M7-1 core

2) NetworkTest
- runs on the Cortex-M7-0 core
- was the inital test, just calls netInitialize()
- link status and IP address are printed on USART

3) DriverTest
- runs on the Cortex-M7-0 core
- does CMSIS driver validation for the EMAC driver
- reports missing components, because Driver Validation still requires Keil::Compiler pack
- test results are output via UART
- mostly only "FAILED"

4) Network
- are the Networt refence applications
- changed used board layer to the Cortex-M7-0 layer
- tested HTTP_Server, website was visible, VIO driver still empty