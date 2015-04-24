#include <sstream>
#include <iostream>
#include "Shell.h"
#include "LsCommand.h"
#include "Node.h"
#include "NodePool.h"

namespace RhIO
{
    std::string LsCommand::getName()
    {
        return "ls";
    }

    std::string LsCommand::getDesc()
    {
        return "Lists the remote entries";
    }

    void LsCommand::process(std::vector<std::string> args)
    {
        auto node = getNode(args);

        // Listing sub directories
        Terminal::setColor("blue", true);
        for (auto child : node->getChildren()) {
            std::cout << child.first;
            std::cout << "/" << std::endl;
        }
        Terminal::clear();

        // Listing commands
        /*
        for (auto command : node->getCommands()) {
            Terminal::setColor("green", true);
            printf("%-23s", command.c_str());
        
            if (auto shellCommand = shell->getCommand(command)) {
                Terminal::setColor("grey", false);
                std::cout << "desc: ";
                Terminal::clear();
                std::cout << shellCommand->getDesc();
            }
            std::cout << std::endl;
            Terminal::clear();
        }
        */

        NodePool pool = shell->poolForNode(node);
        pool.draw();
    }
}
