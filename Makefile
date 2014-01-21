TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

### = YF Commented out
###LIBRARY_IOC_WIN32 += andorCCD
###LIBRARY_IOC_Linux += andorCCD
###LIB_SRCS += andorCCD.cpp

###DATA+=GREY.PAL

###USE_GRAPHICSMAGICK=NO

LDFLAGS+=-L../lib 
LIBRARY_IOC = andorCCD
LIB_SRCS += andorCCD.cpp cin_power.c

#==================================

PROD_NAME = andorCCDApp
PROD_IOC  += $(PROD_NAME)

# <name>.dbd will be created from <name>Include.dbd
DBD += $(PROD_NAME).dbd
$(PROD_NAME)_DBD += andorCCDSupport.dbd
DBD += andorCCDSupport.dbd


PROD_SRCS += andorCCDApp_registerRecordDeviceDriver.cpp andorCCDAppMain.cpp

# Add locally compiled object code
PROD_LIBS += andorCCD cin
LIB_INSTALLS += ../lib/libcin.a

###PROD_SYS_LIBS_Linux += andor

include $(TOP)/ADApp/commonDriverMakefile

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

