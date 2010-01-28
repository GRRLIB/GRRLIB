@echo off

Rem -- Quick'n'dirty library installation util [BC]

Rem -- JPEG library
echo.
echo ---------------------------------------------------------------------------
echo Building JPEG Library...
echo.
cd jpeg
  make clean -s
  if errorlevel 1 goto error
  make all
  if errorlevel 1 goto error
cd ..

echo.
echo ---------------------------------------------------------------------------
echo Installing JPEG Library...
echo.
cd jpeg
  make install
  if errorlevel 1 goto error
cd ..

Rem -- PNG & PNGU libraries
echo.
echo ---------------------------------------------------------------------------
echo Building PNG Library...
echo.
cd png
  make clean -s
  if errorlevel 1 goto error
  make all
  if errorlevel 1 goto error
cd ..

echo.
echo ---------------------------------------------------------------------------
echo Installing PNG Library...
echo.
cd png
  make install
  if errorlevel 1 goto error
cd ..

echo.
echo ---------------------------------------------------------------------------
echo Building PNGU Library...
echo.
cd pngu
  make clean -s
  if errorlevel 1 goto error
  make all
  if errorlevel 1 goto error
cd ..

echo.
echo ---------------------------------------------------------------------------
echo Installing PNGU Library...
echo.
cd pngu
  make install
  if errorlevel 1 goto error
cd ..

Rem -- FreeType library
echo.
echo ---------------------------------------------------------------------------
echo Installing FreeType Library...
echo.
cd freetype
  make install
  if errorlevel 1 goto error
cd ..

echo.
echo Done
goto end
:error
echo.
echo Installation Failed!
:end
Rem -- The End
echo.
