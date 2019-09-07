s="[Desktop Entry]\nName=海天鹰FTP\nComment=一个极简的FTP客户端\nExec=`pwd`/HTYFTP\nIcon=`pwd`/HTYFTP.png\nPath=`pwd`\nTerminal=false\nType=Application\nCategories=Net;"
echo -e $s > HTYFTP.desktop
cp `pwd`/HTYFTP.desktop ~/.local/share/applications/HTYFTP.desktop