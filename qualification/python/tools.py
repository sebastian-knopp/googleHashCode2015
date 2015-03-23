
import os
import zipfile
import tarfile


class CD:
    """Context manager for changing the current working directory"""
    def __init__(self, newPath):
        self.newPath = newPath

    def __enter__(self):
        self.savedPath = os.getcwd()
        os.chdir(self.newPath)

    def __exit__(self, etype, value, traceback):
        os.chdir(self.savedPath)


def extract_tar_gz(from_file, to_path = "."):
    """Extracts a .tar.gz file to a directory"""
    print("Extract: " + from_file)
    tar = tarfile.open(from_file, 'r')
    tar.extractall(to_path)


def zip_directory(path, zip_filename):
    """Zips a complete directory"""
    f = zipfile.ZipFile(zip_filename, 'w', zipfile.ZIP_DEFLATED)
    for root, dirs, files in os.walk(path):
        for file in files:
            f.write(os.path.join(root, file))
    f.close()

