##############################################################################
## Filename:          C:\Users\superman\Desktop\427\SpaceInvadersCreativeHW/drivers/dma_controller_v1_00_a/data/dma_controller_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Mon Dec 05 13:32:24 2016 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "dma_controller" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
