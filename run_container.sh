#! /bin/bash

# This script is used to run the container with the default configuration attached to a common home file system
SOURCE_DIR=/mnt/c/Users/kylec/Programming/Docker_Environment/Deafult_with_caf/

# launch the container
docker run -itd --mount type=bind,source=$SOURCE_DIR,target=/home/usr1 ubuntu_default_env