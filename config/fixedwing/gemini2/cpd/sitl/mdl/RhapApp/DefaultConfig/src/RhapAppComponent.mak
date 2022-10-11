
############# Target type (Debug/Release) ##################
############################################################
CPPCompileDebug=-O0 -g
CPPCompileRelease=
LinkDebug=-g
LinkRelease=

VSB_HOME=

ConfigurationCPPCompileSwitches=   -I. $(LLVM_FLAGS) -I$(OMROOT) -I$(OMROOT)/LangCpp -I$(OMROOT)/LangCpp/oxf -DVxWorks $(INST_FLAGS) $(INCLUDE_PATH) $(CPPCompileDebug) -c 

##################################################
######### Definitions and flags ##################
##################################################
CPU = SIMNT
TOOL = llvm

TARGET_ARCH=x64

ifeq ($(USE_SMP),TRUE)
	VXBUILD=SMP
	ifeq ($(TARGET_ARCH),x64)
		VXBUILD=LP64 SMP
	endif 
else
	ifeq ($(TARGET_ARCH),x64)
		VXBUILD=LP64
     endif
endif
ifeq ($(TARGET_ARCH),x64)
	LINK_64BIT_FLAGS=$(LDFLAG_64BIT)
	LIB_SUFFIX=$(RTP_SUFFIX)$(TOOL)$(TARGET_ARCH)
else
	LIB_SUFFIX=$(RTP_SUFFIX)$(TOOL)
endif

INCLUDE_QUALIFIER=-I
LIB_CMD=$(AR)
LIB_FLAGS=$(ARFLAGS)
RHP_LIB_EXT=.a

ifeq ($(VSB_HOME),)
all :
	@echo The VxWorks Source Build project path has not been defined in Rhapsody. You must provide a value for the property VSBRootDirectory.; exit 0
else
####################################################
############### Context generated macros ###########

FLAGSFILE=
RULESFILE=
OMROOT="C:/Program Files/IBM/Rhapsody/9.0.1/Share"
RHPROOT="C:/Program Files/IBM/Rhapsody/9.0.1"
FRAMEWORK_LIB_ROOT="C:/ProgramData/IBM/Rhapsody/9.0.1x64/UserShare"
UNQUOTEDOMROOT=C:/Program\ Files/IBM/Rhapsody/9.0.1/Share

UNQUOTEDRHPROOT=C:/Program Files/IBM/Rhapsody/9.0.1

UNQUOTED_FRAMEWORK_LIB_ROOT=C:/ProgramData/IBM/Rhapsody/9.0.1x64/UserShare

CPP_EXT=.cpp
H_EXT=.h
OBJ_EXT=.o
EXE_EXT=.out
LIB_EXT=.a

INSTRUMENTATION=None

TIME_MODEL=RealTime

TARGET_TYPE=Executable

TARGET_NAME=RhapAppComponent

all : $(TARGET_NAME)$(EXE_EXT) RhapAppComponent.mak

TARGET_MAIN=MainRhapAppComponent

LIBS=

INCLUDE_PATH= \
  $(INCLUDE_QUALIFIER)$(OMROOT)/LangCpp/osconfig/VxWorks

ADDITIONAL_OBJS=

OBJS= \
  CfsAppBase.o \
  RhapApp.o \
  SampleApp.o



include $(UNQUOTEDOMROOT)/MakeTmpl/vxrhp.mk
endif

LINK_FLAGS=$(LinkDebug)   $(LINK_64BIT_FLAGS) $(LD_EMULATION)
LINK_CMD=$(LD) $(LINK_FLAGS)

OBJ_DIR=

ifeq ($(OBJ_DIR),)
CREATE_OBJ_DIR=
CLEAN_OBJ_DIR=
else
CREATE_OBJ_DIR= $(if $(wildcard $(OBJ_DIR)),, mkdir  $(OBJ_DIR))
CLEAN_OBJ_DIR=
endif


####################################################
############### Predefined macros ##################
$(OBJS) : $(INST_LIBS) $(OXF_LIBS)


