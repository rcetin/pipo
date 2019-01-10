# Pipo

**Pipo** is serial port application. It is designed to facilitate serial port monitoring. Also, it can automate sending sequences.

# Build
To build application, you need **qmake**. 

    sudo apt install qt5-default libqt5serialport5-dev
After installing necessary packets, **pipo** can be built.

    cd pipo
    mkdir build
    qmake ../pipo.pro
    make

That's all! Now you can use **pipo**.

    sudo ./pipo

 

