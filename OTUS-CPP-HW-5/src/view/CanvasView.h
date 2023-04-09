#pragma once

#include <memory>

#include "../services/ICanvasService.h"
#include "../types/Observable.h"

/**
 * Класс View для канваса
 */
class CanvasView: public IObserver {
public:
    CanvasView(const std::shared_ptr<ICanvasService>& canvasService);
    void update() override;
private:
    std::shared_ptr<ICanvasService> _canvasService;
};
