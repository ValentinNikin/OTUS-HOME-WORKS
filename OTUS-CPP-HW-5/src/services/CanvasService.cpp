#include "CanvasService.h"

#include <iostream>

CanvasService::CanvasService(
        const std::shared_ptr<IDocumentsService>& documentsService)
        : _documentsService(documentsService) {}

std::vector<std::shared_ptr<Shape>> CanvasService::getDrawedObjects() {
    return _canvas->getDrawedObjects();
}

void CanvasService::addObject(const std::shared_ptr<Shape>& obj) {
    std::cout << "CanvasService. Added new object to the canvas" << std::endl;

    obj->setId("SomeNewId");
    _canvas->addObject(obj);
    notifyUpdate();
}

void CanvasService::removeObject(const std::string& objectId) {
    // we need some checks that object exist on the canvas
    std::cout << "CanvasService. Removed object from the canvas" << std::endl;
    _canvas->removeObject(objectId);
    notifyUpdate();
}

uint32_t CanvasService::getWidth() const {
    return _canvas->getWidth();
}

uint32_t CanvasService::getHeight() const {
    return _canvas->getHeight();
}

void CanvasService::resizeCanvas(uint32_t newWidth, uint32_t newHeight) {
    // we need some logic to delete objects that out of new canvas area
    std::cout << "CanvasService. Canvas resized" << std::endl;
    _canvas->setWidth(newWidth);
    _canvas->setHeight(newHeight);
    notifyUpdate();
}

void CanvasService::clear() {
    std::cout << "CanvasService. Canvas cleared" << std::endl;
    _canvas->clear();
    notifyUpdate();
}

void CanvasService::update() {
    // if active document changed we need to load canvas from new active document

    std::cout << std::endl << "---" << std::endl;
    std::cout << "CanvasService triggered by DocumentsService. Updating..." << std::endl;
    if (_documentsService->getActiveDocument() != nullptr &&
            _documentsService->getActiveDocument()->getCanvas() != nullptr) {
        _canvas =  std::dynamic_pointer_cast<Canvas>(
                _documentsService->getActiveDocument()->getCanvas());
        _canvas->setWidth(1920);
        _canvas->setHeight(1080);
    }
    else {
        _canvas = std::make_shared<Canvas>();
    }
    std::cout << "---" << std::endl << std::endl;
    notifyUpdate();
}
