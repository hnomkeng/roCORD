## Step by step guide for compilation and dependencies!

1.  git clone https://github.com/Normynator/roCORD.git
2.  cd roCORD(edited)
3.  git checkout development
4.  cd <path to your rathena>/src/map
5.  patch channel.cpp <path to roCORD>/diff/rev1/channel.diff
6.  patch map.cpp <path to roCORD>/diff/rev1/map.diff
7.  patch script.cpp <path to roCORD>/diff/rev1/script.diff
8.  patch CMakeLists.txt <path to roCORD>/diff/rev1/CmakeLists.txt.diff
9.  cd ../../conf
10. mkdir discord
11. cd discord
12. cp <path to roCORD>/config/* .
13. cd ../../src/map(edited)
14. mkdir discord
15. cp <path to roCORD>/src/* discord
16. cp <path to roCORD>/CMakeLists.txt discord
17. cd(edited)
18. git clone https://github.com/Kitware/CMake.git
19. cd CMake
20. ./bootstrap
21. make
22. sudo make install(edited)
23. cd
24. git clone https://github.com/zaphoyd/websocketpp.git
25. git clone https://github.com/nlohmann/json.git
26. cd json
27. mkdir build && cd build
28. /usr/local/bin/cmake ..
29. make
30. sudo make install
31. cd
32. sudo apt-get install libboost-all-dev
33. sudo apt-get install libcurl4-openssl-dev
34. cd <path to your rathena>
35. mkdir build
36. cd build
37. /usr/local/bin/cmake -G"Unix Makefiles" -DINSTALL_TO_SOURCE=ON -DJSON=/usr/local/include/nlohmann -DWEBSOCKETPP=~/websocketpp ..
[01:36] nörm: 38. make install
39. Done!
