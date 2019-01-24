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
  $ patch channel.cpp <path to roCORD>/diff/rev<X>/channel.diff
  $ patch map.cpp <path to roCORD>/diff/rev<X>/map.diff
  $ patch script.cpp <path to roCORD>/diff/rev<X>/script.diff
  $ patch CMakeLists.txt <path to roCORD>/diff/rev<X>/CmakeLists.txt.diff
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
 $ cp -r <path to roCORD>/third_party/* <path to rathena>/3rdparty/.
```


#### Installing and compiling dependencies
Now we take care about the dependencies. We go to our home directory and install all requirements.
```
 $ cd
 $ sudo apt-get install libboost-all-dev
 $ sudo apt-get install libcurl4-openssl-dev
 $ sudo apt-get install libssl1.0-dev
```

#### Compiling and starting rAthena with roCORD!
Last we use all the components to compile roCORD together with rAthena.
```
 $ cd <path to your rathena>
 $ mkdir build
 $ cd build
 $ cmake -G"Unix Makefiles" -DINSTALL_TO_SOURCE=ON ..
 $ make install
```
Done! Now you can leave the build directory and start your server with ./athena-start start.
You should be able to see that roCORD is working when looking at the map-server output.

