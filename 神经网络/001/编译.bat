@echo off

rem utf-8
chcp 65001
echo=
echo=
echo=
gcc -c 00main.c -o objs/00main.obj
echo=
echo=
echo=
gcc -c neural/neural.c -o objs/neural.obj
echo=
echo=
echo=
gcc -c "neural/math base.c" -o "objs/math base.obj"
echo=
echo=
echo=
gcc -c neural/activate.c -o objs/activate.obj
echo=
echo=
echo=
gcc objs/*.obj -o main.exe
echo 编译完成！
pause
echo=
echo=
echo=
main.exe