"""
 
    Copyright (c) 2017 Windhover Labs, L.L.C. All rights reserved.
 
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
 
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
  3. Neither the name Windhover Labs nor the names of its contributors 
     may be used to endorse or promote products derived from this software
     without specific prior written permission.
 
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

"""

__author__ = 'Mathew Benson'

""" 
Utilities for generating a fully formed Windhover Configuration from a
configuration inheritance tree.

Usage:
python wh_defgen.py [path to yaml file]
"""

import sys
import yaml
import yaml_merger
import os
import re
import logging

CONFIG_FILE_NAME = 'wh_config.yaml'

path_matcher = re.compile(r'\$\{([^}^{]+)\}')


def path_constructor(loader, node):
    ''' Extract the matched value, expand env variable, and replace the match '''
    value = node.value
    match = path_matcher.match(value)
    env_var = match.group()[2:-1]
    return os.environ.get(env_var) + value[match.end():]


yaml.add_implicit_resolver('!path', path_matcher)
yaml.add_constructor('!path', path_constructor)


def merge_modules(cfg_current: dict, merged_config: dict):
    for module in cfg_current:
        if not (module in merged_config):
            merged_config[module] = dict()
            if 'definition' in cfg_current[module]:
                design_path = os.path.abspath(cfg_current[module]['definition'])
                with open(design_path, 'r') as defFile:
                    design_yaml = yaml.load(defFile, Loader=yaml.FullLoader)
                    yaml_merger.merge(design_yaml, merged_config[module])

            if 'modules' in cfg_current[module]:
                if not ('modules' in merged_config[module]):
                    merged_config[module]['modules'] = dict()
                merge_modules(cfg_current[module]['modules'], merged_config[module]['modules'])

            yaml_merger.merge(cfg_current[module], merged_config[module])


def merge_parent_config(merged_config: dict, abs_config_base: str, abs_current_dir: str):
    """"
    Recursively traverse the tree to the highest level parent in the configuration directory tree,
    then start merging all the configuration files into one YAML file.
    :param: merged_config All of the YAML configuration files combined into a single dictionary.
    :param: abs_config_base Path to the current base of the current configurationn file.
    :param:
    """
    # Can we go further up?

    if abs_current_dir != abs_config_base:
        # Yes.  Go further.  Get the parent directory of the current directory
        abs_parent_path = os.path.abspath(os.path.join(abs_current_dir, '..'))

        # Merge the parent object
        merge_parent_config(merged_config, abs_config_base, abs_parent_path)

    # Now merge the current config file.  First, is there even a configuration 
    # file here?
    cfg_file_name_current = os.path.join(abs_current_dir, CONFIG_FILE_NAME)
    if os.path.exists(cfg_file_name_current):
        # It does exist.  Now load it.
        with open(cfg_file_name_current, 'r') as cfg_file_current:
            # Load the YAML object.
            cfg_current = yaml.load(cfg_file_current, Loader=yaml.FullLoader)

            merge_modules(cfg_current['modules'], merged_config['modules'])


def main():
    # Load the local configuration file
    # FIXME: Use argparse for handling CLI arguments
    if len(sys.argv) == 3:
        local_config_path = sys.argv[1]
        out_file_name = sys.argv[2]
        local_config_file_name = os.path.join(local_config_path, CONFIG_FILE_NAME)
        with open(local_config_file_name, 'r') as localConfigFile:
            local_config = yaml.load(localConfigFile, Loader=yaml.FullLoader)

            # Get the base directory, i.e. "../../../.."
            config_base = local_config['config_base']

            # Get the absolute path of the base directory, i.e. "/home/mbenson/git/airliner"
            abs_config_base = os.path.abspath(config_base)

            # Get absolute directory of the local config file,
            # i.e. "/home/mbenson/git/airliner/config/bebop2/sitl/target"
            abs_local_dir = os.path.dirname(os.path.abspath(local_config_file_name))

            # Recursively merge config files from the parent, starting with the config base
            logging.info("Flattening configuration definitions")

            merged_config = dict()
            merged_config['modules'] = dict()

            merge_parent_config(merged_config, abs_config_base, abs_local_dir)

            yaml_merger.merge(local_config, merged_config)

            # We don't want to write the configuration to the file system if there is a file
            # already there that contains an identical configuration.  See if there's a file
            # there already.
            write_file = True
            if os.path.exists(out_file_name):
                # It does exist.  Now load it.
                with open(out_file_name, 'r') as outFile:
                    old_object = yaml.load(outFile, Loader=yaml.FullLoader)

                    if old_object == merged_config:
                        write_file = False

                    outFile.close()

            if write_file:
                with open(out_file_name, 'w') as outFile:
                    yaml.dump(merged_config, outFile, indent=2, sort_keys=False)


if __name__ == '__main__':
    main()
