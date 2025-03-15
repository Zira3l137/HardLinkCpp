Remove-Item hardlinkcpp.exe -ErrorAction Ignore
cmake --build build

Copy-Item -Path .\build\hardlinkcpp.exe -Destination .\hardlinkcpp.exe
