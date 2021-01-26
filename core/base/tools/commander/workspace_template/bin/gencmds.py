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
Utilities for parsing and returning the CFE Table Name from a json file.

Usage:
python gencmds.py [path to json file]
"""

import sys
import json
import os
import jinja2
import filecmp
import yaml
from functools import reduce
import requests
import jinja2


def get_object_by_path(key, cfg):    
    """"
    """
    return reduce(lambda c, k: c[k], key.split('.'), cfg)

def main():   
    """"
    """
    # Load the configuration file
    if len(sys.argv) == 6 :
        yamcs_url = sys.argv[1]
        yamcs_instance = sys.argv[2]
        opi_template_file = sys.argv[3]
        namespace = sys.argv[4]
        output_directory = sys.argv[5]
        
        if not os.path.exists(output_directory):
            print("Directory '" + output_directory + "' not found.")
            return -1
        
        templateLoader = jinja2.FileSystemLoader(searchpath="./")
        templateEnv = jinja2.Environment(loader=templateLoader)
        template = templateEnv.get_template(opi_template_file)
                    
        with open(opi_template_file, 'r') as inFile:   
            resp = requests.get(yamcs_url + "/api/mdb/yamcs-cfs/commands", {'q': namespace}) 
          
            if 'commands' in resp.json():
                for command in resp.json()['commands']:
                    if 'argument' in command:
                        print("Generating " + command['qualifiedName'])
                        
                        output = template.render(command)
                        
                        #for arg in command['argument']:
                        #    if (arg['type']['engType'] != 'enumeration') and (arg['type']['engType'] != 'string') and (arg['type']['engType'] != 'integer'):
                        #        print(arg)
                        
                        outFilePath = os.path.join(output_directory, command['name'] + ".opi") 
                        with open(outFilePath, 'w') as outFile:
                            outFile.write(output)

if __name__ == '__main__':
    main()