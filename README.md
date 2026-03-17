# logia

Compiler for logia language

## install/build/develop

### install windows

From a clean computer - Windows (on admin porwershell)

```ps1
choco install git
choco install cmake
choco install code

code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cpp-devtools
code --install-extension ms-vscode.cpptools-extension-pack
code --install-extension ms-vscode.cpptools-themes
code --install-extension ms-vscode.cmake-tools
```

On windows may require:

```ps1
reg add "HKLM\SYSTEM\CurrentControlSet\Control\FileSystem" /v LongPathsEnabled /t REG_DWORD /d 1 /f
```

Download LLVM 22.1.1 from source

Install python, I use pyenv

```ps1
Invoke-WebRequest -UseBasicParsing -Uri "https://raw.githubusercontent.com/pyenv-win/pyenv-win/master/pyenv-win/install-pyenv-win.ps1" -OutFile "./install-pyenv-win.ps1"; &"./install-pyenv-win.ps1"
pyenv.bat install 3.10.11
pyenv global 3.10.11
```

Download and install LLVM

[prebuilt LLVM](https://github.com/vovkos/llvm-package-windows/releases)

* `llvm-**-windows-amd64-msvc17-libcmt-dbg.7z`
* `llvm-**-windows-amd64-msvc17-libcmt.7z`


Or you can build LLVM from source

```
cd llvm-source
mkdir build
# choose one!
# Debug build with assetions
# cmake ..\src\llvm -DCMAKE_BUILD_TYPE=Debug -DLLVM_ENABLE_ASSERTIONS=ON
# Relase build with assetions (recommended)
cmake ..\src\llvm -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_BUILD_EXAMPLES=OFF -DLLVM_BUILD_TESTS=OFF

cmake --build . --target install -j8
```



#### build

Open folder on Visual Studio Code

Configure cmake

* select amdx64

Then `cmake` should be able to build the project.

Depending on your python install method you may require to set:

```cmake
set(Python_EXECUTABLE "xxx/python.exe" CACHE FILEPATH "Path to Python executable")
set(Python3_ROOT_DIR "xxx/")
```

### linux (TODO)


#### build

You can build on console with the following commands

```ps1
cd build
cmake ..
```