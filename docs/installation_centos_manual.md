# Installation for CentOS.

### Requirements:
  - Using a supported rAthena version.
  - CentOS 7 or better.

### Installing dependencies and apply patches:
  - Run the following commands (they require sudo permissions):
  ```
    $ yum install llvm-toolset-7-cmake
    $ yum install boost boost-devel boost-static
    $ yum install libicu-devel
    $ yum install openssl-devel
    $ yum install libcurl-devel

  ```
  - You can either use:
    ```
      scl enable llvm-toolset-7 bash
    ```
    or add the following to your .bashrc or .bash_profile:
    ```
      source scl_source enable llvm-toolset-7
    ```
  - Clone the roCORD respository.
  - Create directories for roCORD conf and src:
  ```
    $ mkdir <path to rathena>/src/map/discord
    $ mkdir <path to rathena>/conf/discord
  ```
  - Copy all required files.
  ```
    $ cp <path to roCORD>/CMakeLists.txt <path to rathena>/src/map/discord/.
    $ cp <path to roCORD>/src/* <path to rathena>/src/map/discord/.
    $ cp <path to roCORD>/config/* <path to rathena>/conf/discord/.
    $ cp -r <path to roCORD>/third_party/* <path to rathena>/3rdparty/.
  ```
  - If you do not use a supported rAthena version the following commands won't work and you need to apply the patches on your own.
  ```
    $ patch <path to rathena>/src/map/channel.cpp <path to roCORD>/diff/rev2/channel.diff
    $ patch <path to rathena>/src/map/map.cpp <path to roCORD>/diff/rev2/map.diff
    $ patch <path to rathena>/src/map/script.cpp <path to roCORD>/diff/rev2/script.diff
    $ patch <path to rathena>/src/map/CMakeLists.txt <path to roCORD>/diff/rev2/CmakeLists.txt.diff

  ```
### Compiling rAthena with roCORD
  - Go into your rAthena folder.
  - Create a "build" folder (with mkdir)
  - Go into the "build" directory.
  - Run cmake:
    ```
      $ cmake -G"Unix Makefiles" -DINSTALL_TO_SOURCE=ON ..
    ```
  - Run "make install".

### Recompiling rAthena with roCORD
  - remove the build dir:
    ```
      $ rm -rf build
    ```
  - create a new build dir and go into it.
    ```
      $ mkdir build && cd build
    ```
  - use CMake:
    ```
      $ cmake -G"Unix Makefiles" -DINSTALL_TO_SOURCE=ON ..
    ```
  - finally run make:
    ```
      $ make install
    ```
### Done
  - Now you can run your server as you normaly do.
