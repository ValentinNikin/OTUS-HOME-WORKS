#include "DocumentsView.h"

#include <iostream>

DocumentsView::DocumentsView(
        const std::shared_ptr<IDocumentsService>& documentsService)
        : _documentsService(documentsService) {}

void DocumentsView::update() {
    std::cout << std::endl << "---" << std::endl;
    std::cout << "DocumentsView triggered by Model. Updating..." << std::endl;
    std::cout << "Opened documents: " << _documentsService->getOpenedDocuments().size() << std::endl;
    if (_documentsService->getActiveDocument() != nullptr) {
        std::cout << "Active document: " << _documentsService->getActiveDocument()->getPath() << std::endl;
    }
    std::cout << "---" << std::endl << std::endl;
}