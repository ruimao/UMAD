@echo off
color 0a

del .\Logs\*.list >nul 2>nul



echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo               Welcome to Batch Test Environment
echo               The Current Time is: %time% %date%
echo               Press any key to start...
pause>nul
cls

:build
del .\Logs\Build.list >nul 2>nul
echo           Index Building Batch Work Started on %time% %date%
echo ===========================================
echo                  Build Log>Logs\Build.log
echo          %time% %date%>>Logs\Build.log
echo ===========================================>>Logs\Build.log

for /f "eol=; tokens=1 delims= " %%z in (DataType.options) do set dataType=%%z& for /f "eol=; tokens=1 delims= " %%j in (BuildModes.options) do set BuildMode=%%j& for /f "eol=; tokens=1 delims= " %%k in (datasize/sizeList_%%z) do set Size=%%k& call BuildTemp.cmd

echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=


:search
del .\Logs\Search.list >nul 2>nul
echo            Searching Batch Work Started on %time% %date%
echo ===========================================
echo                  Search Log>Logs\Search.log
echo          %time% %date%>>Logs\Search.log
echo ===========================================>>Logs\Search.log
for /f "eol=; tokens=1 delims= " %%z in (DataType.options) do set dataType=%%z& for /f "eol=; tokens=1 delims= " %%i in (SearchModes.options) do set SearchMode=%%i& for /f "eol=; tokens=1 delims= " %%j in (BuildModes.options) do set BuildMode=%%j& for /f "eol=; tokens=1 delims= " %%k in (datasize/sizeList_%%z) do set Size=%%k& call SearchTemp.cmd

cls
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo=
echo                         All the Tasks Finished!
echo                         Press any key to exit.........
pause>nul
