#pragma once

#include "./IDocumentsService.h"

/**
 * Сервис управления документами
 */
class DocumentsService : public IDocumentsService {
    std::vector<std::shared_ptr<Document>> getOpenedDocuments() override;
    std::shared_ptr<Document> getActiveDocument() override;
    void createDocument() override;
    void saveDocument(const std::string& documentId) override;
    void saveAsDocument(const std::string& documentId, const std::string& targetPath) override;
    void openDocument(const std::string& path) override;
    void makeActiveDocument(const std::string& documentId) override;
    void closeDocument(const std::string& documentId) override;

private:
    std::vector<std::shared_ptr<Document>> _openedDocuments;
    std::shared_ptr<Document> _activeDocument;
};