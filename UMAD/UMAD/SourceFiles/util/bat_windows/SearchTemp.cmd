set sop=Sequence
set pbop=Balance
if "%pbop%" == "Trisection" for /f "eol=; tokens=1 delims= " %%t in (radiusList/radiusList_%dataType%) do set tR=%%t& call SingleSearch_Trisection.cmd

if not "%pbop%" == "Trisection" call SingleSearch.cmd