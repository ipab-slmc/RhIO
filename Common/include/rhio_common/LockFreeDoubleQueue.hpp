#ifndef RHIO_LOCKFREEDOUBLEQUEUE_HPP
#define RHIO_LOCKFREEDOUBLEQUEUE_HPP

#include <vector>
#include <atomic>

namespace RhIO {

/**
 * LockFreeDoubleQueue
 *
 * Multiple RealTime (RT) writers appending to 
 * a double buffer while a non RT reader 
 * is accessing the second buffer.
 * The writer operation is lock free with
 * no dynamic allocation (with potentially exceptions
 * due to given template type).
 */
template <typename T>
class LockFreeDoubleQueue
{
    public:

        /**
         * Initialization with maximum 
         * pre-allocated queue buffer size
         */
        LockFreeDoubleQueue(size_t maxSize) :
            _maxSize(maxSize),
            _state1(true),
            _state2(false),
            _statePtr(nullptr)
        {
            //State initialization
            _statePtr.store(&_state1);
            //Memory pre-allocation
            _state1.buffer.resize(_maxSize);
            _state2.buffer.resize(_maxSize);
        }

        /**
         * Swap reading and writing internal buffer.
         * Wait for the writers to complete the writing.
         */
        void swapBufferFromReader()
        {
            //Retrieve writing buffer
            State_t* stateOld = _statePtr.load();
            //Reset reading buffer size
            if (stateOld->isWritingTo1) {
                _state2.length.store(0);
            } else {
                _state1.length.store(0);
            }
            //Atomically inverse the flag
            if (stateOld->isWritingTo1) {
                _statePtr.store(&_state2);
            } else {
                _statePtr.store(&_state1);
            }
            //Actively wait that all writer complete 
            //writing to old writing buffer
            while (stateOld->stillWriting.load() != 0) {
                //Wait
            }
        }

        /**
         * Direct access to reading (privately own) buffer
         */
        const std::vector<T>& getBufferFromReader() const
        {
            if (_statePtr.load()->isWritingTo1) {
                return _state2.buffer;
            } else {
                return _state1.buffer;
            }
        }

        /**
         * Return the size of currently own by reader buffer
         */
        size_t getSizeFromReader() const
        {
            size_t length;
            if (_statePtr.load()->isWritingTo1) {
                length = _state2.length.load();
            } else {
                length = _state1.length.load();
            }
            
            //Shrink actual size to maximum size since writers 
            //increase length before checking for length limit
            if (length > _maxSize) {
                length = _maxSize;
            }
            return length;
        }

        /**
         * Append to internal queue the given value.
         * RT, thread safe, no dynamic allocation.
         * If internal buffer is full, given data is dropped.
         * Return false if the value has to be dropped.
         */
        bool appendFromWriter(const T& val)
        {
            bool isOk = false;
            //Retrieve current writting buffer state
            State_t* state = _statePtr.load();
            //Take writing lock on buffer (wait in swapBuffer)
            state->stillWriting.fetch_add(1);
            //Retrieve next to write index and
            //atomically increase it (after reading)
            size_t index = state->length.fetch_add(1);
            //Write to buffer of not full
            if (index < _maxSize) {
                state->buffer[index] = val;
                isOk = true;
            }
            //Release the lock
            state->stillWriting.fetch_sub(1);

            return isOk;
        }

    private:

        /**
         * Structure for buffer state
         */
        struct State_t {
            /**
             * If true, the buffer 1 is 
             * currently used to write on it.
             * Else, buffer 2 is used.
             */
            const bool isWritingTo1;
            /**
             * Hold the number of writers
             * currently writing to the buffer
             */
            std::atomic<int> stillWriting;
            /**
             * Index of the next available 
             * writable cell in the buffer
             */
            std::atomic<size_t> length;
            /**
             * Internal pre-allocated buffers
             */
            std::vector<T> buffer;

            /**
             * Initialization
             */
            State_t(bool isWritting) :
                isWritingTo1(isWritting),
                stillWriting(0),
                length(0),
                buffer()
            {
            }
        };
        
        /**
         * Maximum data contained by each buffers.
         * Use for pre-dynamic allocation.
         */
        const size_t _maxSize;

        /**
         * State for double buffer 1 and 2
         */
        State_t _state1;
        State_t _state2;

        /**
         * Pointer toward the state of the
         * buffer currently used for writting
         */
        std::atomic<State_t*> _statePtr;
};

}

#endif

