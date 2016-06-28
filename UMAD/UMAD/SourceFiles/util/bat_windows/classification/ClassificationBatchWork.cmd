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


:Train
del .\Logs\Train.list >nul 2>nul
echo           Training Model Batch Work Started on %time% %date%
echo ===============================================================================
echo                  Train Log>Logs\Train.log
echo          %time% %date%>>Logs\Train.log
echo ===============================================================================>>Logs\Train.log

for /f "eol=; tokens=1 delims= " %%z in (classifyMethod.options) do set classifyMethod=%%z& for /f "eol=; tokens=1 delims= " %%j in (trainDataSet.options) do set trainDFN=%%j& for /f "eol=; tokens=1 delims= " %%p in (pivotsNum.options) do set P=%%p& call TrainModel.cmd

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


:Test
del .\Logs\Test.list >nul 2>nul
echo            Testing Batch Work Started on %time% %date%
echo ===========================================
echo                  Test Log>Logs\Test.log
echo          %time% %date%>>Logs\Test.log
echo ===========================================>>Logs\Test.log

for /f "eol=; tokens=1 delims= " %%z in (classifyMethod.options) do set classifyMethod=%%z& for /f "eol=; tokens=1 delims= " %%j in (testDataSet.options) do set trainDFN=%%j& for /f "eol=; tokens=1 delims= " %%p in (pivotsNum.options) do set P=%%p& call TestModel.cmd

pause

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
explorer .\UMAD\UMAD\SourceFiles\util\data\
