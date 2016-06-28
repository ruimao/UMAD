if "%trainDFN%"=="iris.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 150 --dim 4 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="sonar.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 208 --dim 60 --ptFN %trainDFN%_LInfinityDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_LInfinityDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="wdbc.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 569 --dim 30 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="diabetes.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 768 --dim 8 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="waveform.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 5000 --dim 21 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="letter-recognition.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 20000 --dim 16 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="ecoli.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 336 --dim 7 --ptFN %trainDFN%_LInfinityDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_LInfinityDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="bal.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 625 --dim 4 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="mammographic-mass.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 961 --dim 5 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="cmc.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 1473 --dim 9 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="DBSCAN_4863.data" set CurCMD=UMAD --task classification --runM test --t string --disFun EditDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 4863 --ptFN %trainDFN%_EditDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EditDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="promoters.data" set CurCMD=UMAD --task classification --runM test --t dna --disFun DNAMetric --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 6042 --dim 57 --ptFN %trainDFN%_DNAMetric_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_DNAMetric_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="OliveOil-test.data" set CurCMD=UMAD --task classification --runM test --t time_series --disFun TimeSeriesMetric --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 30 --dim 570 --searchMode 1 --k %K% --status 1 --ptFN OliveOil.data_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_fft_%P%_%classifyMethod%_%K%.txt
if "%trainDFN%"=="synthetic_control-test.data" set CurCMD=UMAD --task classification --runM test --t time_series --disFun TimeSeriesMetric --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 300 --dim 60 --searchMode 1 --k %K% --status 1 --ptFN synthetic_control.data_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_fft_%P%_%classifyMethod%_%K%.txt
if "%trainDFN%"=="FaceAll-test.data" set CurCMD=UMAD --task classification --runM test --t time_series --disFun TimeSeriesMetric --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 1690 --dim 131 --searchMode 1 --k %K% --status 1 --ptFN FaceAll.data_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_fft_%P%_%classifyMethod%_%K%.txt
if "%trainDFN%"=="CinC_ECG_torso-test.data" set CurCMD=UMAD --task classification --runM test --t time_series --disFun TimeSeriesMetric --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 1378 --dim 1639 --searchMode 1 --k %K% --status 1 --ptFN CinC_ECG_torso.data_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_fft_%P%_%classifyMethod%_%K%.txt
if "%trainDFN%"=="tarck-test.data" set CurCMD=UMAD --task classification --runM test --t time_series --disFun TimeSeriesMetric --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 500 --dim 4 --searchMode 2 --k %K% --status 1 --ptFN tarck.data_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_fft_%P%_%classifyMethod%_%K%.txt
if "%trainDFN%"=="p1.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 4000 --dim 1 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="p2.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 4000 --dim 2 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="p3.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 4000 --dim 3 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="p4.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 4000 --dim 4 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="p5.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 4000 --dim 5 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="p6.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 4000 --dim 6 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="p7.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 4000 --dim 7 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="p8.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 4000 --dim 8 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="p9.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 4000 --dim 9 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10
if "%trainDFN%"=="p10.data" set CurCMD=UMAD --task classification --runM test --t vector --disFun EuclideanDistance --classifyMethod %classifyMethod% --testDFN %trainDFN% --F 4000 --dim 10 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --testMFN Test_%trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%_%K%.txt --status 0 --k %K% --m 10


echo=>>Logs\Test.log
echo=>>Logs\Test.log
echo=>>Logs\Test.log

echo %CurCMD%>>Logs\TestList.list

echo Current Task   : Test Model ( ClassifyMethod: %ClassifyMethod%, TrainDataSet: %trainDFN%, PivotsNum: %P% )
echo Current Task   : Test Model ( ClassifyMethod: %ClassifyMethod%, TrainDataSet: %trainDFN%, PivotsNum: %P% )>>Logs\Test.log

echo Current Command: "%CurCMD%"
echo Current Command: "%CurCMD%">>Logs\Test.log
echo=
echo=>>Logs\Test.log

echo Test Started on %time% %date%
echo Test Started on %time% %date%>>Logs\Test.log

echo Std Output:>>Logs\Test.log
echo ===========================================
echo ===========================================>>Logs\Test.log
echo=
echo=>>Logs\Test.log

cd /d ..\..\..\..\

echo Test Progressing......
..\Debug\%CurCMD%>>..\UMAD\SourceFiles\util\bat_windows\classification\Logs\Test.log
set e=%ERRORLEVEL%

cd /d .\SourceFiles\util\bat_windows\classification\

echo=
echo=>>Logs\Test.log
echo ===========================================
echo ===========================================>>Logs\Test.log
echo Test Ended on %time% %date%, Returned Code: %e% >>Logs\Test.log
echo Test Ended on %time% %date%, Returned Code: %e%

echo=>>Logs\Test.log
echo=>>Logs\Test.log
echo=
echo=
echo=