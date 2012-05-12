#include <BWAPI/Vectorset.h>
#include <BWAPI/Playerset.h>

#include <BWAPI/Unitset.h>
#include <BWAPI/Player.h>

namespace BWAPI
{
  Playerset::Playerset(size_t initialSize) : Vectorset(initialSize) { };
  Playerset::Playerset(const Playerset &other) : Vectorset(other) { };
  Playerset::Playerset(const Playerset &&other) : Vectorset(other) { };

  Unitset Playerset::getUnits() const
  {
    Unitset retSet;    // The return set
    // Iterate each player
    for ( Playerset::iterator i = this->begin(); i != this->end(); ++i )
      retSet += i->getUnits();  // retrieve player's units
    return retSet;
  }

  Race::set Playerset::getRaces() const
  {
    Race::set retSet;  // The return set
    // Iterate each player
    for ( Playerset::iterator i = this->begin(); i != this->end(); ++i )
      retSet.push_back(i->getRace());
    return retSet;
  }


}

