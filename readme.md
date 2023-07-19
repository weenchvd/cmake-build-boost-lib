## Build Boost library at CMake configure time

This project shows how to build a subset of the Boost library at CMake configure time.  
The Boost library doesn't use CMake internally, so we can't use the  
"FetchContent" module as usual. But we can get and build a subset of the Boost library  
using the "bcp" Boost tool.  


## Usage

**OS:**  
Windows  
Linux  

**Toolchain:**  
MSVC  
GCC  

**Command line:**  
`cmake --list-presets`  
`cmake --preset=<preset>`, where `<preset>` is the preset from the output of the previous command  

**IDE:**  
Choose the configure and build presets in your IDE.  


## This project has been tested using the following systems:

**System #1**  

OS: Windows 10 Pro 1809 (17763.107) x64  
IDE: Microsoft Visual Studio Code (64-bit) Version: 1.79.2  
Visual Studio Build Tools 2022 17.6.3  
GCC 13.1.0  
MSYS2 20230526  
CMake 3.27.0  
Ninja 1.11.1  

**System #2**  

OS: Windows 10 Pro 22H2 (19045.2364) x64  
IDE: Microsoft Visual Studio Community 2022 (64-bit) Version 17.6.4  
VCToolsVersion 14.36.32532  
CMake 3.26.0  
Ninja 1.11.0  

**System #3**  

OS: RebornOS Linux x86_64; Kernel: 6.1.34-1-lts; Shell: zsh 5.9  
IDE: Code - OSS (VS Code) Version: 1.79.2  
GCC 13.1.1 20230429  
CMake 3.26.4  
Ninja 1.11.1  
