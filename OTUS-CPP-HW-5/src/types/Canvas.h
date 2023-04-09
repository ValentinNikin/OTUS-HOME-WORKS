#pragma once

#include <vector>
#include <memory>

#include "./Shape.h"

/**
 * Read-only интерфейс канваса, для доступа к нему напрямую из документа
 */
class IReadOnlyCanvas {
public:
    virtual std::vector<std::shared_ptr<Shape>> getDrawedObjects() = 0;
    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual ~IReadOnlyCanvas() = default;
};

/**
 * Канвас.
 * Содержит информацию об объектах нарисованных на канвасе
 */
class Canvas : public IReadOnlyCanvas {
public:
    std::vector<std::shared_ptr<Shape>> getDrawedObjects() override;
    void addObject(const std::shared_ptr<Shape>& object);
    void removeObject(const std::string& objectId);

    uint32_t getWidth() const override;
    uint32_t getHeight() const override;
    void setWidth(uint32_t newWidth);
    void setHeight(uint32_t newHeight);

    void clear();

private:
    std::vector<std::shared_ptr<Shape>> _objects;
    uint32_t _width {0};
    uint32_t _height {0};
};
