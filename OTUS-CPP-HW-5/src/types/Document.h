#pragma once

#include <string>
#include <memory>

#include "./Canvas.h"

/**
 * Документ. Хранит информацию об открытом документе.
 */
class Document {
public:

    Document(
            const std::string& id,
            const std::string& path,
            const std::string& creationDate,
            const std::string& lastUpdateDate,
            const std::shared_ptr<IReadOnlyCanvas>& canvas);

    std::string getId() const;
    std::string getPath() const;
    std::string getCreationDate() const;
    std::string getLastUpdateDate() const;
    void setLastUpdateDate(const std::string& lastUpdateDate);
    std::shared_ptr<IReadOnlyCanvas> getCanvas() const;
private:
    std::string _id;
    std::string _path;
    std::string _creationDate;
    std::string _lastUpdateDate;
    std::shared_ptr<IReadOnlyCanvas> _canvas;
};

/**
 * Builder-класс для создания экземпляров класса Document.
 */
class DocumentBuilder {
public:
    DocumentBuilder() = default;
    ~DocumentBuilder() = default;

    DocumentBuilder& withId(const std::string& id);
    DocumentBuilder& withPath(const std::string& path);
    DocumentBuilder& withCreationDate(const std::string& creationDate);
    DocumentBuilder& withLastUpdateDate(const std::string& lastUpdateDate);
    DocumentBuilder& withCanvas(const std::shared_ptr<IReadOnlyCanvas>& canvas);

    std::unique_ptr<Document> build();

private:
    std::string _id;
    std::string _path;
    std::string _creationDate;
    std::string _lastUpdateDate;
    std::shared_ptr<IReadOnlyCanvas> _canvas;
};