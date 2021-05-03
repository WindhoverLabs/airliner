#############################################################################
#
#   Copyright (c) 2021 Windhover Labs, L.L.C. All rights reserved.
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
# 3. Neither the name Windhover Labs nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
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


buildliner_add_app_def(io_lib
    FILE IO_LIB
    SOURCES
	    ${CMAKE_CURRENT_SOURCE_DIR}/../src/formats/tctf.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/../src/formats/tmtf.c
	    #${CMAKE_CURRENT_SOURCE_DIR}/../src/services/trans_udp.c
	    #${CMAKE_CURRENT_SOURCE_DIR}/../src/services/trans_rs422.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/../src/services/trans_select.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/../src/services/tm_sdlp.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/../src/services/tm_sync.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/../src/services/cop1.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/../src/services/tc_sync.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/io_lib_init.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/io_lib_utils.c

    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../public_inc

    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/formats
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/services

    DESIGN_DOCS ${CMAKE_CURRENT_SOURCE_DIR}/detail_doxy.in
)
