echo off
FOR /L %%A IN (1,1,10000000000) DO (
	echo|set /p= "%%A "
	gen.exe %%A>int 
	a.exe<int>out1 
	brute.exe<int>out2 
	FC out1 out2>nul 
	if errorlevel 1 goto error
	echo Done!
)

goto:eof

:error
echo failed check

:eof
