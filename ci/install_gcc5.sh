#!/bin/bash

set -e
apt-get update
apt-get install --no-install-recommends --no-install-suggests -y \
  g++-5