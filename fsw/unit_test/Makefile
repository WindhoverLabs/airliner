#######################################################################################
#
# This makefile supports unit test execution, debugging, gcov coverage, and
# valgrind coverage.  Some of these options should not be used together.
#
# By default, an optimized unit test binary is built without debug symbols.
# This is suitable for execution, and assessment with valgrind.
#
# GCOV coverage is optional, and disabled by default.
# To enable gcov coverage metrics invoke via "GCOV=1 make".  This builds an
# unoptimized binary.  gcov metrics are parsed for simple results of the
# FLIGHT_OBJS.  Without GCOV a  "make gcov" will run the tests but no coverage
# metrics would be produced.  Valgrind should be run without GCOV.
#
# DEBUG is optional, and disabled by default.  To enable debug symbols
# invoke via "DEBUG=1 make".  DEBUG will be ignored if used with GCOV.
#
# Valgrind support for memcheck and massif assumes valgrind and ms_print are
# installed.

#
# APPDIR is the location of the flight software source files under test
#
APPDIR=$(CFS_APP_SRC)/mpu6050

#
# APPTARGET is the name of the unit test fixture binary
#
APPTARGET=ut_mpu6050

#
# FLIGHT_OBJS specifies flight software object files under test
#
FLIGHT_OBJS := mpu6050_app.o
FLIGHT_OBJS += mpu6050_config_utils.o
FLIGHT_OBJS += mpu6050_cds_utils.o

#
# FLIGHT_INC specifies the directories of the flight software under test
#
FLIGHT_INC := -I$(APPDIR)/fsw/src \
              -I$(CFS_MISSION)/config/mission_inc \
              -I$(CFS_MISSION)/config/pc-linux/inc

#
# UT_OBJS specifies the unit test implementation
#
UT_OBJS := mpu6050_testrunner.o
UT_OBJS += mpu6050_app_test.o
UT_OBJS += mpu6050_cmds_test.o
UT_OBJS += mpu6050_config_tbl_test.o
UT_OBJS += mpu6050_cds_tbl_test.o
UT_OBJS += mpu6050_test_utils.o

#
# UT_TEST_RUNNER specifies the name of the test runner
#
UT_TEST_RUNNER = $(APPTARGET).bin

#
# ut_assert library defines
#
UT_DEFINES := -DUT_VERBOSE
#UT_DEFINES += -DUT_STOP_ON_ASSERT_FAILURE

#
# Setup the source path for this build
#
VPATH := . \
         $(CFS_MISSION)/tools/ut_assert/src \
         $(OSAL)/ut_assert/src \
         $(APPDIR)/fsw/src

#
# Setup the include path for this build for cFE, OSAL, PSP, and ut_assert.
#
FRAMEWORK_INC := -I. \
                 -I$(CFS_MISSION)/tools/ut_assert/inc \
                 -I$(CFE_CORE_SRC)/inc \
                 -I$(CFE_CORE_SRC)/time \
                 -I$(OSAL_SRC)/inc \
                 -I$(CFE_PSP_SRC)/inc \
                 -I$(CFE_PSP_SRC)/pc-linux/inc

###############################################################################

#
# UTA_OBJS specifies ut_assert object files at the cFE level.
#
UTA_OBJS := utassert.o
UTA_OBJS += ut_cfe_es_hooks.o
UTA_OBJS += ut_cfe_es_stubs.o
UTA_OBJS += ut_cfe_evs_hooks.o
UTA_OBJS += ut_cfe_evs_stubs.o
UTA_OBJS += ut_cfe_fs_stubs.o
UTA_OBJS += ut_cfe_psp_eeprom_stubs.o
UTA_OBJS += ut_cfe_psp_memrange_stubs.o
UTA_OBJS += ut_cfe_psp_memutils_stubs.o
UTA_OBJS += ut_cfe_psp_ram_stubs.o
UTA_OBJS += ut_cfe_psp_timer_stubs.o
UTA_OBJS += ut_cfe_psp_watchdog_stubs.o
UTA_OBJS += ut_cfe_sb_hooks.o
UTA_OBJS += ut_cfe_sb_stubs.o
UTA_OBJS += ut_cfe_tbl_hooks.o
UTA_OBJS += ut_cfe_tbl_stubs.o
UTA_OBJS += ut_cfe_time_hooks.o
UTA_OBJS += ut_cfe_time_stubs.o
UTA_OBJS += utlist.o
UTA_OBJS += ut_osapi_stubs.o
UTA_OBJS += ut_osfileapi_stubs.o
UTA_OBJS += uttest.o
UTA_OBJS += uttools.o

