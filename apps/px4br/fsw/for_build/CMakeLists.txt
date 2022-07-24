 #############################################################################
 #
 #   Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions
 # are met:
 #
 # 1. Redistributions of source code must retain the above copyright
 #    notice, this list of conditions and the following disclaimer.
 # 2. Redistributions in binary form must reproduce the above copyright
 #    notice, this list of conditions and the following disclaimer in
 #    the documentation and/or other materials provided with the
 #    distribution.
 # 3. Neither the name Windhover Labs nor the names of its contributors
 #    may be used to endorse or promote products derived from this software
 #    without specific prior written permission.
 #
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 # "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 # LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 # FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 # COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 # INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 # BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 # OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 # AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 # LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 # ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 # POSSIBILITY OF SUCH DAMAGE.
 #
 #############################################################################
        
        
get_property(NANOPB_DIR GLOBAL PROPERTY NANOPB_DIR)
        
message(${NANOPB_DIR})
message(${NANOPB_DIR})
message(${NANOPB_DIR})
message(${NANOPB_DIR})
message(${NANOPB_DIR})
message(${NANOPB_DIR})
message(${NANOPB_DIR})
message(${NANOPB_DIR})
message(${NANOPB_DIR})


#set(PX4_PROTOBUF_MSG_DEFS ${CMAKE_CURRENT_SOURCE_DIR}/../msg_defs/actuator_armed.proto)
#set(PROTOBUF_MSGS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/pb_msgs/)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_MODULE_PATH "${NANOPB_DIR}/extra")
find_package(Nanopb REQUIRED)
#include_directories(${NANOPB_DIR})
#file(MAKE_DIRECTORY ${PROTOBUF_MSGS_DIR})
message(${CMAKE_CURRENT_SOURCE_DIR}/../msg_defs/actuator_armed.proto)
nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/../msg_defs/actuator_armed.proto)

buildliner_add_app_def(px4br
    FILE PX4BR
    SOURCES  
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4_msgs.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4_serialization.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4_serialization.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_app.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_cmd_codes.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_cmds.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_cmds.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_events.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_msg.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_routes.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_routes.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_serialization.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_serialization.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4br_version.h

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/pb_msgs
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/px4_msgs
        ${NANOPB_INCLUDE_DIR}

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in

    REFERENCE_CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/../platform_inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../mission_inc
        
    COMMANDER_DISPLAYS 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cdr
)
