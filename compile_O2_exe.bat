@echo off
echo open "x64 Native Tools Command Prompt for VS 2022" terminal to compile for x64 architecture"
rmdir build
mkdir build
cl src/**.cpp /I include /std:c++17 /O2 /EHsc /Fo:build/ /Fe:build/