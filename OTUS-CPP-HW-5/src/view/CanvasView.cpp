#include "CanvasView.h"

#include <iostream>

CanvasView::CanvasView(
        const std::shared_ptr<ICanvasService>& canvasService)
        : _canvasService(canvasService) {}

void CanvasView::update() {
    std::cout << std::endl << "---" << std::endl;
    std::cout << "CanvasView triggered by Model. Updating..." << std::endl;
    std::cout << "Drawed objects: " << _canvasService->getDrawedObjects().size() << std::endl;
    std::cout << "Canvas width: " << _canvasService->getWidth() << std::endl;
    std::cout << "Canvas height: " << _canvasService->getHeight() << std::endl;
    std::cout << "---" << std::endl << std::endl;
}