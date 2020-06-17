# VRPN-Free-d
VRPN server for Free-d source data provided by UDP stream.

# Building
Recent built was made with Visual Studio 2017. Building procedure is simple:
1. get the sources with git

2. update source code of 3rd party tools used: ```git submodule update --init --recursive```

3. copy vrpn's and quat's projects files:

* ```submodules/quatlib.vcxproj``` to  ```submodules/vrpn/quat/quatlib.vcxproj```
* ```submodules/vrpn.vcxproj``` to ```submodules/vrpn```

4. open solution file with Visual Studio and build

# Test/evaluation operation

1. Setup streaming to your host of Free-d data from camera or Simulator (see a last page of documentation at https://disruptar.com/downloads/) [data_streaming_simulator.png]

2. run compiled binary with arguments:
```
VRPN-Free-d.exe 3884 11111
```
where:
* **3884** is TCP port used by VRPN server
* **11111** is UDP port used to receive UDP data

if you receiving UDP data you will see a messages in console:
```
ID=255|Pan=137.0000|Tilt=146.0000|Roll=  0.0000|X=  0.0000|Y=  0.0000|Z=  0.0000|Zoom=  525|Focus= 2092 
```

3. test from remote server with VRPN tools:
```
vrpn_print_devices.exe FreeD@10.1.5.65:3884
```
resulting will similar to:
```
Tracker FreeD@10.1.5.65:3884, sensor 0:
        pos ( 0.00,  0.00,  0.00); quat (-0.30,  0.95,  0.01,  0.02)
Analog FreeD@10.1.5.65:3884:
         903.00, 1952.00 (2 chans)
Tracker FreeD@10.1.5.65:3884, sensor 0:
        pos ( 0.00,  0.00,  0.00); quat (-0.31,  0.95,  0.00,  0.01)
Analog FreeD@10.1.5.65:3884:
         904.00, 1953.00 (2 chans)
```


