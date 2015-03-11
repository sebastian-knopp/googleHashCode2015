import sys


# This script generates empty stubs for a new class.
#
# This includes:
# - Header file
# - Source file
# - Unit test


libraryDefault = "scheduler"

print("")

#################################################
print("Classname: ")
classname = input()
if (classname is ""):
	print("Empty classname, exit")
	exit()


#################################################
print("")
print("Category (default is 'libs'): ")
category = input()
if (category is ""):
	category = "libs"

validCategories = ["tools", "libs"]	
if (category not in validCategories):
	print("")
	print("Category must be one of: ")
	for i in validCategories:
		print (i)
	exit();


#################################################
print("")
print("Library/Tool (default is '" + libraryDefault + "'): ")
library = input()
if (library is ""):
	library = libraryDefault

print("")


#################################################

namespaces = []
sourceDir = "/"
headerDir = "/"
if (category is "libs"):
	namespaces = [ library ]
	sourceDir = '/src/'
	print("HeaderDir (default is '" + sourceDir + "'): ")
	headerDir = input()
	if (headerDir is ""):
		headerDir = sourceDir


print("Creating in library " + library + " a new class " + classname)


#################################################
header = open(category + '/' + library + headerDir + classname + '.h', 'w')
header.write('#pragma once\n')

if (namespaces):
	header.write('\n')
	header.write('\n')

for n in namespaces:
	header.write('namespace ' + n + ' {\n')

header.write('\n')
header.write('\n')
header.write('class ' + classname + ' {\n')
header.write('\n')
header.write('public:\n')
header.write('\n')
header.write('    ' + classname + '()\n')
header.write('    {\n')
header.write('    }\n')
header.write('\n')
header.write('};\n')

if (namespaces):
	header.write('\n')
	header.write('\n')

for n in namespaces:
	header.write('} // namespace ' + n + '\n')

header.write('\n')
header.close()


#################################################
cpp = open(category + '/' + library + sourceDir + classname + '.cpp', 'w')
cpp.write('#include "' + classname + '.h"\n')

if (namespaces):
	cpp.write('\n')
	cpp.write('\n')

for n in namespaces:
	cpp.write('namespace ' + n + ' {\n')

cpp.write('\n')
cpp.write('\n')

for n in namespaces:
	cpp.write('} // namespace ' + n + '\n')

cpp.write('\n')
cpp.close()


#################################################
unittest = open(category + '/' + library + '/test/' + classname + 'Test.cpp', 'w')
unittest.write('#include "' + classname + '.h"\n')
unittest.write('#include "gtest/gtest.h"\n')

if (namespaces):
	unittest.write('\n')
	unittest.write('\n')

for n in namespaces:
	unittest.write('namespace ' + n + ' {\n')

unittest.write('\n')
unittest.write('\n')
unittest.write('TEST(' + classname + 'Test, empty)\n')
unittest.write('{\n')
unittest.write('    ' + classname + '();\n')
unittest.write('\n')
unittest.write('    EXPECT_TRUE(true);\n')
unittest.write('}\n')

if (namespaces):
	unittest.write('\n')
	unittest.write('\n')

for n in namespaces:
	unittest.write('} // namespace ' + n + '\n')

unittest.write('\n')
unittest.close()


print("Created files successfully.")


