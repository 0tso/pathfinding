#ifndef BUCKET_QUEUE
#define BUCKET_QUEUE

#include <cstdint>
#include <cstring>
#include <cassert>

/**
 * Simple, crude bucket queue.
 * 
 * 
 * -----------------------------
 * Usage notes:
 *
 * Does not destruct objects!  Only use for "trivially copyable" types like int, POD, etc.
 * 
 * Assumes that the key values never decrease, only increase (between rounds of generating successors)
 * 
 * Values can only be added within the interval [curr_value, curr_value + interval * bucket_amount]
 * This interval potentially increases with every call to update_write().
 * Designed for A* with a heuristic like octile distance.
 * Be careful that your heuristic doesn't ever decrease more than the movement cost changes.
 * 
 * After every round of successor-generating, call update_write().
 * 
 * 
 * -----------------------------
 * Implementation notes:
 * 
 * The queue is a circular buffer of buckets.
 * 
 * Each bucket contains a circular buffer of Elements.
 * The BucketHeader of the circular buffer is at the memory location "index * internal_bucket_size".
 * The Elements start after it at "index * internal_bucket_size + sizeof(BucketHeader)".
 * To get the first Element:      "index * internal_bucket_size + sizeof(BucketHeader) + header->read * sizeof(Element)"
 * 
 * If a single bucket exceeds capacity, the whole ring buffer is reallocated and copied to a new location.
 * In that case, every bucket size is doubled.
 * 
 * Each insertion into the queue is based on two variables:
 *  - curr_value = the basis from which to calculate the index, i.e. the lowest value in the queue
 *  - write_curr = the queue's circular buffer's write, i.e. offset to indices
 * When an Element is inserted, the index is calculated like so:
 *  - index = (write_curr + (uint)(value / interval)) % bucket_amount
 * 
 * When update_write() is called, these two values are updated.
 *  -> write_curr matches with the lowest nonempty bucket (read_curr)
 *  -> curr_value matches with the value of this bucket (if empty, set to FLOAT_INFINITY)
 * It cannot be automatically called after every single pop() internally because after popping, the same value can be added back.
 * Therefore call update_write() only after you're sure no smaller values than the currently smallest value will be added.
 * 
 * When pop() is called:
 * read_curr holds the current lowest bucket. Remove one Element from that circular buffer.
 * If the current bucket is now empty, increment read_curr until a nonempty bucket is found.
 * Due to this, the read_curr can also be decremented if a value is added to a lower bucket later, before a call to update_write().
 * 
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
        this->read_curr = 0;
        this->write_curr = 0;
        this->curr_value = std::numeric_limits<float>::infinity();

        buffer.reserve(bucket_amount * internal_bucket_size);
        for(int i = 0; i < bucket_amount; ++i)
        {
            new (at(i)) BucketHeader{};
        }
    }

    /**
     * Checks if the queue is empty.
     */
    bool empty()
    {
        return empty(front());
    }

    Element top()
    {
        assert(!empty());
        return read(front());
    }

    Element pop()
    {
        assert(!empty());

        BucketHeader* header = front();
        Element* f           = read(header);

        header->read++;
        mod_check(bucket_size, header->read);

        if(header->read == header->write)
        {
            header->empty = true;

            auto begin = read_curr;
            while(empty(header))
            {
                read_curr++;
                mod_check(bucket_amount, read_curr);
                if(read_curr == begin)
                {
                    break;
                }
                
                header = front();
            }
        }
        
        return *f;
    }

    void push(float value, T object)
    {
        if(curr_value == std::numeric_limits<float>::infinity())
        {
            curr_value = value;
        }

        assert(value >= curr_value - 0.01f);

        uint32_t index = (write_curr + (uint32_t)((value - curr_value) / interval)) % bucket_amount;

        BucketHeader* header = at(index);
        if(!full(header))
        {
            new (write(header)) Element{value, object};

            header->empty = false;
            header->write++;
            mod_check(bucket_size, header->write);

            if(empty())
            {
                read_curr = index;
            }
            else
            if(mod_diff(bucket_amount, write_curr, index) < mod_diff(bucket_amount, write_curr, read_curr))
            {
                // read_curr is ahead of this new lowest bucket, so pull back

                read_curr = index;
            }
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

                new_h->empty = old_h->empty;
                new_h->read  = 0;
                new_h->write = size(old_h);
                
                if(old_h->write > old_h->read)
                {
                    std::memcpy(read(new_h), read(old_h), size(old_h) * sizeof(Element));
                }
                else if(size(old_h) > 0)
                {
                    uint32_t before_end = bucket_size - old_h->read;
                    uint32_t from_zero  = size(old_h) - before_end;

                    std::memcpy(read(new_h),                 read(old_h),    before_end * sizeof(Element));
                    std::memcpy(read(new_h) + before_end,    old_h + 1,      from_zero  * sizeof(Element));
                }
            }

            buffer.swap(new_buffer);
            bucket_size = new_bucket_size;
            internal_bucket_size = new_internal_bucket_size;

            push(value, object);
        }
    }

    void update_write()
    {
        if(empty())
        {
            curr_value = std::numeric_limits<float>::infinity();
            write_curr = read_curr;
        }
        else
        {
            curr_value += interval * mod_diff(bucket_amount, write_curr, read_curr);
            write_curr = read_curr;
        }
    }

    void clear()
    {
        for(int i = 0; i < bucket_amount; ++i)
        {
            new (at(i)) BucketHeader{};
        }
        curr_value = std::numeric_limits<float>::infinity();
    }

    int get_realloc_amount()
    {
        return realloc_amount;
    }

    float get_curr_value() { return curr_value; }

private:

    struct BucketHeader
    {
        uint32_t read  = 0;
        uint32_t write = 0;
        bool empty     = true;
    };

    inline BucketHeader* at(uint32_t index)
    {
        return (BucketHeader*)&buffer[index * internal_bucket_size];
    }

    inline BucketHeader* front()
    {
        return at(read_curr);
    }

    inline uint32_t size(BucketHeader* b)
    {
        if(empty(b))
            return 0;

        auto diff = mod_diff(bucket_size, b->read, b->write);

        if(diff == 0)
            return bucket_size;
        else
            return diff;
    }

    inline bool empty(BucketHeader* b)
    {
        return b->empty;
    }

    inline bool full(BucketHeader* b)
    {
        return b->read == b->write && !b->empty;
    }

    inline Element* read(BucketHeader* b)
    {
        return (Element*)(b + 1) + b->read;
    }

    inline Element* write(BucketHeader* b)
    {
        return (Element*)(b + 1) + b->write;
    }

    /**
     * Ring buffer check.
     */
    inline void mod_check(uint32_t size, uint32_t& val)
    {
        if(val == size)
            val = 0;
    }

    /**
     * (Modular arithmetic) distance of (b - a)
     * Distance in a ring buffer.
     */
    inline int mod_diff(uint32_t size, uint32_t a, uint32_t b)
    {
        if(b >= a)
            return b - a;
        else
            return size - a + b;
    }

    float curr_value = 0.0f;
    uint32_t read_curr = 0;
    uint32_t write_curr = 0;

    float interval;
    uint32_t bucket_size;
    uint32_t internal_bucket_size;
    uint32_t bucket_amount;

    std::vector<char> buffer;
    int realloc_amount = 0;
};

#endif