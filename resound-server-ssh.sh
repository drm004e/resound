#!/bin/bash
# Quick script to do an ssh with X support connection.
# This can be installed on the resound GUI machine and aliased
# for convenience if required

echo "Opening SSH connection to $1@$2 ..."
ssh -X $1@$2
