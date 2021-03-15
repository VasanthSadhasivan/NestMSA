HIP_PATH?= $(wildcard /opt/rocm-4.0.0/hip)
ifeq (,$(HIP_PATH))
	HIP_PATH=../../..
endif

HIPCC=$(HIP_PATH)/bin/hipcc

TARGET=hcc

SOURCES_GPU = nest_msa_gpu.cpp main_gpu.cpp 
SOURCES_CPU = nest_msa.cpp main_nogpu.cpp

OBJECTS_GPU = $(SOURCES_GPU:.cpp=.o)
OBJECTS_CPU = $(SOURCES_CPU:.cpp=.o)

EXECUTABLE_GPU=./main_gpu
EXECUTABLE_CPU=./main_cpu

.PHONY: test


all: $(EXECUTABLE_GPU) $(EXECUTABLE_CPU) 

CXXFLAGS =-Wall -pg
CXX=$(HIPCC)


$(EXECUTABLE_GPU): $(OBJECTS_GPU)
	$(HIPCC) $(OBJECTS_GPU) $(CXXFLAGS) -o $@

$(EXECUTABLE_CPU): $(OBJECTS_CPU)
	$(HIPCC) $(OBJECTS_CPU) $(CXXFLAGS) -o $@

test: $(EXECUTABLE_GPU) $(EXECUTABLE_CPU)
	$(EXECUTABLE_CPU) seq.txt
	gprof $(EXECUTABLE_CPU) gmon.out > timing_results/cpu_analysis.txt
	rm -f gmon.out
	/opt/rocm-4.0.0/bin/rocprof --stats $(EXECUTABLE_GPU) seq.txt
	mv results.stats.csv timing_results/gpu_analysis.csv
	rm -f results.sys.info.txt
	rm -f results.json
	rm -f results.db
	rm-f  results.csv

setup: clean
	export PATH=$PATH:/opt/rocm-4.0.0/bin/

clean:
	rm -f $(EXECUTABLE_GPU)
	rm -f $(EXECUTABLE_CPU)
	rm -f $(OBJECTS_GPU)
	rm -f $(OBJECTS_CPU)
	rm -f $(HIP_PATH)/src/*.o
	rm -f gmon.out
	rm -f timing_results/cpu_analysis.txt
	rm -f timing_results/gpu_analysis.csv
	rm -f results.csv
	rm -f results.db
	rm -f results.json
	rm -f results.sysinfo.txt
	rm -f nest_msa_gpu
	rm -f nest_msa
	rm -f nest_msa_nogpu
