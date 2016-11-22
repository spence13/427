##############################################################################
## Filename:          C:\Madsen\SpaceInvadersCreativeHW/drivers/nes_controller_v1_00_a/data/nes_controller_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Thu Nov 19 13:48:36 2015 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "nes_controller" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
