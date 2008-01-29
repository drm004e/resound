#!/bin/bash
echo -e "\033[1mwx* directories in /usr/include:\033[0m"
ls -d1 /usr/include/wx*
echo -e "\n"

echo -e "\033[1mwx* directories in /usr/bin:\033[0m"
ls -d1 /usr/bin/wx*
echo -e "\n"

echo -e "\033[1mwx* directories in /usr/lib:\033[0m"
ls -d1 /usr/lib/wx*
echo -e "\n"

echo -e "\033[1mRecursive ls of /usr/lib/wx*\033[0m\n"
ls -R /usr/lib/wx*
echo -e "\n"

echo -e "\n\033[1mwx* directories in /usr/local/include:\033[0m"
ls -d1 /usr/local/include/wx*
echo -e "\n"

echo -e "\033[1mwx* directories in /usr/local/bin:\033[0m"
ls -d1 /usr/local/bin/wx*
echo -e "\n"

echo -e "\033[1mwx* directories in /usr/local/lib:\033[0m"
ls -d1 /usr/local/lib/wx*
echo -e "\n"

echo -e "\033[1mRecursive ls of /usr/local/lib/wx*:\033[0m\n"
ls -R /usr/local/lib/wx*
echo -e "\n"

echo -e "\033[1mContents of /usr/bin/resound*:\033[0m"
ls -d1 /usr/bin/resound*
echo -e "\n"

echo -e "\033[1mContents of /usr/share/resound*:\033[0m"
ls -d1 /usr/share/resound*
echo -e "\n"

echo -e "\033[1mContents of /usr/local/bin/resound*:\033[0m"
ls -d1 /usr/local/bin/resound*
echo -e "\n"

echo -e "\033[1mContents of /usr/local/share/resound*:\033[0m"
ls -d1 /usr/local/share/resound*
echo -e "\n"

echo -e "\033[1mOutput of ldd /usr/bin/resound_gui:\033[0m"
ldd /usr/bin/resound_gui
echo -e "\n"

echo -e "\033[1mOutput of ldd /usr/local/bin/resound_gui:\033[0m"
ldd /usr/local/bin/resound_gui
echo -e "\n"
