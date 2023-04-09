#include "DocumentsController.h"

DocumentsController::DocumentsController(
            const std::shared_ptr<IDocumentsService>& documentsService)
            : _documentsService(documentsService) {}

void DocumentsController::createDocument() {
    _documentsService->createDocument();
}

void DocumentsController::saveDocument(const std::string& documentId) {
    _documentsService->saveDocument(documentId);
}

void DocumentsController::saveAsDocument(const std::string& documentId, const std::string& targetPath) {
    _documentsService->saveAsDocument(documentId, targetPath);
}

void DocumentsController::openDocument(const std::string& path) {
    _documentsService->openDocument(path);
}

void DocumentsController::makeActiveDocument(const std::string& documentId) {
    _documentsService->makeActiveDocument(documentId);
}

void DocumentsController::closeDocument(const std::string& documentId) {
    _documentsService->closeDocument(documentId);
}

std::shared_ptr<DocumentsView> DocumentsController::getView() {
    auto view = std::make_shared<DocumentsView>(_documentsService);
    _documentsService->addObserver(view);
    return view;
}