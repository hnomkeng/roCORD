## Known Problems and Solutions.

### Error:
```
/usr/bin/ld: warning: libcrypto.so.1.0.2, needed by /usr/lib/gcc/x86_64-linux-gnu/6/../../../x86_64-linux-gnu/libcurl.so, may conflict with libcrypto.so.1.1
```
### Solution:

Make sure you have libssl1.0 installed!
```
$ sudo apt-get install libssl1.0-dev
```
