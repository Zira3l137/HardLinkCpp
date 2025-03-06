meson setup build --reconfigure
Remove-Item .\compile_commands.json -ErrorAction Ignore
Set-Location .\build
Copy-Item -Path .\compile_commands.json -Destination ..\compile_commands.json
Set-Location ..

