if "%classifyMethod%" == "knn" for /f "eol=; tokens=1 delims= " %%k in (kNum.options) do set K=%%k& call TrainModel_knn.cmd

if not "%classifyMethod%" == "knn" call TrainModel_others.cmd