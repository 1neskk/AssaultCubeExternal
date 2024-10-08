import os
import subprocess
import platform

def run_command(command):
    process = subprocess.Popen(command, shell=True)
    process.wait()

def main():
    cwd = os.getcwd()
    platform_name = platform.system()

    if os.path.exists(cwd + "/../build"):
        os.rmdir(cwd + "/../build")
    else:
        os.mkdir(cwd + "/../build")

    os.chdir(cwd + "/../build")

    if platform_name == "Windows":
        run_command("cmake -G \"Visual Studio 17 2022\" -A Win32 ..")
        print("Open the project in Visual Studio and build it.")
    else:
        run_command("cmake ..")

    os.chdir(cwd)

if __name__ == "__main__":
    main()