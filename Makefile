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
SPHINX_FSW_BUILD = reference/target

SHELL := /bin/bash

CONFIG_DIR   := config
GENERIC_TARGET_PATHS :=  $(shell find ${CONFIG_DIR} -mindepth 2 -maxdepth 20 -type f -name 'wh_config.yaml' | sed -r 's,^[^/]*/,,' | sed -r 's|/[^/]+$$||' | sort -u)
GENERIC_TARGET_NAMES := $(shell echo ${GENERIC_TARGET_PATHS} )
BUILD_TYPES  := host target
ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

REMOTE_ADDRESS := '192.168.2.14'

export PROJECT_SOURCE_DIR=${PWD}

help::
	@echo 'Specify a target to build.  Available targets are:                              '
	@echo '                                                                                '
	@echo '  Flight Software Builds                                                        '
	@echo '    bebop2                  : This will build flight software for the Parrot    '
	@echo '                              Bebop2 drone.                                     '
	@echo '    bebop2/sitl             : This will build the Software in the Loop          '
	@echo '                              (PC-Linux) version of the Parrot Bebop2 drone.    '
	@echo '    quad                    : This will build flight software for both the      '
	@echo '                              Performance Processing Domain (PPD) and the       '
	@echo '                              Critical Processing Domain (CPD) of the           '
	@echo '                              Windhover On-Board Computer (OBC), as well as the '
	@echo '                              associated ground products.                       '
	@echo '    gemini2                 : This will build flight software for both the      '
	@echo '                              Performance Processing Domain (PPD) and the       '
	@echo '                              Critical Processing Domain (CPD) of the           '
	@echo '                              Windhover On-Board Computer (OBC), as well as the '
	@echo '                              associated ground products.                       '
	@echo '    quad-sitl               : This will build a SITL versions of both the PPD   '
	@echo '                              and CPD flight software for quadcopter.           '
	@echo '    gemini2-sitl            : This will build a SITL versions of both the PPD   '
	@echo '                              and CPD flight software for fixedwing.            '
	@echo '    clean                   : This will clean all flight software build         '
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
	
	
.PHONY: help Makefile docs obc
	

$(GENERIC_TARGET_NAMES)::
	@echo 'Building '$@'.'
	@idx=1; \
	for name in $(GENERIC_TARGET_NAMES); do \
		if [ "$$name" == "$@" ] ; then \
			break; \
		fi; \
		((idx++)); \
	done; \
	TARGET_PATH=$$(echo ${GENERIC_TARGET_PATHS} | cut -d " " -f $$idx); \
		echo "Generating complete design/configuration definition file, wh_defs.yaml"; \
	if [ -f "$(CONFIG_DIR)/$$TARGET_PATH/wh_config.yaml" ]; then \
			mkdir -p build/$$TARGET_PATH/target; \
			python3 core/base/tools/config/wh_defgen.py $(CONFIG_DIR)/$$TARGET_PATH/ build/$$TARGET_PATH/target/wh_defs.yaml; \
	fi; \
		for buildtype in $(BUILD_TYPES); do \
		if [ -d "$(CONFIG_DIR)/$$TARGET_PATH/$$buildtype" ]; then \
				mkdir -p build/$$TARGET_PATH/$$buildtype; \
				(cd build/$$TARGET_PATH/$$buildtype; \
					/usr/bin/cmake -DBUILDNAME:STRING=$$TARGET_PATH -DBUILDTYPE:STRING=$$buildtype -G"Eclipse CDT4 - Unix Makefiles" \
						-DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE CMAKE_BUILD_TYPE=Debug $(ROOT_DIR); \
					$(MAKE) --no-print-directory); \
				fi \
		done;
		
		
quad:: multirotor/quad/ppd multirotor/quad/cpd
	@rm -Rf build/multirotor/quad/target
	@echo 'Done'

gemini2:: fixedwing/gemini2/ppd fixedwing/gemini2/cpd
	@rm -Rf build/fixedwing/gemini2/target
	@echo 'Done'
	
	
