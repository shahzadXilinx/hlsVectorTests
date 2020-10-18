.PHONY: help

help::
	@echo  " Makefile Usage:"
	@echo  ""
	@echo  "  make build TARGET=<sw_emu/hw_emu/hw> "
	@echo  "  Command to generate the design for specified target"
	@echo  ""
	@echo  "  make run TARGET=<sw_emu/hw_emu/hw> "
	@echo  "  Command to generate and run the design for specified target"
	@echo  ""
	@echo  "  make clean TARGET=<sw_emu/hw_emu/hw> "
	@echo  "  Command to remove the generated non-hardware files for specified target"
	@echo  ""


#PLATFORM := $(AWS_PLATFORM)
PLATFORM := xilinx_u200_xdma_201830_2
#PLATFORM := xilinx_aws-vu9p-f1-04261818_dynamic_5_0

ENABLE_STALL_TRACE ?= no

TARGET := sw_emu

HOST_EXE := host.exe

XO_NAME := adder.$(TARGET)

XCLBIN := adder.$(TARGET).xclbin

SRC_REPO := ./src

ifeq ($(TARGET), sw_emu)
BUILD_DIR ?= ./build_sw_emu
endif

ifeq ($(TARGET), hw_emu)
BUILD_DIR ?= ./build_hw_emu
endif

ifeq ($(TARGET), hw)
BUILD_DIR ?= ./build
endif

HOST_SRC_CPP := $(SRC_REPO)/host.cpp 
#HOST_SRC_CPP += $(SRC_REPO)/idct.cpp

KERNEL_SRC_CPP := $(SRC_REPO)/adder.cpp

# Definition of include file locations
OPENCL_INCLUDE:= $(XRT_PATH)/include

KERNEL_INCLUDES := -I./src/


ifneq ($(shell expr $(shell g++ -dumpversion) \>= 5), 1)
ifndef XILINX_HLS
$(error [ERROR]: g++ version older. Please use 5.0 or above.)
else
CXX := $(XILINX_HLS)/tps/lnx64/gcc-6.2.0/bin/g++
LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):$(XILINX_VIVADO)/tps/lnx64/gcc-6.2.0/lib64
$(warning [WARNING]: g++ version older. Using g++ provided by the tool : $(CXX))
endif
endif


XCL2_SRC = ./includes/xcl2/xcl2.cpp

CXXFLAGS += -I./includes/xcl2


#CXXFLAGS += -D__USE_XOPEN2K8
CXXFLAGS += -I$(XILINX_XRT)/include/
CXXFLAGS += -I$(XILINX_HLS)/include/
CXXFLAGS += -I$(SRC_REPO)
CXXFLAGS += -O0 -g -Wall -fmessage-length=0 -std=c++14
CXXFLAGS += $(XCL2_SRC)


CXXLDFLAGS := -L$(XILINX_XRT)/lib/
CXXLDFLAGS += -lOpenCL -lpthread -lrt -lstdc++ -lxilinxopencl 

## Kernel Compiler and Linker Flags

VPPFLAGS := -t $(TARGET)
VPPFLAGS += --platform $(PLATFORM) -R1 --save-temps
VPPFLAGS += --temp_dir $(BUILD_DIR)
VPPFLAGS += --log_dir $(BUILD_DIR)
VPPFLAGS += --profile_kernel data:all:all:all:all

VPPFLAGS += --trace_memory DDR[3]

ifeq ($(ENABLE_STALL_TRACE),yes)
VPPFLAGS += --profile_kernel stall:all:all:all
endif

VPPFLAGS += --config adder.cfg

## Host Executable File Generation

compile_host: $(BUILD_DIR)/$(HOST_EXE)

$(BUILD_DIR)/$(HOST_EXE): $(HOST_SRC_CPP)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(HOST_SRC_CPP) $(CXXLDFLAGS)
	cp ./xrt.ini $(BUILD_DIR)/xrt.ini

## Kernel XO and Xclbin File Generation

$(BUILD_DIR)/$(XO_NAME).xo: $(KERNEL_SRC_CPP)
	mkdir -p $(BUILD_DIR)
	v++ $(VPPFLAGS) -c -k adder $(KERNEL_INCLUDES) $(KERNEL_SRC_CPP) -o $@

$(BUILD_DIR)/$(XCLBIN): $(BUILD_DIR)/$(XO_NAME).xo 
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(TARGET)
	v++ $(VPPFLAGS) -l -o $@ $(BUILD_DIR)/$(XO_NAME).xo 
	

## Emulation Files Generation

EMCONFIG_FILE = emconfig.json

$(BUILD_DIR)/$(EMCONFIG_FILE):
	 emconfigutil --nd 1  --platform $(PLATFORM) --od $(BUILD_DIR)

# primary build targets

.PHONY: all clean

## build the design without running host application

ifeq ($(TARGET), hw)
build: $(BUILD_DIR)/$(HOST_EXE) $(BUILD_DIR)/$(XCLBIN) $(BUILD_DIR)/$(EMCONFIG_FILE)
else
build: $(BUILD_DIR)/$(HOST_EXE) $(BUILD_DIR)/$(XCLBIN) $(BUILD_DIR)/$(EMCONFIG_FILE)
endif

## build the design and then run host application

run: build
	cp xrt.ini $(BUILD_DIR);
ifeq ($(TARGET), hw)
	cd $(BUILD_DIR) && unset XCL_EMULATION_MODE;    ./host.exe  ./$(XCLBIN) 

else
	cd $(BUILD_DIR) && XCL_EMULATION_MODE=$(TARGET) ./host.exe  ./$(XCLBIN)
endif
	
	
## Clean generated files

clean:
	rm -rf $(BUILD_DIR) host*.exe
