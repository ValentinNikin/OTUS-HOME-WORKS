#pragma once

#include <vector>
#include <memory>

#include "types/Document.h"
#include "types/Observable.h"

/**
 * Описание интерфейса для сервиса управления документами
 */
class IDocumentsService : public Observable {
public:
    /**
     * @return список ссылок на открытые документы
     */
    virtual std::vector<std::shared_ptr<Document>> getOpenedDocuments() = 0;

    /**
     * @return ссылка на активный документ
     */
    virtual std::shared_ptr<Document> getActiveDocument() = 0;

    /**
     * Создать новый пустой документ
     */
    virtual void createDocument() = 0;

    /**
     * Сохранение документа
     * @param documentId id открытого документа, который необходимо сохранить
     */
    virtual void saveDocument(const std::string& documentId) = 0;

    /**
     * "Сохранить-как" документ
     * @param documentId id открытого документа, который необходимо сохранить
     * @param targetPath путь куда необходимо сохранить документ
     */
    virtual void saveAsDocument(const std::string& documentId, const std::string& targetPath) = 0;

    /**
     * Открыть документ
     * @param path путь к документу
     */
    virtual void openDocument(const std::string& path) = 0;

    /**
     * Сделать документ активным
     * @param documentId id открытого документа, который необходимо сделать активным
     */
    virtual void makeActiveDocument(const std::string& documentId) = 0;

    /**
     * Закрыть документ
     * @param documentId id открытого документа, который необходимо закрыть
     */
    virtual void closeDocument(const std::string& documentId) = 0;

    virtual ~IDocumentsService() = default;
};