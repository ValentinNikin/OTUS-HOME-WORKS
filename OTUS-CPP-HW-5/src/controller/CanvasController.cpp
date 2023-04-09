#include "CanvasController.h"

CanvasController::CanvasController(
        const std::shared_ptr<ICanvasService>& canvasService)
        : _canvasService(canvasService) {}

void CanvasController::addObject(const std::shared_ptr<Shape>& obj) {
    _canvasService->addObject(obj);
}

void CanvasController::removeObject(const std::string& objectId) {
    _canvasService->removeObject(objectId);
}

void CanvasController::resizeCanvas(uint32_t newWidth, uint32_t newHeight) {
    _canvasService->resizeCanvas(newWidth, newHeight);
}

void CanvasController::clear() {
    _canvasService->clear();
}

std::shared_ptr<CanvasView> CanvasController::getView() {
    auto view = std::make_shared<CanvasView>(_canvasService);
    _canvasService->addObserver(view);
    return view;
}