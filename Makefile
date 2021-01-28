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

SPHINX_OPTS     ?=
SPHINX_BUILD    ?= sphinx-build
SPHINX_SOURCEDIR = .
SPHINX_BUILDDIR  = build/reference/default/target/docs
SPHINX_FSW_BUILD = reference/default

SHELL := /bin/bash

CONFIG_DIR   := config
#TARGET_PATHS := $(shell find config -mindepth 1 -maxdepth 5 -type d -path 'config/shared/*' -prune -o -exec expr {} : '[^/]*/\(.*\)' \; )
TARGET_PATHS :=  $(shell find ${CONFIG_DIR} -mindepth 2 -maxdepth 20 -type f -name 'wh_config.yaml' | sed -r 's,^[^/]*/,,' | sed -r 's|/[^/]+$$||' | sort -u)
TARGET_NAMES := $(shell echo ${TARGET_PATHS} )
BUILD_TYPES  := host target
ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

export PROJECT_SOURCE_DIR=${PWD}

help::
	@echo 'Specify a target to build.  Available targets are:                              '
	@echo '                                                                                '
	@echo '  Flight Software Builds                                                        '
	@echo '    bebop2                  : This will build flight software for the Parrot    '
	@echo '                              Bebop2 drone.                                     '
	@echo '    bebop2/sitl             : This will build the Software in the Loop          '
	@echo '                              (PC-Linux) version of the Parrot Bebop2 drone.    '
	@echo '    ocpoc/s1000             : This will build flight software for the OcPoC-Mini'
	@echo '                              hosted on the S1000 airframe with the Aerotenna   '
	@echo '                              uLanding landing radar system.                    '
	@echo '    ocpoc/quadx             : This will build flight software for the OcPoC-Mini'
	@echo '                              hosted on the generic quad-X airframe with the    '
	@echo '                              Aerotenna uLanding landing radar system.          '
	@echo '                              uLanding landing radar system.                    '
	@echo '    obc/ppd                 : This will build flight software for the           '
	@echo '                              Performance Processing Domain (PPD) of the        '
	@echo '                              Windhover On-Board Computer (OBC).                '
	@echo '    obc/cpd                 : This will build flight software for the           '
	@echo '                              Critical Processing Domain (PPD) of the Windhover '
	@echo '                              Windhover On-Board Computer (OBC).                '
	@echo '    clean                   : This will clean all build flight software build   '
	@echo '                              targets.  This includes the Commander workspace,  '
	@echo '                              if one was generated.                             '
	@echo '                                                                                '
	@echo '                                                                                '
	@echo '  Utilities                                                                     '
	@echo '    python-env              : This generates a Python3 virtual environment in   '
	@echo '                              the "venv" directory with all the python          '
	@echo '                              modules required to use the Buildliner build      '
	@echo '                              system.                                           '
	@echo '    submodule-update        : This just updates all submodules.                 '
	@echo '                                                                                '
	@echo '                                                                                '
	@echo '  Documentation                                                                 '
	@echo '    reference               : This will build the reference version of the      '
	@echo '                              flight software, primarily used for documentation '
	@echo '                              purposes.  This will build using platform and     '
	@echo '                              mission configuration header files defined in the '
	@echo '                              respective source directories, i.e.               '
	@echo '                                  apps/sch/fsw/mission_inc,                     '
	@echo '                                  apps/sch/fsw/platform_inc                     '
	@echo '    docs                    : Generate all documentation from the reference     '
	@echo '                              build.                                            '
	@echo '    docs-doxygen            : Generate only the Doxygen documentation from the  '
	@echo '                              reference build.                                  '
	@echo '    docs-sphinx             : Generate the Sphinx documentation from the        '
	@echo '                              reference build.                                  '
	
.PHONY: help Makefile docs
	

$(TARGET_NAMES)::
	@echo 'Building '$@'.'
	@idx=1; \
	for name in $(TARGET_NAMES); do \
		if [ "$$name" == "$@" ] ; then \
			break; \
		fi; \
		((idx++)); \
	done; \
	TARGET_PATH=$$(echo ${TARGET_PATHS} | cut -d " " -f $$idx); \
		echo "Generating complete design/configuration definition file, 'wh_defs.yaml'"; \
	if [ -f "$(CONFIG_DIR)/$$TARGET_PATH/wh_config.yaml" ]; then \
			mkdir -p build/$$TARGET_PATH/target; \
			python3 core/tools/config/wh_defgen.py $(CONFIG_DIR)/$$TARGET_PATH/ build/$$TARGET_PATH/target/wh_defs.yaml; \
	fi; \
		for buildtype in $(BUILD_TYPES); do \
		if [ -d "$(CONFIG_DIR)/$$TARGET_PATH/$$buildtype" ]; then \
				mkdir -p build/$$TARGET_PATH/$$buildtype; \
				(cd build/$$TARGET_PATH/$$buildtype; \
					cmake -DBUILDNAME:STRING=$$TARGET_PATH -DBUILDTYPE:STRING=$$buildtype -G"Eclipse CDT4 - Unix Makefiles" \
						-DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE CMAKE_BUILD_TYPE=Debug $(ROOT_DIR); \
					$(MAKE) --no-print-directory); \
				fi \
		done;
	
docs-doxygen: 
	mkdir -p build/${SPHINX_FSW_BUILD}/target; \
	(cd build/${SPHINX_FSW_BUILD}/target; cmake -DBUILDNAME:STRING=${SPHINX_FSW_BUILD} -DBUILDTYPE:STRING=target \
		-G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE CMAKE_BUILD_TYPE=Debug $(ROOT_DIR); make docs);
	
docs-sphinx: 
	@echo 'Building $$SPHINX_FSW_BUILD.'
	mkdir -p build/${SPHINX_FSW_BUILD}/target; \
	(cd build/${SPHINX_FSW_BUILD}/target; cmake -DBUILDNAME:STRING=${SPHINX_FSW_BUILD} -DBUILDTYPE:STRING=target \
		-G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE CMAKE_BUILD_TYPE=Debug $(ROOT_DIR));
	@$(SPHINX_BUILD) -M html "$(SOURCE_DIR)" "$(SPHINX_BUILDDIR)" $(SPHINX_OPTS) -c build/$(SPHINX_FSW_BUILD)/target/docs $(O)
	@echo 'Completed'
	
docs: docs-doxygen docs-sphinx
	@echo 'Completed'

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
	
submodule-update: 
	@echo 'Completed'
	@echo 'Updating submodules'
	git submodule update --init --recursive
	

clean::
	@echo 'Cleaning flight software builds                                                 '
	rm -rf build
	
