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

__author__ = 'Luke Doman'

""" 
Script to autogenerate a boilerplate for a cFS application. Some integration is still required post generation.

Usage:
python gen_app.py -n [uppercase app name] -o [path to airliner apps dir]
or
python gen_app.py -j [path to json config file]
"""

import argparse
from cookiecutter.main import cookiecutter
from distutils.dir_util import copy_tree
import json
import os
from shutil import copyfile, rmtree
import sys

DEFAULT_APP_PATH = '/home/vagrant/airliner/apps/'
TEMP_DIR = os.path.join(os.getcwd(), 'temp')
TEMPLATE_PATH = os.path.join(os.getcwd(), 'template')

class Generator(object):
	""" Generator object """
	def __init__(self, extras, output_path):
		self.extras = extras
		self.out_path = output_path

	def validate(self):
		""" Validate any passed parameters """
		if not os.path.exists(self.out_path):
			print "Output path specified does not exist."
			sys.exit(20)

	def setup(self):
		""" Generate temp output directory and clean if needed """
		if os.path.exists(TEMP_DIR):
			rmtree(TEMP_DIR)
		os.mkdir(TEMP_DIR)

	def autogen(self, auto_accept = True):
		""" Call cookiecutter """
		os.chdir(TEMP_DIR)
		cookiecutter(TEMPLATE_PATH, no_input=auto_accept, extra_context=self.extras, overwrite_if_exists=True)

	def migrate(self):
		""" Move generated app and config files to specified location """
		copy_tree(os.path.join(TEMP_DIR, 'output'), self.out_path)

	def cleanup(self):
		""" Perform any tear down operations here """
		rmtree(TEMP_DIR)
		return

	def run(self):
		""" Run all operations """
		self.validate()
		self.setup()
		self.autogen()
		self.migrate()
		self.cleanup()

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Autogenerate a cFS app.')
	parser.add_argument('-n','--app_name', help='Acronym of the cFS app. (Ex: CI)')
	parser.add_argument('-f','--app_full_name', help='ame of the cFS app. (Ex: Command Ingest)')
	parser.add_argument('-o','--output_dir', help='Directory to place the cFS app.')
	parser.add_argument('-j','--json_path', help='Use JSON input file for configuration.')
	args = vars(parser.parse_args())

	# Verify minimun args present
	app_name = args['app_name']
	json_file = args['json_path']
	if not json_file and not app_name:
		print "Error: App name [-n] or JSON path [-j] required"
		sys.exit(1)

	if json_file:
		with open(json_file, 'r') as f:
			args = json.load(f)

	out_path = args['output_dir'] if args['output_dir'] else DEFAULT_APP_PATH
	gen = Generator(args, out_path)
	gen.run()
	
