@echo off

Rem -- Quick'n'dirty library installation util [BC]

Rem -- JPEG library
echo.
echo ---------------------------------------------------------------------------
echo Installing JPEG Library...
echo.
cd jpeg
  make install
cd ..

Rem -- PNG & PNGU libraries
echo.
echo ---------------------------------------------------------------------------
echo Installing PNG Library...
echo.
cd png
  make install
cd ..

echo.
echo ---------------------------------------------------------------------------
echo Building PNGU Library...
echo.
cd pngu
  make clean
  make all
cd ..

echo.
echo ---------------------------------------------------------------------------
echo Installing PNGU Library...
echo.
cd pngu
  make install
cd ..

Rem -- The End
echo.
echo Done
echo.
