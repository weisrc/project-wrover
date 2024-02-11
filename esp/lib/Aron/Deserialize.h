#pragma once
#include "BaseAron.h"

class Deserializer : virtual public Aron
{
public:
    void number(int &data) override { _number(data); }
    void number(short &data) override { _number(data); }
    void number(char &data) override { _number(data); }

    void array() override
    {
        seperate();
        expect('[');
        firstItem = true;
    }

    virtual bool endArray() override
    {
        if (read() != ']')
            return false;
        return true;
    }

    void string(char *&data) override
    {
        bool escape = false;
        seperate();
        expect('"');
        data = at(position);

        size_t start = position;
        size_t end = position;

        while (true)
        {
            char c = read();
            if (escape)
                escape = false;
            else if (c == '\\')
                escape = true;
            else if (c == '"')
                break;
        }

        escape = false;
        end = position - 1;
        position = start;

        while (true)
        {
            char c = read();
            if (escape)
            {
                switch (c)
                {
                case 'n':
                    c = '\n';
                    break;
                case 't':
                    c = '\t';
                    break;
                case '"':
                    c = '"';
                    break;
                case '\\':
                    c = '\\';
                    break;
                }
                *at(position - 1) = c;
                int current = position;
                while (current < end)
                {
                    *at(current) = *at(current + 1);
                    current++;
                }
                end--;
                escape = false;
            }
            else if (c == '\\')
                escape = true;
            else if (c == '"')
            {
                *at(position) = 0;
                break;
            }
        }
    }

protected:
    virtual char *at(size_t position) = 0;

private:
    char read()
    {
        return *at(position++);
    }

    void expect(char c)
    {
        if (read() != c)
            throw SD_UNEXPECTED;
    }
    void seperate()
    {
        if (!firstItem)
            expect(',');
        firstItem = false;
    }
    template <typename T>
    void _number(T &data)
    {
        char c;
        seperate();
        size_t start = position;
        while (true)
        {
            c = read();
            if (c == ',' || c == ']')
            {
                *at(position - 1) = 0;
                break;
            }
        }
        std::sscanf(at(start), "%f", &data);
        position--;
        *at(position) = c;
    }
};