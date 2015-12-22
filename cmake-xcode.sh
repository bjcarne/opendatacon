#!/bin/bash
cmake . "-Bbuild-xcode" -G "Xcode" "-DFULL=ON" "-DASIO_HOME=/usr/local" "-DDNP3_HOME=/tmp/dnp3" "-DMICROHTTPD_HOME=/usr/local" "-DMODBUS_HOME=/usr/local"
