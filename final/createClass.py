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


print("Create class " + classname)


#################################################
header = open(classname + '.h', 'w')
header.write('#pragma once\n')

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

header.write('\n')
header.close()


#################################################
cpp = open(classname + '.cpp', 'w')
cpp.write('#include "' + classname + '.h"\n')

cpp.write('\n')
cpp.write('\n')

cpp.write('\n')
cpp.close()




print("Created files successfully.")


