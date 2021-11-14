cmake^
  --no-warn-unused-cli^
  -DCMAKE_TOOLCHAIN_FILE:STRING=~/vcpkg/scripts/buildsystems/vcpkg.cmake^
  -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE^
  -B build -S . -G "Visual Studio 16 2019" -T host=x64 -A x64^


@REM cmake --no-warn-unused-cli -DCMAKE_TOOLCHAIN_FILE:STRING="~/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -B build -S . -G Ninja -DCMAKE_C_COMPILER:FILEPATH="C:\Program Files\LLVM\bin\clang.exe" -DCMAKE_CXX_COMPILER:FILEPATH="C:\Program Files\LLVM\bin\clang++.exe" -DCMAKE_LINKER:FILEPATH="C:\Program Files\LLVM\bin\lld-link.exe"

@REM cmake^
@REM   --no-warn-unused-cli^
@REM   -DCMAKE_TOOLCHAIN_FILE:STRING=~/vcpkg/scripts/buildsystems/vcpkg.cmake^
@REM   -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE^
@REM   -DCMAKE_BUILD_TYPE:STRING=Debug^
@REM   -G Ninja^
@REM   "-DCMAKE_C_COMPILER:FILEPATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\x64\bin\clang.exe"^
@REM   "-DCMAKE_CXX_COMPILER:FILEPATH=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\x64\bin\clang.exe"^
@REM   -B build^
@REM   -S .^
