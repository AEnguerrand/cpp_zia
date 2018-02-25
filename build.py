import shutil
import os
import sys
import subprocess

print ("---------- Go to Directory -----\n")

os.chdir(os.path.expanduser("./build"))

print ("\n---------- Cmake ----------\n")

cmakeCmd = ['cmake', ".."]
if sys.platform.startswith('win'):
    shell = True
else:
    shell = False

retCode = subprocess.check_call(cmakeCmd, stderr=subprocess.STDOUT, shell=shell)

print ("\n---------- Build ----------\n")

cmakeCmd = ['cmake', "--build", "."]
if sys.platform.startswith('win'):
    shell = True
else:
    shell = False

retCode = subprocess.check_call(cmakeCmd, stderr=subprocess.STDOUT, shell=shell)

print ("\n---------- Move dll into Build/Modules folder ----------\n")

source = os.listdir("../Modules/")

if not os.path.exists("./Modules"):
		os.makedirs("./Modules")

for files in source:
	shutil.copy2("../Modules/" + files, "./Modules/" + files )

print ("\n---------- Finish ----------\n")
