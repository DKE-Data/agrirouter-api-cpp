#!/bin/sh
scriptname=${0##*/}
lib_path="${PWD}/../../"

error ()
{
    echo "Error ${scriptname}: $1"
    exit $2
}

#Add /usr/local/bin to PATH if needed
if ! echo $PATH | grep -q "/usr/local/bin"; then
    export PATH=$PATH:/usr/local/bin
fi

#Check if protoc is installed
which protoc >/dev/null 2>&1
if [ ! $? -eq 0 ]; then
    error "protoc not installed. Get and install: https://github.com/google/protobuf" 1
fi

for file in $(find . -name '*.proto')
do
    # Compile c++ files from protos
    echo "$scriptname: Compiling $file ..."

    # Compile pb files
    protoc $file --cpp_out=${PWD}
    if [ ! $? -eq 0 ]; then
        error "Compiling $file failed. An error occured while generating pb.cc/pb.h file." 2
    fi
    echo "$scriptname: Compiled $file successfully"
done

# Move generated files into lib subfolder
echo "$scriptname: Moving output files to library subfolders."
for file in $(find . -name '*.pb.h')
do
    destination_path="${lib_path}inc/Protobuf/$file"
    mkdir -p $(dirname $destination_path) && mv $file $destination_path
done

for file in $(find . -name '*.pb.*')
do
    destination_path="${lib_path}inc/Protobuf/$file"
    mkdir -p $(dirname $destination_path) && mv $file $destination_path
done
echo "Protobuf generated successful"


