[![Build Status](https://travis-ci.org/bk138/Multicast-Client-Server-Example.svg?branch=master)](https://travis-ci.org/bk138/Multicast-Client-Server-Example)
[![Build status](https://ci.appveyor.com/api/projects/status/iyjew3rnkfg1r7dm/branch/master?svg=true)](https://ci.appveyor.com/project/bk138/multicast-client-server-example/branch/master)
[![Help making this possible](https://img.shields.io/badge/liberapay-donate-yellow.png)](https://liberapay.com/bk138/donate)
[![Become a patron](https://img.shields.io/badge/patreon-donate-yellow.svg)](https://www.patreon.com/bk138)
[![Flattr this git repo](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=dontmind&url=https://github.com/bk138/Multicast-Client-Server-Example&title=Multicast-Client-Server-Example&language=&tags=github&category=software) 

This is a simple educational example of a multicast 
client and server, running under UNIX and Win32.

fixed: set a special local ip when multi local ips exist.

# Building
(windows build tool:  cmake , visual studio)
To compile, use

    mkdir build
    cd build
    cmake ..
    cmake --build .

#Running
  (multicast send) server <local ip> 225.0.1.3 5786
   (loop send, special hex data)         Debug\server.exe <local ip> 227.5.5.55 23335 -1 -1 
   
(only running in windows10) Release\VC_redist.x64.exe
                            Release\server.exe <local ip> 227.5.5.55 23335 -1 -1
