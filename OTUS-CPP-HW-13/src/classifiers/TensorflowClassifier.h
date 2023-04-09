#pragma once

#include <string>
#include <memory>

#include "tensorflow/c/c_api.h"

#include "Classifier.h"

class TensorflowClassifier : public Classifier {
public:
    TensorflowClassifier(const std::string& modelPath, const std::size_t width, const size_t height);

    TensorflowClassifier(const TensorflowClassifier&) = delete;
    TensorflowClassifier& operator=(const TensorflowClassifier&) = delete;

    size_t numClasses() const override;
    size_t predict(const features_t&) const override;
    probas_t predictProba(const features_t&) const override;

protected:
    static void deleteTfSession(TF_Session*);

    using tf_graph = std::unique_ptr<TF_Graph, decltype(&TF_DeleteGraph)>;
    using tf_buffer = std::unique_ptr<TF_Buffer, decltype(&TF_DeleteBuffer)>;
    using tf_import_graph_def_options = std::unique_ptr<TF_ImportGraphDefOptions, decltype(&TF_DeleteImportGraphDefOptions)>;
    using tf_status = std::unique_ptr<TF_Status, decltype(&TF_DeleteStatus)>;
    using tf_session_options = std::unique_ptr<TF_SessionOptions, decltype(&TF_DeleteSessionOptions)>;
    using tf_tensor = std::unique_ptr<TF_Tensor, decltype(&TF_DeleteTensor)>;
    using tf_session = std::unique_ptr<TF_Session, decltype(&deleteTfSession)>;

protected:
    tf_graph _graph{TF_NewGraph(), TF_DeleteGraph};
    tf_session_options _sessionOpts{TF_NewSessionOptions(), TF_DeleteSessionOptions};
    tf_session _session = {nullptr, deleteTfSession};
    TF_Operation* _inputOp = nullptr;
    TF_Operation* _outputOp = nullptr;
    size_t _width;
    size_t _height;
};