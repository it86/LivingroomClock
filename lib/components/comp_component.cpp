#include "components.h"

Component::Component(const uint16_t& color)
  : color(color)
{}

const uint16_t Component::getColor() const {
  return color;
}

const bool Component::isVisible() const {
  return visible_next;
}

void Component::setColor(const uint16_t& color) {
  this->color = color;
}

void Component::setVisible(const bool& visible) {
  this->visible_next = visible;
}
