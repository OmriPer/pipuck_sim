# installation
## 1. clone the repository
```sh
git clone git@github.com:OmriPer/pipuck2_ws.git
cd pipuck2_ws
```
## 2. Install System Dependencies
```sh
# Update and upgrade
sudo apt update && sudo apt upgrade -y

# Build tools
sudo apt install build-essential cmake git wget curl -y

# ARGoS dependencies
sudo apt install libfreeimage-dev libfreeimageplus-dev \
    qtbase5-dev qt5-qmake qtbase5-dev-tools \
    freeglut3-dev libxi-dev libxmu-dev \
    liblua5.3-dev lua5.3 \
    doxygen graphviz libgraphviz-dev asciidoc \
    libgl1-mesa-dev libglu1-mesa-dev x11-apps -y
```
- tested on Ubuntu 24.04, other versions may require slightly different packages
## 3. Run the Install Script
The repository includes an automated install script that builds and installs ARGoS3
```sh
chmod +x install.sh
./install.sh
```
The script will:
1. Build ARGoS core with Release configuration and documentation disabled
3. Install ARGoS system-wide
4. Update the library cache
## 4. Verify the Installation
```sh
# Verify ARGoS runs
argos3 -q all
# Verify pipuck plugin is installed
argos3 -q pipuck
```
