# This is where we actually build the elf2cfetbl tool
set(CMAKE_C_FLAGS "-g -m32 -g -Wformat=0")
set(CMAKE_CXX_FLAGS "-g -m32 -g -Wformat=0")
add_executable(elf2cfetbl ${CMAKE_CURRENT_SOURCE_DIR}/elf2cfetbl.c )

target_include_directories(elf2cfetbl PUBLIC ${PARSED_ARGS_CONFIG})
target_include_directories(elf2cfetbl PUBLIC ${CFE_INC_DIRS})
target_include_directories(elf2cfetbl PUBLIC ${OSAL_INC_DIRS})
target_include_directories(elf2cfetbl PUBLIC ${PSP_INC_DIRS})
target_include_directories(elf2cfetbl PUBLIC ${PARSED_ARGS_TARGET_PSP}/inc)