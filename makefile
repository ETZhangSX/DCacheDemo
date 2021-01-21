#-----------------------------------------------------------------------

APP       := DCacheDemo
TARGET    := DCacheClient
CONFIG    :=
STRIP_FLAG:= N
TARS2CPP_FLAG:= --promise-downward --json

INCLUDE   += -I./
LIB       +=
LOCAL_SRC   += $(sort $(wildcard ./*.cpp))
LOCAL_SRC   += $(sort $(TARS_CPP) $(HCE_CPP))

#-----------------------------------------------------------------------

include /usr/local/tars/cpp/makefile/makefile.tars
