#include "BuildingPositionSet.h"

#include <Util/Strings.h>
#include <tinyxml.h>
#include <Util/Xml.h>

namespace BWAI
{      
  //------------------------------------- CONSTRUCTOR -------------------------
  BuildingPositionSet::BuildingPositionSet(TiXmlElement *xmlElement)
  {
    this->tileWidth  = Util::Xml::getRequiredU16Attribute(xmlElement, "width");
    this->tileHeight = Util::Xml::getRequiredU16Attribute(xmlElement, "height");
        
    const char * shortcutAttribute = xmlElement->Attribute("shortcut");
    if (shortcutAttribute != NULL)
      this->shortcut = shortcutAttribute;
  
    for (TiXmlElement* position = xmlElement->FirstChildElement("position"); position != NULL; position = position->NextSiblingElement("position"))
      this->positions.push_back(new BuildingPosition(BW::TilePosition(position)));
  }
  //------------------------------------- DESTRUCTOR ---------------------------
  BuildingPositionSet::~BuildingPositionSet()
  {
    for each (BuildingPosition* i in this->positions)
      delete i;
  }
}