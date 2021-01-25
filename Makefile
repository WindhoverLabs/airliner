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

SHELL := /bin/bash

CONFIG_DIR   := config
TARGET_NAMES := core-only/pc-linux slim/pc-linux slim-apps-only/pc-linux full/pc-linux full-apps-only/pc-linux 
BUILD_TYPES  := host target
ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
LOCAL_GIT_FOLDER_DIR  := ~/gitsource
LIB_DIR := ~/xsdk-workspace/firmware_r5_0_bsp/psu_cortexr5_0/lib

export PROJECT_SOURCE_DIR=${PWD}

help::
	@echo '---=== Buildliner ===---                                                        '
	@echo '                                                                                '
	@echo 'Specify a target to build.  Available targets are:                              '
	@echo '                                                                                '
	@echo '  Flight Software Builds                                                        '
	@echo '    core-only/pc-linux      : This builds only the Core Binary for Linux running'
	@echo '                              on a PC.                                          '
	@echo '    slim/pc-linux           : This builds the simplest, most basic build with   '
	@echo '                              the Core Binary and applications for scheduling,  '
	@echo '                              commanding, and telemetry.                        '  
	@echo '    slim-apps-only/pc-linux : This builds the simplest, most basic build with   '
	@echo '                              the Core Binary and applications for scheduling,  '
	@echo '                              commanding, and telemetry.                        ' 
	@echo '    full/pc-linux           : This builds a full compliment of the Core Binary  '
	@echo '                              and the most common apps for all targets and      '
	@echo '                              use cases.                                        '
	@echo '    full-apps-only/pc-linux : This builds a full compliment of the Core Binary  '
	@echo '                              and the most common apps for all targets and      '
	@echo '                              use cases.                                        '
	@echo '    clean                   : This will clean all build flight software build   '
	@echo '                              targets.  This includes the Commander workspace,  '
	@echo '                              if one was generated.                             '
	@echo '                                                                                '
	@echo '  Utilities                                                                     '
	@echo '    python-env              : This generates a Python3 virtual environment in   '
	@echo '                              the "venv" directory with all the python          '
	@echo '                              modules required to use the Buildliner build      '
	@echo '                              system.                                           '
       
	
.PHONY: help Makefile
	

$(TARGET_NAMES)::
	@echo 'Building '$@'.'
	@idx=1; \
	for name in $(TARGET_NAMES); do \
		if [ "$$name" == "$@" ] ; then \
			break; \
		fi; \
		((idx++)); \
	done; \
	TARGET_NAME=$$(echo ${TARGET_NAMES} | cut -d " " -f $$idx); \
		echo "Generating complete design/configuration definition file, 'wh_defs.yaml'"; \
	echo "$(CONFIG_DIR)/$$TARGET_NAME/wh_config.yaml"; \
	if [ -f "$(CONFIG_DIR)/$$TARGET_NAME/wh_config.yaml" ]; then \
		mkdir -p build/$$TARGET_NAME/target; \
		python3 core/base/tools/config/wh_defgen.py $(CONFIG_DIR)/$$TARGET_NAME/ build/$$TARGET_NAME/target/wh_defs.yaml; \
	fi; \
	for buildtype in $(BUILD_TYPES); do \
		if [ -d "$(CONFIG_DIR)/$$TARGET_NAME/$$buildtype" ]; then \
			mkdir -p build/$$TARGET_NAME/$$buildtype; \
			(cd build/$$TARGET_NAME/$$buildtype; \
			cmake -DBUILDNAME:STRING=$$TARGET_NAME -DBUILDTYPE:STRING=$$buildtype -G"Eclipse CDT4 - Unix Makefiles" \
			-DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE CMAKE_BUILD_TYPE=Debug $(ROOT_DIR); \
			$(MAKE) --no-print-directory); \
		fi; \
	done;
	

python-env::
	virtualenv -p python3 venv || exit -1
	(source venv/bin/activate || exit -1; pip install -r core/tools/auto-yamcs/src/requirements.txt || exit -1)
	@echo 'Created python3 virtual environment.                                            '
	@echo '                                                                                '
	@echo 'Activate:                                                                       '
	@echo '    source venv/bin/activate                                                    '
	@echo '                                                                                '
	@echo 'Deactivate:                                                                     '
	@echo '    deactivate                                                                  '
	@echo '                                                                                '


deploy::
	@if test -f "tools/git_tools/deploy.py"; then \
		if test ! -f "tools/git_tools/config.yaml"; then \
			echo '"tools/git_tools/config.yaml" not found. Using the template.'; \
			python3 tools/git_tools/deploy.py tools/git_tools/config-template.yaml || exit -1; \
			rc=$?; \
		else \
			python3 tools/git_tools/deploy.py tools/git_tools/config.yaml || exit -1; \
			rc=$?; \
		fi; \
		if [[ $rc -ne 0 ]]; then \
		    echo 'Deploy failed.                                                      '; \
		else \
		    rm tools/git_tools/deploy.py; \
		    echo '                                                                        '; \
		    echo 'Project is deployed.                                                    '; \
		    echo 'The deploy script is deleted to prevent accidental redeploy.            '; \
		    echo 'Check this newly created project into your own version control system.  '; \
		    echo '                                                                        '; \
		fi; \
	else \
		echo 'Project is already deployed and cannot be redeployed.                   '; \
		echo '                                                                        '; \
	fi
	

clean::
	@echo 'Cleaning flight software builds                                                 '
	rm -rf build
	
