#ifndef INC_COMMAND_PROCESSING_COMMAND_READER_H_
#define INC_COMMAND_PROCESSING_COMMAND_READER_H_

#include "command-processing/car-command.h"

class CommandReader {
  public:
    virtual CarCommand getNextCommand();
};

#endif