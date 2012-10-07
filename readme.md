To compile the file you need to have installed essential gtk development files
After you install gtk development files type following command to compile
gcc main.c `pkg-config --cflags --libs gtk+-3.0` -o <object name>

gtk+-3.0 may vary depending on the version installed in your system.