ifeq ($(INSTRUMENTATION),Animation)
INST_FLAGS=-DOMANIMATOR -DUSE_IOSTREAM
INST_INCLUDES=-I$(OMROOT)/LangCpp/aom
INST_LIBS= $(FRAMEWORK_LIB_ROOT)/LangCpp/lib/vxaomanim$(CPU)$(LIB_SUFFIX)$(RHP_LIB_EXT)
OXF_LIBS=$(FRAMEWORK_LIB_ROOT)/LangCpp/lib/vxoxfinst$(CPU)$(LIB_SUFFIX)$(RHP_LIB_EXT) $(FRAMEWORK_LIB_ROOT)/LangCpp/lib/vxomcomappl$(CPU)$(LIB_SUFFIX)$(RHP_LIB_EXT)
SOCK_LIB=
else
ifeq ($(INSTRUMENTATION),Tracing)
INST_FLAGS=-DOMTRACER -DUSE_IOSTREAM
INST_INCLUDES=-I$(OMROOT)/LangCpp/aom -I$(OMROOT)/LangCpp/tom
INST_LIBS=$(FRAMEWORK_LIB_ROOT)/LangCpp/lib/vxtomtrace$(CPU)$(LIB_SUFFIX)$(RHP_LIB_EXT) $(FRAMEWORK_LIB_ROOT)/LangCpp/lib/vxaomtrace$(CPU)$(LIB_SUFFIX)$(RHP_LIB_EXT)
OXF_LIBS= $(FRAMEWORK_LIB_ROOT)/LangCpp/lib/vxoxfinst$(CPU)$(LIB_SUFFIX)$(RHP_LIB_EXT) $(FRAMEWORK_LIB_ROOT)/LangCpp/lib/vxomcomappl$(CPU)$(LIB_SUFFIX)$(RHP_LIB_EXT)
SOCK_LIB=
else
ifeq ($(INSTRUMENTATION),None)
INST_FLAGS=
INST_INCLUDES=
INST_LIBS=
OXF_LIBS=$(FRAMEWORK_LIB_ROOT)/LangCpp/lib/vxoxf$(CPU)$(LIB_SUFFIX)$(RHP_LIB_EXT)
SOCK_LIB=
endif
endif
endif



###############################################################
################## Context generated dependencies #############






CfsAppBase.o : CfsAppBase.cpp CfsAppBase.h    SampleApp.h 
	@echo Compiling CfsAppBase.cpp
	$(CREATE_OBJ_DIR)
	@$(CXX) $(C++FLAGS) $(ConfigurationCPPCompileSwitches)  -o CfsAppBase.o CfsAppBase.cpp




RhapApp.o : RhapApp.cpp RhapApp.h    SampleApp.h CfsAppBase.h 
	@echo Compiling RhapApp.cpp
	$(CREATE_OBJ_DIR)
	@$(CXX) $(C++FLAGS) $(ConfigurationCPPCompileSwitches)  -o RhapApp.o RhapApp.cpp




SampleApp.o : SampleApp.cpp SampleApp.h    CfsAppBase.h RhapApp.h 
	@echo Compiling SampleApp.cpp
	$(CREATE_OBJ_DIR)
	@$(CXX) $(C++FLAGS) $(ConfigurationCPPCompileSwitches)  -o SampleApp.o SampleApp.cpp







$(TARGET_MAIN)$(OBJ_EXT) : $(TARGET_MAIN)$(CPP_EXT) $(OBJS) 
		@echo Compiling $(TARGET_MAIN)$(CPP_EXT)
		@$(CXX) $(C++FLAGS) $(ConfigurationCPPCompileSwitches) -o $(TARGET_MAIN)$(OBJ_EXT) $(TARGET_MAIN)$(CPP_EXT)

#################################################################
define BUILD_EXE
@echo Linking and Munching $(TARGET_NAME)$(EXE_EXT)
@$(LINK_CMD) -r -o $(TARGET_NAME).tmp \
$(TARGET_MAIN)$(OBJ_EXT) $(OBJS) $(ADDITIONAL_OBJS) \
$(LIBS) \
$(INST_LIBS) \
$(OXF_LIBS) \
$(INST_LIBS) \
$(SOCK_LIB)
@ $(RM) $(TARGET_NAME)$(EXE_EXT) ctdt.c ctdt.o
@$(NM) $(TARGET_NAME).tmp | $(MUNCH) > ctdt.c
@$(CC) $(CC_ARCH_SPEC) $(CFLAGS) $(ConfigurationCPPCompileSwitches) -o ctdt.o -c ctdt.c
@$(LINK_CMD) -r $(LinkDebug) $(OMLDSCRIPT) -o $@ $(TARGET_NAME).tmp ctdt.o
@ $(RM) ctdt.c ctdt.o $(TARGET_NAME).tmp
endef



############ Predefined linking instructions #####################
$(TARGET_NAME)$(EXE_EXT): $(OBJS) $(ADDITIONAL_OBJS) $(TARGET_MAIN)$(OBJ_EXT) RhapAppComponent.mak 
	$(BUILD_EXE)
	

$(TARGET_NAME)$(RHP_LIB_EXT) : $(OBJS) $(ADDITIONAL_OBJS) RhapAppComponent.mak
	@echo Building library $@
	@$(LIB_CMD) $(LIB_FLAGS) $(TARGET_NAME)$(RHP_LIB_EXT) $(OBJS) $(ADDITIONAL_OBJS) $(LIBS)

clean:
cleanall: clean
	@echo Cleanup
	$(RM) $(TARGET_MAIN)$(OBJ_EXT)
	$(RM) $(TARGET_NAME)$(RHP_LIB_EXT)
	$(RM) $(TARGET_NAME)$(EXE_EXT)
	$(RM) CfsAppBase.o
	$(RM) RhapApp.o
	$(RM) SampleApp.o
	$(CLEAN_OBJ_DIR)
