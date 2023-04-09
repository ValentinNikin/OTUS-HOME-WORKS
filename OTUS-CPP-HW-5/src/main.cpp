
#include "controller/CanvasController.h"
#include "controller/DocumentsController.h"

#include "services/DocumentsService.h"
#include "services/CanvasService.h"

#include "view/CanvasView.h"
#include "view/DocumentsView.h"

int main (int, char **) {

    auto documentsService = std::make_shared<DocumentsService>();
    auto canvasService = std::make_shared<CanvasService>(documentsService);
    documentsService->addObserver(canvasService);

    auto documentsController = std::make_shared<DocumentsController>(documentsService);
    auto canvasController = std::make_shared<CanvasController>(canvasService);

    auto documentsView = documentsController->getView();
    auto canvasView = canvasController->getView();

    documentsController->createDocument();
    documentsController->saveAsDocument("someId", "/path/to/document.txt");

    auto rectangle = std::make_shared<Rectangle>("", 0, Point{0, 10}, 100, 50);
    canvasController->addObject(rectangle);
    canvasController->removeObject("SomeId");

    return 0;
}
