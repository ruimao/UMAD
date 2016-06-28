if "%trainDFN%"=="iris.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 150 --dim 4 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="sonar.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 208 --dim 60 --ptFN %trainDFN%_LInfinityDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="wdbc.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 569 --dim 30 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="diabetes.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 768 --dim 8 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="waveform.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 5000 --dim 21 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="letter-recognition.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 20000 --dim 16 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="ecoli.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 336 --dim 7 --ptFN %trainDFN%_LInfinityDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="bal.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 625 --dim 4 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="mammographic-mass.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 961 --dim 5 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="cmc.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 1473 --dim 9 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="DBSCAN_4863.data" set CurCMD=UMAD --task classification --runM train --t string --disFun EditDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 4863 --ptFN %trainDFN%_EditDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="promoters.data" set CurCMD=UMAD --task classification --runM train --t dna --disFun DNAMetric --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 6042 --dim 57 --ptFN %trainDFN%_DNAMetric_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="OliveOil.data" set CurCMD=UMAD --task classification --runM train --t time_series --disFun TimeSeriesMetric --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 30 --dim 570 --searchMode 1 --ptFN %trainDFN%_TimeSeriesMetric_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="synthetic_control.data" set CurCMD=UMAD --task classification --runM train --t time_series --disFun TimeSeriesMetric --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 300 --dim 60 --searchMode 1 --ptFN %trainDFN%_TimeSeriesMetric_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="FaceAll.data" set CurCMD=UMAD --task classification --runM train --t time_series --disFun TimeSeriesMetric --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 560 --dim 131 --searchMode 1 --ptFN %trainDFN%_TimeSeriesMetric_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="CinC_ECG_torso.data" set CurCMD=UMAD --task classification --runM train --t time_series --disFun TimeSeriesMetric --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 40 --dim 1639 --searchMode 1 --ptFN %trainDFN%_TimeSeriesMetric_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="tarck.data" set CurCMD=UMAD --task classification --runM train --t time_series --disFun TimeSeriesMetric --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 3231 --dim 4 --searchMode 2 --ptFN %trainDFN%_TimeSeriesMetric_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="p1.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 4000 --dim 1 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="p2.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 4000 --dim 2 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="p3.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 4000 --dim 3 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="p4.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 4000 --dim 4 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="p5.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 4000 --dim 5 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="p6.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 4000 --dim 6 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="p7.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 4000 --dim 7 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="p8.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 4000 --dim 8 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="p9.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 4000 --dim 9 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt
if "%trainDFN%"=="p10.data" set CurCMD=UMAD --task classification --runM train --t vector --disFun EuclideanDistance --psm fft --v %P% --classifyMethod %classifyMethod% --trainDFN %trainDFN% --init 4000 --dim 10 --ptFN %trainDFN%_EuclideanDistance_fft_%P%_%classifyMethod%.txt


echo=>>Logs\Train.log
echo=>>Logs\Train.log
echo=>>Logs\Train.log

echo %CurCMD%>>Logs\TrainList.list

echo Current Task : Train Model ( ClassifyMethod: %ClassifyMethod%, TrainDataSet: %trainDFN%, PivotsNum: %P% )
echo Current Task : Train Model ( ClassifyMethod: %ClassifyMethod%, TrainDataSet: %trainDFN%, PivotsNum: %P% )>>Logs\Train.log

echo Current Command: "%CurCMD%"
echo Current Command: "%CurCMD%">>Logs\Train.log
echo=
echo=>>Logs\Train.log

echo Train Started on %time% %date%
echo Train Started on %time% %date%>>Logs\Train.log

echo Std Output:>>Logs\Train.log
echo ===========================================
echo ===========================================>>Logs\Train.log
echo=
echo=>>Logs\Train.log

cd /d ..\..\..\..\

echo Train Progressing......

..\Debug\%CurCMD%>>..\UMAD\SourceFiles\util\bat_windows\classification\Logs\Train.log

set e=%ERRORLEVEL%

cd /d ..\UMAD\SourceFiles\util\bat_windows\classification\

echo=
echo=>>Logs\Train.log
echo ===========================================
echo ===========================================>>Logs\Train.log
echo Train Ended on %time% %date%, Returned Code: %e% >>Logs\Train.log
echo Train Ended on %time% %date%, Returned Code: %e%

echo=>>Logs\Train.log
echo=>>Logs\Train.log
echo=
echo=
echo=