quad-workspace::
	@echo 'Generating PPD ground tools data.'
	@make -C build/multirotor/quad/ppd/target ground-tools
	@echo 'Generating CPD ground tools data.'
	@make -C build/multirotor/quad/cpd/target ground-tools
	@echo 'Adding XTCE configuration to registries.'
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/multirotor/quad/cpd/target/wh_defs.yaml --overwrite build/multirotor/quad/cpd/target/wh_defs.yaml
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/multirotor/quad/ppd/target/wh_defs.yaml --overwrite build/multirotor/quad/ppd/target/wh_defs.yaml
	@echo 'Generating combined registry.'
	@rm -Rf build/multirotor/quad/commander_workspace >/dev/null
	@mkdir -p build/multirotor/quad/commander_workspace/etc
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/multirotor/quad/commander_workspace/etc/registry.yaml --yaml_input build/multirotor/quad/cpd/target/wh_defs.yaml --yaml_path /modules/cpd
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/multirotor/quad/commander_workspace/etc/registry.yaml --yaml_input build/multirotor/quad/ppd/target/wh_defs.yaml --yaml_path /modules/ppd
	@echo 'Generating Commander workspace.'
	@python3 core/base/tools/commander/generate_workspace.py build/multirotor/quad/commander_workspace/etc/registry.yaml build/multirotor/quad/commander_workspace/
	@echo 'Generating CPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/multirotor/quad/cpd/target/wh_defs.yaml ${PWD}/build/multirotor/quad/cpd/target/wh_defs.db ${PWD}/build/multirotor/quad/commander_workspace/mdb/cpd.xml
	@echo 'Generating PPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/multirotor/quad/ppd/target/wh_defs.yaml ${PWD}/build/multirotor/quad/ppd/target/wh_defs.db ${PWD}/build/multirotor/quad/commander_workspace/mdb/ppd.xml

gemini2-workspace::
	@echo 'Generating PPD ground tools data.'
	@make -C build/fixedwing/gemini2/ppd/target ground-tools
	@echo 'Generating CPD ground tools data.'
	@make -C build/fixedwing/gemini2/cpd/target ground-tools
	@echo 'Adding XTCE configuration to registries.'
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/fixedwing/gemini2/cpd/target/wh_defs.yaml --overwrite build/fixedwing/gemini2/cpd/target/wh_defs.yaml
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/fixedwing/gemini2/ppd/target/wh_defs.yaml --overwrite build/fixedwing/gemini2/ppd/target/wh_defs.yaml
	@echo 'Generating combined registry.'
	@rm -Rf build/fixedwing/gemini2/commander_workspace >/dev/null
	@mkdir -p build/fixedwing/gemini2/commander_workspace/etc
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/fixedwing/gemini2/commander_workspace/etc/registry.yaml --yaml_input build/fixedwing/gemini2/cpd/target/wh_defs.yaml --yaml_path /modules/cpd
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/fixedwing/gemini2/commander_workspace/etc/registry.yaml --yaml_input build/fixedwing/gemini2/ppd/target/wh_defs.yaml --yaml_path /modules/ppd
	@echo 'Generating Commander workspace.'
	@python3 core/base/tools/commander/generate_workspace.py build/fixedwing/gemini2/commander_workspace/etc/registry.yaml build/fixedwing/gemini2/commander_workspace/
	@echo 'Generating CPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/fixedwing/gemini2/cpd/target/wh_defs.yaml ${PWD}/build/fixedwing/gemini2/cpd/target/wh_defs.db ${PWD}/build/fixedwing/gemini2/commander_workspace/mdb/cpd.xml
	@echo 'Generating PPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/fixedwing/gemini2/ppd/target/wh_defs.yaml ${PWD}/build/fixedwing/gemini2/ppd/target/wh_defs.db ${PWD}/build/fixedwing/gemini2/commander_workspace/mdb/ppd.xml

