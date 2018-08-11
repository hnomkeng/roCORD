# roCORD.v2 - Proof of Concept
Disclaimer: THIS VERSION IS UNSTABLE!
In this branch, you can find all files required for testing the proof of concept C++ implementation of roCORD.

### Required packages for Debian
- [libcurl](https://packages.debian.org/en/stretch/libcurl4-openssl-dev)
- [nlohmann](https://packages.debian.org/en/sid/nlohmann-json-dev)
- [websocketpp](https://packages.debian.org/stretch/libwebsocketpp-dev)

##### Why are these packages required?
libcurl is responsible for API calls over HTTPS and websocketpp implements Secure WebSockets. Both are required because to come up with the crypto for the underlying TLS protocol. nlohmann is there to make it easier to handle JSON format required by the Discord API.

### Why is this version unstable and unclean?
I decided to release this version because I wanted to share my progress in first place. So please DO NOT use this version in production if you don't know what you are doing.
The multithreading in this version can cause UNDEFINED behaviour and therefore cause random crashes.

### Known Bugs:
- Multithreading can cause undefined behavior (will be fixed in the final release)
- Special characters are not supported (will also be fixed in the final release)

### Available features:
PoC of .v2 is already fully supporting current encryption standards. All traffic is encrypted with TLS1.2 or TLS1.1.
Also, the discord API certificate is verified on everytime the bot starts, for this I included a bundle of CA Root Certificates from Mozilla created Wed Jun 20 03:12:06 2018 GMT. If you do not trust this certificate bundle feel free to use your own. The CA Root Certificates are no secrets means you do not have to worry about leaking them since they are publicly known, so you can freely use the bundle provided in the repository.

### What's next?
Now I will focus on rewriting everything as clean and structured code. Also, I will work on a fix for the known bugs.
This will take a bit but I will provide constant updates. After the final release of .v2 I am starting to implement more and more features.
The stable release will contain:
- Bug fix for multithreading and special characters
- JSON config files
- A guide on how to integrate roCORD.v2

#### Thanks for supporting this project.
Greetings,
norm
