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
 
add_subdirectory(${CFE_CORE_SRC}/es   es)
add_subdirectory(${CFE_CORE_SRC}/evs  evs)
add_subdirectory(${CFE_CORE_SRC}/fs   fs)
add_subdirectory(${CFE_CORE_SRC}/sb   sb)
add_subdirectory(${CFE_CORE_SRC}/tbl  tbl)
add_subdirectory(${CFE_CORE_SRC}/time time)

# Put all the CFE source lists together in one variable for convenience.
set(CFE_SRC ${CFE_ES_SRC} ${CFE_EVS_SRC} ${CFE_FS_SRC} ${CFE_SB_SRC} ${CFE_TBL_SRC} ${CFE_TIME_SRC} PARENT_SCOPE)

set(CFE_ES_SRC ${CFE_ES_SRC} PARENT_SCOPE)
set(CFE_EVS_SRC ${CFE_EVS_SRC} PARENT_SCOPE)
set(CFE_FS_SRC ${CFE_FS_SRC} PARENT_SCOPE)
set(CFE_SB_SRC ${CFE_SB_SRC} PARENT_SCOPE)
set(CFE_TBL_SRC ${CFE_TBL_SRC} PARENT_SCOPE)
set(CFE_TIME_SRC ${CFE_TIME_SRC} PARENT_SCOPE)
