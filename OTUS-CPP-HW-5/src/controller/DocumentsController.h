#pragma once

#include <string>
#include <memory>

#include "../services/IDocumentsService.h"
#include "../view/DocumentsView.h"

/**
 * Контроллер. Связывает DocumentView и DocumentService
 */
class DocumentsController {
public:
    DocumentsController(const std::shared_ptr<IDocumentsService>& documentsService);

    void createDocument();
    void saveDocument(const std::string& documentId);
    void saveAsDocument(const std::string& documentId, const std::string& targetPath);
    void openDocument(const std::string& path);
    void makeActiveDocument(const std::string& documentId);
    void closeDocument(const std::string& documentId);

    std::shared_ptr<DocumentsView> getView();

private:
    std::shared_ptr<IDocumentsService> _documentsService;
};

