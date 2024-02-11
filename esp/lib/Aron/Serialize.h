#pragma once

#include "BaseAron.h"

class Serializer : virtual public Aron
{

public:
    void string(char *&data) override
    {
        seperate();
        write('"');
        while (*data)
        {
            switch (*data)
            {
            case '\t':
                write('\\');
                write('t');
                break;
            case '"':
                write('\\');
                write('"');
                break;
            case '\\':
                write('\\');
                write('\\');
                break;
            default:
                write(*data);
            }
        }
        write('"');
    }

    void number(int &data) override { _number(data); }
    void number(short &data) override { _number(data); }
    void number(char &data) override { _number(data); }

    void boolean(bool &data) override
    {
        seperate();
        if (data)
        {
            write('t');
            write('r');
            write('u');
            write('e');
        }
        else
        {
            write('f');
            write('a');
            write('l');
            write('s');
            write('e');
        }
    }

    void array() override
    {
        seperate();
        write('[');
        level++;
        firstItem = true;
    }

    bool endArray()
    {
        write(']');
        return true;
    }

private:
    template <typename T>
    void _number(T &data)
    {
        seperate();
        std::string str = std::to_string(data);
        for (char c : str)
            write(c);
    }

    virtual void seperate()
    {
        if (!firstItem)
            write(',');
        firstItem = false;
    }
};