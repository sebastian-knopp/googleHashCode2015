
import python.environment
import python.schedulertools
import os
import subprocess
import sys


WORKING_DIR = os.getcwd()
CMAKE_EXE = os.path.join(os.environ['USERPROFILE'], "Dropbox", "programs", "cmake", "bin", "cmake.exe")


def call_cmake(environment):
    environment.print()

    cmake_call = CMAKE_EXE
    cmake_call += " -DCMAKE_PREFIX_PATH=C:\\Qt\\5.3\\msvc2013\\"

    if (environment.compiler == "msvc" and environment.make_system == "msvc"):
        cmake_call += " -G\"Visual Studio 12 2013\""
    else:
        cmake_call += " -G\"Ninja\""

    if (environment.compiler == "msvc"):
        cmake_call += " -DCXX=cl"

#    if (environment.compiler == "msvc"):
#        cmake_call += " -DCMAKE_SYSTEM_NAME=MSVC"

    cmake_call += " -DCMAKE_BUILD_TYPE=" + environment.build_type
    cmake_call += " " + WORKING_DIR

    print(cmake_call)
    #sys.exit(0)

    build_dir_name = "scheduling-impl"
    build_dir_name += "-" + environment.compiler
    build_dir_name += "-" + environment.address_model

    build_dir = os.path.join("..", build_dir_name)
    if (not os.path.isdir(build_dir)):
        os.mkdir(build_dir)

    with python.schedulertools.CD(build_dir):
        try:
            subprocess.check_call(cmake_call)
        except subprocess.CalledProcessError as e:
            print()
            print("Caught CMake error (" + str(e.returncode) + ")")


def main(argv):
    e = python.environment.Environment()
    try:
        e.read_commandline(argv)
    except python.environment.InvalidEnvironmentException as ex:
        print(ex)
        sys.exit(1)

    call_cmake(e)


if __name__ == "__main__":
   main(sys.argv[1:])

