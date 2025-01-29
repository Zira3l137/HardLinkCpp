Remove-Item hardlinkcpp.exe -ErrorAction Ignore
Set-Location .\build
meson compile

Copy-Item -Path .\hardlinkcpp.exe -Destination ..\hardlinkcpp.exe
Set-Location ..
