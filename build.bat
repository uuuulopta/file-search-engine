cd build
cmake -G "MinGW Makefiles" ../
cmake --build .
cd ../
mv build\compile_commands.json compile_commands.json
