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
Utilities for parsing and returning the CFE Table Name from a json file.

Usage:
python parse_configuration.py [path to json file]
"""

import sys
import json
import yaml
import os
import jinja2
import filecmp
import shutil
import errno



def copyAll(src_path, dst_path, is_templated=False, rootModule=None, targetModule=None, modPath=None):
    os.makedirs(dst_path, exist_ok=True)
    files = next(os.walk(src_path))[2]
    folders = next(os.walk(src_path))[1]
    for file in files: # Copy the files
        src_file = src_path + "/" + file
        dst_file = dst_path + "/" + file
        if os.path.exists(dst_file): # Delete the old files if already exist
            os.remove(dst_file)
        if not is_templated:
            shutil.copy(src_file, dst_file)
        else:
            cpuID = getCpuId(rootModule, modPath)

            data = {'module': targetModule, 'root': rootModule, "cpu_id": cpuID}
            if 'module_type' in targetModule:
                module_type = targetModule['module_type']
            else:
                module_type = 'NOT_SUPPORTED'

            j2Env = jinja2.Environment(loader=jinja2.FileSystemLoader(src_path))
            output = j2Env.get_template(file).render(data)

            # If there is an existing file, only write the file if the contents have changed.
            writeFile = True

            if os.path.exists(dst_file):
                # It does exist.  Now load it.
                with open(dst_file, 'r') as outFile:
                    oldData = outFile.read()
                    if oldData == output:
                        writeFile = False

            if writeFile:
                # Save the results
                with open(dst_file, "w") as dst_file:
                    dst_file.write(output)

    for folder in folders: # Merge again with the subdirectories
        scr_folder = src_path + "/" + folder
        dst_folder = dst_path + "/" + folder
        copyAll(scr_folder, dst_folder, is_templated, rootModule, targetModule, modPath)
        
    
        
def getCpuId(rootModule, modPath):
    cpuID = modPath.split("/")[0]
    
    return cpuID
        


def generateCommanderDisplay(rootModule, targetModule, templateDir, templateFile, outputName, cdrBasePath, modPath):    
    if 'app_name' in targetModule.keys():
        app_name = targetModule['app_name']
        shortName = targetModule['short_name']
        outPath = os.path.join(cdrBasePath, "Displays", modPath)
        
        cpuID = getCpuId(rootModule, modPath)
        
        data = {'module': targetModule, 'root': rootModule, "cpu_id": cpuID}
        if 'module_type' in targetModule:
            module_type = targetModule['module_type']
        else:
            module_type = 'NOT_SUPPORTED'            

        outFileName = os.path.join(outPath, outputName)
        os.makedirs(outPath, exist_ok=True)
        
        j2Env = jinja2.Environment(loader=jinja2.FileSystemLoader(templateDir))
        output = j2Env.get_template(templateFile).render(data)
            
        # If there is an existing file, only write the file if the contents have changed.
        writeFile = True

        if os.path.exists(outFileName):
            # It does exist.  Now load it.
            with open(outFileName, 'r') as outFile:
                oldData = outFile.read()
                if oldData == output:
                    writeFile = False
                    
        if writeFile:
            # Save the results
            with open(outFileName, "w") as outputFile:
                outputFile.write(output)
    
    if 'modules' in targetModule.keys():
        for moduleName in targetModule['modules']:
            newModPath = modPath + moduleName + "/"
            generateCommanderDisplay(rootModule, targetModule['modules'][moduleName], templateDir, templateFile, outputName, cdrBasePath, newModPath) 
    
             

def parseModule(rootModule, currentModule, currentModuleName, dirBase, cdrBasePath, modPath):    
    if 'definition' in currentModule:
        dirBase = os.path.abspath(os.path.dirname(currentModule['definition']))
        
    if 'commander' in currentModule.keys():
                
        if 'server_overlay' in currentModule['commander']:
            srcPath = os.path.join(dirBase, currentModule['commander']['server_overlay'])
            
            if os.path.isdir(srcPath):
                copyAll(srcPath, cdrBasePath)
            else:
                print("Cannot apply server overlay '/" + modPath + "'.  'server_overlay' '" + srcPath + "' not found.  Skipping.")
                
        if 'displays' in currentModule['commander']:
            srcPath = os.path.join(dirBase, currentModule['commander']['displays'])
            dstPath = os.path.join(cdrBasePath, "Displays", modPath)
            
            if os.path.isdir(srcPath):
                copyAll(srcPath, dstPath, True, rootModule, currentModule, modPath)
            else:
                print("Cannot populate displays for '/" + modPath + "'.  'displays' '" + srcPath + "' not found.  Skipping.")
                
        if 'templates' in currentModule['commander']:
            for cdrTypeName in currentModule['commander']['templates']:
                objDisplayType = currentModule['commander']['templates'][cdrTypeName]

                # Get the template directory
                cdrDirectory = os.path.dirname(os.path.join(dirBase, objDisplayType['template']))

                # Get the template filename
                cdrFileName = os.path.basename(objDisplayType['template'])

                if not os.path.isfile(cdrDirectory + "/" + cdrFileName):
                    print("Cannot generate from template '/" + modPath + cdrTypeName + "'.  File '" + cdrDirectory + "/" + cdrFileName + "' not found.  Skipping.")
                else:
                    outFileName = objDisplayType['output']

                    if 'scope' in objDisplayType.keys():
                        if objDisplayType['scope'] == 'GLOBAL':
                            generateCommanderDisplay(rootModule, rootModule, cdrDirectory, cdrFileName, outFileName, cdrBasePath, "")
                        else:
                            generateCommanderDisplay(rootModule, currentModule, cdrDirectory, cdrFileName, outFileName, cdrBasePath, modPath)
                    else:
                        generateCommanderDisplay(rootModule, currentModule, cdrDirectory, cdrFileName, outFileName, cdrBasePath, modPath)

                        # parseModuleCommanderTemplates(rootModule, currentModule, dirBase, cdrBasePath, modPath)
    
    if 'modules' in currentModule.keys():
        for moduleName in currentModule['modules']:    
            newModPath = modPath + moduleName + "/"
            parseModule(rootModule, currentModule['modules'][moduleName], moduleName, dirBase, cdrBasePath, newModPath)       
    
             

# Load the local configuration file
if len(sys.argv) == 3:
    configFileName = sys.argv[1]
    cdrBasePath = sys.argv[2]
    
    with open(configFileName, 'r') as configFile:
        
        config = yaml.load(configFile, Loader=yaml.FullLoader)
        
        # Copy in the workspace template
        src_path = os.path.join(os.path.dirname(__file__), "workspace_template")
        copyAll(src_path, cdrBasePath)
        
        # Now parse the root module
        parseModule(config, config, "", "", cdrBasePath, "")
                