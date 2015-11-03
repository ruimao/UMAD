set lastQuery=1000

if "%dataType%"=="vector" set CurCMD=UMAD --runOption search --searchMode %SearchMode% --indexName vector_%Size%_%BuildMode%_v2_f3_m100 --n uniformvector-20dim-1m.txt --t vector --dim 6 --q uniformvector-20dim-1m.txt --fq 0 --l %lastQuery% --a 0.4 --i 0 --y 0.05 --ver 0 --init %Size% --buildMode %BuildMode% --res results_search_vector_datasize_%Size%_buildMode_%BuildMode%_searchMode_%SearchMode%.txt
if "%dataType%"=="string" set CurCMD=UMAD --runOption search --searchMode %SearchMode% --indexName string_%Size%_%BuildMode%_v2_f3_m100 --n English.dic --t string --dim 2 --q English.dic --fq 0 --l %lastQuery% --a 3 --i 0 --y 0.5 --ver 0 --init %Size% --buildMode %BuildMode% --res results_search_string_datasize_%Size%_buildMode_%BuildMode%_searchMode_%SearchMode%.txt
if "%dataType%"=="image" set CurCMD=UMAD --runOption search --searchMode %SearchMode% --indexName image_%Size%_%BuildMode%_v2_f3_m100 --n allfeas.dat --t image --dim 66 --q allfeas.dat --fq 0 --l %lastQuery% --a 0.3 --i 0 --y 0.05 --ver 0 --init %Size% --buildMode %BuildMode% --res results_search_image_datasize_%Size%_buildMode_%BuildMode%_searchMode_%SearchMode%.txt
if "%dataType%"=="dna" set CurCMD=UMAD --runOption search --searchMode %SearchMode% --indexName dna_%Size%_%BuildMode%_v2_f3_m100 --n dna.con --t dna --frag 18 --q dna.con --fq 0 --l %lastQuery% --a 6 --i 0 --y 1 --ver 0 --init %Size% --buildMode %BuildMode% --res results_search_dna_datasize_%Size%_buildMode_%BuildMode%_searchMode_%SearchMode%.txt
if "%dataType%"=="peptide" set CurCMD=UMAD --runOption search --searchMode %SearchMode% --indexName peptide_%Size%_%BuildMode%_v2_f3_m100 --n nrall.fasta --t peptide --frag 6 --q nrall.fasta --fq 0 --l %lastQuery% --a 6 --i 0 --y 1 --ver 0 --init %Size% --buildMode %BuildMode% --res results_search_peptide_datasize_%Size%_buildMode_%BuildMode%_searchMode_%SearchMode%.txt

echo=>>Logs\Search.log
echo=>>Logs\Search.log
echo=>>Logs\Search.log

echo %CurCMD%>>Logs\SearchList.list

echo Current Task   : Search Index ( Size: %Size%, Search-Mode: %SearchMode%, Built-Mode: %BuildMode% )
echo Current Task   : Search Index ( Size: %Size%, Search-Mode: %SearchMode%, Built-Mode: %BuildMode% )>>Logs\Search.log

echo Current Command: "%CurCMD%"
echo Current Command: "%CurCMD%">>Logs\Search.log
echo=
echo=>>Logs\Search.log

echo Search Started on %time% %date%
echo Search Started on %time% %date%>>Logs\Search.log

echo Std Output:>>Logs\Search.log
echo ===========================================
echo ===========================================>>Logs\Search.log
echo=
echo=>>Logs\Search.log

cd /d ..\..\..\

echo Search Progressing......
..\Debug\%CurCMD%>>..\UMAD\SourceFiles\util\bat_windows\Logs\Search.log
set e=%ERRORLEVEL%

cd /d .\SourceFiles\util\bat_windows\

echo=
echo=>>Logs\Search.log
echo ===========================================
echo ===========================================>>Logs\Search.log
echo Search Ended on %time% %date%, Returned Code: %e% >>Logs\Search.log
echo Search Ended on %time% %date%, Returned Code: %e%

echo=>>Logs\Search.log
echo=>>Logs\Search.log
echo=
echo=
echo=