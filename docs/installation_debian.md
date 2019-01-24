# Installation for Debian.

### Requirements:
  - Using a supported rAthena version.
  - Debian Stretch or better.
  - roCORD folder is placed next to the rAthena folder.
  - The rAthena folder is called "rathena".

### Installing dependencies and apply patches:
  - Run the following commands (they require sudo permissions):
  ```
    $ apt-get install libcurl4-openssl-dev
    $ apt-get install libboost-all-dev
    $ apt-get install libssl-dev
    $ apt-get install libicu-dev
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
