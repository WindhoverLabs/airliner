"""
 
    Copyright (c) 2021 Windhover Labs, L.L.C. All rights reserved.
 
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
Usage:
python push_subtrees.py [path to yaml file]
"""

import sys
import os
import yaml
import subprocess


def push_subtrees(module_name, prefix, config):
    if not module_name in config:
        print("" + module_name + " module not found.")
        print('')
        print('*****************************************')
        return -1

    module = config[module_name]

    if not 'url' in module:
        print("" + module_name + " URL not defined.")
        print('')
        print('*****************************************')
        return -1
    url = module['url']

    if not 'path' in module:
        print("" + module_name + " path not defined.")
        print('')
        print('*****************************************')
        return -1
    path = module['path']

    if not 'branch' in module:
        print("" + module_name + " branch not defined.")
        print('')
        print('*****************************************')
        return -1
    branch = module['branch']

    if not 'strategy' in module:
        print("" + module_name + " strategy not defined.")
        print('')
        print('*****************************************')
        return -1
    strategy = module['strategy']

    # Push the subtree back to the mothership repo
    if strategy == 'subtree':
        remote_name = prefix + module_name
        subprocess.call(["git", "subtree", "push", "-P", path, remote_name, branch])
        print('')
        print('*****************************************')
    elif strategy == 'submodule':
        pass
    else:
        print('Undefined strategy of ' + strategy)
        print('')
        print('*****************************************')
        return -1

    return 0



def main():   
    """"
    """
    # Load the configuration file
    if len(sys.argv) == 2 :
        config_file_name = sys.argv[1]
                    
        with open(config_file_name, 'r') as inFile:   
            config = yaml.load(inFile, Loader=yaml.FullLoader)
            
            result = push_subtrees('core', '', config)
            if result != 0:
                return result
            
            core = config['core']

            if not 'osal' in core:
                print('OSAL module not found')
                return -1
            for osal_name in config['core']['osal']:
                result = push_subtrees(osal_name, 'core-osal-', config['core']['osal'])
                if result != 0:
                    return result

            if not 'psp' in core:
                print('PSP module not found')
                return -1
            for psp_name in config['core']['psp']:
                result = push_subtrees(psp_name, 'core-psp-', config['core']['psp'])
                if result != 0:
                    return result

            if 'apps' in config:
                for app_name in config['apps']:
                    result = push_subtrees(app_name, 'app-', config['apps'])
                    if result != 0:
                        return result

    else:
        print("Incorrect parameters.")
        return -1

if __name__ == '__main__':
    main()
