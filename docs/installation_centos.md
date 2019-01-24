# Installation for CentOS.

### Requirements:
  - Using a supported rAthena version.
  - CentOS 7 or better.
  - roCORD folder is placed next to the rAthena folder.
  - The rAthena folder is called "rathena".

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
  - Place the downloaded files next to your rAthena folder.
  - WARNING: following script diffs your files automatically, it can cause problems if you use other source mods or don't use a supported rA commit.
  - Go into roCORD folder and run the install.sh script.
  ```
    $ ./install.sh
  ```
### Compiling rAthena with roCORD
  - Go into your rAthena folder.
  - You should find a "build" directory. (If not then something went wrong)
  - Go into the "build" directory.
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
