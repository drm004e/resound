#!/bin/bash

# Run HDSPMixer
hdspmixer &
sleep 5

# Run jackd and wait
/usr/bin/jackd -R -dalsa -r44100 -p256 -n2 -D -Chw:0 -Phw:0 &
sleep 5

# Run resound server with 24 I/O
resound_server -i8 -o24 -v &
sleep 10

# Make jack input connections
jack_connect system:capture_7 Resound:Input0
jack_connect system:capture_8 Resound:Input1

# Make jack output connections
jack_connect Resound:Output0 system:playback_1
jack_connect Resound:Output1 system:playback_2
