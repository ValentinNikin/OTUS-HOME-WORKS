#pragma once

#include "types/Observable.h"
#include "types/Canvas.h"
#include "types/Shape.h"

/**
 * Описание интерфейса для сервис управления рисованием на канвасе
 */
class ICanvasService : public Observable, public IObserver {
public:
    /**
     * @return список ссылок на отрисованные на канвасе объекты
     */
    virtual std::vector<std::shared_ptr<Shape>> getDrawedObjects() = 0;

    /**
     * Добавить объект на канвас
     * @param obj ссылка на добавляемый объект
     */
    virtual void addObject(const std::shared_ptr<Shape>& obj) = 0;

    /**
     * Удалить объект с канваса
     * @param objectId id объекта, которые необходимо удалить
     */
    virtual void removeObject(const std::string& objectId) = 0;

    /**
     * @return ширина канваса
     */
    virtual uint32_t getWidth() const = 0;

    /**
     * @return высота канваса
     */
    virtual uint32_t getHeight() const = 0;

    /**
     * Изменить размер канваса
     * @param newWidth новое значение ширины
     * @param newHeight новое значение высоты
     */
    virtual void resizeCanvas(uint32_t newWidth, uint32_t newHeight) = 0;

    /**
     * Очистить канвас
     */
    virtual void clear() = 0;

    virtual ~ICanvasService() = default;
};