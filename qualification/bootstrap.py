
from subprocess import call
import os
import getopt
import sys
import python.tools


EXT_SOURCES = os.path.join(os.environ['USERPROFILE'], "Dropbox", "ext-src")
BOOST_VERSION = "boost_1_56_0"
ZLIB_VERSION = "zlib-1.2.8"


WORKING_DIR = os.getcwd()


def bootstrap(compiler, nmbThreads):
    if (compiler != "mingw" and
        compiler != "msvc"):
        exit("unknown compiler")

    print("Bootstrapping scheduler build... (" + compiler + ")")
    print("")

    with python.tools.CD("ext"):
        if (not os.path.isdir("ext")):
            os.mkdir("ext")
        with python.tools.CD("ext"):

            # zlib
            python.tools.extract_tar_gz(os.path.join(EXT_SOURCES, ZLIB_VERSION + ".tar.gz"))
            ZLIB_DIR = os.path.join(os.getcwd(), ZLIB_VERSION)
            if (" " in ZLIB_DIR):
                exit("ZLIB_DIR must not contain spaces.")
            if (not os.path.isdir(ZLIB_DIR)):
                exit("ZLIB_DIR is not an existing directory (" + ZLIB_DIR + ")")

            print ("ZLIB_DIR:", ZLIB_DIR)
            print("")

            # boost
            python.tools.extract_tar_gz(os.path.join(EXT_SOURCES, BOOST_VERSION + ".tar.gz"))
            with python.tools.CD(BOOST_VERSION):

                print("Bootstrapping boost ...")
                f_boostBootstrapStd = open(os.path.join(WORKING_DIR, "boost-bootstrap.log"), "w")
                f_boostBootstrapErr = open(os.path.join(WORKING_DIR, "boost-bootstrap-error.log"), "w")
                bootstrapBoost = "bootstrap.bat " + compiler
                call(bootstrapBoost, stdout = f_boostBootstrapStd, stderr = f_boostBootstrapErr)

                print("Building boost ...")
                toolset = compiler
                if (compiler == "mingw"):
                    toolset = "gcc"

                buildBoost = "b2"
                buildBoost += " toolset=" + toolset

                if (compiler == "msvc"):
                    buildBoost += " link=static"
                    buildBoost += " runtime-link=shared"
                    buildBoost += " threading=multi"

                buildBoost += " -j" + nmbThreads
                buildBoost += " --with-iostreams"
                buildBoost += " --with-chrono"
                buildBoost += " --with-date_time"
                buildBoost += " --with-filesystem"
                buildBoost += " --with-log"
                buildBoost += " --with-program_options"
                buildBoost += " --with-regex"
                buildBoost += " --with-signals"
                buildBoost += " --with-serialization"
                buildBoost += " --with-system"
                buildBoost += " --with-thread"
                buildBoost += " -sZLIB_SOURCE=" + ZLIB_DIR
                print(buildBoost)

                f_boostBuildStd = open(os.path.join(WORKING_DIR, "boost-build.log"), "w")
                f_boostBuildErr = open(os.path.join(WORKING_DIR, "boost-build-error.log"), "w")

                try:
                    call(buildBoost, stdout = f_boostBuildStd, stderr = f_boostBuildErr)
                except OSError as e:
                    print ("Exception: ", e)
                    print ("Possible reasons:")
                    print ("- The bootstrapping process of boost failed, see boost-bootstrap-error.log.")
                    print ("- Compiling for msvc without using the msvc command prompt.")




def main(argv):

    compiler = ""
    nmbThreads = "4"

    try:
        opts, args = getopt.getopt(argv,"c:t:",["compiler=", "nmbThreads="])
    except getopt.GetoptError:
        print ("Usage:", sys.argv[0], '-c <compiler> -t <nmbThreads>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-c", "--compiler"):
            compiler = arg
        if opt in ("-t", "--nmbThreads"):
            nmbThreads = arg

    if compiler == "":
        print("Please specify compiler.")
    else:
        bootstrap(compiler, nmbThreads)


if __name__ == "__main__":
   main(sys.argv[1:])
