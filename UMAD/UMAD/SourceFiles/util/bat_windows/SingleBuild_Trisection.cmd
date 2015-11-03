if "%dataType%"=="vector" set CurCMD=UMAD --runOption build --buildMode %BuildMode% --indexName vector_%Size%_%BuildMode%_v2_f3_m100_tR%tR% --n uniformvector-20dim-1m.txt --v 2 --f 3 --t vector --e MVPIndex --psm fft --dpm PivotWise --m 100 --init %Size% --F %Size% --s 2000 --dim 10 --res results_vector_build_%Size%i_%BuildMode%_fft_PivotWise_MVPIndex_v2_f3_m100_dim20_tR%tR%.txt --tR %tR% --sop %sop% --pbop %pbop%
if "%dataType%"=="string" set CurCMD=UMAD --runOption build --buildMode %BuildMode% --indexName string_%Size%_%BuildMode%_v2_f3_m100_tR%tR% --n English.dic --v 2 --f 3 --t string --e MVPIndex --psm fft --dpm PivotWise --m 100 --init %Size% --F %Size% --s 2000 --dim 2 --res results_string_build_%Size%i_%BuildMode%_fft_PivotWise_MVPIndex_v2_f3_m100_dim2_tR%tR%.txt --tR %tR% --sop %sop% --pbop %pbop%
if "%dataType%"=="image" set CurCMD=UMAD --runOption build --buildMode %BuildMode% --indexName image_%Size%_%BuildMode%_v2_f3_m100_tR%tR% --n allfeas.dat --v 2 --f 3 --t image --e MVPIndex --psm fft --dpm PivotWise --m 100 --init %Size% --F %Size% --s 2000 --dim 66 --res results_image_build_%Size%i_%BuildMode%_fft_PivotWise_MVPIndex_v2_f3_m100_dim66_tR%tR%.txt --tR %tR% --sop %sop% --pbop %pbop%
if "%dataType%"=="dna" set CurCMD=UMAD --runOption build --buildMode %BuildMode% --indexName dna_%Size%_%BuildMode%_v2_f3_m100_tR%tR% --n dna.con --v 2 --f 3 --t dna --e MVPIndex --psm fft --dpm PivotWise --m 100 --init %Size% --F %Size% --s 2000 --frag 18 --res results_dna_build_%Size%i_%BuildMode%_fft_PivotWise_MVPIndex_v2_f3_m100_frag18_tR%tR%.txt --tR %tR% --sop %sop% --pbop %pbop%
if "%dataType%"=="peptide" set CurCMD=UMAD --runOption build --buildMode %BuildMode% --indexName peptide_%Size%_%BuildMode%_v2_f3_m100_tR%tR% --n nrall.fasta --v 2 --f 3 --t peptide --e MVPIndex --psm fft --dpm PivotWise --m 100 --init %Size% --F %Size% --s 2000 --frag 6 --res results_peptide_build_%Size%i_%BuildMode%_fft_PivotWise_MVPIndex_v2_f3_m100_frag6_tR%tR%.txt --tR %tR% --sop %sop% --pbop %pbop%


echo=>>Logs\Build.log
echo=>>Logs\Build.log
echo=>>Logs\Build.log

echo %CurCMD%>>Logs\BuildList.list

echo Current Task   : Build Index ( Size: %Size%, Build-Mode: %BuildMode% )
echo Current Task   : Build Index ( Size: %Size%, Build-Mode: %BuildMode% )>>Logs\Build.log

echo Current Command: "%CurCMD%"
echo Current Command: "%CurCMD%">>Logs\Build.log
echo=
echo=>>Logs\Build.log

echo Build Started on %time% %date%
echo Build Started on %time% %date%>>Logs\Build.log

echo Std Output:>>Logs\Build.log
echo ===========================================
echo ===========================================>>Logs\Build.log
echo=
echo=>>Logs\Build.log

cd /d ..\..\..\

echo Build Progressing......
..\Debug\%CurCMD%>>..\UMAD\SourceFiles\util\bat_windows\Logs\Build.log
set e=%ERRORLEVEL%

cd /d .\SourceFiles\util\bat_windows\

echo=
echo=>>Logs\Build.log
echo ===========================================
echo ===========================================>>Logs\Build.log
echo Build Ended on %time% %date%, Returned Code: %e% >>Logs\Build.log
echo Build Ended on %time% %date%, Returned Code: %e%

echo=>>Logs\Build.log
echo=>>Logs\Build.log
echo=
echo=
echo=