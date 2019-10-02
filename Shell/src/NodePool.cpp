#include <iomanip>
#include <iostream>
#include "Terminal.h"
#include "NodePool.h"

namespace RhIO
{
    NodePool::NodePool()
        : dirty(false), handler(PoolUpdateHandler()), timestamp(0)
    {
    }
            
    void NodePool::setCallback(PoolUpdateHandler handler_)
    {
        handler = handler_;
    }

    void NodePool::update()
    {
        if (handler) {
            handler(this);
        }
        dirty = false;
    }

    void NodePool::draw(bool fullName)
    {
        for (auto nodeVal : *this) {
            auto val = nodeVal.value;

            std::cout << std::left;
            Terminal::setColor("white", val->persisted);
            if (fullName) {
                std::cout << std::setw(45) << nodeVal.getName();
            } else {
                std::cout << std::setw(30) << nodeVal.value->name;
            }
            std::cout << " ";

            Terminal::setColor("magenta", false);
            std::cout << std::setw(7) << Node::getType(val);
            Terminal::clear();

            std::cout << std::setw(17) << Node::toString(val);

            std::cout << " ";
            Terminal::setColor("magenta", false);
            std::cout << std::setw(30) << val->comment;
            Terminal::clear();

            if (val->persisted) {
                Terminal::setColor("magenta", false);
                std::cout << " persisted: ";
                Terminal::clear();
                std::cout << Node::persistedToString(val);
            }
            std::cout << std::endl;
        }
    }
}
