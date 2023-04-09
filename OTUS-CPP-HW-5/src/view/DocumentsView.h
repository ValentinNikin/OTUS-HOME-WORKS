#pragma once

#include <memory>

#include "../services/IDocumentsService.h"
#include "../types/Observable.h"

/**
 * Класс View для открытых документов
 */
class DocumentsView : public IObserver {
public:
    DocumentsView(const std::shared_ptr<IDocumentsService>& documentsService);
    void update() override;
private:
    std::shared_ptr<IDocumentsService> _documentsService;
};

