#include "TensorflowClassifier.h"

#include <stdexcept>
#include <algorithm>
#include <cassert>

TensorflowClassifier::TensorflowClassifier(
                const std::string& modelPath,
                const size_t width,
                const size_t height)
        : _width{width}, _height{height} {

    tf_status status{TF_NewStatus(), TF_DeleteStatus};

    TF_Buffer* runOpts = nullptr;
    const char *tags = "serve";

    _session.reset(TF_LoadSessionFromSavedModel(
            _sessionOpts.get(), runOpts, modelPath.c_str(), &tags, 1, _graph.get(), nullptr, status.get()));
    if (TF_GetCode(status.get()) != TF_OK) {
        throw std::invalid_argument("Unable to import graph from '" + modelPath + "': " + std::string(TF_Message(status.get())));
    }

    _inputOp = TF_GraphOperationByName(_graph.get(), "serving_default_input");
    if (_inputOp == nullptr) {
        throw std::runtime_error("Input not found");
    }

    _outputOp = TF_GraphOperationByName(_graph.get(), "StatefulPartitionedCall");
    if (_outputOp == nullptr) {
        throw std::runtime_error("Output not found");
    }
}

void TensorflowClassifier::deleteTfSession(TF_Session *tf_session) {
    tf_status status{TF_NewStatus(), TF_DeleteStatus};
    TF_DeleteSession(tf_session, status.get());
    if (TF_GetCode(status.get()) != TF_OK) {
        throw std::runtime_error("Unable to delete TF_Session: \"" + std::string(TF_Message(status.get())));
    }
}

namespace {

void dummy_deleter(void *, size_t, void *) {}

}

size_t TensorflowClassifier::numClasses() const {
    return 10;
}

size_t TensorflowClassifier::predict(const features_t& feat) const {
    auto proba = predictProba(feat);
    auto argmax = std::max_element(proba.begin(), proba.end());
    return std::distance(proba.begin(), argmax);
}

Classifier::probas_t TensorflowClassifier::predictProba(const features_t& feat) const {
    assert(_width * _height == feat.size());

    // Preprocess input features
    features_t preprocFeatures;
    preprocFeatures.reserve(feat.size());

    // Divide each bytes by 255
    std::transform(
            feat.cbegin(), feat.cend(), std::back_inserter(preprocFeatures),
            [](const auto& val) { return val / 255;});

    // Create variables to store the size of the input and output variables
    const std::size_t numBytesIn = _width * _height * sizeof(float);
    const std::size_t numBytesOut = numClasses() * sizeof(float);

    // Set input dimensions - this should match the dimensionality of the input in
    // the loaded graph, in this case it's three dimensional.
    int64_t inDims[] = {1, static_cast<int64_t>(_width), static_cast<int64_t>(_height), 1};
    int64_t outDims[] = {1, static_cast<int64_t>(numClasses())};

    std::vector<TF_Output> inputs;
    TF_Output inputOpout = {_inputOp, 0};
    inputs.push_back(inputOpout);

    std::vector<TF_Tensor*> inputValues;
    tf_tensor input {
        TF_NewTensor(
                TF_FLOAT, inDims, 4, reinterpret_cast<void*>(preprocFeatures.data()), numBytesIn, &dummy_deleter, nullptr),
        TF_DeleteTensor
    };
    inputValues.push_back(input.get());

    std::vector<TF_Output> outputs;
    TF_Output outputOpout = {_outputOp, 0};
    outputs.push_back(outputOpout);

    // Create TF_Tensor* vector
    std::vector<TF_Tensor*> outputValues(outputs.size(), nullptr);

    // Similar to creating the input tensor, however here we don't yet have the
    // output values, so we use TF_AllocateTensor()
    tf_tensor outputValue{TF_AllocateTensor(TF_FLOAT, outDims, 2, numBytesOut), TF_DeleteTensor};
    outputValues.push_back(outputValue.get());

    tf_status status{TF_NewStatus(), TF_DeleteStatus};

    TF_SessionRun(_session.get(), nullptr,
                  &inputs[0], &inputValues[0], inputs.size(),
                  &outputs[0], &outputValues[0], outputs.size(),
                  nullptr, 0, nullptr, status.get());
    if (TF_GetCode(status.get()) != TF_OK) {
        throw std::runtime_error("Unable to run session from graph: " + std::string(TF_Message(status.get())));
    }

    probas_t probas;
    float* outVals = static_cast<float*>(TF_TensorData(outputValues[0]));
    for (size_t i = 0; i < numClasses(); ++i) {
        probas.push_back(*outVals++);
    }

    return probas;
}