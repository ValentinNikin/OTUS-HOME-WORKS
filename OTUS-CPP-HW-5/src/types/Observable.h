#pragma once

#include <vector>
#include <memory>

/**
 * Интерфейс, который необходимо реализовать, для того чтобы получать
 * нотификацию об изменении состояния модели.
 */
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update() = 0;
};

/**
 * Максимально простая реализация паттерна "Наблюдатель".
 * Используется для нотификации о произошедших в модели событиях.
 */
class Observable {
public:
    /**
     * Добавить "наблюдателя"
     * @param observer ссылка на экземпляр класса "наблюдателя"
     */
    void addObserver(const std::shared_ptr<IObserver>& observer);
protected:
    /**
     * Оповестить подписчиков об обновлении.
     */
    void notifyUpdate();
private:
    std::vector<std::shared_ptr<IObserver>> _observers;
};
