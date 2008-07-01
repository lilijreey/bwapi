#include "Command.h"
namespace BWAPI
{
  //---------------------------- CONSTRUCTOR -----------------------------------
  Command::Command(const std::vector<Unit*> &executors)
  :executors(executors)
  {
  }
  //---------------------------- CONSTRUCTOR -----------------------------------
  Command::Command(Unit* executor)
  {
    executors.push_back(executor);
  }
  //----------------------------- DESTRUCTOR -----------------------------------
  Command::~Command()
  {
  }
  //----------------------------------------------------------------------------
}