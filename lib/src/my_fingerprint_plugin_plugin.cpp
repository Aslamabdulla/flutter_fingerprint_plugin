

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>
#include <sstream>
#include <vector>

#include "sgfplib.h"

namespace {

class MyFingerprintPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  MyFingerprintPlugin();

  virtual ~MyFingerprintPlugin();

 private:
  void Initialize();
  void Terminate();
  bool OpenDevice();
  void CloseDevice();
  std::vector<uint8_t> CaptureFingerprint();
  int GetFingerprintQuality(const std::vector<uint8_t>& fingerprint);
  bool MatchFingerprints(const std::vector<uint8_t>& fingerprint1, const std::vector<uint8_t>& fingerprint2);
  void EnableAutoOn();
  void HandleAutoCapture();

  // Fingerprint SDK related
  HSGFPM hsgfpm;
  BYTE* fingerBuffer;
  DWORD bufferSize;
};

// static
void MyFingerprintPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
      registrar->messenger(), "my_fingerprint_plugin",
      &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<MyFingerprintPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto& call, auto result) {
        if (call.method_name().compare("initialize") == 0) {
          plugin_pointer->Initialize();
          result->Success();
        } else if (call.method_name().compare("terminate") == 0) {
          plugin_pointer->Terminate();
          result->Success();
        } else if (call.method_name().compare("openDevice") == 0) {
          if (plugin_pointer->OpenDevice()) {
            result->Success();
          } else {
            result->Error("DeviceOpenFailed", "Failed to open device");
          }
        } else if (call.method_name().compare("closeDevice") == 0) {
          plugin_pointer->CloseDevice();
          result->Success();
        } else if (call.method_name().compare("captureFingerprint") == 0) {
          auto fingerprint = plugin_pointer->CaptureFingerprint();
          result->Success(flutter::EncodableValue(fingerprint));
        } else if (call.method_name().compare("getFingerprintQuality") == 0) {
          auto args = std::get_if<flutter::EncodableList>(call.arguments());
          if (args && args->size() == 1) {
            auto fingerprint = std::get<std::vector<uint8_t>>(args->at(0));
            int quality = plugin_pointer->GetFingerprintQuality(fingerprint);
            result->Success(flutter::EncodableValue(quality));
          } else {
            result->Error("InvalidArguments", "Invalid arguments for getFingerprintQuality");
          }
        } else if (call.method_name().compare("matchFingerprints") == 0) {
          auto args = std::get_if<flutter::EncodableList>(call.arguments());
          if (args && args->size() == 2) {
            auto fingerprint1 = std::get<std::vector<uint8_t>>(args->at(0));
            auto fingerprint2 = std::get<std::vector<uint8_t>>(args->at(1));
            bool matched = plugin_pointer->MatchFingerprints(fingerprint1, fingerprint2);
            result->Success(flutter::EncodableValue(matched));
          } else {
            result->Error("InvalidArguments", "Invalid arguments for matchFingerprints");
          }
        } else if (call.method_name().compare("enableAutoOn") == 0) {
          plugin_pointer->EnableAutoOn();
          result->Success();
        } else if (call.method_name().compare("handleAutoCapture") == 0) {
          plugin_pointer->HandleAutoCapture();
          result->Success();
        } else {
          result->NotImplemented();
        }
      });

  registrar->AddPlugin(std::move(plugin));
}

MyFingerprintPlugin::MyFingerprintPlugin() : hsgfpm(nullptr), fingerBuffer(nullptr), bufferSize(0) {
  // Initialize the SDK
}

MyFingerprintPlugin::~MyFingerprintPlugin() {
  // Cleanup
  Terminate();
}

void MyFingerprintPlugin::Initialize() {
  // Initialize SDK
  if (SGFPM_Init(SGFPM_DEVICE_AUTO_DETECT, &hsgfpm) != SGFDX_ERROR_NONE) {
    // Handle error
  }
}

void MyFingerprintPlugin::Terminate() {
  // Terminate SDK
  if (hsgfpm != nullptr) {
    SGFPM_Terminate(hsgfpm);
    hsgfpm = nullptr;
  }
  if (fingerBuffer != nullptr) {
    delete[] fingerBuffer;
    fingerBuffer = nullptr;
  }
}

bool MyFingerprintPlugin::OpenDevice() {
  // Open fingerprint device
  return (SGFPM_OpenDevice(hsgfpm, SG_DEV_AUTO) == SGFDX_ERROR_NONE);
}

void MyFingerprintPlugin::CloseDevice() {
  // Close fingerprint device
  SGFPM_CloseDevice(hsgfpm);
}

std::vector<uint8_t> MyFingerprintPlugin::CaptureFingerprint() {
  // Capture fingerprint image
  bufferSize = 0;
  SGFPM_GetMaxImageSize(hsgfpm, &bufferSize);
  fingerBuffer = new BYTE[bufferSize];
  if (SGFPM_GetImage(hsgfpm, fingerBuffer) != SGFDX_ERROR_NONE) {
    // Handle error
    return {};
  }
  return std::vector<uint8_t>(fingerBuffer, fingerBuffer + bufferSize);
}

int MyFingerprintPlugin::GetFingerprintQuality(const std::vector<uint8_t>& fingerprint) {
  // Get fingerprint image quality
  int quality = 0;
  if (SGFPM_GetImageQuality(hsgfpm, &fingerprint[0], bufferSize, &quality) != SGFDX_ERROR_NONE) {
    // Handle error
    return -1;
  }
  return quality;
}

bool MyFingerprintPlugin::MatchFingerprints(const std::vector<uint8_t>& fingerprint1, const std::vector<uint8_t>& fingerprint2) {
  // Match fingerprint templates
  SGFPM_FingerprintTemplate fpTemplate1, fpTemplate2;
  BOOL matched;
  if (SGFPM_CreateTemplate(hsgfpm, &fingerprint1[0], &fpTemplate1) != SGFDX_ERROR_NONE) {
    // Handle error
    return false;
  }
  if (SGFPM_CreateTemplate(hsgfpm, &fingerprint2[0], &fpTemplate2) != SGFDX_ERROR_NONE) {
    // Handle error
    return false;
  }
  if (SGFPM_MatchTemplates(hsgfpm, &fpTemplate1, &fpTemplate2, &matched) != SGFDX_ERROR_NONE) {
    // Handle error
    return false;
  }
  return matched;
}

void MyFingerprintPlugin::EnableAutoOn() {
  // Enable auto-on feature
  SGFPM_EnableAutoOnEvent(hsgfpm, TRUE, NULL, NULL);
}

void MyFingerprintPlugin::HandleAutoCapture() {
  // Handle auto capture
  auto fingerprint = CaptureFingerprint();
  // Send captured fingerprint to Dart side
}

}  // namespace

void MyFingerprintPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  MyFingerprintPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
