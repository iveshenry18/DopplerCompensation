//
// Created by Henry Ives on 4/19/24.
//
#include <errno.h> // Error number definitions
#include <fcntl.h> // File control definitions
#include <iostream>
#include <stdexcept>
#include <string>
#include <termios.h> // POSIX terminal control definitions
#include <unistd.h> // UNIX standard function definitions

#pragma once

class SerialPort
{
public:
    SerialPort (const std::string& portName)
    {
        openPort (portName);
        configurePort();
    }
    ~SerialPort()
    {
        close (fd);
    }

    void openPort (const std::string& portName)
    {
        fd = open (portName.c_str(), O_RDONLY | O_NOCTTY | O_NDELAY);
        if (fd == -1)
        {
            throw std::runtime_error ("open_port: Unable to open " + portName);
        }
        fcntl (fd, F_SETFL, 0); // Clear all flags on descriptor, enabling direct I/O
    }

    void configurePort()
    {
        struct termios tty;
        memset (&tty, 0, sizeof tty);

        if (tcgetattr (fd, &tty) != 0)
        {
            throw std::runtime_error ("init_serialport: Error from tcgetattr");
        }

        cfsetospeed (&tty, B9600); // Set baud rate
        cfsetispeed (&tty, B9600); // Set read rate

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK; // ignore break signal
        tty.c_lflag = 0; // no signaling chars, no echo,
            // no canonical processing
        tty.c_oflag = 0; // no remapping, no delays
        tty.c_cc[VMIN] = 0; // read doesn't block
        tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD); // ignore modem controls,
            // enable reading
        tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
        tty.c_cflag |= 0;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
            throw std::runtime_error ("init_serialport: Error from tcsetattr");
        }
    }

    /**
     * Read the most recent line (delimited by '\n') from the serial port
     *
     * If no data is available, returns an empty string
     */
    std::string readLastLine()
    {
        std::string line;
        char c;
        while (read (fd, &c, 1) > 0)
        {
            if (c == '\n')
            {
                return line;
            }
            line += c;
        }
        return line;
    }

private:
    int fd; // File descriptor for the port
};