quad-sitl-workspace::
	@echo 'Generating PPD ground tools data.'
	@make -C build/multirotor/quad/ppd/sitl/target ground-tools
	@echo 'Generating CPD ground tools data.'
	@make -C build/multirotor/quad/cpd/sitl/target ground-tools
	@echo 'Generating Simlink ground tools data.'
	@make -C build/multirotor/quad/simlink/target ground-tools
	@echo 'Adding XTCE configuration to registries.'
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/multirotor/quad/cpd/sitl/target/wh_defs.yaml --overwrite build/multirotor/quad/cpd/sitl/target/wh_defs.yaml
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/multirotor/quad/ppd/sitl/target/wh_defs.yaml --overwrite build/multirotor/quad/ppd/sitl/target/wh_defs.yaml
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/multirotor/quad/simlink/target/wh_defs.yaml --overwrite build/multirotor/quad/simlink/target/wh_defs.yaml
	@echo 'Generating combined registry.'
	@rm -Rf build/multirotor/quad/sitl_commander_workspace >/dev/null
	@mkdir -p build/multirotor/quad/sitl_commander_workspace/etc
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/multirotor/quad/sitl_commander_workspace/etc/registry.yaml --yaml_input build/multirotor/quad/cpd/sitl/target/wh_defs.yaml --yaml_path /modules/cpd
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/multirotor/quad/sitl_commander_workspace/etc/registry.yaml --yaml_input build/multirotor/quad/ppd/sitl/target/wh_defs.yaml --yaml_path /modules/cpd
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/multirotor/quad/sitl_commander_workspace/etc/registry.yaml --yaml_input build/multirotor/quad/simlink/target/wh_defs.yaml --yaml_path /modules/cpd
	@echo 'Generating Commander workspace.'
	@python3 core/base/tools/commander/generate_workspace.py build/multirotor/quad/sitl_commander_workspace/etc/registry.yaml build/multirotor/quad/sitl_commander_workspace/
	@echo 'Generating CPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/multirotor/quad/cpd/sitl/target/wh_defs.yaml ${PWD}/build/multirotor/quad/cpd/sitl/target/wh_defs.db ${PWD}/build/multirotor/quad/sitl_commander_workspace/mdb/cpd.xml
	@echo 'Generating PPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/multirotor/quad/ppd/sitl/target/wh_defs.yaml ${PWD}/build/multirotor/quad/ppd/sitl/target/wh_defs.db ${PWD}/build/multirotor/quad/sitl_commander_workspace/mdb/ppd.xml
	@echo 'Generating Simlink XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/multirotor/quad/simlink/target/wh_defs.yaml ${PWD}/build/multirotor/quad/simlink/target/wh_defs.db ${PWD}/build/multirotor/quad/sitl_commander_workspace/mdb/simlink.xml

quad-hitl-workspace::
	@echo 'Generating PPD ground tools data.'
	@make -C build/multirotor/quad/ppd/target ground-tools
	@echo 'Generating CPD ground tools data.'
	@make -C build/multirotor/quad/cpd/target ground-tools
	@echo 'Generating Simlink ground tools data.'
	@make -C build/multirotor/quad/simlink/hitl/target ground-tools
	@echo 'Adding XTCE configuration to registries.'
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/multirotor/quad/cpd/target/wh_defs.yaml --overwrite build/multirotor/quad/cpd/target/wh_defs.yaml
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/multirotor/quad/ppd/target/wh_defs.yaml --overwrite build/multirotor/quad/ppd/target/wh_defs.yaml
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/multirotor/quad/simlink/hitl/target/wh_defs.yaml --overwrite build/multirotor/quad/simlink/hitl/target/wh_defs.yaml
	@echo 'Generating combined registry.'
	@rm -Rf build/multirotor/quad/hitl_commander_workspace >/dev/null
	@mkdir -p build/multirotor/quad/hitl_commander_workspace/etc
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/multirotor/quad/hitl_commander_workspace/etc/registry.yaml --yaml_input build/multirotor/quad/cpd/target/wh_defs.yaml --yaml_path /modules/cpd
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/multirotor/quad/hitl_commander_workspace/etc/registry.yaml --yaml_input build/multirotor/quad/ppd/target/wh_defs.yaml --yaml_path /modules/cpd
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/multirotor/quad/hitl_commander_workspace/etc/registry.yaml --yaml_input build/multirotor/quad/simlink/hitl/target/wh_defs.yaml --yaml_path /modules/cpd
	@echo 'Generating Commander workspace.'
	@python3 core/base/tools/commander/generate_workspace.py build/multirotor/quad/hitl_commander_workspace/etc/registry.yaml build/multirotor/quad/hitl_commander_workspace/
	@echo 'Generating CPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/multirotor/quad/cpd/target/wh_defs.yaml ${PWD}/build/multirotor/quad/cpd/target/wh_defs.db ${PWD}/build/multirotor/quad/hitl_commander_workspace/mdb/cpd.xml
	@echo 'Generating PPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/multirotor/quad/ppd/target/wh_defs.yaml ${PWD}/build/multirotor/quad/ppd/target/wh_defs.db ${PWD}/build/multirotor/quad/hitl_commander_workspace/mdb/ppd.xml
	@echo 'Generating Simlink XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/multirotor/quad/simlink/hitl/target/wh_defs.yaml ${PWD}/build/multirotor/quad/simlink/hitl/target/wh_defs.db ${PWD}/build/multirotor/quad/hitl_commander_workspace/mdb/simlink.xml

