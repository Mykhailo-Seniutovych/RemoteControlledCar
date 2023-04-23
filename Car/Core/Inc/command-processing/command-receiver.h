#ifndef INC_COMMAND_PROCESSING_COMMAND_RECEIVER_H_
#define INC_COMMAND_PROCESSING_COMMAND_RECEIVER_H_

#include "command-processing/car-command.h"

class CommandReceiver {
  public:
    CarCommand getNextCommand();
    void initializeReceiver();
};

#endif