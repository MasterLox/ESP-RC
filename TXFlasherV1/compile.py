import subprocess
import os
import shutil

# Get the current user's home directory
home_dir = os.path.expanduser("~")

# Construct the destination folder path
destination_folder = os.path.join(home_dir, "Documents", "Arduino", "libraries")

# Path to the folder to be copied
source_folder = "libraries"

# Create the destination directory if it doesn't exist
os.makedirs(destination_folder, exist_ok=True)

# Copy the files and folders to the destination, overwriting existing files
for root, dirs, files in os.walk(source_folder):
    # Get the relative path from the source folder
    relative_path = os.path.relpath(root, source_folder)
    
    # Construct the corresponding destination folder
    dest_folder = os.path.join(destination_folder, relative_path)
    
    # Create the destination folder if it doesn't exist
    os.makedirs(dest_folder, exist_ok=True)
    
    # Copy the files to the destination, overwriting existing files
    for file in files:
        src_file = os.path.join(root, file)
        dest_file = os.path.join(dest_folder, file)
        shutil.copy2(src_file, dest_file)

print("Folder copied successfully!")

def install_platformio():
    cmd = "pip install -U platformio"
    subprocess.call(cmd, shell=True)

def build_project():
    project_dir = '\esprc'
    cwd = os.getcwd()
    os.chdir(cwd + project_dir)
    build_process = subprocess.Popen(['python', '-m', 'platformio', 'run'])

    build_process.wait()

def flash_project():
    project_dir = '\esprc'
    cwd = os.getcwd()
    os.chdir(cwd + project_dir)
    flash_process = subprocess.Popen(['python', '-m', 'platformio', 'run', '--target', 'upload'])
    flash_process.wait()

if __name__ == '__main__':
    install_platformio()
    #build_project()
    flash_project()
