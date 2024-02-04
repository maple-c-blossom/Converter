$files = Get-Item *.png
foreach($f in $files)
{
    $mip = "-ml 2"
    Start-Process -FilePath Converter.exe -ArgumentList $f,$mip -Wait
}
pause