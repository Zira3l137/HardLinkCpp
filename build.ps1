Remove-Item linkfiles.exe -ErrorAction Ignore
Set-Location .\builddir
meson compile

Copy-Item -Path .\linkfiles.exe -Destination ..\linkfiles.exe
Set-Location ..
