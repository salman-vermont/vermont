#!/bin/sh

# Vars
VMT='../vermont'
EXC='fbudpexporter.xml'
WRC='fbwriter.xml'

# Start exporter
$VMT -f $EXC > /dev/null 2>&1 &
