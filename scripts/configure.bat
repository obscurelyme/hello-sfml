cmake --no-warn-unused-cli -DCMAKE_TOOLCHAIN_FILE:STRING=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -B build -S . -G "Visual Studio 16 2019" -T host=x64 -A x64
