#include "Document.h"

Document::Document(
        const std::string& id,
        const std::string& path,
        const std::string& creationDate,
        const std::string& lastUpdateDate,
        const std::shared_ptr<IReadOnlyCanvas>& canvas)
        : _id(id),
        _path(path),
        _creationDate(creationDate),
        _lastUpdateDate(lastUpdateDate),
        _canvas(canvas) {}

std::string Document::getId() const {
    return _id;
}

std::string Document::getPath() const {
    return _path;
}

std::string Document::getCreationDate() const {
    return _creationDate;
}

std::string Document::getLastUpdateDate() const {
    return _lastUpdateDate;
}

void Document::setLastUpdateDate(const std::string& lastUpdateDate) {
    _lastUpdateDate = lastUpdateDate;
}

std::shared_ptr<IReadOnlyCanvas> Document::getCanvas() const {
    return _canvas;
}

DocumentBuilder& DocumentBuilder::withId(const std::string& id) {
    _id = id;
    return *this;
}

DocumentBuilder& DocumentBuilder::withPath(const std::string& path) {
    _path = path;
    return *this;
}

DocumentBuilder& DocumentBuilder::withCreationDate(const std::string& creationDate) {
    _creationDate = creationDate;
    return *this;
}

DocumentBuilder& DocumentBuilder::withLastUpdateDate(const std::string& lastUpdateDate) {
    _lastUpdateDate = lastUpdateDate;
    return *this;
}

DocumentBuilder& DocumentBuilder::withCanvas(const std::shared_ptr<IReadOnlyCanvas>& canvas) {
    _canvas = canvas;
    return *this;
}

std::unique_ptr<Document> DocumentBuilder::build() {
    return std::make_unique<Document>(
            _id, _path, _creationDate, _lastUpdateDate, _canvas);
}
