#pragma once

#include "./IDocumentsService.h"
#include "./ICanvasService.h"
#include "../types/Shape.h"

/**
 * Сервис управления рисованием на канвасе
 */
class CanvasService : public ICanvasService {
public:
    CanvasService(const std::shared_ptr<IDocumentsService>& documentsService);

    std::vector<std::shared_ptr<Shape>> getDrawedObjects() override;
    void addObject(const std::shared_ptr<Shape>& obj) override;
    void removeObject(const std::string& objectId) override;
    uint32_t getWidth() const override;
    uint32_t getHeight() const override;
    void resizeCanvas(uint32_t newWidth, uint32_t newHeight) override;
    void clear() override;

protected:
    void update() override;
private:
    std::shared_ptr<Canvas> _canvas;
    std::shared_ptr<IDocumentsService> _documentsService;
};
