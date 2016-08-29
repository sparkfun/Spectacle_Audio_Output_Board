# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Dropbox\Projects\Spectacle\Spectacle_Audio_Output_Board\Firmware\Spectacle_Audio_Output_Board\Spectacle_Audio_Output_Board.cydsn\Spectacle_Audio_Output_Board.cyprj
# Date: Tue, 05 Jul 2016 21:11:39 GMT
#set_units -time ns
create_clock -name {Clock_1(routed)} -period 88.577097505668945 -waveform {0 44.2885487528345} [list [get_pins {ClockBlock/dclk_1}]]
create_clock -name {CyXTAL} -period 88.577097505668945 -waveform {0 44.2885487528345} [list [get_pins {ClockBlock/xtal}]]
create_clock -name {CyILO} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 13.888888888888886 -waveform {0 6.94444444444444} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 13.888888888888886 -waveform {0 6.94444444444444} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 3 7} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_clock -name {CyBUS_CLK(fixed-function)} -period 41.666666666666657 -waveform {0 13.8888888888889} [get_pins {ClockBlock/clk_bus_glb_ff}]
create_generated_clock -name {emFile_Clock_1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 7 13} -nominal_period 88.577097505668945 [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {Clock_2} -source [get_pins {ClockBlock/clk_sync}] -edges {1 27 51} -nominal_period 354.30839002267578 [list [get_pins {ClockBlock/dclk_glb_2}]]
create_generated_clock -name {UART_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 79 157} -nominal_period 1083.333333333333 [list [get_pins {ClockBlock/dclk_glb_3}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Dropbox\Projects\Spectacle\Spectacle_Audio_Output_Board\Firmware\Spectacle_Audio_Output_Board\Spectacle_Audio_Output_Board.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Dropbox\Projects\Spectacle\Spectacle_Audio_Output_Board\Firmware\Spectacle_Audio_Output_Board\Spectacle_Audio_Output_Board.cydsn\Spectacle_Audio_Output_Board.cyprj
# Date: Tue, 05 Jul 2016 21:11:25 GMT
