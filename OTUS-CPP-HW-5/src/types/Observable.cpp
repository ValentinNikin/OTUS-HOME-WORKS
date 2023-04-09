#include "Observable.h"

void Observable::addObserver(const std::shared_ptr<IObserver>& observer) {
    _observers.push_back(observer);
}

void Observable::notifyUpdate() {
    for (const auto& o : _observers) {
        o->update();
    }
}