#
# Compiler/linker definitions
#
CC=gcc
LINKER=gcc

#
# Compiler and Linker Options
#

ifdef DEBUG
DEBUGGER = -g
else
DEBUGGER =
endif

ifdef GCOV
GCOV_COPT = -fprofile-arcs -ftest-coverage -pg -p
GCOV_LOPT = -fprofile-arcs -ftest-coverage -pg -p -lgcov
OPTLEVEL = -O0
# override debug when using with gcov
DEBUGGER =
else
GCOV_COPT =
GCOV_LOPT =
OPTLEVEL = -O2
endif

# Default, don't apply any specific warnings to 3rd party code
WARNINGS =

# Warnings applied to flight code
FLIGHT_WARNINGS = -Wall -Wstrict-prototypes -Wundef -Wextra -Wpedantic

# For reference, here is an example list of various warnings you could apply to flight code:
#FLIGHT_WARNINGS = -Wall -Wstrict-prototypes -Wundef -Wpedantic -Wextra -Wconversion -Wdouble-promotion -Wcast-qual -Wcast-align -Wformat=2 -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wundef

LANGCFLAGS = --std=c99
PLATFORMCFLAGS = -DSOFTWARE_LITTLE_BIT_ORDER -D_EL -D_ix86_ -m32
PLATFORMLFLAGS = -m32

CFLAGS = $(DEBUGGER) $(GCOV_COPT) $(OPTLEVEL) $(UT_DEFINES) $(LANGCFLAGS) $(PLATFORMCFLAGS)

LOPT = $(GCOV_LOPT) $(PLATFORMLFLAGS)

$(FLIGHT_OBJS): WARNINGS=$(FLIGHT_WARNINGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(WARNINGS) $(FLIGHT_INC) $(FRAMEWORK_INC) $< -o $@

#
# The default "make" target
#
all: $(UT_TEST_RUNNER)

$(UT_TEST_RUNNER): $(UT_OBJS) $(UTA_OBJS) $(FLIGHT_OBJS)
	$(LINKER) $(LOPT) $(UT_OBJS) $(UTA_OBJS) $(FLIGHT_OBJS) -o $(UT_TEST_RUNNER)

.PHONY: clean run clean_run gcov clean_gcov vg_mem vg_massif clean_vg

clean: clean_gcov clean_run clean_vg
	@rm -f *.o *.gcno *.c.gcov $(UT_TEST_RUNNER)

run: $(UT_TEST_RUNNER)
	-lcov --directory . --zerocounters
	-rm -Rf coverage
	-mkdir -p coverage
	-./$(UT_TEST_RUNNER)

clean_run:
	@rm -f gmon.out

# Present gcov metrics for FLIGHT_OBJS (assumes gcov installed)
# Note, must build with "GCOV=1 make" to get an instrumented test runner.
gcov: run 
	@echo
	@gcov $(FLIGHT_OBJS:.o=.gcda) | sed 'N;s/\n/ /' | \
		sed -n '/File/p' | sed '/ads/d' | sed -e '/\.h/d'  | \
		sed 's/ Lines executed:/ /; s/File/gcov:/; s/of// '
	@echo
	@lcov --directory . --capture --output-file ./coverage/ut.info
	@genhtml -t $(UT_TEST_RUNNER) -o ./coverage ./coverage/ut.info

clean_gcov:
	@rm -f *.gcda *.info

# Valgrind memcheck (memory error detection)
vg_mem: $(UT_TEST_RUNNER)
	valgrind --tool=memcheck ./$(UT_TEST_RUNNER)

# Valgrind massif (heap & stack profiler)
vg_massif: $(UT_TEST_RUNNER)
	valgrind --tool=massif --heap=yes --stacks=yes  --massif-out-file=massif.out ./$(UT_TEST_RUNNER)
	ms_print massif.out

clean_vg:
	@rm -f massif.out*

# Disable suffixes and get rid of archaic RCS and SCCS match-everything rules.
# This is mostly for debugging. make -d spews without this.

.SUFFIXES:

%:: %,v
%:: RCS/%,v
%:: RCS/%
%:: s.%
%:: SCCS/s.%

#######################################################################################
    

