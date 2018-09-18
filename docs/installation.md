## Step by step guide for compilation and dependencies!

##### Attention!
Make sure to set "path to your rathena" and "path to roCORD" corretly.
It's required when a command has "<" ">" in it.

Example:

```
	$ cd <path to your rathena>/src/map
```
In my case will result in:
```
	$ cd /home/norm/rathena/src/map
```


#### Get roCORD and apply the patches.
First we get roCORD src files.
```
  $ git clone https://github.com/Normynator/roCORD.git
  $ cd roCORD
  $ git checkout development
```
Now we go into your rathena folder and apply the patches.
```
  $ cd <path to your rathena>/src/map
  $ patch channel.cpp <path to roCORD>/diff/rev1/channel.diff
  $ patch map.cpp <path to roCORD>/diff/rev1/map.diff
  $ patch script.cpp <path to roCORD>/diff/rev1/script.diff
  $ patch CMakeLists.txt <path to roCORD>/diff/rev1/CmakeLists.txt.diff
```
Next we are going to create folders for the roCORD config & src files.
Also copying the files to them.

Config files:
```
  $ cd ../../conf
  $ mkdir discord
  $ cd discord
  $ cp <path to roCORD>/config/* .
```

Source files:
```
 $ cd ../../src/map
 $ mkdir discord
 $ cp <path to roCORD>/src/* discord
 $ cp <path to roCORD>/CMakeLists.txt discord
```


#### Installing and compiling dependencies
Now we take care about the dependencies. We go to our home directory and install all requirements.

We start with the newest CMake version. (if you have version 3.12 or higher you can skip this part)
```
 $ cd
 $ git clone https://github.com/Kitware/CMake.git
 $ cd CMake
 $ ./bootstrap
 $ make
 $ sudo make install
```

Next up, websocket++ and nlohmann's json library. (websocket++ is a header only library, so no need to install that)
```
 $ cd
 $ git clone https://github.com/zaphoyd/websocketpp.git
```
```
 $ cd
 $ git clone https://github.com/nlohmann/json.git
 $ cd json
 $ mkdir build && cd build
 $ /usr/local/bin/cmake ..
 $ make
 $ sudo make install
```

We are done with the dependencies which are not provided by OS package-manager.
We just need to install some more packages and are ready to go.
```
 $ cd
 $ sudo apt-get install libboost-all-dev
 $ sudo apt-get install libcurl4-openssl-dev
```

#### Compiling and starting rAthena with roCORD!
Last we use all the components to compile roCORD together with rAthena.
```
 $ cd <path to your rathena>
 $ mkdir build
 $ cd build
 $ /usr/local/bin/cmake -G"Unix Makefiles" -DINSTALL_TO_SOURCE=ON -DJSON=/usr/local/include/nlohmann -DWEBSOCKETPP=~/websocketpp ..
 $ make install
```
Done! Now you can leave the build directory and start your server with ./athena-start start.
You should be able to see that roCORD is working when looking at the map-server output.

