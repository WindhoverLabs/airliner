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
from _ast import Or

__author__ = 'Mathew Benson'

""" 
Usage:
python symtab_gen.py [path to yaml file]
"""

import sys
import getopt
import os
import jinja2
from jinja2 import Environment, FileSystemLoader


def print_help():
    print("symtab_gen.py -i <inputfile> -o <outputfile> -t <templatefile>")
    

def main(argv):
    inputfile = ""
    outputfile = ""
    templatefile = ""
    
    try:
        opts, args = getopt.getopt(argv,"hi:o:t:",["ifile=","ofile=","tfile"])
    except getopt.GetoptError:
        print_help()
        sys.exit(2)
        
    for opt, arg in opts:
        if opt == '-h':
            print_help()
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-o", "--ofile"):
            outputfile = arg
        elif opt in ("-t", "--tfile"):
            templatefile = arg
            
    if inputfile == "" or outputfile == "" or templatefile == "":
        print_help()
        sys.exit(-1)
            
    with open(inputfile, 'r') as in_file:
        symbols = []
        
        line = in_file.readline()
        while line:
            line = in_file.readline()
            
            fields = line[:-1].split(" ")
            
            if len(fields) == 3:
                if fields[1] == 'A' or \
                   fields[1] == 'B' or \
                   fields[1] == 'D' or \
                   fields[1] == 'T':
                    symbol = {"name": fields[2], "address": fields[0]}
                    symbols.append(symbol)
                    
    #templateLoader = jinja2.FileSystemLoader(searchpath=templatefile)
    
    filename = os.path.basename(templatefile)
    dirname = os.path.dirname(os.path.realpath(templatefile))
    
    templateEnv = jinja2.Environment(loader=FileSystemLoader(dirname))
    template = templateEnv.get_template(filename)
    outputText = template.render(symbols=symbols)  # this is where to put args to the template renderer

    with open(outputfile, "w") as out_file:
        out_file.write(outputText)
            

if __name__ == "__main__":
    main(sys.argv[1:])
    