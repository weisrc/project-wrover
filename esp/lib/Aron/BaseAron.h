#pragma once
#include <string>

enum AronError
{
    SD_UNEXPECTED,
    SD_OOM,
};

class Aron
{
public:
    virtual void string(char *&data) = 0;
    
    virtual void number(int &data) = 0;
    virtual void number(short &data) = 0;
    virtual void number(char &data) = 0;

    virtual void boolean(bool &data) = 0;

    virtual void array() = 0;
    virtual bool endArray() = 0;

    size_t size()
    {
        return position;
    }

    void init() {
        firstItem = false;
        level = 0;
        position = 0;
    }

protected:
    bool firstItem;
    size_t level;
    size_t position;
    virtual void add(char c) = 0;

    void write(char c)
    {
        add(c);
        position++;
    }
};