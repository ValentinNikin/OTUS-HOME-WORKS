#include "Canvas.h"

std::vector<std::shared_ptr<Shape>> Canvas::getDrawedObjects() {
    return _objects;
}

void Canvas::addObject(const std::shared_ptr<Shape>& object) {
    _objects.push_back(object);
    (void)object;
}

void Canvas::removeObject(const std::string& objectId) {
    (void)objectId;
}

uint32_t Canvas::getWidth() const {
    return _width;
}

uint32_t Canvas::getHeight() const {
    return _height;
}

void Canvas::setWidth(uint32_t newWidth) {
    _width = newWidth;
}

void Canvas::setHeight(uint32_t newHeight) {
    _height = newHeight;
}

void Canvas::clear() {

}