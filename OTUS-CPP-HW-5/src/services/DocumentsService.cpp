#include "DocumentsService.h"

#include <iostream>

#include "../types/Document.h"

std::vector<std::shared_ptr<Document>> DocumentsService::getOpenedDocuments() {
    return _openedDocuments;
}

std::shared_ptr<Document> DocumentsService::getActiveDocument() {
    return _activeDocument;
}

void DocumentsService::createDocument() {
    std::cout << "Document created" << std::endl;

    auto newDocument = DocumentBuilder()
                        .withId("SomeId")
                        .withPath("/path/to/document.txt")
                        .withCreationDate("14.10.20")
                        .withLastUpdateDate("21.11.21")
                        .withCanvas(std::make_shared<Canvas>())
                        .build();

    _openedDocuments.push_back(std::move(newDocument));
    _activeDocument = _openedDocuments[_openedDocuments.size() - 1];

    notifyUpdate();
}

void DocumentsService::saveDocument(const std::string& documentId) {
    std::cout << "Document with id '" << documentId << "' saved" << std::endl;
    notifyUpdate();
}

void DocumentsService::saveAsDocument(const std::string& documentId, const std::string& targetPath) {
    std::cout << "Document with id '" << documentId << "' saved to the next path '" << targetPath << "'" << std::endl;
    notifyUpdate();
}

void DocumentsService::openDocument(const std::string& path) {
    std::cout << "Document '" << path << "' is opened" << std::endl;
    notifyUpdate();
}

void DocumentsService::makeActiveDocument(const std::string& documentId) {
    std::cout << "Document with id '" << documentId << "' is active" << std::endl;
    notifyUpdate();
}

void DocumentsService::closeDocument(const std::string& documentId) {
    std::cout << "Document with id '" << documentId << "' is closed" << std::endl;
    notifyUpdate();
}