gemini2-sitl-workspace::
	@echo 'Generating PPD ground tools data.'
	@make -C build/fixedwing/gemini2/ppd/sitl/target ground-tools
	@echo 'Generating CPD ground tools data.'
	@make -C build/fixedwing/gemini2/cpd/sitl/target ground-tools
	@echo 'Generating Simlink ground tools data.'
	@make -C build/fixedwing/gemini2/simlink/target ground-tools
	@echo 'Adding XTCE configuration to registries.'
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/fixedwing/gemini2/cpd/sitl/target/wh_defs.yaml --overwrite build/fixedwing/gemini2/cpd/sitl/target/wh_defs.yaml
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/fixedwing/gemini2/ppd/sitl/target/wh_defs.yaml --overwrite build/fixedwing/gemini2/ppd/sitl/target/wh_defs.yaml
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/fixedwing/gemini2/simlink/target/wh_defs.yaml --overwrite build/fixedwing/gemini2/simlink/target/wh_defs.yaml
	@echo 'Generating combined registry.'
	@rm -Rf build/fixedwing/gemini2/sitl_commander_workspace >/dev/null
	@mkdir -p build/fixedwing/gemini2/sitl_commander_workspace/etc
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/fixedwing/gemini2/sitl_commander_workspace/etc/registry.yaml --yaml_input build/fixedwing/gemini2/cpd/sitl/target/wh_defs.yaml --yaml_path /modules/cpd
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/fixedwing/gemini2/sitl_commander_workspace/etc/registry.yaml --yaml_input build/fixedwing/gemini2/ppd/sitl/target/wh_defs.yaml --yaml_path /modules/ppd
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/fixedwing/gemini2/sitl_commander_workspace/etc/registry.yaml --yaml_input build/fixedwing/gemini2/simlink/target/wh_defs.yaml --yaml_path /modules/simlink
	@echo 'Generating Commander workspace.'
	@python3 core/base/tools/commander/generate_workspace.py build/fixedwing/gemini2/sitl_commander_workspace/etc/registry.yaml build/fixedwing/gemini2/sitl_commander_workspace/
	@echo 'Generating CPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/fixedwing/gemini2/cpd/sitl/target/wh_defs.yaml ${PWD}/build/fixedwing/gemini2/cpd/sitl/target/wh_defs.db ${PWD}/build/fixedwing/gemini2/sitl_commander_workspace/mdb/cpd.xml
	@echo 'Generating PPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/fixedwing/gemini2/ppd/sitl/target/wh_defs.yaml ${PWD}/build/fixedwing/gemini2/ppd/sitl/target/wh_defs.db ${PWD}/build/fixedwing/gemini2/sitl_commander_workspace/mdb/ppd.xml
	@echo 'Generating Simlink XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/fixedwing/gemini2/simlink/target/wh_defs.yaml ${PWD}/build/fixedwing/gemini2/simlink/target/wh_defs.db ${PWD}/build/fixedwing/gemini2/sitl_commander_workspace/mdb/simlink.xml

gemini2-hitl-workspace::
	@echo 'Generating PPD ground tools data.'
	@make -C build/fixedwing/gemini2/ppd/target ground-tools
	@echo 'Generating CPD ground tools data.'
	@make -C build/fixedwing/gemini2/cpd/target ground-tools
	@echo 'Generating Simlink ground tools data.'
	@make -C build/fixedwing/gemini2/simlink/hitl/target ground-tools
	@echo 'Adding XTCE configuration to registries.'
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/fixedwing/gemini2/cpd/target/wh_defs.yaml --overwrite build/fixedwing/gemini2/cpd/target/wh_defs.yaml
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/fixedwing/gemini2/ppd/target/wh_defs.yaml --overwrite build/fixedwing/gemini2/ppd/target/wh_defs.yaml
	@yaml-merge  core/base/tools/commander/xtce_config.yaml build/fixedwing/gemini2/simlink/hitl/target/wh_defs.yaml --overwrite build/fixedwing/gemini2/simlink/hitl/target/wh_defs.yaml
	@echo 'Generating combined registry.'
	@rm -Rf build/fixedwing/gemini2/hitl_commander_workspace >/dev/null
	@mkdir -p build/fixedwing/gemini2/hitl_commander_workspace/etc
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/fixedwing/gemini2/hitl_commander_workspace/etc/registry.yaml --yaml_input build/fixedwing/gemini2/cpd/target/wh_defs.yaml --yaml_path /modules/cpd
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/fixedwing/gemini2/hitl_commander_workspace/etc/registry.yaml --yaml_input build/fixedwing/gemini2/ppd/target/wh_defs.yaml --yaml_path /modules/ppd
	@python3 core/base/tools/config/yaml_path_merger.py --yaml_output build/fixedwing/gemini2/hitl_commander_workspace/etc/registry.yaml --yaml_input build/fixedwing/gemini2/simlink/hitl/target/wh_defs.yaml --yaml_path /modules/simlink
	@echo 'Generating Commander workspace.'
	@python3 core/base/tools/commander/generate_workspace.py build/fixedwing/gemini2/hitl_commander_workspace/etc/registry.yaml build/fixedwing/gemini2/hitl_commander_workspace/
	@echo 'Generating CPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/fixedwing/gemini2/cpd/target/wh_defs.yaml ${PWD}/build/fixedwing/gemini2/cpd/target/wh_defs.db ${PWD}/build/fixedwing/gemini2/hitl_commander_workspace/mdb/cpd.xml
	@echo 'Generating PPD XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/fixedwing/gemini2/ppd/target/wh_defs.yaml ${PWD}/build/fixedwing/gemini2/ppd/target/wh_defs.db ${PWD}/build/fixedwing/gemini2/hitl_commander_workspace/mdb/ppd.xml
	@echo 'Generating Simlink XTCE'
	@core/tools/auto-yamcs/src/generate_xtce.sh ${PWD}/build/fixedwing/gemini2/simlink/hitl/target/wh_defs.yaml ${PWD}/build/fixedwing/gemini2/simlink/hitl/target/wh_defs.db ${PWD}/build/fixedwing/gemini2/hitl_commander_workspace/mdb/simlink.xml

