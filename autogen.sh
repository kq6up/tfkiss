#!/bin/sh
# autogen.sh - Bootstrap the build system
# Run this once after cloning the repository, before ./configure
#
# Requires: autoconf, automake, aclocal
#
set -e
echo "Bootstrapping tfkiss build system..."
autoreconf -i
echo "Done. Now run:"
echo "  ./configure"
echo "  make"
echo "  make install"
