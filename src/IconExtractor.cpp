#include <napi.h>
#ifdef WIN32
#include "IconExtractorWindows.h"
#define IconExtractorImpl IconExtractorWindows
#elif __APPLE__
#include "IconExtractorMacOSX.h"
#define IconExtractorImpl IconExtractorMacOSX
#else
#include "IconExtractorLinux.h"
#define IconExtractorImpl IconExtractorLinux
#endif

IconExtractorImpl &instance() {
  static IconExtractorImpl instance;
  return instance;
}

class IconExtractorWorker : public Napi::AsyncWorker {
public:
  IconExtractorWorker(const Napi::Function &callback,
    const std::string &executable,
    const std::string &output,
    int width,
    const std::string &format)
    : AsyncWorker(callback)
    , m_Executable(executable)
    , m_Output(output)
    , m_Width(width)
    , m_Format(format) {}

  ~IconExtractorWorker() {}

  void Execute() {
    try {
      instance().extractIconToPngFile(m_Executable, m_Output, m_Width, m_Format);
    }
    catch (const std::exception &e) {
      SetError(e.what());
    }
  }

  virtual void OnOK() override {
    Callback().Call(Receiver().Value(), std::initializer_list<napi_value>{ Env().Null(), Napi::Boolean::New(Env(), true) });
  }

private:
  std::string m_Executable;
  std::string m_Output;
  int m_Width;
  std::string m_Format;

  std::string m_Error;

};

/*

NAN_METHOD(extractIconToFile) {
  int width = To<int>(info[2]).FromJust();
  Callback *callback = new Callback(info[4].As<Function>());

  AsyncQueueWorker(new IconExtractorWorker(callback, *Nan::Utf8String(info[0]), *Nan::Utf8String(info[1]), width, *Nan::Utf8String(info[3])));
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, New<v8::String>("extractIconToFile").ToLocalChecked(),
    GetFunction(New<v8::FunctionTemplate>(extractIconToFile)).ToLocalChecked());
}

NODE_MODULE(iconextract, Init)

*/


Napi::Value extractIconToFile(const Napi::CallbackInfo& info) {
  Napi::Function callback = info[4].As<Napi::Function>();

  auto worker = new IconExtractorWorker(callback, info[0].ToString(), info[1].ToString(), info[2].ToNumber(), info[3].ToString());

  worker->Queue();
  return info.Env().Undefined();
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("extractIconToFile", Napi::Function::New(env, extractIconToFile));
  return exports;
}

NODE_API_MODULE(iconExtract, Init)

