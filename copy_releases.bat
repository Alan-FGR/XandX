mkdir Releases\x86
copy Release\Injector.exe Releases\x86\Injector.exe
copy Release\Library.dll Releases\x86\Library.dll
copy EasyHook\EasyHook32.dll Releases\x86\EasyHook32.dll

mkdir Releases\x64
copy x64\Release\Library.dll Releases\x64\Library.dll
copy x64\Release\Injector.exe Releases\x64\Injector.exe
copy EasyHook\EasyHook64.dll Releases\x64\EasyHook64.dll

