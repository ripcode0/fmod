## FMOD (Audio Library)
!["fmod logo"](./sub/fmod1.png)
### FMOD Pre-Build API
* This repository provides a pre-built API based on `FMOD`.
* It includes precompiled binaries and headers for seamless integration without requiring you to build `FMOD` from source

### Features
* Supports **FMOD Core API v1.10.20**
* Provides pre-built `DLLs` 
* `CMake` integration

### Usages
* **Standalone**
    ```cmd
    git clone https://github.com/ripcode0/fmod.git
    cmake -S . -B build -DENABLE_EXAMPLE=ON
    cmake --build build
    ```
* **Installation**
    ```cmd
    cmake --install build --prefix <install path>
    ```
    * Root CMakeLists.txt
    ```cmake
    set(CMAKE_PREFIX_PATH <install path>)
    find_package(fmod REQUIRED)
    traget_link_libraries(<excutable> PRIVATE fmod) #fmod::fmod also work
    ```
* **Copying DLLs**
    * After completing the build or installation, **FMOD** is a `SHARED` library,  
    so on `Windows`, you need to copy the required `DLL` files along with your application.  
    **Note:** If the `DLL` files are missing, the program may fail to run or produce errors.
    ```cmake
    add_custom_command(
        TARGET <excutable> 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${fmod_SHARED_LIB}
        $<TARGET_FILE_DIR:<excutable>>
    )
    ```
* **build-in Variables**
    * `fmod_INCLUDE_DIRS`
    * `fmod_LIBRARIES`
    * `fmod_SHARED_LIB`


---
Powered by [FMOD Studio](https://www.fmod.com/) – © Firelight Technologies Pty Ltd.  

Commercial use requires a valid license. See [FMOD Licensing](https://www.fmod.com/licensing).

