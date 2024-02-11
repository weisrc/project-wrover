#pragma once
#include <string.h>

enum SerdesMode
{
    SD_SERIALIZE,
    SD_DESERIALIZE,
    SD_UNEXPECTED,
    SD_OOM,
};

enum SerdesType
{
    SD_STOP,
    SD_BYTE = sizeof(char),
    SD_SHORT = sizeof(short),
    SD_START,
    SD_INT = sizeof(int)
};

/*
Serdes: Serialize and Deserialize
It is designed such that any receiver can determine when a message ends.
When deserializing, the strings are not copied. They remain in the buffer.
The structure is somewhat similar to JSON, but without records.
*/
class Serdes
{
protected:
    char *original;
    char *head;
    char *skip;
    char *limit;

    SerdesMode mode;
    SerdesType type;
    size_t level;

    void write(char c)
    {
        if (head < limit)
        {
            *head = c;
            head++;
        }
        else
        {
            mode = SD_OOM;
        }
    }

    char read()
    {
        char c;
        if (head < limit)
        {
            c = *head;
            head++;
        }
        else
        {
            mode = SD_OOM;
        }
        return c;
    }

    void write(void *data, size_t size)
    {
        if (head + size < limit)
        {
            memcpy(head, data, size);
            head += size;
        }
        else
        {
            mode = SD_OOM;
        }
    }

    void read(void *data, size_t size)
    {
        if (head + size < limit)
        {
            memcpy(data, head, size);
            head += size;
        }
        else
        {
            mode = SD_OOM;
        }
    }

    char expect(SerdesType t)
    {
        char c = read();
        if (c != t)
            mode = SD_UNEXPECTED;
        return c == t;
    }

public:
    /*
    @param data the buffer, its lifetime must superseed the Serdes instance
    */
    Serdes(char *data, size_t size)
    {
        original = data;
        limit = data + size;
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
        skip = original;
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
                write(*(data++));
            stop();
        }
        else if (mode == SD_DESERIALIZE)
        {
            if (!expect(SD_START))
                return;
            data = head;
            while (read() != SD_STOP)
                continue;
        }
    }

    // Serialize or deserialize a value (char, short, int, long, float, double)
    template <typename T>
    void value(T &data)
    {
        if (mode == SD_SERIALIZE)
        {
            write(sizeof(T));
            write(&data, sizeof(T));
        }
        else if (mode == SD_DESERIALIZE)
        {
            if (!expect((SerdesType)sizeof(T)))
                return;

            read(&data, sizeof(T));
        }
    }

    /*
    Start a new container or expect one if deserializing.
    */
    void start()
    {
        if (mode == SD_SERIALIZE)
            write(SD_START);

        else if (mode == SD_DESERIALIZE)
        {
            if (!expect(SD_START))
                return;
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
            write(SD_STOP);
            return true;
        }
        else if (mode == SD_DESERIALIZE)
        {
            if (*head == SD_STOP) {
                head++;
                return true;
            }

            if (withError)
                mode = SD_UNEXPECTED;
            return false;
        }
    }

    /*
    Ok checks if the buffer is valid
    @return true if the buffer is valid
    */
    bool ok()
    {
        return mode != SD_UNEXPECTED && mode != SD_OOM;
    }

    /*
    Append a character to the buffer
    @param c the character to append
    @return true if the buffer is ready to be deserialized
    */
    bool append(char c)
    {

        if (mode != SD_DESERIALIZE)
            return false;

        write(c);

        switch (type)
        {
        case SD_START:
            switch (c)
            {
            case SD_BYTE:
            case SD_SHORT:
            case SD_INT:
                type = (SerdesType)c;
                skip = head + c;
                break;
            case SD_START:
                level++;
                break;
            case SD_STOP:
                level--;
                if (level == 0)
                {
                    reset();
                    return true;
                }
                break;
            }
            break;

        case SD_BYTE:
        case SD_SHORT:
        case SD_INT:
            if (head != skip)
                return false;
            if (level == 0)
            {
                reset();
                return true;
            }
            type = SD_START;
            break;
        }

        return false;
    }
};
