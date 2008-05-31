#!/bin/bash

# Run HDSPMixer
hdspmixer &
sleep 5

# Run jackd and wait
/usr/bin/jackd -R -dalsa -r44100 -p512 -n2 -D -Chw:0 -Phw:0 &
sleep 5

# Run resound server with 24 I/O
resound_server -i8 -o24 &
sleep 10

# Make jack input connections
#jack_connect system:capture_7 Resound:Input0
#jack_connect system:capture_8 Resound:Input1

# for loop version
for ((i=0;i<8;i++)); do
#sleep 0.2
jack_connect system:capture_`expr $i + 1` Resound:Input$i
done

# Make jack output connections
# for loop version
for ((i=0;i<24;i++)); do
#sleep 0.2
jack_connect Resound:Output$i system:playback_`expr $i + 1`
done



