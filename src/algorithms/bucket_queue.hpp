#ifndef BUCKET_QUEUE
#define BUCKET_QUEUE

#include <cstdint>
#include <cstring>
#include <cassert>

/**
 * Simple, crude bucket queue.
 * Does not destruct objects!  Only use for "trivially copyable" types like int, POD, etc.
 * 
 * Assumes that the key values never decrease, only increase.
 * 
 * Values can only be added within the interval [curr_value, curr_value + interval * bucket_amount]
 * This interval increases every time a bucket is depleted.
 * Designed for A* with a heuristic like octile_distance.
 * 
 */
template<typename T>
class BucketQueue
{
public:
    struct Element
    {
        float value;
        T object;

        bool operator==(const Element&) const = default;
        bool operator!=(const Element&) const = default;
    };

    BucketQueue() {}
    BucketQueue(float interval, uint32_t bucket_amount, uint32_t bucket_size)
    {
        this->interval = interval;
        this->bucket_amount = bucket_amount;
        this->bucket_size = bucket_size;
        this->internal_bucket_size = (this->bucket_size * sizeof(Element)) + sizeof(BucketHeader);
        this->curr = 0;
        this->curr_value = 0.0f;

        buffer.reserve(bucket_amount * internal_bucket_size);
        for(int i = 0; i < bucket_amount; ++i)
        {
            new (at(i)) BucketHeader{};
        }
    }

    bool empty()
    {
        return empty(front());
    }

    Element top()
    {
        assert(!empty());
        return *last(front());
    }

    Element pop()
    {
        assert(!empty());

        BucketHeader* header = front();
        Element* f           = last(header);

        header->size--;

        auto begin = curr;
        while(empty(header))
        {
            curr_value += interval;
            curr++;
            check_mod(bucket_amount, curr);
            if(curr == begin)
                break;
            
            header = front();
        }
        
        return *f;
    }

    void push(float value, T object)
    {
        if(empty())
        {
            curr_value = value;
        }

        assert(value >= curr_value - 0.01f);

        uint32_t index = (curr + (uint32_t)((value - curr_value) / interval)) % bucket_amount;

        BucketHeader* header = at(index);
        if(!full(header))
        {
            new (last_empty(header)) Element{value, object};
            header->size++;
        }
        else
        {
            // Reallocate
            realloc_amount++;

            uint32_t new_bucket_size = bucket_size * 2;
            uint32_t new_internal_bucket_size = (new_bucket_size * sizeof(Element)) + sizeof(BucketHeader);
            std::vector<char> new_buffer;
            new_buffer.reserve(new_internal_bucket_size * bucket_amount);

            for(int i = 0; i < bucket_amount; ++i)
            {
                BucketHeader* old_h = at(i);
                BucketHeader* new_h = (BucketHeader*)&new_buffer[i * new_internal_bucket_size];
                new_h->size = old_h->size;
                memcpy(new_h + 1, old_h + 1, old_h->size * sizeof(Element));
            }

            buffer.swap(new_buffer);
            bucket_size = new_bucket_size;
            internal_bucket_size = new_internal_bucket_size;

            push(value, object);
        }
    }

    int get_realloc_amount()
    {
        return realloc_amount;
    }

private:

    struct BucketHeader
    {
        uint32_t size = 0;
    };

    inline BucketHeader* at(uint32_t index)
    {
        return (BucketHeader*)&buffer[index * internal_bucket_size];
    }

    inline BucketHeader* front()
    {
        return at(curr);
    }

    inline bool empty(BucketHeader* b)
    {
        return b->size == 0;
    }

    inline bool full(BucketHeader* b)
    {
        return b->size == bucket_size;
    }

    inline Element* last(BucketHeader* b)
    {
        return (Element*)(b + 1) + b->size - 1;
    }

    inline Element* last_empty(BucketHeader* b)
    {
        return (Element*)(b + 1) + b->size;
    }

    inline void check_mod(uint32_t size, uint32_t& val)
    {
        if(val == size)
            val = 0;
    }

    float curr_value = 0.0f;
    uint32_t curr = 0;

    float interval;
    uint32_t bucket_size;
    uint32_t internal_bucket_size;
    uint32_t bucket_amount;

    std::vector<char> buffer;
    int realloc_amount = 0;
};

#endif