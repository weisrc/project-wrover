#pragma once
#include <string.h>

enum SerdesMode
{
    SD_SERIALIZE,
    SD_DESERIALIZE,
    SD_ERROR
};

enum SerdesType
{
    SD_STOP,
    SD_BYTE = sizeof(char),
    SD_SHORT = sizeof(short),
    SD_START,
    SD_INT = sizeof(int)
};

class Serdes
{
private:
    char *original;
    char *head;
    char *limit;

    SerdesMode mode;
    SerdesType type;
    size_t level;

public:
    Serdes(char *data)
    {
        original = data;
        reset();
    }

    size_t size()
    {
        return head - original;
    }

    void serialize()
    {
        mode = SD_SERIALIZE;
        reset();
    }

    void deserialize()
    {
        mode = SD_DESERIALIZE;
        reset();
    }

    void reset()
    {
        head = original;
        limit = original;
        type = SD_START;
        level = 0;
    }

    void string(char *&data)
    {
        if (mode == SD_SERIALIZE)
        {
            start();
            while (*data)
            {
                *head = *data;
                head++;
                data++;
            }
            stop();
        }
        else if (mode == SD_DESERIALIZE)
        {
            if (*head != SD_START)
            {
                mode = SD_ERROR;
                return;
            }

            head++;
            data = head;
            while (*head != SD_STOP)
                head++;
            head++;
        }
    }

    template <typename T>
    void value(T &data)
    {
        if (mode == SD_SERIALIZE)
        {
            *head = sizeof(T);
            head++;
            memcpy(head, &data, sizeof(T));
            head += sizeof(T);
        }
        else if (mode == SD_DESERIALIZE)
        {
            if (*head != sizeof(T))
            {
                mode = SD_ERROR;
                return;
            }

            head++;
            memcpy(&data, head, sizeof(T));
            head += sizeof(T);
        }
    }

    void start()
    {
        if (mode == SD_SERIALIZE)
        {
            *head = SD_START;
            head++;
        }
        else if (mode == SD_DESERIALIZE)
        {
            if (*head == SD_START)
                head++;
            else
                mode = SD_ERROR;
        }
    }

    bool stop(bool withError = false)
    {
        if (mode == SD_SERIALIZE)
        {
            *head = SD_STOP;
            head++;
            return true;
        }
        else if (mode == SD_DESERIALIZE)
        {
            if (*head == SD_STOP)
            {
                head++;
                return true;
            }
            if (withError)
                mode = SD_ERROR;
            return false;
        }
    }

    bool shouldEndArray()
    {
        return *head == SD_STOP;
    }

    bool ok()
    {
        return mode != SD_ERROR;
    }

    bool append(char c)
    {

        *head = c;
        head++;

        switch (type)
        {
        case SD_START:
            switch (c)
            {
            case SD_BYTE:
            case SD_SHORT:
            case SD_INT:
                type = (SerdesType)c;
                limit = head + c;
                break;
            case SD_START:
                level++;
                break;
            case SD_STOP:
                if (level == 0)
                {
                    deserialize();
                    return true;
                }
                level--;
                break;
            }
            break;

        case SD_BYTE:
        case SD_SHORT:
        case SD_INT:
            if (head != limit)
                return false;
            if (level == 0)
            {
                deserialize();
                return true;
            }
            type = SD_START;
            break;
        }

        return false;
    }
};
