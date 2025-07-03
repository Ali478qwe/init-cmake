#!/bin/bash


Target_Script="/usr/local/bin/init_cmake"

if [ "$1" == "--uninstall" ]; then
    echo "🔧Deleting init_cmake ..."
    sudo rm -f "$Target_Script"   
    echo "❌ init_cmake Removed successfully"
    exit 0
fi


if [ "$0" != "$Target_Script" ]; then
   if [ ! -d /uer/local/bin ]; then
      sudo mkdir -p "/usr/local/bin"
   fi  
   sudo cp "$0" "$Target_Script"
   sudo chmod +x "$Target_Script"
   echo "✅ Global activation completed."
   echo "✅ Now Can Use: "init_cmake""
   echo "✅ For Delete : sudo rm /usr/local/bin/init_cmake Or init_cmake --uninstall"
   exit 0
fi



echo "🌈Enter name Project:"
read Project_name
echo "💾Enter name main file (main.cpp):"
read main_cpp
Project_path="$(pwd)/$Project_name"



mkdir -p "$Project_path"/{build,include,src,output}
echo ""
echo "📌Project created in '$Project_path' And Folders :"
echo "📁 $Project_path/build"
echo "📁 $Project_path/inculde"
echo "📁 $Project_path/src"
echo "📁 $Project_path/output"

cat << EOF > "$Project_path/src/$main_cpp"
#include <iostream>


int main(){

    std::cout << "The project $Project_name is working🔥";
    return 0;

}
EOF

cat << EOF > "$Project_path/CMakeLists.txt"
cmake_minimum_required(VERSION 3.10)

project($Project_name)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(MAKE_DIRECTORY "\${CMAKE_SOURCE_DIR}/output")


add_executable($Project_name 
                src/$main_cpp 
               )

set_target_properties(${Project_name} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "\${CMAKE_SOURCE_DIR}/output"
)

EOF


cat <<EOF > "$Project_path/README.md"
# $Project_name💾
🎨For build(bash):
   cd build
   cmake ..
   make
   ./$Project_name

EOF

echo "
🎨For build(bash):
   cd build
   cmake ..
   make
   ./$Project_name"
echo ""
