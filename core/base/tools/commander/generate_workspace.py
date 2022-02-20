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


def copyanything(src, dst):
  files = next(os.walk(scr_path))[2]
  folders = next(os.walk(scr_path))[1]
  for file in files: # Copy the files
    scr_file = scr_path + "/" + file
    dir_file = dir_path + "/" + file
    if os.path.exists(dir_file): # Delete the old files if already exist
      os.remove(dir_file)
    shutil.copy(scr_file, dir_file)
  for folder in folders: # Merge again with the subdirectories
    scr_folder = scr_path + "/" + folder
    dir_folder = dir_path + "/" + folder
    if not os.path.exists(dir_folder): # Create the subdirectories if dont already exist
      os.mkdir(dir_folder)
    merge(scr_folder, dir_folder)


def generateCommanderDisplay(rootModule, targetModule, templateDir, templateFile, outputName, cdrBasePath, modPath):    
    if 'app_name' in targetModule.keys():
        app_name = targetModule['app_name']
        shortName = targetModule['short_name']
        outPath = os.path.join(cdrBasePath, modPath)
        
        data = {'module': targetModule, 'root': rootModule}
        if 'module_type' in targetModule:
            module_type = targetModule['module_type']
        else:
            module_type = 'NOT_SUPPORTED'            

        outFileName = os.path.join(outPath, outputName)
        if not os.path.exists(outPath):
            os.makedirs(outPath)
        
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
    
             

def parseModulePhase2(rootModule, currentModule, currentModuleName, dirBase, cdrBasePath, modPath):    
    if 'definition' in currentModule:
        dirBase = os.path.abspath(os.path.dirname(currentModule['definition']))
        
    if 'commander' in currentModule.keys():
        if 'templates' in currentModule['commander']:           
            for cdrTypeName in currentModule['commander']['templates']:
                objDisplayType = currentModule['commander']['templates'][cdrTypeName]
    
                # Get the template directory
                cdrDirectory = os.path.dirname(os.path.join(dirBase, objDisplayType['template']))
    
                # Get the template filename
                cdrFileName = os.path.basename(objDisplayType['template'])
        
                outFileName = objDisplayType['output']
        
                if 'scope' in objDisplayType.keys():
                    if objDisplayType['scope'] == 'GLOBAL':  
                        generateCommanderDisplay(rootModule, rootModule, cdrDirectory, cdrFileName, outFileName, cdrBasePath, "")
                    else:
                        generateCommanderDisplay(rootModule, currentModule, cdrDirectory, cdrFileName, outFileName, cdrBasePath, modPath)
                else:
                    generateCommanderDisplay(rootModule, currentModule, cdrDirectory, cdrFileName, outFileName, cdrBasePath, modPath)  
            
                    parseModuleCommanderTemplates(rootModule, currentModule, dirBase, cdrBasePath, modPath)
    
    if 'modules' in currentModule.keys():
        for moduleName in currentModule['modules']:    
            newModPath = modPath + moduleName + "/"
            parseModulePhase2(rootModule, currentModule['modules'][moduleName], moduleName, dirBase, cdrBasePath, newModPath)    
    
             

def parseModulePhase1(rootModule, currentModule, currentModuleName, dirBase, cdrBasePath, modPath):    
    if 'definition' in currentModule:
        dirBase = os.path.abspath(os.path.dirname(currentModule['definition']))
        
    if 'commander' in currentModule.keys():
        if 'displays' in currentModule['commander']:
            srcPath = os.path.join(dirBase, currentModule['commander']['displays'])
            dstPath = os.path.join(cdrBasePath, "Displays", modPath)
            
            if os.path.isdir(srcPath):
                copyanything(srcPath, dstPath)
            else:
                print("Cannot populate displays for '/" + modPath + "'.  'displays' '" + srcPath + "' not found.  Skipping.")
                
        if 'server_overlay' in currentModule['commander']:
            srcPath = os.path.join(dirBase, currentModule['commander']['server_overlay'])
            
            if os.path.isdir(srcPath):
                copyanything(srcPath, cdrBasePath)
            else:
                print("Cannot apply server overlay '" + modPath + "'.  'server_overlay' '" + srcPath + "' not found.  Skipping.")
    
    if 'modules' in currentModule.keys():
        for moduleName in currentModule['modules']:    
            newModPath = modPath + moduleName + "/"
            parseModulePhase1(rootModule, currentModule['modules'][moduleName], moduleName, dirBase, cdrBasePath, newModPath)       
    
             

def parseModuleForMsgIDs(rootModule, currentModule, currentModuleName, modPath):         
    if 'commander' in currentModule.keys():
        if 'base_path' in currentModule['commander']:
            srcPath = os.path.join(dirBase, currentModule['commander']['base_path'])
            dstPath = os.path.join(cdrBasePath, modPath)
            shutil.copyanything(srcPath, dstPath)
    
    if 'modules' in currentModule.keys():
        for moduleName in currentModule['modules']:    
            newModPath = modPath + moduleName + "/"
            parseModulePhase1(rootModule, currentModule['modules'][moduleName], moduleName, dirBase, cdrBasePath, newModPath)    
        
        
  
# Load the local configuration file
if len(sys.argv) == 3:
    configFileName = sys.argv[1]
    cdrBasePath = sys.argv[2]
    
    with open(configFileName, 'r') as configFile:
        
        config = yaml.load(configFile, Loader=yaml.FullLoader)
        
        dirBase = ""
        os.makedirs(cdrBasePath + "/Displays", exist_ok=True)
        #parseModuleForMsgIDs(config, config, "", "")
        parseModulePhase1(config, config, "", dirBase, cdrBasePath, "")
        parseModulePhase2(config, config, "", dirBase, cdrBasePath, "")
                