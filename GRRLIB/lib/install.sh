#!/bin/bash

set -x

# Quick'n'dirty library installation util [BC]

# JPEG library
echo -e "\n--------------------------------------------------------------------"
echo -e "Installing JPEG Library...\n"
pushd jpeg
  make install
popd

# PNG & PNGU libraries
echo -e "\n--------------------------------------------------------------------"
echo -e "Installing PNG Library...\n"
cd png
  make install
cd ..

echo -e "\n--------------------------------------------------------------------"
echo -e "Building PNGU Library...\n"
cd pngu
  make clean
  make all
cd ..

echo -e "\n--------------------------------------------------------------------"
echo -e "Installing PNGU Library...\n"
cd pngu
  make install
cd ..

# FreeType library
echo -e "\n--------------------------------------------------------------------"
echo -e "Installing FreeType Library...\n"
cd freetype
  make install
cd ..

# The End
echo -e "\nDone\n"
