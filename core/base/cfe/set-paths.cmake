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
 
set(CFE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CFE_DOCS_DIR ${CFE_DIR}/docs)
set(CFE_FSW ${CFE_DIR}/fsw)
set(CFE_CORE_SRC ${CFE_FSW}/src)
set(CFE_TOOLS ${CFE_DIR}/tools)
set(CFE_UNIT_TEST ${CFE_FSW}/unit-test)
set(CFE_COMMANDER_DISPLAYS ${CFE_DIR}/cdr)
 
set(CFE_INC_DIRS
    ${CFE_CORE_SRC}/inc
    ${CFE_CORE_SRC}/inc/private
    ${CFE_CORE_SRC}/es
    ${CFE_CORE_SRC}/evs
    ${CFE_CORE_SRC}/fs
    ${CFE_CORE_SRC}/sb
    ${CFE_CORE_SRC}/tbl
    ${CFE_CORE_SRC}/time
)



