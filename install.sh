#!/bin/bash

echo "### Installing Dependencies"
apt-get update
apt-get install -y g++ cmake pkg-config qt5-default libdatrie-dev libgraphviz-dev git
RUN DEBIAN_FONTEND=noninteractive apt-get install -y mysql-server libqt5sql5-mysql 

echo "### Creating Build Directory"
mkdir Build
cd Build

echo "### Generating Make Files"
cmake ./../

echo "### Executing Generated Make Files"
make

echo "### Starting MySQL server"
/etc/init.d/mysql start

echo "### Creating atm Database"
mysql -u root -e "Create Database atm"

echo "### Populating atm Database"
mysql -u root -D atm < ../database/atm_filled.sql

echo "### Done ###"

