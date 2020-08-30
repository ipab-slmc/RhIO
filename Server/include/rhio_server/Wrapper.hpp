#ifndef RHIO_WRAPPER_HPP
#define RHIO_WRAPPER_HPP

#include <vector>
#include <string>
#include <stdexcept>
#include "RhIO.hpp"
#include "rhio_common/Value.hpp"
#include "rhio_server/ValueNode.hpp"

namespace RhIO {

/**
 * Wrapper
 *
 * Object wrapper around an existing value 
 * used to speed up real time calls 
 * since no string name is needed.
 */
class WrapperBool
{
    public:
        
        /**
         * Default uninitialized
         */
        WrapperBool() :
            _node(nullptr),
            _ptrValue(nullptr)
        {
        }

        /**
         * Initialize with a RhIO node and the name of 
         * an existing boolean value stored within it
         */
        WrapperBool(
            IONode& node,
            const std::string& name) :
            _node(&node),
            _ptrValue(&(node.accessValueBool(name)))
        {
        }

        /**
         * Bind the wrapper to given RhIO node and given
         * name of existing boolean stored within it.
         * Only if the wrapper is still uninitialized.
         */
        void bind(
            IONode& node,
            const std::string& name)
        {
            if (_node != nullptr || _ptrValue != nullptr) {
                throw std::logic_error(
                    "RhIO::WrapperBool::bind: "
                    "Wrapper already assigned");
            }
            _node = &node;
            _ptrValue = &(node.accessValueBool(name));
        }

        /**
         * Return the boolean value
         */
        double get() const
        {
            if (_ptrValue == nullptr) {
                throw std::logic_error(
                    "RhIO::WrapperBool::get: uninitialized");
            }
            return _ptrValue->value.load();
        }
        operator bool() const 
        { 
            return get(); 
        }

        /**
         * Set the boolean value with optional 
         * timestamp information
         */
        void set(
            bool val,
            int64_t timestamp = getRhIOTime())
        {
            if (_node == nullptr || _ptrValue == nullptr) {
                throw std::logic_error(
                    "RhIO::WrapperBool::set: uninitialized");
            }
            _node->assignRTBool(*_ptrValue, val, timestamp);
        }
        void operator=(bool val) 
        { 
            set(val);
        }

    private:

        /**
         * Pointer towards a node and a boolean value 
         * element stored in the map container.
         */
        IONode* _node;
        ValueBool* _ptrValue;
};
class WrapperInt
{
    public:
        
        /**
         * Default uninitialized
         */
        WrapperInt() :
            _node(nullptr),
            _ptrValue(nullptr)
        {
        }

        /**
         * Initialize with a RhIO node and the name of 
         * an existing int value stored within it
         */
        WrapperInt(
            IONode& node,
            const std::string& name) :
            _node(&node),
            _ptrValue(&(node.accessValueInt(name)))
        {
        }

        /**
         * Bind the wrapper to given RhIO node and given
         * name of existing integer stored within it.
         * Only if the wrapper is still uninitialized.
         */
        void bind(
            IONode& node,
            const std::string& name)
        {
            if (_node != nullptr || _ptrValue != nullptr) {
                throw std::logic_error(
                    "RhIO::WrapperInt::bind: "
                    "Wrapper already assigned");
            }
            _node = &node;
            _ptrValue = &(node.accessValueInt(name));
        }

        /**
         * Return the integer value
         */
        double get() const
        {
            if (_ptrValue == nullptr) {
                throw std::logic_error(
                    "RhIO::WrapperInt::get: uninitialized");
            }
            return _ptrValue->value.load();
        }
        operator int64_t() const 
        { 
            return get(); 
        }
        operator int() const 
        { 
            return get(); 
        }

        /**
         * Set the integer value with optional 
         * timestamp information
         */
        void set(
            int64_t val,
            int64_t timestamp = getRhIOTime())
        {
            if (_node == nullptr || _ptrValue == nullptr) {
                throw std::logic_error(
                    "RhIO::WrapperInt::set: uninitialized");
            }
            _node->assignRTInt(*_ptrValue, val, timestamp);
        }
        void operator=(int64_t val) 
        { 
            set(val);
        }
        void operator=(int val) 
        { 
            set(val);
        }

    private:

        /**
         * Pointer towards a node and a integer value 
         * element stored in the map container.
         */
        IONode* _node;
        ValueInt* _ptrValue;
};
class WrapperFloat
{
    public:
        
        /**
         * Default uninitialized
         */
        WrapperFloat() :
            _node(nullptr),
            _ptrValue(nullptr)
        {
        }

        /**
         * Initialize with a RhIO node and the name of 
         * an existing float value stored within it
         */
        WrapperFloat(
            IONode& node,
            const std::string& name) :
            _node(&node),
            _ptrValue(&(node.accessValueFloat(name)))
        {
        }

        /**
         * Bind the wrapper to given RhIO node and given
         * name of existing float stored within it.
         * Only if the wrapper is still uninitialized.
         */
        void bind(
            IONode& node,
            const std::string& name)
        {
            if (_node != nullptr || _ptrValue != nullptr) {
                throw std::logic_error(
                    "RhIO::WrapperFloat::bind: "
                    "Wrapper already assigned");
            }
            _node = &node;
            _ptrValue = &(node.accessValueFloat(name));
        }

        /**
         * Return the float value
         */
        double get() const
        {
            if (_ptrValue == nullptr) {
                throw std::logic_error(
                    "RhIO::WrapperFloat::get: uninitialized");
            }
            return _ptrValue->value.load();
        }
        operator double() const 
        { 
            return get(); 
        }
        operator float() const 
        { 
            return get(); 
        }

        /**
         * Set the float value with optional 
         * timestamp information
         */
        void set(
            double val,
            int64_t timestamp = getRhIOTime())
        {
            if (_node == nullptr || _ptrValue == nullptr) {
                throw std::logic_error(
                    "RhIO::WrapperFloat::set: uninitialized");
            }
            _node->assignRTFloat(*_ptrValue, val, timestamp);
        }
        void operator=(double val) 
        { 
            set(val);
        }
        void operator=(float val) 
        { 
            set(val);
        }

    private:

        /**
         * Pointer towards a node and a float value 
         * element stored in the map container.
         */
        IONode* _node;
        ValueFloat* _ptrValue;
};

}

#endif

