$files = Get-Item *.png
foreach($f in $files)
{
    Start-Process -FilePath Converter.exe -ArgumentList $f -Wait
}
pause