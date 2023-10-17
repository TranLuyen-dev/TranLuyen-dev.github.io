@echo off
set "par1=%~1"
goto :par1Check
:push
set /p "par1=Enter Name Commit: "
if "%par1%"=="" set par1='default_oke'
:par1Check
if "%par1%"=="" goto :push
git add .
git commit -m %par1%
git push