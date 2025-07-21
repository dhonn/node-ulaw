// addon.cc
#include <napi.h>
#include <stdint.h>
#include <stddef.h>

// existing decode
extern "C" void decode_samples(const uint8_t* in, int16_t* out, size_t len);

// new encode
extern "C" void encode_samples(const int16_t* in, uint8_t* out, size_t len);

Napi::Buffer<int16_t> DecodeUlaw(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (!info[0].IsBuffer()) {
        Napi::TypeError::New(env, "Buffer expected").ThrowAsJavaScriptException();
        return {};
    }
    auto inBuf  = info[0].As<Napi::Buffer<uint8_t>>();
    size_t len  = inBuf.Length();
    auto outBuf = Napi::Buffer<int16_t>::New(env, len);
    decode_samples(inBuf.Data(), outBuf.Data(), len);
    return outBuf;
}

Napi::Buffer<uint8_t> EncodeUlaw(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (!info[0].IsBuffer()) {
        Napi::TypeError::New(env, "Buffer expected").ThrowAsJavaScriptException();
        return {};
    }
    auto inBuf  = info[0].As<Napi::Buffer<int16_t>>();
    size_t len  = inBuf.Length();
    auto outBuf = Napi::Buffer<uint8_t>::New(env, len);
    encode_samples(inBuf.Data(), outBuf.Data(), len);
    return outBuf;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("decodeUlaw", Napi::Function::New(env, DecodeUlaw));
    exports.Set("encodeUlaw", Napi::Function::New(env, EncodeUlaw));
    return exports;
}

NODE_API_MODULE(g711_table, Init)
