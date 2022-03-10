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
Utility for parsing and returning the CFE Table Names from the main
json file for a specific application.

Usage:
python get_table_names.py [path to main json file] [app name]
"""

import sys
import json

if len(sys.argv) == 3:
    configFileName = sys.argv[1]
    appName = sys.argv[2]

    tableList = []

    with open(configFileName, 'r') as configFile:
        config = json.load(configFile)

        if appName in config['modules'].keys():   
            # The application does exist
            objModule = config['modules'][appName]
            
            if 'tables' in objModule.keys():
                # The application does have a tables object
                objTables = objModule['tables']

                for tableTypeName in objTables:
                    objTableType = objModule['tables'][tableTypeName]

                    if '0' in objTableType.keys():
                        # This is an array of tables.  Iterate through all the array elements.
                        tblIdx = 0;
                        while True:
                            if str(tblIdx) not in objTableType.keys():
                                # Done parsing tables
                                break
                            #output = j2_env.get_template(fileNameTemplate).render(objModule)
                            #Path("/my/directory").mkdir(parents=True, exist_ok=True)
                           
                            objData = {
                                "data": objModule,
                                "instance": str(tblIdx)
                            }

                            objTableInstance = objTableType[str(tblIdx)]

                            tableList.append(objTableInstance['file_name']) 

                            tblIdx = tblIdx + 1
        
        seperator = ';'
        print(seperator.join(tableList))
                