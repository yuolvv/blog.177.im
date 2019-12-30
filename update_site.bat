::Update Site

set nowtime = %date:~0,4%-%date:~5,2%-%date:~8,2%_%time:~0,2%:%time:~3,2%:%time:~6,2%
echo nowtime

git pull

git add .
git commit -m "Update my site"%nowtime%
git push

pause
