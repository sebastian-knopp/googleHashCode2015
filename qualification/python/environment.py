
import getopt
import sys


class InvalidEnvironmentException(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)


class Environment:
    """Describes system parameters of a build environment"""

    address_model = "32"
    build_type = "RelWithDebInfo"
    compiler = "msvc"
    make_system = "ninja"

    def validate(self, value, allowed_values):
        if (not value in allowed_values):
            msg = value + " must be one of: "
            msg += ", ".join(allowed_values)
            raise InvalidEnvironmentException(msg)

    def print(self):
        tabs = 16
        print ("----------------------------------------")
        print ("  address_model\t: ".expandtabs(tabs) + self.address_model)
        print ("  build_type\t: ".expandtabs(tabs) + self.build_type)
        print ("  compiler\t: ".expandtabs(tabs) + self.compiler)
        print ("  make_system\t: ".expandtabs(tabs) + self.make_system)
        print ("----------------------------------------")

    def read_commandline (self, argv):
        try:
            opts, args = getopt.getopt(argv, "a:b:c:m:", ["address_model=", "build_type=", "compiler=", "make_system="])
        except getopt.GetoptError as e:
            print ("Invalid command line options:", e.msg)
            sys.exit(2)
        for opt, arg in opts:
            if opt in ("-a", "--address_model"):
                self.validate(arg, ["32", "64"])
                self.address_model = arg
            if opt in ("-b", "--build_type"):
                self.validate(arg, ["Release", "Debug", "RelWithDebInfo"])
                self.build_type = arg
            if opt in ("-c", "--compiler"):
                self.validate(arg, ["gcc", "msvc"])
                self.compiler = arg
            if opt in ("-m", "--make_system"):
                self.validate(arg, ["ninja", "msvc"])
                self.make_system = arg

