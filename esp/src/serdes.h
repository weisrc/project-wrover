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
    /*
    @param data the buffer, its lifetime must superseed the Serdes instance
    */
    Serdes(char *data)
    {
        original = data;
        reset();
    }

    // Get the size of the buffer
    size_t size()
    {
        return head - original;
    }

    // Set the mode to serialize
    void serialize()
    {
        mode = SD_SERIALIZE;
        reset();
    }

    // Set the mode to deserialize
    void deserialize()
    {
        mode = SD_DESERIALIZE;
        reset();
    }

    // Reset the states
    void reset()
    {
        head = original;
        limit = original;
        type = SD_START;
        level = 0;
    }

    // Serialize or deserialize a cstring.
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

    // Serialize or deserialize a value (char, short, int, long, float, double)
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

    /*
    Start a new container or expect one if deserializing.
    */
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

    /*
    Stop the current container or expect it to end if deserializing.
    @param withError if true, the buffer will be marked as invalid
    */
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

    /* 
    Ok checks if the buffer is valid
    @return true if the buffer is valid
    */
    bool ok()
    {
        return mode != SD_ERROR;
    }

    /* 
    Append a character to the buffer
    @param c the character to append
    @return true if the buffer is ready to be deserialized
    */
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