quad-sitl:: multirotor/quad/ppd/sitl multirotor/quad/cpd/sitl multirotor/quad/simlink
	@ln -s cf build/multirotor/quad/cpd/sitl/target/target/exe/ram || /bin/true
	@echo 'Done'

gemini2-sitl:: fixedwing/gemini2/ppd/sitl fixedwing/gemini2/cpd/sitl fixedwing/gemini2/simlink
	@ln -s cf build/fixedwing/gemini2/cpd/sitl/target/target/exe/ram || /bin/true
	@echo 'Done'

quad-hitl:: multirotor/quad/ppd multirotor/quad/cpd multirotor/quad/simlink/hitl
	@echo 'Done'

gemini2-hitl:: fixedwing/gemini2/ppd fixedwing/gemini2/cpd fixedwing/gemini2/simlink/hitl
	@echo 'Done'

docs-doxygen:
	mkdir -p build/${SPHINX_FSW_BUILD}/target; \
	(cd build/${SPHINX_FSW_BUILD}/target; /usr/bin/cmake -DBUILDNAME:STRING=${SPHINX_FSW_BUILD} \
		-G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE CMAKE_BUILD_TYPE=Debug $(ROOT_DIR); make docs);

docs-sphinx: 
	@echo 'Building $$SPHINX_FSW_BUILD.'
	mkdir -p build/${SPHINX_FSW_BUILD}/target; \
	(cd build/${SPHINX_FSW_BUILD}/target; /usr/bin/cmake -DBUILDNAME:STRING=${SPHINX_FSW_BUILD} \
		-G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE CMAKE_BUILD_TYPE=Debug $(ROOT_DIR));
	@$(SPHINX_BUILD) -M html "$(SOURCE_DIR)" "$(SPHINX_BUILDDIR)" $(SPHINX_OPTS) -c build/$(SPHINX_FSW_BUILD)/target/docs $(O)
	@echo 'Completed'

docs: docs-doxygen docs-sphinx
	@echo 'Completed'

python-env::
	virtualenv -p python3 venv || exit -1
	(source venv/bin/activate || exit -1; pip install -r core/tools/auto-yamcs/src/requirements.txt || exit -1; pip install pyyaml || exit -1; pip install numpy || exit)
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

gemini2-remote-install::
	@echo 'Installing onto test flight vehicle at $(REMOTE_ADDRESS)'
	ssh-keygen -f "${HOME}/.ssh/known_hosts" -R "$(REMOTE_ADDRESS)"
	-ssh windhover@$(REMOTE_ADDRESS) rm -Rf exe
	-ssh windhover@$(REMOTE_ADDRESS) rm airliner.elf
	scp -r build/fixedwing/gemini2/ppd/target/target/exe windhover@$(REMOTE_ADDRESS):~
	scp build/fixedwing/gemini2/cpd/target/target/exe/airliner.elf windhover@$(REMOTE_ADDRESS):~
	scp config/fixedwing/gemini2/ppd/target/airliner.service windhover@$(REMOTE_ADDRESS):~

gemini2-local-install::
	@echo 'Installing onto test flight vehicle at /media/${USER}/'
	-sudo rm -Rf /media/${USER}/rootfs/opt/airliner
	sudo cp -R build/fixedwing/gemini2/ppd/target/target/exe /media/${USER}/rootfs/opt/airliner
	sudo cp build/fixedwing/gemini2/cpd/target/target/exe/airliner.elf /media/${USER}/rootfs/lib/firmware

clean::
	@echo 'Cleaning flight software builds                                                 '
	rm -rf build
	
