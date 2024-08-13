# Play Radio

## Project Description

The "Play Radio" project is a client-server application designed to play radio.

## Building and Installing the Project

Before building the project, make sure your machine has all the required dependencies: 

`cmake`, `libboost-all-dev`, `libfmt-dev`, `libgtest-dev`, `libgmock-dev` 

If these are not yet installed, you can do so with the following command in Ubuntu:

`sudo apt update && sudo apt-get install cmake libboost-all-dev libfmt-dev libgtest-dev libgmock-dev -y`

**The application can be built as follows:**
````
git clone https://github.com/khromenokroman/play_radio.git
cd play_radio
mkdir -p build && cd build
cmake .. -DPATCH_VERSION=99
cmake --build .
````

**To run the tests, use:**

`./tests/play_radio_test`

**To build the package, use:**

`cmake --build . --target package`

**Running the Application**

The server can be started as follow (1993 is the port):

`./src/server/radio_server 1993` or `/usr/bin/radio_server 1993`

The client can be started as follow:

`./src/client/radio_client` or `/usr/bin/radio_client`

Upon installing the package, a file storing the list of station at  `/etc/radio/radio_list` is created. 

When starting the client, a random station is picked from this list and sent to the server to start streaming.

**There is also a unit file provided:**
````
[Unit]
Description=Radio Server Service
After=network.target

[Service]
PAMName=login
ExecStart=/usr/bin/radio_server 1993
User=raipo
Restart=on-failure
RestartSec=3
LimitNOFILE=1000

[Install]
WantedBy=multi-user.target
````
Remember to replace `User=raipo` with your actual username. This file is used to run the server service.

**!!! IMPORTANT !!!** 

The server should not be running as root, it should be running from a non-privileged user.