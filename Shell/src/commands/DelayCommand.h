#pragma once

#include "Command.h"

namespace RhIO
{
    class DelayCommand : public Command
    {
        public:
            virtual std::string getName();
            virtual std::string getDesc();
            virtual std::string getUsage();
            virtual void process(std::vector<std::string> args);
    };
}
