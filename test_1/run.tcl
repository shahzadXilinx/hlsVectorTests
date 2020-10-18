# Create project
open_project proj -reset

# Add the testbench
add_files -tb main.cpp -cflags "-std=gnu++14"

# Add the kernel
add_files kernel.cpp -cflags "-std=gnu++14"

set_top kernel

# Create solution
open_solution -reset solution

set_part virtexuplus
create_clock -period 3

config_flow -target vitis
config_interface -default_slave_interface s_axilite

csim_design
csynth_design
cosim_design
#export_design -rtl verilog -flow syn

close_solution

close_project

exit
